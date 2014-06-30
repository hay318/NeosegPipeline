#include "PreProcessingData.h"

PreProcessingData::PreProcessingData(QString module) : Script(module)
{
   m_skullStripping_suffix = "-stripped";
   m_rescaling_suffix = "-rescaled";
   m_positive_suffix = "-positive";
   m_converting_suffix = "-converted";
   m_correcting_suffix = "-corrected";
   m_closing_suffix = "-closed";
}

void PreProcessingData::setSkullStripping(bool skullStripping)
{
   m_skullStripping = skullStripping; 
}
void PreProcessingData::setCorrecting(bool correcting)
{
   m_correcting = correcting; 
}
void PreProcessingData::setUsingSmoothedMask(bool usingSmoothedMask)
{
   m_usingSmoothedMask = usingSmoothedMask; 
}

void PreProcessingData::initializeScript()
{
   definePython();
   importGeneralModules();
   importXmlModules();

   m_script += "import re\n";
   m_script += "import csv\n";
  
   defineExecutable("ImageMath");
   defineExecutable("SegPostProcessCLP");
   defineExecutable("unu");
   defineExecutable("N4ITKBiasFieldCorrection");

   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n"; 

   m_script += "runningProcess = None\n\n"; 
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
   QString basename; 
   if(m_skullStripping)
   {
      basename = m_prefix + image + m_skullStripping_suffix;
   }
   else
   {
      basename = m_prefix + image;   
   }

   QString positiveImage_name = basename + m_positive_suffix + m_suffix + ".nrrd";
   QString positiveImage_path = m_module_dir->filePath(positiveImage_name); 

   m_outputs.insert(image + "_positive", positiveImage_path);  
   m_log = "- Removing negative values in " + image;
   m_argumentsList << "ImageMath" << image + "_stripped" << "'-threshMask'" << "'0,100000'" << "'-outfile'" << image + "_positive";
   execute(); 

   m_unnecessaryFiles << positiveImage_path; 

   QString convertedImage_name = basename + m_converting_suffix + m_suffix + ".nrrd";
   QString convertedImage_path = m_module_dir->filePath(convertedImage_name); 

   m_outputs.insert(image + "_converted", convertedImage_path);  
   m_log = "- Converting the " + image + " in float";
   m_argumentsList_1 << "unu" << "'convert'" << "'-i'" << image + "_positive" << "'-t'" << "'float'"; 
   m_argumentsList_2 << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << image + "_converted"; 
   executePipe(); 

   m_unnecessaryFiles << convertedImage_path; 

   QString correctedImage_name = basename + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedImage_path = m_module_dir->filePath(correctedImage_name);

   m_log = "- Correcting the inhomogeneity of the " + image;
   m_outputs.insert(image + "_corrected", correctedImage_path);
   m_argumentsList << "N4ITKBiasFieldCorrection" << image + "_converted" << "'--maskimage'" << "mask" << image + "_corrected";
   execute();   

   return correctedImage_path;
}

QString PreProcessingData::smoothMask()
{
   QString smoothedMask_name = m_prefix + "mask-smoothed" + m_suffix + ".nrrd";
   QString smoothedMask_path = m_module_dir->filePath(smoothedMask_name); 

   m_log = "Smoothing mask";
   m_inputs.insert("mask", m_neo.mask);   
   m_outputs.insert("smoothedMask", smoothedMask_path); 
   m_argumentsList << "ImageMath" << "mask" << "'-smooth'" << "'-gauss'" << "'-size'" << "'1'" << "'-outfile'" << "smoothedMask";
   execute(); 

   m_unnecessaryFiles << smoothedMask_path;

   return smoothedMask_path;
}

void PreProcessingData::implementRun()
{
   m_script += "def run():\n\n";  

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   m_script += "\tlogger.info('=== Preprocessing Data ===')\n";

   m_script += "\tT1 = '" + m_neo.T1 + "'\n"; 
   m_script += "\tT2 = '" + m_neo.T2 + "'\n"; 
   m_script += "\tmask = '" + m_neo.mask + "'\n\n";

   QString correctedT1_name = m_prefix + "T1" + m_skullStripping_suffix + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedT1_path = m_module_dir->filePath(correctedT1_name);

   QString correctedT2_name = m_prefix + "T2" + m_skullStripping_suffix + m_correcting_suffix + m_suffix + ".nrrd";
   QString correctedT2_path = m_module_dir->filePath(correctedT2_name);

   m_outputs.insert("finalT1", correctedT1_path);
   m_outputs.insert("finalT2", correctedT2_path);
   checkFinalOutputs(); 

   m_script += "\tlogger.info('')\n";

   if(m_skullStripping)
   {
      m_neo.T1 = skullStripImage("T1");
      m_neo.T2 = skullStripImage("T2");
   }

   if(m_correcting)
   {
      m_neo.T1 = correctImage("T1");
      m_neo.T2 = correctImage("T2");
   }

   if(m_usingSmoothedMask)
   {
      m_neo.smoothedMask = smoothMask(); 
   }

   // Cleaning for keven data
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "mask-dilated" + m_suffix + ".nrrd");
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "mask-sum" + m_suffix + ".nrrd");
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "mask-ring" + m_suffix + ".nrrd");
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "mask-ring-cleaned" + m_suffix + ".nrrd");
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "CSF-ring" + m_suffix + ".nrrd");
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "T2-extraCSF" + m_suffix + ".nrrd");
   m_unnecessaryFiles << m_module_dir->filePath(m_prefix + "T2-_intensitySummary.csv");

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

