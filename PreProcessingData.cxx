#include "PreProcessingData.h"

PreProcessingData::PreProcessingData(QString module) : Script(module)
{
   m_skullStripping_suffix = "-stripped";
   m_rescaling_suffix = "-rescaled";
   m_converting_suffix = "-converted";
   m_correcting_suffix = "-corrected";
   m_closing_suffix = "-closed";
}

void PreProcessingData::initializeScript()
{
   definePython();
   importGeneralModules();
   importXmlModules();
  
   defineExecutable("ImageMath");
   defineExecutable("SegPostProcessCLP");
   defineExecutable("unu");
   defineExecutable("N4ITKBiasFieldCorrection");

   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n"; 

   m_script += "runningProcess = None\n\n"; 

   m_script += "T1 = '" + m_neo.T1 + "'\n"; 
   m_script += "T2 = '" + m_neo.T2 + "'\n"; 
   m_script += "mask = '" + m_neo.mask + "'\n\n";
}

QString PreProcessingData::skullStripImage(QString image)
{
   QString strippedImage_name = m_prefix + image + m_skullStripping_suffix + m_suffix + ".nrrd";
   QString strippedImage_path = m_module_dir->filePath(strippedImage_name);

   m_outputs.insert(image + "_stripped", strippedImage_path);
   m_log = "- Skull-stripping the " + image;   
   m_argumentsList << "SegPostProcessCLP" << "mask" << image + "_stripped"<< "'--skullstripping'" << image << "'--MWM'" << "'1'" << "'--WM'" << "'2'" << "'--GM'" << "'3'" << "'--CSF'" << "'4'"; 
   execute(); 

   m_unnecessaryFiles << strippedImage_path; 

   return strippedImage_path;
}


QString PreProcessingData::correctImage(QString image)
{
   QString convertedImage_name = m_prefix + image + m_skullStripping_suffix + m_converting_suffix + m_suffix + ".nrrd";
   QString convertedImage_path = m_module_dir->filePath(convertedImage_name); 

   m_outputs.insert(image + "_converted", convertedImage_path);  
   m_log = "- Converting the " + image + " in float";
   m_argumentsList_1 << "unu" << "'convert'" << "'-i'" << image + "_stripped" << "'-t'" << "'float'"; 
   m_argumentsList_2 << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << image + "_converted"; 
   executePipe(); 

   m_unnecessaryFiles << convertedImage_path; 

   QString correctedImage_name = m_prefix + image + m_skullStripping_suffix + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedImage_path = m_module_dir->filePath(correctedImage_name);

   m_log = "- Correcting the inhomogeneity of the " + image;
   m_outputs.insert(image + "_corrected", correctedImage_path);
   m_argumentsList << "N4ITKBiasFieldCorrection" << "'--inputimage'" << image + "_converted" << "'--maskimage'" << "mask" << "'--outputimage'" << image + "_corrected";
   execute();   

   return correctedImage_path;
}

void PreProcessingData::implementRun()
{
   m_script += "def run():\n\n";  

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   m_script += "\tlogger.info('=== Preprocessing Data ===')\n";

   QString correctedT1_name = m_prefix + "T1" + m_skullStripping_suffix + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedT1_path = m_module_dir->filePath(correctedT1_name);

   QString correctedT2_name = m_prefix + "T2" + m_skullStripping_suffix + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedT2_path = m_module_dir->filePath(correctedT2_name);

   m_outputs.insert("finalT1", correctedT1_path);
   m_outputs.insert("finalT2", correctedT2_path);
   checkFinalOutputs(); 

   m_script += "\tlogger.info('')\n";

   m_neo.T1 = skullStripImage("T1");
   m_neo.T2 = skullStripImage("T2");

   m_neo.T1 = correctImage("T1");
   m_neo.T2 = correctImage("T2");
}

void PreProcessingData::update()
{
   initializeScript();
   implementStop(); 
   implementCheckFileExistence(); 
   implementExecute(); 
   implementExecutePipe(); 
   implementRun();

   writeScript();
}

Neo PreProcessingData::getOutput()
{
   return m_neo;
}

