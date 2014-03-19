#include "PreProcessingData.h"

PreProcessingData::PreProcessingData(QString module) : Script(module)
{
   m_skullStripping_suffix = "-stripped";
   m_rescaling_suffix = "-rescaled";
   m_correcting_suffix = "-corrected";
   m_closing_suffix = "-closed";
}

void PreProcessingData::setSkullStripping(bool skullStripping) 
{
   m_skullStripping=skullStripping;
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

QString PreProcessingData::closeMask(QString &script)
{
   QStringList argumentsList;

   QFileInfo* mask_file = new QFileInfo(m_neo.mask);
   QString closedMask_name = mask_file->baseName() + m_closing_suffix + ".nrrd";
   QString closedMask_path = m_module_dir->filePath(closedMask_name); 

   script += "\tmask_closed = '" + closedMask_path + "'\n";    

   script += "\tif checkFileExistence(mask_closed)==False:\n";   

   argumentsList << "ImageMath" << "mask" << "'-dilate'"<< "'5,1'" << "'-outfile'" << "mask_closed";  
   script += "\t\tlogging.info('- Dilating the brain mask (radius=5)...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "mask_closed" << "'-erode'"<< "'5,1'" << "'-outfile'" << "mask_closed"; 
   script += "\t\tlogging.info('- Eroding the brain mask (radius=5)...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";

   return closedMask_path;
}

QString PreProcessingData::skullStripImage(QString &script, QString image)
{
   QStringList argumentsList;

   QString image_path; 

   if(image=="T1")
   {
      image_path = m_neo.T1; 
   }
   if(image=="T2")
   {
      image_path = m_neo.T2;
   }

   QFileInfo* image_file = new QFileInfo(image_path);
   QString strippedImage_name = image_file->baseName() + m_skullStripping_suffix + ".nrrd";
   QString strippedImage_path = m_module_dir->filePath(strippedImage_name); 
   script += "\t" + image + "_stripped = '" + strippedImage_path + "'\n";    

   script += "\tif checkFileExistence(" + image + "_stripped)==False:\n";   

   argumentsList << "SegPostProcessCLP" << "mask" << image + "_stripped"<< "'--skullstripping'" << "T1" << "'--MWM'" << "'1'" << "'--WM'" << "'2'" << "'--GM'" << "'3'" << "'--CSF'" << "'4'"; 
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";

   return strippedImage_path;
}


QString PreProcessingData::correctImage(QString &script, QString image)
{
   QStringList argumentsList;

   QString previousImage; 

   if(!m_skullStripping)
   {
      previousImage = image + "_stripped";
   }
   else
   {
      previousImage = image;
   }

   QString convertedImage_name = m_neo.prefix + "-" + image + "-converted.nrrd";
   QString convertedImage_path = m_module_dir->filePath(convertedImage_name); 
   script += "\t" + image + "_converted = '" + convertedImage_path + "'\n"; 

   script += "\tif checkFileExistence(" + image + "_converted)==False:\n"; 

   argumentsList << "unu" << "'convert'" << "'-i'" << previousImage << "'-t'" << "'float'"; 
   script += "\t\targs1 = " + listToString(argumentsList) + "\n";

   argumentsList.clear();
   argumentsList << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << image + "_converted"; 
   script += "\t\targs2 = " + listToString(argumentsList) + "\n";

   script += "\t\tlogging.info('- Converting the image in float...')\n";
   script += "\t\texecutePipe(args1, args2)\n\n";

   QString correctedImage_name = m_neo.prefix + "-" + image + "-corrected.nrrd";
   QString correctedImage_path = m_module_dir->filePath(correctedImage_name);
   script += "\t" + image + "_corrected = '" + correctedImage_path + "'\n"; 

   script += "\tif checkFileExistence(" + image + "_corrected)==False:\n"; 

   argumentsList.clear();
   argumentsList << "N4ITKBiasFieldCorrection" << "'--inputimage'" << image + "_converted" << "'--maskimage'" << "mask" << "'--outputimage'" << image + "_corrected";
   script += "\t\tlogging.info('- Correcting the inhomogeneity of the image...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";  

   return correctedImage_path;
}

void PreProcessingData::implementRun(QString &script)
{
   script += "def run():\n\n";  

   script += "\tsignal.signal(signal.SIGINT, stop)\n";
   script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   script += "\tlogging.info('=== Preprocessing Data ===')\n";
   script += "\tlogging.debug('')\n";

   //m_neo.mask = closeMask(script); 

   if(!m_skullStripping)
   {
      script += "\tlogging.info('Skull-stripping the T1...')\n";  
      m_neo.T1 = skullStripImage(script, "T1");

      script += "\tlogging.info('Skull-stripping the T2...')\n";
      m_neo.T2 = skullStripImage(script, "T2");
   }
   
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

