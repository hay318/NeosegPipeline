#include "NeosegExecution.h"

NeosegExecution::NeosegExecution(QString module) : Script(module)
{
}
void NeosegExecution::setNewAtlas(bool newAtlas)
{
   m_newAtlas = newAtlas; 
}
void NeosegExecution::setExistingAtlas(QString atlas)
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
   defineExecutable("Neoseg");
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
void NeosegExecution::addInputImages()
{
   m_images.insert("T1", m_neo.T1); 
   m_images.insert("T2", m_neo.T2); 
   if(m_usingFA)
   {
      m_images.insert("FA", m_neo.FA); 
   }
   if(m_usingAD)
   {
      m_images.insert("AD", m_neo.AD); 
   }

   QDir atlas_dir(m_atlas);

   m_images.insert("templateT1", atlas_dir.filePath("templateT1.nrrd"));
   m_images.insert("templateT2", atlas_dir.filePath("templateT2.nrrd"));
   m_images.insert("white", atlas_dir.filePath("white.nrrd"));
   m_images.insert("gray", atlas_dir.filePath("gray.nrrd"));
   m_images.insert("csf", atlas_dir.filePath("csf.nrrd"));
   m_images.insert("rest", atlas_dir.filePath("rest.nrrd"));
}

void NeosegExecution::addOutputImages()
{
   QString T1corrected_name = QFileInfo(m_neo.T1).baseName() + "_corrected_" + m_neo.prefix + ".nrrd";  
   m_images.insert("T1_corrected", m_module_dir->filePath(T1corrected_name)); 
   
   QString T2corrected_name = QFileInfo(m_neo.T2).baseName() + "_corrected_" + m_neo.prefix + ".nrrd";  
   m_images.insert("T2_corrected", m_module_dir->filePath(T2corrected_name)); 

   QString basename = QFileInfo(m_neo.T2).baseName();

   QString template_name = basename + "_template_" + m_neo.prefix + ".nrrd";  
   m_images.insert("template", m_module_dir->filePath(template_name));  

   QString labels_name = basename + "_EMonly_labels_" + m_neo.prefix + ".nrrd";  
   m_images.insert("labels", m_module_dir->filePath(labels_name)); 

   QString posterior0_name = basename + "_EMonly_posterior0_" + m_neo.prefix + ".nrrd";  
   m_images.insert("posterior0", m_module_dir->filePath(posterior0_name)); 

   QString posterior1_name = basename + "_EMonly_posterior1_" + m_neo.prefix + ".nrrd";  
   m_images.insert("posterior1", m_module_dir->filePath(posterior1_name)); 

   QString posterior2_name = basename + "_EMonly_posterior2_" + m_neo.prefix + ".nrrd";  
   m_images.insert("posterior2", m_module_dir->filePath(posterior2_name)); 

   QString posterior3_name = basename + "_EMonly_posterior3_" + m_neo.prefix + ".nrrd";  
   m_images.insert("posterior3", m_module_dir->filePath(posterior3_name)); 
}

void NeosegExecution::changeOrigin(int origin[3])
{
   QMap<QString, QString>::iterator it;

   for(it = m_images.begin(); it != m_images.end(); ++it)
   {
      m_log = "- Change origin to " + QString::number(origin[0]) + "," + QString::number(origin[1]) + "," + QString::number(origin[2]);
      m_inputs.insert(it.key(), it.value());
      m_argumentsList << "ImageMath" << it.key() << "'-changeOrig'" << "'" + QString::number(origin[0]) + "," + QString::number(origin[1]) + "," + QString::number(origin[2]) + "'" << "'-outfile'" << it.key(); 
      execute(); 
   }
}

void NeosegExecution::writeXMLFile()
{   
   QString XML_path = m_module_dir->filePath("parameters.xml");
   m_script += "\tparameters_path = \"" + XML_path + "\"\n"; 

   m_script += "\tif checkFileExistence(parameters_path)==False:\n";   
   
   m_script += "\t\tlogger.info('Writing the XML file...')\n";  
   m_script += "\t\tsegmentationParameters = Element('SEGMENTATION-PARAMETERS')\n";

   addSubElement("segmentationParameters", "suffix", "SUFFIX", m_neo.prefix);
   addSubElement("segmentationParameters", "atlasDirectory", "ATLAS-DIRECTORY", m_atlas);
   addSubElement("segmentationParameters", "atlasOrientation", "ATLAS-ORIENTATION", "RAI");
   addSubElement("segmentationParameters", "atlasFormat", "ATLAS-FORMAT", "NRRD");
   addSubElement("segmentationParameters", "outputDirectory", "OUTPUT-DIRECTORY", m_module_path);
   addSubElement("segmentationParameters", "outputFormat", "OUTPUT-FORMAT", "NRRD");

   m_script += "\t\tT2 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement("T2", "T2File", "FILE", m_neo.T2);   
   addSubElement("T2", "T2Orientation", "ORIENTATION", "RAI"); 

   m_script += "\t\tT1 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement("T1", "T1File", "FILE", m_neo.T1);   
   addSubElement("T1", "T1Orientation", "ORIENTATION", "RAI");
 
   if(m_usingFA)
   {
      m_script += "\t\tFA = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement("FA", "FAFile", "FILE", m_neo.FA);   
      addSubElement("FA", "FAOrientation", "ORIENTATION", "RAI"); 
   }


   if(m_usingAD)
   {
      m_script += "\t\tAD = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement("AD", "ADFile", "FILE", m_neo.AD);   
      addSubElement("AD", "ADOrientation", "ORIENTATION", "RAI"); 
   }

   addSubElement("segmentationParameters", "filterIterations", "FILTER-ITERATIONS", QString::number(m_parameters->getNumberOfIterations()));
   addSubElement("segmentationParameters", "filterTimeStep", "FILTER-TIME-STEP", QString::number(m_parameters->getTimeStep()));
   addSubElement("segmentationParameters", "filterMethod", "FILTER-METHOD", m_parameters->getFilterMethod());
   addSubElement("segmentationParameters", "maxBiasDegree", "MAX-BIAS-DEGREE", QString::number(m_parameters->getMaxBiasDegree()));
   addSubElement("segmentationParameters", "prior1", "PRIOR-1", QString::number(m_parameters->getPrior1()));
   addSubElement("segmentationParameters", "prior2", "PRIOR-2", QString::number(m_parameters->getPrior2()));
   addSubElement("segmentationParameters", "prior3", "PRIOR-3", QString::number(m_parameters->getPrior3()));
   addSubElement("segmentationParameters", "prior4", "PRIOR-4", QString::number(m_parameters->getPrior4()));
   addSubElement("segmentationParameters", "prior5", "PRIOR-5", QString::number(m_parameters->getPrior5()));

   addSubElement("segmentationParameters", "doAtlasWarp", "DO-ATLAS-WARP", QString::number(0));
   addSubElement("segmentationParameters", "atlasWarpGridX", "ATLAS-WARP-GRID-X", QString::number(5));
   addSubElement("segmentationParameters", "atlasWarpGridY", "ATLAS-WARP-GRID-Y", QString::number(5));
   addSubElement("segmentationParameters", "atlasWarpGridZ", "ATLAS-WARP-GRID-Z", QString::number(5));

   addSubElement("segmentationParameters", "mahalanobisThreshold", "MAHALANOBIS-THRESHOLD", QString::number(4));
   addSubElement("segmentationParameters", "parzenKernelWidth", "PARZEN-KERNEL-WIDTH", QString::number(m_parameters->getInitialParzenKernelWidth()));
   addSubElement("segmentationParameters", "priorThreshold", "PRIOR-THRESHOLD", QString::number(m_parameters->getPriorThreshold()));

   QString referenceImageIndex; 

   if(m_parameters->getReferenceImage()=="T2")
   {
      referenceImageIndex = "1";   
   }    
   if(m_parameters->getReferenceImage()=="T1")
   {
      referenceImageIndex = "2";   
   }    

   addSubElement("segmentationParameters", "referenceImageIndex", "REFERENCE-IMAGE-INDEX", referenceImageIndex);
   addSubElement("segmentationParameters", "referenceModality", "REFERENCE-MODALITY", m_parameters->getReferenceImage());

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

   QString T2ToTemplate_name = T2_name + "_to_" + referenceTemplate_name + "_" + m_neo.prefix + ".affine"; 
   QString T2ToTemplate_path = m_module_dir->filePath(T2ToTemplate_name); 

   m_script += "\tT2ToTemplate = '" + T2ToTemplate_path + "'\n";

   m_script += "\tif checkFileExistence(T2ToTemplate)==False:\n";   
   m_script += "\t\twriteAffineTransformationFile(T2ToTemplate)\n\n";

   QString T2ToT1_name = T2_name + "_to_" + T1_name + "_" + m_neo.prefix + ".affine"; 
   QString T2ToT1_path = m_module_dir->filePath(T2ToT1_name); 

   m_script += "\tT2ToT1 = '" + T2ToT1_path + "'\n";
   m_script += "\tif checkFileExistence(T2ToT1)==False:\n";  
   m_script += "\t\twriteAffineTransformationFile(T2ToT1)\n\n";

   if(m_usingFA)
   {
      QString FA_name = (QFileInfo(m_neo.FA)).baseName(); 

      QString T2ToFA_name = T2_name + "_to_" + FA_name + "_" + m_neo.prefix + ".affine"; 
      QString T2ToFA_path = m_module_dir->filePath(T2ToFA_name); 

      m_script += "\tT2ToFA = '" + T2ToFA_path + "'\n";
      m_script += "\tif checkFileExistence(T2ToFA)==False:\n";  
      m_script += "\t\twriteAffineTransformationFile(T2ToFA)\n\n";
   }

   if(m_usingAD)
   {
      QString AD_name = (QFileInfo(m_neo.AD)).baseName(); 

      QString T2ToAD_name = T2_name + "_to_" + AD_name + "_" + m_neo.prefix + ".affine"; 
      QString T2ToAD_path = m_module_dir->filePath(T2ToAD_name); 

      m_script += "\tT2ToAD = '" + T2ToAD_path + "'\n";
      m_script += "\tif checkFileExistence(T2ToAD)==False:\n";  
      m_script += "\t\twriteAffineTransformationFile(T2ToAD)\n\n";
   }
}

void NeosegExecution::runNeoseg()
{
   QFileInfo* T2 = new QFileInfo(m_neo.T2);
   QString segmentation_name = T2->baseName() + "_EMonly_labels_" + m_neo.prefix + ".nrrd"; 
   QString segmentation_path = m_module_dir->filePath(segmentation_name);

   m_log = "Running Neoseg";
   m_outputs.insert("seg_4Labels", segmentation_path); 
   m_argumentsList << "Neoseg" << "parameters_path";
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

   addSubElement("parameters", "Input", "INPUT", m_neo.seg3Labels);

   addSubElement("parameters", "Threshold", "THRESHOLD", QString::number(m_sizeThreshold));

   m_script += "\t\tprobabilityMaps = SubElement(parameters, 'PROBABILITY-MAPS')\n";
   QString white = QDir(m_atlas).filePath("white.nrrd");
   addSubElement("probabilityMaps", "white", "WHITE", white);
   QString gray = QDir(m_atlas).filePath("gray.nrrd");
   addSubElement("probabilityMaps", "gray", "GRAY", gray);
   QString csf = QDir(m_atlas).filePath("csf.nrrd");
   addSubElement("probabilityMaps", "csf", "CSF", csf);

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
   addSubElement("parameters", "Output", "OUTPUT", output);

   m_script += "\t\tXML = xml.dom.minidom.parseString(ElementTree.tostring(parameters))\n";
   m_script += "\t\tpretty_xml_as_string = XML.toprettyxml()\n";

   m_script += "\t\tparameters = open(parameters_path, 'w')\n";
   m_script += "\t\tparameters.write(pretty_xml_as_string)\n";  
   m_script += "\t\tparameters.close()\n";
   
   m_script += "\telse:\n";
   m_script += "\t\tlogger.info('- Writing the XML file : Done ')\n";


   m_log = "- Reassigning white matter";
   m_outputs.insert("seg_WMReassigned", output);
   m_argumentsList << "ReassignWhiteMatter" << "'--parameters'" << "parameters_path";
   execute(); 
   m_unnecessaryFiles << m_XML_path;

   m_neo.seg3Labels = output;

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
      seg_name = m_prefix + "seg-3Labels-WMReassigned" + m_suffix + ".nrrd";
   }
   if(m_computing3LabelsSeg && !m_reassigningWhiteMatter)
   {
      seg_name = m_prefix + "seg-3Labels" + m_suffix + ".nrrd";
   }
   if(!m_computing3LabelsSeg && m_reassigningWhiteMatter)
   {
      seg_name = m_prefix + "seg-WMReassigned" + m_suffix + ".nrrd";
   }
   QString seg_path = m_module_dir->filePath(seg_name); 

   m_outputs.insert("finalSeg", seg_path); 
   checkFinalOutputs();

   m_script += "\tlogger.debug('')\n\n";

   addInputImages(); 
 
   if(m_neo.origin[0] != 0 || m_neo.origin[1] != 0 || m_neo.origin[2] != 0)
   {
      m_script += "\t# Change Origin #\n";
      int origin[3] = {0,0,0};
      changeOrigin(origin);
   }

   m_script += "\t# Write XML File #\n";
   writeXMLFile();

   m_script += "\t# Write Affine Transformation File #\n";
   writeAffineTranformationFiles();
 
   m_script += "\t# Run Neoseg #\n";
   runNeoseg();

   if(m_neo.origin[0] != 0 || m_neo.origin[1] != 0 || m_neo.origin[2] != 0)
   {
      addOutputImages(); 

      m_script += "\t# Change Origin #\n";
      changeOrigin(m_neo.origin);
   }

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



