#include "PreProcessingData.h"

PreProcessingData::PreProcessingData(QString module) : Script(module)
{
   m_skullStripping_suffix = "-stripped";
   m_rescaling_suffix = "-rescaled";
   m_converting_suffix = "-converted";
   m_correcting_suffix = "-corrected";
   m_closing_suffix = "-closed";
}

void PreProcessingData::setAddingExtraCSF(bool addingExtraCSF)
{
   m_addingExtraCSF = addingExtraCSF; 
}

void PreProcessingData::initializeScript()
{
   definePython();
   importGeneralModules();
   importXmlModules();

   m_script += "import re\n";
   m_script += "import csv\n";
  
   defineExecutable("ImageMath");
   defineExecutable("ImageStat");
   defineExecutable("SegPostProcessCLP");
   defineExecutable("unu");
   defineExecutable("N4ITKBiasFieldCorrection");
   defineExecutable("CleanRingMask");

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
   m_argumentsList << "N4ITKBiasFieldCorrection" << image + "_converted" << "'--maskimage'" << "mask" << image + "_corrected";
   execute();   

   return correctedImage_path;
}

QString PreProcessingData::dilateMask()
{
   // Dilate Mask
   QString dilatedMask_name = m_prefix + "mask-dilated" + m_suffix + ".nrrd";
   QString dilatedMask_path = m_module_dir->filePath(dilatedMask_name);

   m_log = "Dilated mask";
   m_inputs.insert("mask", m_neo.mask);   
   m_outputs.insert("dilatedMask", dilatedMask_path); 
   m_argumentsList << "ImageMath" << "mask" << "'-dilate'" << "'3,1'" << "'-outfile'" << "dilatedMask";
   execute(); 

   return dilatedMask_path;
}

QString PreProcessingData::addExtraCSF()
{
   // Sum Mask
   QString sumMask_name = m_prefix + "mask-sum" + m_suffix + ".nrrd";
   QString sumMask_path = m_module_dir->filePath(sumMask_name);

   m_log = "Sum both masks";   
   m_outputs.insert("sumMask", sumMask_path); 
   m_argumentsList << "ImageMath" << "mask" << "'-add'" << "dilatedMask" << "'-outfile'" << "sumMask";
   execute(); 

   // Ring Mask
   QString ringMask_name = m_prefix + "mask-ring" + m_suffix + ".nrrd";
   QString ringMask_path = m_module_dir->filePath(ringMask_name);

   m_log = "Computing ring mask";   
   m_outputs.insert("ringMask", ringMask_path); 
   m_argumentsList << "ImageMath" << "sumMask" << "'-extractLabel'" << "'1'" << "'-outfile'" << "ringMask";
   execute(); 

   // Clean Ring Mask 
   QString cleanedRingMask_name = m_prefix + "mask-ring-cleaned" + m_suffix + ".nrrd";
   QString cleanedRingMask_path = m_module_dir->filePath(cleanedRingMask_name);

   m_log = "- Cleaning ring mask";
   m_outputs.insert("cleanedRingMask", cleanedRingMask_path);
   m_argumentsList << "CleanRingMask" << "'--input'" << "ringMask" << "'--output'" << "cleanedRingMask";
   execute(); 

   // CSF intensity
   QString outbase_name = m_prefix + "T2-"; 
   QString outbase_path = m_module_dir->filePath(outbase_name);

   m_log = "Computing CSF intensity on T2"; 
   m_outputs.insert("outbase", outbase_path);
   m_argumentsList << "ImageStat" << "T2" << "'-label'" << "mask" << "'-intensitySummary'" << "'-outbase'" << "outbase";
   execute();

   // CSF ring
   QString csv_name = outbase_name + "_intensitySummary.csv"; 
   QString csv_path = m_module_dir->filePath(csv_name);

   QString oper = "2,' + str([row for row in csv.reader(open(csv_file, 'rb'))][9][2]) + '";

   QString CSFRing_name = m_prefix + "CSF-ring" + m_suffix + ".nrrd";
   QString CSFRing_path = m_module_dir->filePath(CSFRing_name);

   m_log = "Multyplying ring mask by CSF intensity value";
   m_inputs.insert("csv_file", csv_path);
   m_inputs.insert("oper", oper);
   m_outputs.insert("CSFRing", CSFRing_path); 
   m_argumentsList << "ImageMath" << "ringMask" << "'-constOper'" << "oper" << "'-type'" << "'float'" << "'-outfile'" << "CSFRing";
   execute(); 

   // T2 with extra CSF
   QString T2WithExtraCSF_name = m_prefix + "T2-extraCSF" + m_suffix + ".nrrd";
   QString T2WithExtraCSF_path = m_module_dir->filePath(T2WithExtraCSF_name);

   m_log = "Adding CSF ring to T2"; 
   m_outputs.insert("T2Atlas_withExtraCSF", T2WithExtraCSF_path); 
   m_argumentsList << "ImageMath" << "T2" << "'-add'" << "CSFRing" << "'-type'" << "'float'" << "'-outfile'" << "T2Atlas_withExtraCSF";
   execute();

   return T2WithExtraCSF_path;
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

   m_neo.T1 = skullStripImage("T1");
   m_neo.T2 = skullStripImage("T2");

   m_neo.T1 = correctImage("T1");
   m_neo.T2 = correctImage("T2");

   if(m_addingExtraCSF)
   {
      m_neo.dilatedMask = dilateMask(); 
      m_neo.T2Modified = addExtraCSF(); 
   }
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

