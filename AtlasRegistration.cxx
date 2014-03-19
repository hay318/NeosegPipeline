#include "AtlasRegistration.h"

AtlasRegistration::AtlasRegistration(QString module) : Script(module)
{

}

// SET // 
void AtlasRegistration::setAtlasPopulation(std::vector<Atlas> atlasPopulation) 
{
   m_atlasPopulation=atlasPopulation;
}
void AtlasRegistration::setComputingSystem(QString computingSystem)
{
   m_computingSystem=computingSystem;
}
void AtlasRegistration::setNumberOfCores(int nbCores)
{
   m_nbCores=nbCores;
} 
void AtlasRegistration::setAntsParameters(AntsParameters* parameters) 
{
   m_parameters=parameters;
}


// Define Output //  

Atlas AtlasRegistration::defineRegisteredAtlas(Atlas atlas)
{
   Atlas atlasReg = atlas;

   QString atlas_path = m_module_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path); 

   QString T1Reg_name = atlas.name + "_to_" + m_neo.prefix + "-T1.nrrd";
   atlasReg.T1 = atlas_dir->filePath(T1Reg_name); 

   QString T2Reg_name = atlas.name + "_to_" + m_neo.prefix + "-T2.nrrd";
   atlasReg.T2 = atlas_dir->filePath(T2Reg_name);

   if(atlas.probabilistic)
   {
      QString whiteReg_name = atlas.name + "_to_" + m_neo.prefix + "-white.nrrd";
      atlasReg.white = atlas_dir->filePath(whiteReg_name);

      QString grayReg_name = atlas.name + "_to_" + m_neo.prefix + "-gray.nrrd";
      atlasReg.gray = atlas_dir->filePath(grayReg_name);

      QString csfReg_name = atlas.name + "_to_" + m_neo.prefix + "-csf.nrrd";
      atlasReg.csf = atlas_dir->filePath(csfReg_name);
   }

   else
   {
      QString segReg_name = atlas.name + "_to_" + m_neo.prefix + "-seg.nrrd";
      atlasReg.seg = atlas_dir->filePath(segReg_name);
   }

   return atlasReg;
}

void AtlasRegistration::defineRegisteredAtlasPopulation()
{
   Atlas atlas;

   std::vector<Atlas>::const_iterator it;

   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      atlas=defineRegisteredAtlas(*it);
      m_atlasPopulationRegistered.push_back(atlas);
   }   
}


// Checking results // 

bool AtlasRegistration::checkRegisteredAtlas(Atlas atlas)
{
   QFile* T1Reg_file = new QFile (atlas.T1);
   QFile* T2Reg_file = new QFile (atlas.T2);

   QFile* segReg_file = new QFile (atlas.seg);

   QFile* whiteReg_file = new QFile (atlas.white);
   QFile* grayReg_file = new QFile (atlas.gray);
   QFile* csfReg_file = new QFile (atlas.csf);

   if (!atlas.probabilistic && T1Reg_file->exists() && T2Reg_file->exists() && segReg_file->exists())
   {
      return true;
   } 

   if (atlas.probabilistic && T1Reg_file->exists() && T2Reg_file->exists() && whiteReg_file->exists() && grayReg_file->exists() && csfReg_file->exists())
   {
      return true;
   } 

   return false;
}

bool AtlasRegistration::checkRegisteredAtlasPopulation()
{
   std::vector<Atlas>::const_iterator it;

   for (it = m_atlasPopulationRegistered.begin(); it != m_atlasPopulationRegistered.end(); ++it)
   {  
      if(!checkRegisteredAtlas(*it))
      {
         return false;
      } 
   }
   return true;
}


// Creating Directories //
void AtlasRegistration::createAtlasDirectories()
{
   std::vector<Atlas>::iterator it;

   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      if (!m_module_dir->exists((*it).name))
      { 
         m_module_dir->mkdir((*it).name);
      }
   }
}

void AtlasRegistration::initializeScript(QString &script)
{
   script += "#!/usr/bin/env python\n";

   // Import the librairies needed 
   script += "import os\n"; 
   script += "import sys\n";
   script += "import signal\n";
   script += "import subprocess\n";
   script += "import logging\n";   
   script += "import time\n";
   script += "import array\n";
   script += "from collections import namedtuple\n\n";

   script += "ANTS = '" + m_executables->getExecutablePath("ANTS") + "'\n";
   script += "ResampleVolume2 = '" + m_executables->getExecutablePath("ResampleVolume2") + "'\n\n";

   script += "runningProcess = None\n\n";  
}

void AtlasRegistration::implementExecute(QString &script)
{
   script += "def execute(args, logFile):\n";
   script += "\tlogFile.write(subprocess.list2cmdline(args))\n";
   script += "\trunningProcess = subprocess.Popen(args,stdout=logFile,stderr=logFile)\n";
   script += "\tlogging.info(runningProcess)\n";
   script += "\trunningProcess.wait()\n\n";
}

void AtlasRegistration::implementRegisterAtlas(QString &script, bool probabilistic)
{   
   if(probabilistic)
   {
      script += "def main(T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log):\n";
   } 
   else
   {
      script += "def main(T1Atlas, T2Atlas, segAtlas, outbase, log):\n";
   }

   script += "\tsignal.signal(signal.SIGINT, stop)\n";
   script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   script += "\t# Inputs # \n";
   script += "\tT1 = '" + m_neo.T1 + "'\n";
   script += "\tT2 = '" + m_neo.T2 + "'\n\n "; 

   script += "\t# Transformations # \n";
   script += "\toutput = outbase + '_.nii.gz'\n";
   script += "\taffine = outbase + '_Affine.txt'\n";
   script += "\twarp = outbase + '_Warp.nii.gz'\n\n"; 

   script += "\t# Outputs # \n";
   script += "\tT1Reg = outbase + '-T1.nrrd'\n";
   script += "\tT2Reg = outbase + '-T2.nrrd'\n";

   if(probabilistic)
   {
      script += "\twhiteReg = outbase + '-white.nrrd'\n";
      script += "\tgrayReg = outbase + '-gray.nrrd'\n";
      script += "\tcsfReg = outbase + '-csf.nrrd'\n\n";
   }
   
   else
   {
      script += "\tsegReg = outbase + '-seg.nrrd'\n\n";
   }

   QString log = variable("log");
   script += "\t# Log File #\n";
   script += "\tlogFile=open(log, \"w\") \n\n";

   script += "\t# Calculate transformations # \n";
   QString modality1 = "'CC[' + T1 + ',' + T1Atlas + '," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]'";
   QString modality2 = "'CC[' + T2 + ',' + T2Atlas + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]'";
   QString iterations = QString::number(m_parameters->getIterationsJ()) + "x" + QString::number(m_parameters->getIterationsK()) + "x" + QString::number(m_parameters->getIterationsL());
   QString transformation = m_parameters->getTransformationType() + "[" + QString::number(m_parameters->getGradientStepLength()) + "]"; //"," + QString::number(m_parameters->getNumberOfTimeSteps()) + "," + QString::number(m_parameters->getDeltaTime()) + "]";
   QString regularization = m_parameters->getRegularizationType() + "[" + QString::number(m_parameters->getGradientFieldSigma()) + "," + QString::number(m_parameters->getDeformationFieldSigma()) + "]"; //"," + QString::number(m_parameters->getTruncation()) + "]";

   script += "\tif checkFileExistence(affine)==False or checkFileExistence(warp)==False: \n";
   script += "\t\tmodality1 = " + modality1 + "\n";
   script += "\t\tmodality2 = " + modality2 + "\n";
   script += "\t\titerations = '" + iterations + "'\n";
   script += "\t\ttransformation = '" + transformation + "'\n";
   script += "\t\tregularization = '" + regularization + "'\n";

   QStringList argumentsList; 
   argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization"; 
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args, logFile)\n\n"; 

   argumentsList.clear();
   argumentsList << "'text_subst.pl'" << "'MatrixOffsetTransformBase_double_3_3'" << "'AffineTransform_double_3_3'" << "affine"; 
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args, logFile)\n\n"; 

   script += "\t# Apply transformations # \n";

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "T1Atlas" << "T1Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   script += "\tif checkFileExistence(T1Reg)==False:\n";
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args, logFile)\n\n"; 

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "T2Atlas" << "T2Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   script += "\tif checkFileExistence(T2Reg)==False:\n";
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args, logFile)\n\n"; 

   if(probabilistic)
   {
      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "whiteAtlas" << "whiteReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "\tif checkFileExistence(whiteReg)==False:\n";
      script += "\t\targs = " + listToString(argumentsList) + "\n";
      script += "\t\texecute(args, logFile)\n\n"; 

      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "grayAtlas" << "grayReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "\tif checkFileExistence(grayReg)==False:\n";
      script += "\t\targs = " + listToString(argumentsList) + "\n";
      script += "\t\texecute(args, logFile)\n\n"; 

      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "csfAtlas" << "csfReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "\tif checkFileExistence(csfReg)==False:\n";
      script += "\t\targs = " + listToString(argumentsList) + "\n";
      script += "\t\texecute(args, logFile)\n\n"; 
   }
   
   else
   {
      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "segAtlas" << "segReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "\tif checkFileExistence(segReg)==False:\n";
      script += "\t\targs = " + listToString(argumentsList) + "\n";
      script += "\t\texecute(args, logFile)\n\n"; 
   }
}

void AtlasRegistration::writeRegisterAtlas() // args = (T1Atlas, T2Atlas, output, log)
{
   QString registerAtlas;

   initializeScript(registerAtlas);

   implementStop(registerAtlas);
   implementCheckFileExistence(registerAtlas);
   implementExecute(registerAtlas);
   implementRegisterAtlas(registerAtlas, 0);

   registerAtlas += "if __name__ == '__main__':\n";
   registerAtlas += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])\n";

   QString script_path = m_processing_dir->filePath("registerAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerAtlas.toStdString() << std::endl; 
}

void AtlasRegistration::writeRegisterProbabilisticAtlas() // args = (T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log)
{
   QString registerProbabilisticAtlas;

   initializeScript(registerProbabilisticAtlas);

   implementStop(registerProbabilisticAtlas);
   implementCheckFileExistence(registerProbabilisticAtlas);
   implementExecute(registerProbabilisticAtlas);
   implementRegisterAtlas(registerProbabilisticAtlas, 1);

   registerProbabilisticAtlas += "if __name__ == '__main__':\n";
   registerProbabilisticAtlas += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7])\n";

   QString script_path = m_processing_dir->filePath("registerProbabilisticAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerProbabilisticAtlas.toStdString() << std::endl; 
}

void AtlasRegistration::defineRegisterAtlasParameters(QString &script, Atlas atlas)
{
   script += "\t# Registration " + atlas.name + " to " + m_neo.prefix + "\n"; 

   script += "\tT1Atlas = '" + atlas.T1 + "'\n";
   script += "\tT2Atlas = '" + atlas.T2 + "'\n";

   if(atlas.probabilistic)
   {
      script += "\twhiteAtlas = '" + atlas.white + "'\n";
      script += "\tgrayAtlas = '" + atlas.gray + "'\n";
      script += "\tcsfAtlas = '" + atlas.csf + "'\n";
   }
   
   else
   {
      script += "\tsegAtlas = '" + atlas.seg + "'\n";
   }

   QString atlas_path = m_module_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path);

   QString output_name = atlas.name + "_to_" + m_neo.prefix;     
   QString output = atlas_dir->filePath(output_name);
   script += "\toutput = '" + output + "'\n";

   QString log_name = atlas.name + "_to_" + m_neo.prefix + ".log";     
   QString log = atlas_dir->filePath(log_name);
   script += "\tlog = '" + log + "'\n\n";
}

void AtlasRegistration::submitRegisterAtlasJob(QString &script)
{
   /*QString bsub = "bsub -n 1 " ;

   QString executable = "python " + variable("atlasRegistration_script") + " " + variable("T1Atlas") + " " + variable("T2Atlas") + " " + variable("segAtlas") + " " + variable("output") + " " + variable("log");

   QString command = bsub + " " + executable; 
   //script += "\tos.system(\"" + command + "\") \n\n";*/
}

void AtlasRegistration::executeRegisterAtlasProcess(QString &script, Atlas atlas, int i) 
{
   QString QString_i; 
   QString_i.setNum(i);

   script += "\twhile len(runningRegistrations) >= nbRegMax : \n";
   script += "\t\tcheckrunningRegistrations(runningRegistrations)\n";
   script += "\t\ttime.sleep(10)\n";

   QString process_name = "process_" + QString_i;
   QString registration_name = "registration_" + QString_i;

   QString command;

   if(atlas.probabilistic)
   {
      command = "['python', probabilisticAtlasRegistration_script, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log]";
   }
   
   else
   {
      command = "['python', atlasRegistration_script, T1Atlas, T2Atlas, segAtlas, output, log]";
   }

   script += "\tlogging.info('Registering " + atlas.name + "...')\n";
   script += "\t" + process_name + " = subprocess.Popen(" + command + ")\n";
   script += "\t" + registration_name + " = registration('" + atlas.name + "', " + process_name + ")\n";
   script += "\trunningRegistrations.append(" + registration_name + ")\n\n";
}


void AtlasRegistration::implementRegisterAtlasPopulation(QString &script)
{
   int i;
   script += "def signal_handler(signal, frame):\n";
   script += "\tprint 'Signal received'\n";
   script += "\tfor process in runningRegistrations:\n";
   script += "\t\tprint process.pid\n";
   script += "\t\tprocess.terminate()\n";
   script += "\tsys.exit(0)\n\n";

   script += "def checkrunningRegistrations(runningRegistrations):\n";
   script += "\tfor registration in runningRegistrations :\n";
   script += "\t\tif (registration.process).poll()==0:\n";
   script += "\t\t\tlogging.info('Registering %s done' %registration.name)\n";
   script += "\t\t\trunningRegistrations.remove(registration)\n\n";

   script += "import registerAtlas\n";
   script += "import registerProbabilisticAtlas\n";

   script += "signal.signal(signal.SIGINT, signal_handler)\n";
   script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";

   script += "def run():\n";

   script += "\tlogging.info('=== Atlas Registration ===')\n";
   script += "\tlogging.debug('')\n";

   QString atlasRegistration_script = m_processing_dir->filePath("registerAtlas.py");
   script += "\tatlasRegistration_script = '" + atlasRegistration_script + "'\n";

   QString probabilisticAtlasRegistration_script = m_processing_dir->filePath("registerProbabilisticAtlas.py");
   script += "\tprobabilisticAtlasRegistration_script = '" + probabilisticAtlasRegistration_script + "'\n\n";

   script += "\tos.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '1' \n";
   script += "\tcheck = int(os.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS']) \n\n";

   script += "\tnbRegMax = " + QString::number(m_nbCores) +  "\n";
   script += "\trunningRegistrations = []\n\n";

   script += "\tregistration = namedtuple('registration', 'name process')\n\n";

   std::vector<Atlas>::iterator it;
   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      i = std::distance(m_atlasPopulation.begin(), it);

      Atlas atlas= *(it);

      if (!checkRegisteredAtlas(m_atlasPopulationRegistered[i]))
      { 
         defineRegisterAtlasParameters(script, atlas);

         if (m_computingSystem == "Killdevil")
         {
            submitRegisterAtlasJob(script);
         }

         if (m_computingSystem == "Local")
         {
            i = std::distance(m_atlasPopulation.begin(), it);
            executeRegisterAtlasProcess(script, *it, i);
         }
      }
   }
   script += "\texit_codes = [(registration.process).wait() for registration in runningRegistrations]\n";
}

void AtlasRegistration::writeRegisterAtlasPopulation()
{
   QString registerAtlasPopulation;

   initializeScript(registerAtlasPopulation);

   implementRegisterAtlasPopulation(registerAtlasPopulation);

   writeScript(registerAtlasPopulation);
}

void AtlasRegistration::update()
{     
   createAtlasDirectories();
   writeRegisterAtlas();
   writeRegisterProbabilisticAtlas();
   writeRegisterAtlasPopulation();
}

std::vector< Atlas > AtlasRegistration::getOutput() { return m_atlasPopulationRegistered; }

