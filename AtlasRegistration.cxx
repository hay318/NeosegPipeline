#include "AtlasRegistration.h"

// SET // 

void AtlasRegistration::setNeo(Neo neo) {m_neo=neo;}

void AtlasRegistration::setAtlasPopulation(std::vector<Atlas> atlasPopulation) {m_atlasPopulation=atlasPopulation;}

void AtlasRegistration::setOutputPath(QString outputPath) 
{
   m_output_dir = new QDir(outputPath); 

   QString atlasRegistration_path = m_output_dir->filePath("AtlasRegistration");
   m_atlasRegistration_dir = new QDir(atlasRegistration_path);
}

void AtlasRegistration::setComputingSystem(QString computingSystem) {m_computingSystem=computingSystem;}

void AtlasRegistration::setNumberOfCores(int nbCores){m_nbCores=nbCores;} 

void AtlasRegistration::setAntsParameters(AntsParameters* parameters) {m_parameters=parameters;}

void AtlasRegistration::setExecutablePaths(ExecutablePaths* executables) {m_executables=executables;}

// Define Output //  

Atlas AtlasRegistration::defineRegisteredAtlas(Atlas atlas)
{
   Atlas atlasReg = atlas;

   QString atlas_path = m_atlasRegistration_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path); 

   QString T1Reg_name = atlas.name + "_to_neo-T1.nrrd";
   atlasReg.T1 = atlas_dir->filePath(T1Reg_name); 

   QString T2Reg_name = atlas.name + "_to_neo-T2.nrrd";
   atlasReg.T2 = atlas_dir->filePath(T2Reg_name);

   if(atlas.probabilistic)
   {
      QString whiteReg_name = atlas.name + "_to_neo-white.nrrd";
      atlasReg.white = atlas_dir->filePath(whiteReg_name);

      QString grayReg_name = atlas.name + "_to_neo-gray.nrrd";
      atlasReg.gray = atlas_dir->filePath(grayReg_name);

      QString csfReg_name = atlas.name + "_to_neo-csf.nrrd";
      atlasReg.csf = atlas_dir->filePath(csfReg_name);
   }

   else
   {
      QString segReg_name = atlas.name + "_to_neo-seg.nrrd";
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

void AtlasRegistration::createDirectory()
{
   if (!m_output_dir->exists("AtlasRegistration"))
   {
      m_output_dir->mkdir("AtlasRegistration");
   }
}

void AtlasRegistration::createAtlasDirectories()
{
   std::vector<Atlas>::iterator it;

   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      if (!m_atlasRegistration_dir->exists((*it).name))
      { 
         m_atlasRegistration_dir->mkdir((*it).name);
      }
   }
}


// Writing Scripts //

QString AtlasRegistration::variable(QString variable_name) {return "\" + " + variable_name + " + \"";}

QString AtlasRegistration::str(QString str) {return "\"" + str + "\"";}

QString AtlasRegistration::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

      return arguments; 
}


void AtlasRegistration::initializeScript(QString &script)
{
   script += "#!/usr/bin/env python\n";

   // Import the librairies needed  
   script += "import os \n";
   script += "import sys \n";
   script += "import signal \n";
   script += "import subprocess \n";
   script += "import time \n";
   script += "import array \n";

   script += "ANTS = " + str(m_executables->getExecutablePath("ANTS")) + "\n";
   script += "ResampleVolume2 = " + str(m_executables->getExecutablePath("ResampleVolume2")) + "\n\n";
}


void AtlasRegistration::implementRegisterAtlas(QString &script, bool probabilistic)
{
   script += "def signal_handler(signal, frame):\n";
   script += "\tprint '*************** Signal received! ******************'\n";
   script += "\tprint runningProcess\n";
   script += "\tprint runningProcess.pid\n";
   script += "\trunningProcess.terminate()\n";
   script += "\tsys.exit(0)\n\n";

   script += "def checkFileExistence(fileName):\n";
   script += "\ttry:\n";
   script += "\t\twith open(fileName):\n";
   script += "\t\t\treturn True\n";
   script += "\texcept IOError:\n";
   script += "\t\treturn False\n\n";
   
   script += "signal.signal(signal.SIGINT, signal_handler)\n";
   script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";
   
   script += "# Arguments # \n";
   script += "T1Atlas = sys.argv[1]\n";
   script += "T2Atlas = sys.argv[2]\n";

   if(probabilistic)
   {
      script += "whiteAtlas = sys.argv[3]\n";
      script += "grayAtlas = sys.argv[4]\n";
      script += "csfAtlas = sys.argv[5]\n";
      script += "outbase = sys.argv[6]\n";
      script += "log = sys.argv[7]\n\n"; 
   } 

   else
   {
      script += "segAtlas = sys.argv[3]\n";
      script += "outbase = sys.argv[4]\n";
      script += "log = sys.argv[5]\n\n"; 
   }

   script += "# Inputs # \n";
   script += "T1 = " + str(m_neo.T1) + "\n";
   script += "T2 = " + str(m_neo.T2) + "\n\n "; 

   script += "# Transformations # \n";
   script += "output = outbase + " + str("_.nii.gz") + "\n";
   script += "affine = outbase + " + str("_Affine.txt") + "\n";
   script += "warp = outbase + " + str("_Warp.nii.gz") + "\n\n"; 

   script += "# Outputs # \n";
   script += "T1Reg = outbase + " + str("-T1.nrrd") + "\n";
   script += "T2Reg = outbase + " + str("-T2.nrrd") + "\n";

   if(probabilistic)
   {
      script += "whiteReg = outbase + " + str("-white.nrrd") + "\n";
      script += "grayReg = outbase + " + str("-gray.nrrd") + "\n";
      script += "csfReg = outbase + " + str("-csf.nrrd") + "\n\n";
   }
   
   else
   {
      script += "segReg = outbase + " + str("-seg.nrrd") + "\n\n";
   }

   QString log = variable("log");
   script += "# Log File # \n";
   script += "logFile=open(log, \"w\") \n\n";

   script += "# Calculate transformations # \n";
   QString modality1 = "CC[" + variable("T1") + "," + variable("T1Atlas") + "," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]";
   QString modality2 = "CC[" + variable("T2") + "," + variable("T2Atlas") + "," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
   QString iterations = QString::number(m_parameters->getIterationsJ()) + "x" + QString::number(m_parameters->getIterationsK()) + "x" + QString::number(m_parameters->getIterationsL());
   QString transformation = m_parameters->getTransformationType() + "[" + QString::number(m_parameters->getGradientStepLength()) + "]"; //"," + QString::number(m_parameters->getNumberOfTimeSteps()) + "," + QString::number(m_parameters->getDeltaTime()) + "]";
   QString regularization = m_parameters->getRegularizationType() + "[" + QString::number(m_parameters->getGradientFieldSigma()) + "," + QString::number(m_parameters->getDeformationFieldSigma()) + "]"; //"," + QString::number(m_parameters->getTruncation()) + "]";

   script += "if checkFileExistence(affine)==False or checkFileExistence(warp)==False: \n";
   script += "\tmodality1 = " + str(modality1) + "\n";
   script += "\tmodality2 = " + str(modality2) + "\n";
   script += "\titerations = " + str(iterations) + "\n";
   script += "\ttransformation = " + str(transformation) + "\n";
   script += "\tregularization = " + str(regularization) + "\n";

   QStringList argumentsList; 
   argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization"; 
   script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile) \n\n";

   script += "runningProcess = subprocess.call(['text_subst.pl', 'MatrixOffsetTransformBase_double_3_3', 'AffineTransform_double_3_3', affine], stdout=logFile, stderr=logFile)\n\n";

   script += "# Apply transformations # \n";

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "T1Atlas" << "T1Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   script += "if checkFileExistence(T1Reg)==False:\n";
   script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile)\n\n";

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "T2Atlas" << "T2Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   script += "if checkFileExistence(T2Reg)==False:\n";
   script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile)\n\n";

   if(probabilistic)
   {
      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "whiteAtlas" << "whiteReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "if checkFileExistence(whiteReg)==False:\n";
      script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile)\n\n";

      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "grayAtlas" << "grayReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "if checkFileExistence(grayReg)==False:\n";
      script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile)\n\n";

      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "csfAtlas" << "csfReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "if checkFileExistence(csfReg)==False:\n";
      script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile)\n\n";
   }
   
   else
   {
      argumentsList.clear();
      argumentsList << "ResampleVolume2" << "segAtlas" << "segReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      script += "if checkFileExistence(segReg)==False:\n";
      script += "\trunningProcess = subprocess.call([" + listToString(argumentsList) + "], stdout=logFile, stderr=logFile)\n\n";
   }
}

void AtlasRegistration::writeRegisterAtlas() // args = (T1Atlas, T2Atlas, output, log)
{
   QString registerAtlas;

   initializeScript(registerAtlas);

   implementRegisterAtlas(registerAtlas, 0);

   QString script_path = m_atlasRegistration_dir -> filePath("RegisterAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerAtlas.toStdString() << std::endl; 
}

void AtlasRegistration::writeRegisterProbabilisticAtlas() // args = (T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log)
{
   QString registerProbabilisticAtlas;

   initializeScript(registerProbabilisticAtlas);

   implementRegisterAtlas(registerProbabilisticAtlas, 1);

   QString script_path = m_atlasRegistration_dir -> filePath("RegisterProbabilisticAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerProbabilisticAtlas.toStdString() << std::endl; 
}

void AtlasRegistration::defineRegisterAtlasParameters(QString &script, Atlas atlas)
{
   script += "\t# Registration " + atlas.name + " to neo \n"; 

   script += "\tT1Atlas = " + str(atlas.T1) + "\n";
   script += "\tT2Atlas = " + str(atlas.T2) + "\n";

   if(atlas.probabilistic)
   {
      script += "\twhiteAtlas = " + str(atlas.white) + "\n";
      script += "\tgrayAtlas = " + str(atlas.gray) + "\n";
      script += "\tcsfAtlas = " + str(atlas.csf) + "\n";
   }
   
   else
   {
      script += "\tsegAtlas = " + str(atlas.seg) + "\n";
   }

   QString atlas_path = m_atlasRegistration_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path);

   QString output_name = atlas.name + "_to_neo";     
   QString output = atlas_dir->filePath(output_name);
   script += "\toutput = " + str(output) + "\n";

   QString log_name = atlas.name + "_to_neo.log";     
   QString log = atlas_dir->filePath(log_name);
   script += "\tlog = " + str(log) + "\n\n";
}

void AtlasRegistration::submitRegisterAtlasJob(QString &script)
{
   /*QString bsub = "bsub -n 1 " ;

   QString executable = "python " + variable("atlasRegistration_script") + " " + variable("T1Atlas") + " " + variable("T2Atlas") + " " + variable("segAtlas") + " " + variable("output") + " " + variable("log");

   QString command = bsub + " " + executable; 
   //script += "\tos.system(\"" + command + "\") \n\n";*/
}

void AtlasRegistration::executeRegisterAtlasProcess(QString &script, int i, bool probabilistic) 
{
   QString QString_i; 
   QString_i.setNum(i);

   script += "\twhile len(runningProcesses) > nbRegMax : \n";
   script += "\t\tcheckRunningProcesses(runningProcesses)\n";
   script += "\t\ttime.sleep(10)\n";

   QString process_name = "process_" + QString_i;

   QString command;

   if(probabilistic)
   {
      command = "['python', probabilisticAtlasRegistration_script, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log]";
   }
   
   else
   {
      command = "['python', atlasRegistration_script, T1Atlas, T2Atlas, segAtlas, output, log]";
   }

   script += process_name + " = subprocess.Popen(" + command + ")\n";
   script += "\trunningProcesses.append(" + process_name + ")\n\n";
}


void AtlasRegistration::implementRegisterAtlasPopulation(QString &script)
{
   int i;

   script += "def signal_handler(signal, frame):\n";
   script += "\tprint 'Signal received'\n";
   script += "\tfor process in runningProcesses:\n";
   script += "\t\tprint process.pid\n";
   script += "\t\tprocess.terminate()\n";
   script += "\tsys.exit(0)\n\n";

   script += "def checkRunningProcesses(runningProcesses):\n";
   script += "\tfor process in runningProcesses :\n";
   script += "\t\tif process.poll()==0:\n";
   script += "\t\t\trunningProcesses.remove(process)\n\n";



   script += "signal.signal(signal.SIGINT, signal_handler)\n";
   script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";

   script += "def run():\n";

   script += "\t# Submit atlas registration jobs \n\n";

   QString atlasRegistration_script = m_atlasRegistration_dir->filePath("RegisterAtlas.py");
   script += "\tatlasRegistration_script = " + str(atlasRegistration_script) + "\n";

   QString probabilisticAtlasRegistration_script = m_atlasRegistration_dir->filePath("RegisterProbabilisticAtlas.py");
   script += "\tprobabilisticAtlasRegistration_script = " + str(probabilisticAtlasRegistration_script) + "\n\n";

   script += "\tos.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '1' \n";
   script += "\tcheck = int(os.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS']) \n\n";

   script += "\tnbRegMax = " + QString::number(m_nbCores) +  "\n";
   script += "\trunningProcesses = []\n\n";


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
            executeRegisterAtlasProcess(script,i,(*it).probabilistic);
         }
      }
   }

   script += "\texit_codes = [process.wait() for process in runningProcesses]\n";
}

void AtlasRegistration::writeRegisterAtlasPopulation()
{
   QString registerAtlasPopulation;

   initializeScript(registerAtlasPopulation);

   implementRegisterAtlasPopulation(registerAtlasPopulation);

   QString script_path = m_output_dir->filePath("RegisterAtlasPopulation.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerAtlasPopulation.toStdString() << std::endl;   

   script_stream->close();
}

void AtlasRegistration::update()
{
   createDirectory();       
   createAtlasDirectories();
   writeRegisterAtlas();
   writeRegisterProbabilisticAtlas();
   writeRegisterAtlasPopulation();
}

std::vector< Atlas > AtlasRegistration::getOutput() { return m_atlasPopulationRegistered; }

