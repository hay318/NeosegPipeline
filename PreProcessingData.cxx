#include "PreProcessingData.h"


// Executable Paths
QString ImageMath="/tools/bin_linux/ImageMath";
QString SegPostProcessCLP="/tools/bin_linux64/SegPostProcessCLP";


PreProcessingData::PreProcessingData (QString output_path) : m_output_path(output_path) 
{
   m_preProcessingData_dir = new QDir (output_path + "/PreProcessingData"); 
   m_skullStripping_suffix = "-stripped.gipl";
}


QString PreProcessingData::getOutputPath(QString input_path, QString suffix)
{
   QFileInfo* input_file = new QFileInfo(input_path);
   QString output_name = input_file->baseName() + suffix;
   QString output_path = m_preProcessingData_dir->filePath(output_name); 

   return output_path;
} 


bool PreProcessingData::checkExistingResults(Neo neo)
{
   QString T1Stripped_path = getOutputPath (neo.T1_path, m_skullStripping_suffix);
   QString T2Stripped_path = getOutputPath (neo.T1_path, m_skullStripping_suffix);

   if(QFile(T1Stripped_path).exists() && QFile(T2Stripped_path).exists())
   { 
      std::cout<<T1Stripped_path.toStdString()<<" already exists"<<std::endl;
      std::cout<<T2Stripped_path.toStdString()<<" already exists"<<std::endl;
      return true;
   }

   else 
   {
      std::cout<<T1Stripped_path.toStdString()<<" do not exist"<<std::endl;
      std::cout<<T2Stripped_path.toStdString()<<" do not exist"<<std::endl;
      return false;
   }
}


void PreProcessingData::createDirectory()
{
   if (!m_preProcessingData_dir->exists())
   {
     QDir* output_dir = new QDir(m_output_path); 
     output_dir->mkdir("PreProcessingData");
   }
}


void PreProcessingData::createScript()
{
   QString script_path = m_preProcessingData_dir -> filePath("preProcessingData.py");

   // Create a new empty script (overwrite if it already exists) 
   m_script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);

   // Define the version of Python used
   m_script += "#!/usr/bin/python3.3 \n\n";

   // Import the librairies needed  
   m_script += "from subprocess import call \n";
   m_script += "import os \n";
   m_script += "import sys \n\n";
}

QString PreProcessingData::runNeoseg(QString image)
{}


QString PreProcessingData::skullStripImage(QString image_path, QString mask_path)
{
   QString skullStrippedImage_path = getOutputPath (image_path, m_skullStripping_suffix);

   QString command = SegPostProcessCLP + " " + mask_path + " " + skullStrippedImage_path + " --skullstripping " + image_path + " --MWM 1 --WM 2 --GM 3 --CSF 4";
   m_script += "os.system(\"" + command + "\") \n\n";   

   return skullStrippedImage_path;
}



Neo PreProcessingData::skullStripNeo(Neo neo, QString mask_path)
{
   neo.T1_path = skullStripImage(neo.T1_path, mask_path);
   neo.T2_path = skullStripImage(neo.T2_path, mask_path);

   return neo;
}


/*void PreProcessingData::centerImage()
{
   m_T1centered = m_script_dir + "/T1centered.gipl.gz"; 
   command = ImageMath + " " + m_T1 + " -changeOrig 0,0,0 -outfile " + m_T1centered;
   *m_script << "os.system(\"" + command + "\")" << std::endl;

   m_T2centered = m_script_dir + "/T2centered.gipl.gz";    
   command = ImageMath + " " + m_T2 + " -changeOrig 0,0,0 -outfile " + m_T2centered;
   *m_script << "os.system(\"" + command + "\") \n" << std::endl;
}*/


