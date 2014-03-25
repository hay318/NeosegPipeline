#include "PreProcessingData.h"

PreProcessingData::PreProcessingData(QString module) : Script(module)
{
   m_skullStripping_suffix = "-stripped";
   m_rescaling_suffix = "-rescaled";
   m_converting_suffix = "-converted";
   m_correcting_suffix = "-corrected";
   m_closing_suffix = "-closed";
}

void PreProcessingData::initializeScript(QString &script)
{
   definePython(script);
   importGeneralModules(script);
   importXmlModules(script);
  
   defineExecutable(script, "ImageMath");
   defineExecutable(script, "SegPostProcessCLP");
   defineExecutable(script, "unu");
   defineExecutable(script, "N4ITKBiasFieldCorrection");

   m_script += "runningProcess = None\n\n"; 

   script += "T1 = '" + m_neo.T1 + "'\n"; 
   script += "T2 = '" + m_neo.T2 + "'\n"; 
   script += "mask = '" + m_neo.mask + "'\n\n";
}

QString PreProcessingData::skullStripImage(QString &script, QString image)
{
   QString strippedImage_name = m_prefix + image + m_skullStripping_suffix + m_suffix + ".nrrd";
   QString strippedImage_path = m_module_dir->filePath(strippedImage_name);

   m_outputs.insert(image + "_stripped", strippedImage_path);
   m_log = "Skull-stripping the " + image;   
   m_argumentsList << "SegPostProcessCLP" << "mask" << image + "_stripped"<< "'--skullstripping'" << image << "'--MWM'" << "'1'" << "'--WM'" << "'2'" << "'--GM'" << "'3'" << "'--CSF'" << "'4'"; 
   execute(script); 

   return strippedImage_path;
}


QString PreProcessingData::correctImage(QString &script, QString image)
{
   QString convertedImage_name = m_prefix + image + m_skullStripping_suffix + m_converting_suffix + m_suffix + ".nrrd";
   QString convertedImage_path = m_module_dir->filePath(convertedImage_name); 

   m_outputs.insert(image + "_converted", convertedImage_path);  
   m_log = "- Converting the image in float";
   m_argumentsList_1 << "unu" << "'convert'" << "'-i'" << image + "_stripped" << "'-t'" << "'float'"; 
   m_argumentsList_2 << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << image + "_converted"; 
   executePipe(script); 

   QString correctedImage_name = m_prefix + image + m_skullStripping_suffix + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedImage_path = m_module_dir->filePath(correctedImage_name);

   m_log = "- Correcting the inhomogeneity of the image";
   m_outputs.insert(image + "_corrected", correctedImage_path);
   m_argumentsList << "N4ITKBiasFieldCorrection" << "'--inputimage'" << image + "_converted" << "'--maskimage'" << "mask" << "'--outputimage'" << image + "_corrected";
   execute(script);   

   return correctedImage_path;
}

void PreProcessingData::implementRun(QString &script)
{
   script += "def run():\n\n";  

   script += "\tsignal.signal(signal.SIGINT, stop)\n";
   script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   script += "\tlogging.info('=== Preprocessing Data ===')\n";
   script += "\tlogging.debug('')\n";

   m_neo.T1 = skullStripImage(script, "T1");
   m_neo.T2 = skullStripImage(script, "T2");

   script += "\tlogging.info('Correcting the inhomogeneity of the T1 :')\n";   
   m_neo.T1 = correctImage(script, "T1");

   script += "\tlogging.info('Correcting the inhomogeneity of the T2 :')\n";   
   m_neo.T2 = correctImage(script, "T2");
}

void PreProcessingData::update()
{
   QString preProcessData;

   initializeScript(preProcessData);
   implementStop(preProcessData); 
   implementCheckFileExistence(preProcessData); 
   implementExecute(preProcessData); 
   implementExecutePipe(preProcessData); 
   implementRun(preProcessData);

   writeScript(preProcessData);
}

Neo PreProcessingData::getOutput()
{
   return m_neo;
}

