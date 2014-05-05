#include "NeosegExecution.h"

NeosegExecution::NeosegExecution(QString module) : Script(module)
{
}

void NeosegExecution::setAtlas(QString atlas)
{
   m_atlas = atlas;
}
void NeosegExecution::setUsingFA(bool usingFA)
{
   m_usingFA = usingFA;
}
void NeosegExecution::setUsingAD(bool usingAD)
{
   m_usingAD = usingAD;
}
void NeosegExecution::setNeosegParameters(NeosegParameters* parameters)
{
   m_parameters = parameters;
}
void NeosegExecution::setComputing3LabelsSeg(bool computing3LabelsSeg)
{
   m_computing3LabelsSeg = computing3LabelsSeg;
}
void NeosegExecution::setReassigningWhiteMatter(bool reassigningWhiteMatter)
{
   m_reassigningWhiteMatter = reassigningWhiteMatter;
}
void NeosegExecution::setSizeThreshold(int sizeThreshold)
{
   m_sizeThreshold = sizeThreshold;
}

void NeosegExecution::initializeScript()
{
   definePython();
   importGeneralModules();
   importXmlModules();
  
   defineExecutable("ImageMath");
   defineExecutable("neoseg");
   defineExecutable("ReassignWhiteMatter"); 

   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n";

   m_script += "runningProcess = None\n\n";   
}

void NeosegExecution::implementWriteAffineTranformationFile()
{
   m_script += "def writeAffineTransformationFile(fileName):\n";

   m_script += "\tfile = open(fileName, 'w')\n"; 

   m_script += "\tfile.write('# Chained affine transform\\n')\n";
   m_script += "\tfile.write('#\\n')\n";
   m_script += "\tfile.write('# Translations:\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('# Rotations:\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('# Scaling:\\n')\n";
   m_script += "\tfile.write('1.00\\n')\n";
   m_script += "\tfile.write('1.00\\n')\n";
   m_script += "\tfile.write('1.00\\n')\n";
   m_script += "\tfile.write('# Skews:\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('0.00\\n')\n";
   m_script += "\tfile.write('# Center of rotation (source):\\n')\n";
   m_script += "\tfile.write('61.50\\n')\n";
   m_script += "\tfile.write('77.50\\n')\n";
   m_script += "\tfile.write('94.50\\n')\n";
   m_script += "\tfile.write('# Center of rotation (target):\\n')\n";
   m_script += "\tfile.write('61.50\\n')\n";
   m_script += "\tfile.write('77.50\\n')\n";
   m_script += "\tfile.write('94.50\\n')\n";
   m_script += "\tfile.write('# Forward composition order?\\n')\n";
   m_script += "\tfile.write('1\\n')\n";

   m_script += "\tfile.close()\n\n";
}

void NeosegExecution::writeXMLFile()
{   
   QString XML_path = m_module_dir->filePath("parameters.xml");
   m_script += "\tparameters_path = \"" + XML_path + "\"\n"; 

   m_script += "\tif checkFileExistence(parameters_path)==False:\n";   
   
   m_script += "\t\tlogger.info('Writing the XML file...')\n";  
   m_script += "\t\tsegmentationParameters = Element('SEGMENTATION-PARAMETERS')\n";

   addSubElement(m_script, "segmentationParameters", "suffix", "SUFFIX", m_prefix);
   addSubElement(m_script, "segmentationParameters", "atlasDirectory", "ATLAS-DIRECTORY", m_atlas);
   addSubElement(m_script, "segmentationParameters", "atlasOrientation", "ATLAS-ORIENTATION", "RAI");
   addSubElement(m_script, "segmentationParameters", "atlasFormat", "ATLAS-FORMAT", "NRRD");
   addSubElement(m_script, "segmentationParameters", "outputDirectory", "OUTPUT-DIRECTORY", m_module_path);
   addSubElement(m_script, "segmentationParameters", "outputFormat", "OUTPUT-FORMAT", "NRRD");

   m_script += "\t\tT2 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement(m_script,"T2", "T2File", "FILE", m_neo.T2);   
   addSubElement(m_script,"T2", "T2Orientation", "ORIENTATION", "RAI"); 

   m_script += "\t\tT1 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement(m_script,"T1", "T1File", "FILE", m_neo.T1);   
   addSubElement(m_script,"T1", "T1Orientation", "ORIENTATION", "RAI");
 
   if(m_usingFA)
   {
      m_script += "\t\tFA = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement(m_script,"FA", "FAFile", "FILE", m_neo.FA);   
      addSubElement(m_script,"FA", "FAOrientation", "ORIENTATION", "RAI"); 
   }

   if(m_usingAD)
   {
      m_script += "\t\tAD = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement(m_script,"AD", "ADFile", "FILE", m_neo.AD);   
      addSubElement(m_script,"AD", "ADOrientation", "ORIENTATION", "RAI"); 
   }

   addSubElement(m_script, "segmentationParameters", "filterIterations", "FILTER-ITERATIONS", QString::number(m_parameters->getNumberOfIterations()));
   addSubElement(m_script, "segmentationParameters", "filterTimeStep", "FILTER-TIME-STEP", QString::number(m_parameters->getTimeStep()));
   addSubElement(m_script, "segmentationParameters", "filterMethod", "FILTER-METHOD", m_parameters->getFilterMethod());
   addSubElement(m_script, "segmentationParameters", "maxBiasDegree", "MAX-BIAS-DEGREE", QString::number(m_parameters->getMaxBiasDegree()));
   addSubElement(m_script, "segmentationParameters", "prior1", "PRIOR-1", QString::number(m_parameters->getPrior1()));
   addSubElement(m_script, "segmentationParameters", "prior2", "PRIOR-2", QString::number(m_parameters->getPrior2()));
   addSubElement(m_script, "segmentationParameters", "prior3", "PRIOR-3", QString::number(m_parameters->getPrior3()));
   addSubElement(m_script, "segmentationParameters", "prior4", "PRIOR-4", QString::number(m_parameters->getPrior4()));
   addSubElement(m_script, "segmentationParameters", "prior5", "PRIOR-5", QString::number(m_parameters->getPrior5()));

   addSubElement(m_script, "segmentationParameters", "doAtlasWarp", "DO-ATLAS-WARP", QString::number(0));
   addSubElement(m_script, "segmentationParameters", "atlasWarpGridX", "ATLAS-WARP-GRID-X", QString::number(5));
   addSubElement(m_script, "segmentationParameters", "atlasWarpGridY", "ATLAS-WARP-GRID-Y", QString::number(5));
   addSubElement(m_script, "segmentationParameters", "atlasWarpGridZ", "ATLAS-WARP-GRID-Z", QString::number(5));

   addSubElement(m_script, "segmentationParameters", "mahalanobisThreshold", "MAHALANOBIS-THRESHOLD", QString::number(4));
   addSubElement(m_script, "segmentationParameters", "parzenKernelWidth", "PARZEN-KERNEL-WIDTH", QString::number(m_parameters->getInitialParzenKernelWidth()));
   addSubElement(m_script, "segmentationParameters", "priorThreshold", "PRIOR-THRESHOLD", QString::number(m_parameters->getPriorThreshold()));

   QString referenceImageIndex; 

   if(m_parameters->getReferenceImage()=="T2")
   {
      referenceImageIndex = "1";   
   }    
   if(m_parameters->getReferenceImage()=="T1")
   {
      referenceImageIndex = "2";   
   }    

   addSubElement(m_script, "segmentationParameters", "referenceImageIndex", "REFERENCE-IMAGE-INDEX", referenceImageIndex);
   addSubElement(m_script, "segmentationParameters", "referenceModality", "REFERENCE-MODALITY", m_parameters->getReferenceImage());

   m_script += "\t\tXML = xml.dom.minidom.parseString(ElementTree.tostring(segmentationParameters))\n";
   m_script += "\t\tpretty_xml_as_string = XML.toprettyxml()\n";

   m_script += "\t\tparameters = open(parameters_path, 'w')\n";
   m_script += "\t\tparameters.write(pretty_xml_as_string)\n";  
   m_script += "\t\tparameters.close()\n\n";
}


void NeosegExecution::writeAffineTranformationFiles()
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

   QString T2ToTemplate_name = T2_name + "_to_" + referenceTemplate_name + "_" + m_prefix + ".affine"; 
   QString T2ToTemplate_path = m_module_dir->filePath(T2ToTemplate_name); 

   m_script += "\tT2ToTemplate = '" + T2ToTemplate_path + "'\n";

   m_script += "\tif checkFileExistence(T2ToTemplate)==False:\n";   
   m_script += "\t\twriteAffineTransformationFile(T2ToTemplate)\n\n";

   QString T2ToT1_name = T2_name + "_to_" + T1_name + "_" + m_prefix + ".affine"; 
   QString T2ToT1_path = m_module_dir->filePath(T2ToT1_name); 

   m_script += "\tT2ToT1 = '" + T2ToT1_path + "'\n";
   m_script += "\tif checkFileExistence(T2ToT1)==False:\n";  
   m_script += "\t\twriteAffineTransformationFile(T2ToT1)\n\n";

   if(m_usingFA)
   {
      QString FA_name = (QFileInfo(m_neo.FA)).baseName(); 

      QString T2ToFA_name = T2_name + "_to_" + FA_name + "_" + m_prefix + ".affine"; 
      QString T2ToFA_path = m_module_dir->filePath(T2ToFA_name); 

      m_script += "\tT2ToFA = '" + T2ToFA_path + "'\n";
      m_script += "\tif checkFileExistence(T2ToFA)==False:\n";  
      m_script += "\t\twriteAffineTransformationFile(T2ToFA)\n\n";
   }

   if(m_usingAD)
   {
      QString AD_name = (QFileInfo(m_neo.AD)).baseName(); 

      QString T2ToAD_name = T2_name + "_to_" + AD_name + "_" + m_prefix + ".affine"; 
      QString T2ToAD_path = m_module_dir->filePath(T2ToAD_name); 

      m_script += "\tT2ToAD = '" + T2ToAD_path + "'\n";
      m_script += "\tif checkFileExistence(T2ToAD)==False:\n";  
      m_script += "\t\twriteAffineTransformationFile(T2ToAD)\n\n";
   }
}

void NeosegExecution::runNeoseg()
{
   QFileInfo* T2 = new QFileInfo(m_neo.T2);
   QString segmentation_name = T2->baseName() + "_EMonly_labels_" + m_prefix + ".nrrd"; 
   QString segmentation_path = m_module_dir->filePath(segmentation_name);

   m_log = "Running Neoseg";
   m_outputs.insert("seg_4Labels", segmentation_path); 
   m_argumentsList << "neoseg" << "parameters_path";
   execute(); 

   m_neo.seg4Labels = segmentation_path;
}

void NeosegExecution::mergeWhiteMatters()
{
   // Myelinated WM
   QString myelinatedWM_name = m_prefix + "myelinatedWM" + m_suffix + ".nrrd";
   QString myelinatedWM_path = m_module_dir->filePath(myelinatedWM_name); 

   m_log = "- Extracting the myelinated white matter";
   m_inputs.insert("seg_4Labels", m_neo.seg4Labels);
   m_outputs.insert("myelinatedWM", myelinatedWM_path);
   m_argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'1'" << "'-outfile'" << "myelinatedWM"; 
   execute();

   m_unnecessaryFiles << myelinatedWM_path;

   // WM
   QString WM_name = m_prefix + "WM" + m_suffix + ".nrrd";
   QString WM_path = m_module_dir->filePath(WM_name); 
   
   m_log = "- Extracting the white matter";
   m_outputs.insert("WM", WM_path);
   m_argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'2'" << "'-outfile'" << "WM"; 
   execute();

   m_unnecessaryFiles << WM_path;

   // GM
   QString GM_name = m_prefix + "GM" + m_suffix + ".nrrd";
   QString GM_path = m_module_dir->filePath(GM_name); 

   m_log = "- Extracting the gray matter";
   m_outputs.insert("GM", GM_path); 
   m_argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'3'" << "'-outfile'" << "GM"; 
   execute(); 

   m_unnecessaryFiles << GM_path;

   QString multipliedGM_name = m_prefix + "GM-multiplied" + m_suffix + ".nrrd";
   QString multipliedGM_path = m_module_dir->filePath(multipliedGM_name); 

   m_log = "- Multiplying the gray matter by 2";
   m_outputs.insert("GM_multiplied", multipliedGM_path);
   m_argumentsList << "ImageMath" << "GM" << "'-constOper'" << "'2,2'" << "'-outfile'" << "GM_multiplied"; 
   execute();

   m_unnecessaryFiles << multipliedGM_path;
 
   // CSF
   QString CSF_name = m_prefix + "CSF.nrrd";
   QString CSF_path = m_module_dir->filePath(CSF_name); 

   m_log = "- Extracting the CSF";
   m_outputs.insert("CSF", CSF_path); 
   m_argumentsList << "ImageMath" << "seg_4Labels" << "'-extractLabel'" << "'4'" << "'-outfile'" << "CSF"; 
   execute();

   m_unnecessaryFiles << CSF_path;

   QString multipliedCSF_name = m_prefix + "CSF-multiplied" + m_suffix + ".nrrd";
   QString multipliedCSF_path = m_module_dir->filePath(multipliedCSF_name); 

   m_log = "- Multiplying the CSF by 3";
   m_outputs.insert("CSF_multiplied", multipliedCSF_path); 
   m_argumentsList << "ImageMath" << "CSF" << "'-constOper'" << "'2,3'" << "'-outfile'" << "CSF_multiplied"; 
   execute();

   m_unnecessaryFiles << multipliedCSF_path;

   // 3-labels Seg 
   QString mergedWM_name = m_prefix + "WM-merged" + m_suffix + ".nrrd";
   QString mergedWM_path = m_module_dir->filePath(mergedWM_name); 

   m_log = "- Adding myelinated WM and WM";
   m_outputs.insert("WM_merged", mergedWM_path); 
   m_argumentsList << "ImageMath" << "WM" << "'-add'" << "myelinatedWM" << "'-outfile'" << "WM_merged"; 
   execute();   

   m_unnecessaryFiles << mergedWM_path;

   QString tempSeg3Labels_name = m_prefix + "seg-3Labels-temp" + m_suffix + ".nrrd";
   QString tempSeg3Labels_path = m_module_dir->filePath(tempSeg3Labels_name); 

   m_log = "- Adding GM";
   m_outputs.insert("seg3Labels_temp", tempSeg3Labels_path); 
   m_argumentsList << "ImageMath" << "WM_merged" << "'-add'" << "GM_multiplied" << "'-outfile'" << "seg3Labels_temp"; 
   execute();

   m_unnecessaryFiles << tempSeg3Labels_path;

   QString seg3Labels_name = m_prefix + "seg-3Labels" + m_suffix + ".nrrd";
   QString seg3Labels_path = m_module_dir->filePath(seg3Labels_name); 

   m_log = "- Adding CSF";
   m_outputs.insert("seg3Labels", seg3Labels_path); 
   m_argumentsList << "ImageMath" << "seg3Labels_temp" << "'-add'" << "CSF_multiplied" << "'-outfile'" << "seg3Labels"; 
   execute(); 

   m_neo.seg3Labels = seg3Labels_path;
}

void NeosegExecution::reassignWhiteMatter()
{
   QString m_XML_path = m_module_dir->filePath("parameters_reassignWhiteMatter.xml");
   m_script += "\tparameters_path = \"" + m_XML_path + "\"\n"; 

   m_script += "\tif checkFileExistence(parameters_path)==False:\n";   
   m_script += "\t\tlogger.info('- Writing the XML file...')\n";

   m_script += "\t\tparameters = Element('REASSIGN-WHITE-MATTER-PARAMETERS')\n";

   addSubElement(m_script, "parameters", "Input", "INPUT", m_neo.seg3Labels);

   addSubElement(m_script, "parameters", "Threshold", "THRESHOLD", QString::number(m_sizeThreshold));

   m_script += "\t\tprobabilityMaps = SubElement(parameters, 'PROBABILITY-MAPS')\n";
   QString white = QDir(m_atlas).filePath("white.nrrd");
   addSubElement(m_script, "probabilityMaps", "white", "WHITE", white);
   QString gray = QDir(m_atlas).filePath("gray.nrrd");
   addSubElement(m_script, "probabilityMaps", "gray", "GRAY", gray);
   QString csf = QDir(m_atlas).filePath("csf.nrrd");
   addSubElement(m_script, "probabilityMaps", "csf", "CSF", csf);

   QString output_name; 
   if(m_computing3LabelsSeg)
   {
      output_name = m_prefix + "seg-3labels-WMReassigned" + m_suffix + ".nrrd"; 
   }
   else
   {
      output_name = m_prefix + "seg-4labels-WMReassigned" + m_suffix + ".nrrd"; 
   }
   QString output = m_module_dir->filePath(output_name); 
   addSubElement(m_script, "parameters", "Output", "OUTPUT", output);

   m_script += "\t\tXML = xml.dom.minidom.parseString(ElementTree.tostring(parameters))\n";
   m_script += "\t\tpretty_xml_as_string = XML.toprettyxml()\n";

   m_script += "\t\tparameters = open(parameters_path, 'w')\n";
   m_script += "\t\tparameters.write(pretty_xml_as_string)\n";  
   m_script += "\t\tparameters.close()\n";
   
   m_script += "\telse:\n";
   m_script += "\t\tlogger.info('- Writing the XML file : Done ')\n";


   m_log = "- Reassigning white matter";
   m_outputs.insert("seg_WMReassigned", output);
   m_argumentsList << "ReassignWhiteMatter" << "parameters_path";
   execute(); 
   m_unnecessaryFiles << m_XML_path;

}

void NeosegExecution::implementRun()
{
   m_script += "def run():\n";

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   m_script += "\tlogger.info('=== Neoseg Execution ===')\n";
   
   QString seg_name;  
   if(m_computing3LabelsSeg && m_reassigningWhiteMatter)
   {
      seg_name = m_prefix + "3Labels-WMReassigned" + m_suffix + ".nrrd";
   }
   if(m_computing3LabelsSeg && !m_reassigningWhiteMatter)
   {
      seg_name = m_prefix + "3Labels" + m_suffix + ".nrrd";
   }
   if(!m_computing3LabelsSeg && m_reassigningWhiteMatter)
   {
      seg_name = m_prefix + "WMReassigned" + m_suffix + ".nrrd";
   }
   QString seg_path = m_module_dir->filePath(seg_name); 

   m_outputs.insert("finalSeg", seg_path); 
   checkFinalOutputs();

   m_script += "\tlogger.debug('')\n\n";

   m_script += "\t# Write XML File #\n";
   writeXMLFile();
   
   m_script += "\t# Write Affine Transformation File #\n";
   writeAffineTranformationFiles();

   m_script += "\t# Run Neoseg #\n";
   runNeoseg();

   if(m_computing3LabelsSeg)
   {
      m_script += "\t# Merge White Matters #\n";
      mergeWhiteMatters();
   }

   if(m_reassigningWhiteMatter)
   {
      m_script += "\t# Reassign White Matter #\n";
      reassignWhiteMatter(); 
   }

} 

void NeosegExecution::update()
{
   initializeScript();

   implementStop();
   implementCheckFileExistence();
   implementExecute();
   implementWriteAffineTranformationFile();
   implementRun();

   writeScript();
}

Neo NeosegExecution::getOutput() 
{
   return m_neo;
}



