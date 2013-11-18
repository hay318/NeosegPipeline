#include "AtlasRegistration.h"

// Constructor //

AtlasRegistration::AtlasRegistration (QString output_path, QString computingSystem) : m_computingSystem(computingSystem)
{
   m_output_dir = new QDir(output_path); 

   QString atlasRegistration_path = m_output_dir->filePath("AtlasRegistration");
   m_atlasRegistration_dir = new QDir(atlasRegistration_path);

   if (m_computingSystem == "Killdevil")
   {
      m_ANTS = "/nas02/home/f/b/fbudin/tools/ANTS";
      m_ResampleVolume2 = "/nas02/home/f/b/fbudin/tools/ResampleVolume2";
   }

   if (m_computingSystem == "Lab Server") 
   {
      m_ANTS = "/tools/bin_linux64/ANTS";
      m_ResampleVolume2 = "/tools/bin_linux64/ResampleVolume2";
   }
}


// Checking results // 

Atlas AtlasRegistration::defineOutput_Atlas(Atlas atlas)
{
   Atlas atlasReg = atlas;

   QString atlas_path = m_atlasRegistration_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path); 

   QString T1Reg_name = atlas.name + "_to_neo-T1.nrrd";
   atlasReg.T1 = atlas_dir->filePath(T1Reg_name); 

   QString T2Reg_name = atlas.name + "_to_neo-T2.nrrd";
   atlasReg.T2 = atlas_dir->filePath(T2Reg_name);

   QString segReg_name = atlas.name + "_to_neo-seg.nrrd";
   atlasReg.seg = atlas_dir->filePath(segReg_name);

   return atlasReg;
}

bool AtlasRegistration::checkExistingResults_Atlas(Atlas atlas)
{
   Atlas atlasReg = defineOutput_Atlas(atlas);

   QFile* T1Reg_file = new QFile (atlasReg.T1);
   QFile* T2Reg_file = new QFile (atlasReg.T2);
   QFile* segReg_file = new QFile (atlasReg.seg);

   if ( T1Reg_file->exists() && T2Reg_file->exists() && segReg_file->exists() )
   {
      return true;
   } 

   else
   {
      return false; 
   }
}

bool AtlasRegistration::checkExistingResults(std::vector<Atlas> atlasPopulation)
{
   std::vector<Atlas>::const_iterator it;

   for (it = atlasPopulation.begin(); it != atlasPopulation.end(); ++it)
   {  
      std::cout<<"coucou check results"<<((*it).name.toStdString())<<std::endl;
      
      if(!checkExistingResults_Atlas(*it))
      {
         return false;
      } 
   }
   return true;
}

// Creating Directories //

void AtlasRegistration::createDirectory()
{
   if (!m_output_dir->exists("AtlasRegistration"))
   {
      m_output_dir->mkdir("AtlasRegistration");
   }
}

void AtlasRegistration::createAtlasDirectories(std::vector<Atlas> atlasPopulation)
{
   std::vector<Atlas>::iterator it;

   for (it = atlasPopulation.begin(); it != atlasPopulation.end(); ++it)
   {  
      if (!checkExistingResults_Atlas(*it))
      { 
         m_atlasRegistration_dir->mkdir((*it).name);
      }
   }
}


// Writing Scripts //

QString AtlasRegistration::variable(QString variable_name) {return "\" + " + variable_name + " + \"";}

QString AtlasRegistration::str(QString str) {return "\"" + str + "\"";}


void AtlasRegistration::initializeScript(QString &script)
{
   script += "#!/usr/bin/python3.3 \n";

   // Import the librairies needed  
   script += "import os \n";
   script += "import sys \n";
   script += "import subprocess \n";
   script += "import time \n";
   script += "from threading import Thread \n \n";
}


void AtlasRegistration::implementRegisterAtlas(QString &script, Neo neo)
{
   QString command;

   script += "def main(T1Atlas, T2Atlas, output, log): \n\n";

   script += "\tprint('coucou\n\n')";

   script += "\t# Inputs # \n";
   script += "\tT1 = " + str(neo.T1_path) + "\n";
   script += "\tT2 = " + str(neo.T2_path) + "\n\n "; 

   script += "\t# Transformations # \n";
   script += "\taffine = output + " + str("_to_neo_Affine.txt") + "\n";
   script += "\twarp = output + " + str("_to_neo_Warp.nii.gz") + "\n\n"; 

   script += "\t# Outputs # \n";
   script += "\tT1Reg = output + " + str("_to_neo-T1.nrrd") + "\n";
   script += "\tT2Reg = output + " + str("_to_neo-T2.nrrd") + "\n";
   script += "\tsegReg = output + " + str("_to_neo-seg.nrrd") + "\n\n";

   QString log = variable("log");

   script += "\t# Calculate transformations # \n";

   QString m_model = "CC"; 
   QString m_weight = "1";
   QString m_radius = "4";
   QString m_iterations = "200x75x50";


   QString dimension = "3";
   QString model = "-m " + m_model; 
   QString modality_1 = "[" + variable("T1") + "," + variable("T1Atlas") + "," + m_weight + "," + m_radius + "]";
   QString modality_2 = "[" + variable("T2") + "," + variable("T2Atlas") + "," + m_weight + "," + m_radius + "]";
   QString output = "-o " + variable("output");
   QString iterations = "-i " + m_iterations;
   QString regularization = "-r Gauss[3,1]";
   QString transformations = "-t SyN[0.125]";

   command = m_ANTS + " " + dimension + " " + model + modality_1 + " " + model + modality_2 + " " + output + " " + iterations + " " + regularization + " " + transformations + " > " + log + "";
   script += "\tos.system(\"" + command + "\") \n\n";


   script += "\t# Apply transformations # \n";

   QString inputImage;
   QString outputImage;
   QString reference = "--Reference " + variable("T2");
   QString interpolation = "-i bs";
   QString hFieldType = "--hfieldtype displacement";
   QString defField = "--defField " + variable("affine");
   QString transformationFile = "--transformationFile " + variable("warp"); 

   inputImage = variable("T1Atlas");
   outputImage = variable("T1Reg");
   command = m_ResampleVolume2 + " " + inputImage + " " + outputImage + " " + reference + " " + interpolation + " " + hFieldType + " " + defField + " " + transformationFile + " > " + log + ""; 
   script += "\tos.system(\"" + command + "\") \n\n";  

   inputImage = variable("T2Atlas");
   outputImage = variable("T2Reg");
   command = m_ResampleVolume2 + " " + inputImage + " " + outputImage + " " + reference + " " + interpolation + " " + hFieldType + " " + defField + " " + transformationFile + " > " + log + ""; 
   script += "\tos.system(\"" + command + "\") \n\n"; 

   inputImage = variable("segAtlas");
   outputImage = variable("segReg");
   interpolation = "-i nn";
   command = m_ResampleVolume2 + " " + inputImage + " " + outputImage + " " + reference + " " + interpolation + " " + hFieldType + " " + defField + " " + transformationFile + " > " + log + ""; 
   script += "\tos.system(\"" + command + "\") \n\n"; 


   script += "if __name__ == " + str("__main__") + ": \n";
   script += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])";
}

void AtlasRegistration::writeRegisterAtlas(Neo neo) // args = (T1Atlas, T2Atlas, output, log)
{
   QString registerAtlas;

   initializeScript(registerAtlas);

   implementRegisterAtlas(registerAtlas, neo);

   QString script_path = m_atlasRegistration_dir -> filePath("RegisterAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerAtlas.toStdString() << std::endl; 
}


void AtlasRegistration::defineRegisterAtlasParameters(QString &script, Atlas atlas)
{
   script += "# Registration " + atlas.name + " to neo \n"; 

   script += "\tT1Atlas = " + str(atlas.T1) + "\n";
   script += "\tT2Atlas = " + str(atlas.T2) + "\n";

   QString atlas_path = m_atlasRegistration_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path);

   QString output_name = atlas.name + "_to_neo_.nii.gz";     
   QString output = atlas_dir->filePath(output_name);
   script += "\toutput = " + str(output) + "\n";

   QString log_name = atlas.name + "_to_neo.log";     
   QString log = atlas_dir->filePath(log_name);
   script += "\tlog = " + str(log) + "\n\n";
}

void AtlasRegistration::submitRegisterAtlasJob(QString &script)
{
   QString bsub = "\tbsub -n 1 " ;

   QString executable = "python " + variable("atlasRegistration_script") + " " + variable("T1Atlas") + " " + variable("T2Atlas") + " " + variable("output") + " " + variable("log");

   QString command = bsub + " " + executable; 
   script += "\tos.system(\"" + command + "\") \n\n";
}

void AtlasRegistration::executeRegisterAtlasProcess(QString &script, int i) 
{
   QString QString_i; 
   QString_i.setNum(i);

   script += "\twhile nbReg >= nbRegMax : \n";
   script += "\t\ttime.sleep(10)\n";
   script += "\tnbReg++\n\n";

   QString process_name = "process_" + QString_i;

   QString command = "['python', atlasRegistration_script, T1Atlas, T2Atlas, output, log]";

   script += "\t" + process_name + " = subprocess.Popen(" + command + ")\n\n";

   //script += "\t" + process_name + "."
}

void AtlasRegistration::implementTestRegistrationDone(QString &script)
{
   script += "def testRegistrationDone(output) : \n\n";

   script += "\t# Outputs # \n";
   script += "\tT1Reg = output + " + str("_to_neo-T1.nrrd") + "\n";
   script += "\tT2Reg = output + " + str("_to_neo-T2.nrrd") + "\n";
   script += "\tsegReg = output + " + str("_to_neo-seg.nrrd") + "\n\n";
  
   script += "\tif os.path.exist(T1Reg) == 0 : \n";
   script += "\t\treturn False\n\n";

   script += "\tif os.path.exist(T2Reg) == 0 : \n";
   script += "\t\treturn False\n\n";

   script += "\tif os.path.exist(segReg) == 0 : \n";
   script += "\t\treturn False\n\n";

   script += "\treturn True\n";
}

void AtlasRegistration::implementRegisterAtlasPopulation(QString &script, std::vector<Atlas> atlasPopulation)
{
   int i;

   implementTestRegistrationDone(script);

   script += "def main(): \n";

   script += "\t# Submit atlas registration jobs \n";

   QString atlasRegistration_script = m_atlasRegistration_dir->filePath("RegisterAtlas.py");
   script += "\tatlasRegistration_script = " + str(atlasRegistration_script) + "\n\n";

   script += "\tos.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '1' \n";
   script += "\tcheck = int(os.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS']) \n";
   script += "\tprint check \n\n";

   script += "\tnbReg = 0 \n";
   script += "\tnbRegMax = 4 \n";

   std::vector<Atlas>::iterator it;
   for (it = atlasPopulation.begin(); it != atlasPopulation.end(); ++it)
   {  
      i = std::distance(atlasPopulation.begin(), it);

      Atlas atlas= *(it);

      if (!checkExistingResults_Atlas(atlas))
      { 
         defineRegisterAtlasParameters(script, atlas);

         if (m_computingSystem == "Killdevil")
         {
            submitRegisterAtlasJob(script);
         }

         if (m_computingSystem == "Lab Server")
         {
            i = std::distance(atlasPopulation.begin(), it);
            executeRegisterAtlasProcess(script,i);
         }

         //os.path.exists()
      }
   }
   script += "if __name__ == " + str("__main__") + ": \n";
   script += "\tmain()";

}

void AtlasRegistration::writeRegisterAtlasPopulation(std::vector<Atlas> atlasPopulation)
{
   QString registerAtlasPopulation;

   initializeScript(registerAtlasPopulation);

   implementRegisterAtlasPopulation(registerAtlasPopulation, atlasPopulation);

   QString script_path = m_atlasRegistration_dir -> filePath("RegisterAtlasPopulation.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerAtlasPopulation.toStdString() << std::endl;   
}


void AtlasRegistration::updateAtlasPopulation(std::vector<Atlas> &atlasPopulation)
{
   std::vector<Atlas>::iterator it;

   for (it = atlasPopulation.begin(); it != atlasPopulation.end(); ++it)
   {   
      (*it) = defineOutput_Atlas(*it);
   }
}
