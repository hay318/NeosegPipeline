#include "PreProcessingData.h"

PreProcessingData::PreProcessingData ()
{
   m_skullStripping_suffix = "-stripped";
   m_correcting_suffix = "-corrected";
   m_closing_suffix = "-closed";
}

void PreProcessingData::setNeo(Neo neo) {m_neo=neo;}

void PreProcessingData::setSkullStripping(bool skullStripping) {m_skullStripping=skullStripping;}

void PreProcessingData::setOutputPath(QString output_path) 
{
   m_output_dir = new QDir(output_path);

   QString preProcessingData_path = m_output_dir->filePath("PreProcessingData");
   m_preProcessingData_dir = new QDir(preProcessingData_path); 
}

void PreProcessingData::setExecutablePaths(ExecutablePaths* executables) {m_executables=executables;}

void PreProcessingData::definePreProcessedData() 
{
   QFileInfo* T1_file = new QFileInfo(m_neo.T1);
   QString preProcessedT1_name = T1_file->baseName() + m_skullStripping_suffix + m_correcting_suffix + ".nrrd";
   m_preProcessedNeo.T1 = m_preProcessingData_dir->filePath(preProcessedT1_name); 

   QFileInfo* T2_file = new QFileInfo(m_neo.T2);
   QString preProcessedT2_name = T2_file->baseName() + m_skullStripping_suffix + m_correcting_suffix + ".nrrd";
   m_preProcessedNeo.T2 = m_preProcessingData_dir->filePath(preProcessedT2_name); 

   QFileInfo* mask_file = new QFileInfo(m_neo.mask);
   QString preProcessedMask_name = mask_file->baseName() + m_closing_suffix + ".nrrd";
   m_preProcessedNeo.mask = m_preProcessingData_dir->filePath(preProcessedMask_name); 

}

bool PreProcessingData::checkPreProcessedData()
{
   QFile* T1PreProcessed_file = new QFile(m_preProcessedNeo.T1);
   QFile* T2PreProcessed_file = new QFile(m_preProcessedNeo.T2);
   QFile* maskPreProcessed_file = new QFile(m_preProcessedNeo.mask);

   if(T1PreProcessed_file->exists() && T2PreProcessed_file->exists() && maskPreProcessed_file->exists())
   { 
      return true;
   }

   return false;
}

void PreProcessingData::createDirectory()
{
   if (!m_output_dir->exists("PreProcessingData"))
   {
     m_output_dir->mkdir("PreProcessingData");
   }
}


QString PreProcessingData::variable(QString variable_name) {return "\" + " + variable_name + " + \"";}

QString PreProcessingData::str(QString str) {return "\"" + str + "\"";}

QString PreProcessingData::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

   return arguments; 
}

void PreProcessingData::initializeScript(QString &script)
{
   // Define the version of Python used
   script += "#!/usr/bin/env python\n";

   // Import the librairies needed  
   script += "import os\n";
   script += "import sys\n";
   script += "import logging \n";
   script += "import subprocess\n\n";

   script += "ImageMath = " + str(m_executables->getExecutablePath("ImageMath")) + "\n";
   script += "SegPostProcessCLP = " + str(m_executables->getExecutablePath("SegPostProcessCLP")) + "\n";
   script += "N4ITKBiasFieldCorrection = " + str(m_executables->getExecutablePath("N4ITKBiasFieldCorrection")) + "\n";

   script += "T1 = " + str(m_neo.T1) + "\n"; 
   script += "T2 = " + str(m_neo.T2) + "\n"; 
   script += "mask = " + str(m_neo.mask) + "\n\n";
}

void PreProcessingData::implementLogStdoutAndStderr(QString &script)
{
   script += "def logStdoutAndStderr(runningProcess):\n";

   script += "\twhile True:\n";
   script += "\t\tline = runningProcess.stdout.readline()\n";
   script += "\t\tif line:\n";
   script += "\t\t\tlogging.info(line.rstrip())\n";
   script += "\t\tif not line: break\n";

   script += "\twhile True:\n";
   script += "\t\tline = runningProcess.stderr.readline()\n";
   script += "\t\tif line:\n";
   script += "\t\t\tlogging.error(line.rstrip())\n";
   script += "\t\tif not line: break\n";

   script += "\trunningProcess.wait()\n\n";
}

void PreProcessingData::implementLogStderr(QString &script)
{
   script += "def logStderr(runningProcess):\n";

   script += "\twhile True:\n";
   script += "\t\tline = runningProcess.stderr.readline()\n";
   script += "\t\tif line:\n";
   script += "\t\t\tlogging.error(line.rstrip())\n";
   script += "\t\tif not line: break\n";

   script += "\trunningProcess.wait()\n\n";
}

void PreProcessingData::closeMask(QString &script)
{
   QStringList argumentsList;

   script += "\tmask_closed = " + str(m_preProcessedNeo.mask) + "\n";    

   argumentsList << "ImageMath" << "mask" << "'-dilate'"<< "'5,1'" << "'-outfile'" << "mask_closed"; 
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   script += "\tlogStderr(runningProcess)\n\n";   

   argumentsList.clear();
   argumentsList << "ImageMath" << "mask_closed" << "'-erode'"<< "'5,1'" << "'-outfile'" << "mask_closed"; 
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   script += "\tlogStderr(runningProcess)\n\n"; 
}

void PreProcessingData::skullStripImages(QString &script)
{
   QStringList argumentsList;

   QFileInfo* T1_file = new QFileInfo(m_neo.T1);
   QString strippedT1_name = T1_file->baseName() + m_skullStripping_suffix;
   QString strippedT1_path = m_preProcessingData_dir->filePath(strippedT1_name); 

   script += "\tT1_stripped = " + str(strippedT1_path) + ".nrrd\n";    

   argumentsList << "SegPostProcessCLP" << "mask" << "T1_stripped"<< "'--skullstripping'" << "T1" << "'--MWM'" << "'1'" << "'--WM'" << "'2'" << "'--GM'" << "'3'" << "'--CSF'" << "'4'"; 
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";


   QFileInfo* T2_file = new QFileInfo(m_neo.T2);
   QString strippedT2_name = T2_file->baseName() + m_skullStripping_suffix;
   QString strippedT2_path = m_preProcessingData_dir->filePath(strippedT2_name); 

   script += "\tT2_stripped = " + str(strippedT2_path) + ".nrrd\n";    

   argumentsList.clear();
   argumentsList << "SegPostProcessCLP" << "mask" << "T2_stripped"<< "'--skullstripping'" << "T2" << "'--MWM'" << "'1'" << "'--WM'" << "'2'" << "'--GM'" << "'3'" << "'--CSF'" << "'4'"; 
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";   
}


void PreProcessingData::correctImages(QString &script)
{
   QStringList argumentsList;

   QString T1, T2; 

   if(m_skullStripping)
   {
      T1 = "T1";
      T2 = "T2";
   }

   else
   {
      T1 = "T1_stripped";
      T2 = "T2_stripped";
   }

   script += "\tT1_corrected = " + str(m_preProcessedNeo.T1) + "\n"; 

   argumentsList << "N4ITKBiasFieldCorrection" << "'--inputimage'" << T1 << "'--maskimage'" << "mask" << "'--outputimage'" << "T1_corrected";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";   

   script += "\tT2_corrected = " + str(m_preProcessedNeo.T2) + "\n"; 

   argumentsList.clear();
   argumentsList << "N4ITKBiasFieldCorrection" << "'--inputimage'" << T2 << "'--maskimage'" << "mask" << "'--outputimage'" << "T2_corrected";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";   
}

void PreProcessingData::implementPreProcessData(QString &script)
{
   script += "def run():\n\n";  

   closeMask(script); 

   if(!m_skullStripping)
   {
      skullStripImages(script);
   }

   correctImages(script);
}

void PreProcessingData::writePreProcessData()
{
   QString preProcessData;

   initializeScript(preProcessData);
   implementLogStdoutAndStderr(preProcessData); 
   implementLogStderr(preProcessData); 

   implementPreProcessData(preProcessData);

   QString script_path = m_output_dir->filePath("preProcessData.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << preProcessData.toStdString() << std::endl;
   script_stream->close();
}

void PreProcessingData::update()
{
   createDirectory(); 
   writePreProcessData();
}

Neo PreProcessingData::getOutput() {return m_preProcessedNeo;}

/*void PreProcessingData::centerImage()
{
   m_T1centered = m_script_dir + "/T1centered.gipl.gz"; 
   command = ImageMath + " " + m_T1 + " -changeOrig 0,0,0 -outfile " + m_T1centered;
   *m_script << "os.system(\"" + command + "\")" << std::endl;

   m_T2centered = m_script_dir + "/T2centered.gipl.gz";    
   command = ImageMath + " " + m_T2 + " -changeOrig 0,0,0 -outfile " + m_T2centered;
   *m_script << "os.system(\"" + command + "\") \n" << std::endl;
}*/

