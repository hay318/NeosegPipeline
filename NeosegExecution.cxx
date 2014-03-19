#include "NeosegExecution.h"

NeosegExecution::NeosegExecution(QString module) : Script(module)
{
}

void NeosegExecution::setAtlas(QString atlas)
{
   m_atlas = atlas;
}
void NeosegExecution::setNeosegParameters(NeosegParameters* parameters)
{
   m_parameters = parameters;
}
void NeosegExecution::setUsingFA(bool usingFA)
{
   m_usingFA = usingFA;
}
void NeosegExecution::setUsingAD(bool usingAD)
{
   m_usingAD = usingAD;
}
void NeosegExecution::setComputing3LabelsSeg(bool computing3LabelsSeg)
{
   m_computing3LabelsSeg = computing3LabelsSeg;
}

void NeosegExecution::defineSegmentation()
{
   QFileInfo* T2 = new QFileInfo(m_neo.T2);
   QString segmentation_name = T2->baseName() + "_EMonly_labels_" + m_neo.prefix + ".nrrd"; 
   m_segmentation = m_module_dir->filePath(segmentation_name);
}

bool NeosegExecution::checkSegmentation()
{
   QFile* segmentation_file = new QFile (m_segmentation);
   
   if ( segmentation_file->exists() )
   {
      return true;
   }  
   else
   {
      return false; 
   }
}

void NeosegExecution::initializeScript(QString &script)
{
   definePython(script);
   importGeneralModules(script);
   importXmlModules(script);
  
   defineExecutable(script, "ImageMath");
   defineExecutable(script, "neoseg");

   script += "runningProcess = None\n\n";   
}

void NeosegExecution::implementWriteAffineTranformationFile(QString& script)
{
   script += "def writeAffineTransformationFile(fileName):\n";

   script += "\tfile = open(fileName, 'w')\n"; 

   script += "\tfile.write('# Chained affine transform\\n')\n";
   script += "\tfile.write('#\\n')\n";
   script += "\tfile.write('# Translations:\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('# Rotations:\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('# Scaling:\\n')\n";
   script += "\tfile.write('1.00\\n')\n";
   script += "\tfile.write('1.00\\n')\n";
   script += "\tfile.write('1.00\\n')\n";
   script += "\tfile.write('# Skews:\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('0.00\\n')\n";
   script += "\tfile.write('# Center of rotation (source):\\n')\n";
   script += "\tfile.write('61.50\\n')\n";
   script += "\tfile.write('77.50\\n')\n";
   script += "\tfile.write('94.50\\n')\n";
   script += "\tfile.write('# Center of rotation (target):\\n')\n";
   script += "\tfile.write('61.50\\n')\n";
   script += "\tfile.write('77.50\\n')\n";
   script += "\tfile.write('94.50\\n')\n";
   script += "\tfile.write('# Forward composition order?\\n')\n";
   script += "\tfile.write('1\\n')\n";

   script += "\tfile.close()\n\n";
}

void NeosegExecution::writeXMLFile(QString &script)
{   
   QString XML_path = m_module_dir->filePath("parameters.xml");
   script += "\tparameters_path = \"" + XML_path + "\"\n"; 

   script += "\tif checkFileExistence(parameters_path)==False:\n";   
   
   script += "\t\tlogging.info('Writing the XML file...')\n";  
   script += "\t\tsegmentationParameters = Element('SEGMENTATION-PARAMETERS')\n";

   addSubElement(script, "segmentationParameters", "suffix", "SUFFIX", m_neo.prefix);
   addSubElement(script, "segmentationParameters", "atlasDirectory", "ATLAS-DIRECTORY", m_atlas);
   addSubElement(script, "segmentationParameters", "atlasOrientation", "ATLAS-ORIENTATION", "RAI");
   addSubElement(script, "segmentationParameters", "atlasFormat", "ATLAS-FORMAT", "NRRD");
   addSubElement(script, "segmentationParameters", "outputDirectory", "OUTPUT-DIRECTORY", m_module_path);
   addSubElement(script, "segmentationParameters", "outputFormat", "OUTPUT-FORMAT", "NRRD");

   script += "\t\tT2 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement(script,"T2", "T2File", "FILE", m_neo.T2);   
   addSubElement(script,"T2", "T2Orientation", "ORIENTATION", "RAI"); 

   script += "\t\tT1 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement(script,"T1", "T1File", "FILE", m_neo.T1);   
   addSubElement(script,"T1", "T1Orientation", "ORIENTATION", "RAI");
 
   if(m_usingFA)
   {
      script += "\t\tFA = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement(script,"FA", "FAFile", "FILE", m_neo.FA);   
      addSubElement(script,"FA", "FAOrientation", "ORIENTATION", "RAI"); 
   }

   if(m_usingAD)
   {
      script += "\t\tAD = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement(script,"AD", "ADFile", "FILE", m_neo.AD);   
      addSubElement(script,"AD", "ADOrientation", "ORIENTATION", "RAI"); 
   }

   addSubElement(script, "segmentationParameters", "filterIterations", "FILTER-ITERATIONS", QString::number(m_parameters->getNumberOfIterations()));
   addSubElement(script, "segmentationParameters", "filterTimeStep", "FILTER-TIME-STEP", QString::number(m_parameters->getTimeStep()));
   addSubElement(script, "segmentationParameters", "filterMethod", "FILTER-METHOD", m_parameters->getFilterMethod());
   addSubElement(script, "segmentationParameters", "maxBiasDegree", "MAX-BIAS-DEGREE", QString::number(m_parameters->getMaxBiasDegree()));
   addSubElement(script, "segmentationParameters", "prior1", "PRIOR-1", QString::number(m_parameters->getPrior1()));
   addSubElement(script, "segmentationParameters", "prior2", "PRIOR-2", QString::number(m_parameters->getPrior2()));
   addSubElement(script, "segmentationParameters", "prior3", "PRIOR-3", QString::number(m_parameters->getPrior3()));
   addSubElement(script, "segmentationParameters", "prior4", "PRIOR-4", QString::number(m_parameters->getPrior4()));
   addSubElement(script, "segmentationParameters", "prior5", "PRIOR-5", QString::number(m_parameters->getPrior5()));

   addSubElement(script, "segmentationParameters", "doAtlasWarp", "DO-ATLAS-WARP", QString::number(0));
   addSubElement(script, "segmentationParameters", "atlasWarpGridX", "ATLAS-WARP-GRID-X", QString::number(5));
   addSubElement(script, "segmentationParameters", "atlasWarpGridY", "ATLAS-WARP-GRID-Y", QString::number(5));
   addSubElement(script, "segmentationParameters", "atlasWarpGridZ", "ATLAS-WARP-GRID-Z", QString::number(5));

   addSubElement(script, "segmentationParameters", "mahalanobisThreshold", "MAHALANOBIS-THRESHOLD", QString::number(4));
   addSubElement(script, "segmentationParameters", "parzenKernelWidth", "PARZEN-KERNEL-WIDTH", QString::number(m_parameters->getInitialParzenKernelWidth()));
   addSubElement(script, "segmentationParameters", "priorThreshold", "PRIOR-THRESHOLD", QString::number(m_parameters->getPriorThreshold()));

   QString referenceImageIndex; 

   if(m_parameters->getReferenceImage()=="T2")
   {
      referenceImageIndex = "1";   
   }    
   if(m_parameters->getReferenceImage()=="T1")
   {
      referenceImageIndex = "2";   
   }    

   addSubElement(script, "segmentationParameters", "referenceImageIndex", "REFERENCE-IMAGE-INDEX", referenceImageIndex);
   addSubElement(script, "segmentationParameters", "referenceModality", "REFERENCE-MODALITY", m_parameters->getReferenceImage());

   script += "\t\tXML = xml.dom.minidom.parseString(ElementTree.tostring(segmentationParameters))\n";
   script += "\t\tpretty_xml_as_string = XML.toprettyxml()\n";

   script += "\t\tparameters = open(parameters_path, 'w')\n";
   script += "\t\tparameters.write(pretty_xml_as_string)\n";  
   script += "\t\tparameters.close()\n\n";
}


void NeosegExecution::writeAffineTranformationFiles(QString& script)
{
   QString T1_name = (QFileInfo(m_neo.T1)).baseName(); 
   QString T2_name = (QFileInfo(m_neo.T2)).baseName(); 

   QString referenceImage_name = m_parameters->getReferenceImage();
   QString referenceTemplate_name;
   
   if(referenceImage_name=="T1")
   {
      referenceTemplate_name = "templateT1";
   }  

   if(referenceImage_name=="T2")
   {
      referenceTemplate_name = "templateT2";
   }  

   QString T2ToTemplate_name = T2_name + "_to_" + referenceTemplate_name + "_" + m_neo.prefix + ".affine"; 
   QString T2ToTemplate_path = m_module_dir->filePath(T2ToTemplate_name); 

   script += "\tT2ToTemplate = '" + T2ToTemplate_path + "'\n";

   script += "\tif checkFileExistence(T2ToTemplate)==False:\n";   
   script += "\t\twriteAffineTransformationFile(T2ToTemplate)\n\n";

   QString T2ToT1_name = T2_name + "_to_" + T1_name + "_" + m_neo.prefix + ".affine"; 
   QString T2ToT1_path = m_module_dir->filePath(T2ToT1_name); 

   script += "\tT2ToT1 = '" + T2ToT1_path + "'\n";
   script += "\tif checkFileExistence(T2ToT1)==False:\n";  
   script += "\t\twriteAffineTransformationFile(T2ToT1)\n\n";

   if(m_usingFA)
   {
      QString FA_name = (QFileInfo(m_neo.FA)).baseName(); 

      QString T2ToFA_name = T2_name + "_to_" + FA_name + "_" + m_neo.prefix + ".affine"; 
      QString T2ToFA_path = m_module_dir->filePath(T2ToFA_name); 

      script += "\tT2ToFA = '" + T2ToFA_path + "'\n";
      script += "\tif checkFileExistence(T2ToFA)==False:\n";  
      script += "\t\twriteAffineTransformationFile(T2ToFA)\n\n";
   }

   if(m_usingAD)
   {
      QString AD_name = (QFileInfo(m_neo.AD)).baseName(); 

      QString T2ToAD_name = T2_name + "_to_" + AD_name + "_" + m_neo.prefix + ".affine"; 
      QString T2ToAD_path = m_module_dir->filePath(T2ToAD_name); 

      script += "\tT2ToAD = '" + T2ToAD_path + "'\n";
      script += "\tif checkFileExistence(T2ToAD)==False:\n";  
      script += "\t\twriteAffineTransformationFile(T2ToAD)\n\n";
   }
}

void NeosegExecution::runNeoseg(QString &script)
{
   QStringList argumentsList;

   QFileInfo* T2 = new QFileInfo(m_neo.T2);
   QString segmentation_name = T2->baseName() + "_EMonly_labels_" + m_neo.prefix + ".nrrd"; 
   QString segmentation_path = m_module_dir->filePath(segmentation_name);

   script += "\tseg4Labels = '" + segmentation_path + "'\n";
   script += "\tif checkFileExistence(seg4Labels)==False:\n";  

   argumentsList << "neoseg" << "parameters_path";
   script += "\t\tlogging.info('Running Neoseg...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";   

   m_neo.seg4Labels = segmentation_path;
}

void NeosegExecution::mergeWhiteMatters(QString &script)
{
   QStringList argumentsList;

   QString seg3labels_name = m_neo.prefix + "-seg-3labels.nrrd";
   QString seg3labels_path = m_module_dir->filePath(seg3labels_name); 
   script += "\tseg_3Labels = '" + seg3labels_path + "'\n"; 

   script += "\tif checkFileExistence(seg_3Labels)==False:\n";

   script += "\t\tlogging.info('Computing a 3-labels segmentation...')\n";  
   script += "\t\tseg_4Labels = '" + m_neo.seg4Labels + "'\n";

   // Myelinated WM
   QString myelinatedWM_name = m_neo.prefix + "-myelinatedWM.nrrd";
   QString myelinatedWM_path = m_module_dir->filePath(myelinatedWM_name); 
   script += "\t\tmyelinatedWM = '" + myelinatedWM_path + "'\n"; 

   argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'1'" << "'-outfile'" << "myelinatedWM"; 
   script += "\t\tlogging.info('- Extracting the myelinated white matter...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";   

   // WM
   QString WM_name = m_neo.prefix + "-WM.nrrd";
   QString WM_path = m_module_dir->filePath(WM_name); 
   script += "\t\tWM = '" + WM_path + "'\n"; 

   argumentsList.clear();
   argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'2'" << "'-outfile'" << "WM"; 
   script += "\t\tlogging.info('- Extracting the white matter...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";   

   // GM
   QString GM_name = m_neo.prefix + "-GM.nrrd";
   QString GM_path = m_module_dir->filePath(GM_name); 
   script += "\t\tGM = '" + GM_path + "'\n"; 

   argumentsList.clear();
   argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'3'" << "'-outfile'" << "GM"; 
   script += "\t\tlogging.info('- Extracting the gray matter...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";  

   argumentsList.clear(); 
   argumentsList << "ImageMath" << "GM" << "'-constOper'" << "'2,2'" << "'-outfile'" << "GM"; 
   script += "\t\tlogging.info('- Multiplying the gray matter by 2...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n"; 

   // CSF
   QString CSF_name = m_neo.prefix + "-CSF.nrrd";
   QString CSF_path = m_module_dir->filePath(CSF_name); 
   script += "\t\tCSF = '" + CSF_path + "'\n"; 

   argumentsList.clear();
   argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'4'" << "'-outfile'" << "CSF"; 
   script += "\t\tlogging.info('- Extracting the CSF...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";  

   argumentsList.clear(); 
   argumentsList << "ImageMath" << "CSF" << "'-constOper'" << "'2,3'" << "'-outfile'" << "CSF"; 
   script += "\t\tlogging.info('- Multiplying the CSF by 3...')\n";  
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n"; 

   // 3-labels Seg 
   script += "\t\tlogging.info('- Adding all the labels...')\n";  

   argumentsList.clear(); 
   argumentsList << "ImageMath" << "WM" << "'-add'" << "myelinatedWM" << "'-outfile'" << "seg_3Labels"; 
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n";  

   argumentsList.clear(); 
   argumentsList << "ImageMath" << "seg_3Labels" << "'-add'" << "GM" << "'-outfile'" << "seg_3Labels"; 
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n"; 

   argumentsList.clear(); 
   argumentsList << "ImageMath" << "seg_3Labels" << "'-add'" << "CSF" << "'-outfile'" << "seg_3Labels"; 
   script += "\t\targs = " + listToString(argumentsList) + "\n";
   script += "\t\texecute(args)\n\n"; 

   m_neo.seg3Labels = seg3labels_path;
}

void NeosegExecution::implementRun(QString &script)
{
   script += "def run():\n";

   script += "\tsignal.signal(signal.SIGINT, stop)\n";
   script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   script += "\tlogging.debug('')\n\n";
   script += "\tlogging.info('=== Neoseg Execution ===')\n";
   script += "\tlogging.debug('')\n\n";

   script += "\t# Write XML File #\n";
   writeXMLFile(script);
   
   script += "\t# Write Affine Transformation File #\n";
   writeAffineTranformationFiles(script);

   script += "\t# Run Neoseg #\n";
   runNeoseg(script);

   if(m_computing3LabelsSeg)
   {
      script += "\t# Merge White Matters #\n";
      mergeWhiteMatters(script);
   }
} 

void NeosegExecution::update()
{
   QString executeNeoseg;

   initializeScript(executeNeoseg);

   implementStop(executeNeoseg);
   implementCheckFileExistence(executeNeoseg);
   implementExecute(executeNeoseg);
   implementWriteAffineTranformationFile(executeNeoseg);
   implementRun(executeNeoseg);

   writeScript(executeNeoseg);
}

Neo NeosegExecution::getOutput() 
{
   return m_neo;
}



