#include "NeosegExecution.h"

NeosegExecution::NeosegExecution()
{
   m_Neoseg = "/work/mcherel/project/neoseg/bin/bin_lessRefinement/neoseg";
   m_suffix = "NP";

   m_usingFA = true; 
   m_usingAD = true; 
   
}

void NeosegExecution::setNeo(Neo neo) {m_neo = neo;}

void NeosegExecution::setAtlas(QString atlas) {m_atlas = atlas;}

void NeosegExecution::setNeosegParameters(NeosegParameters* parameters) {m_parameters = parameters;}

void NeosegExecution::setExecutablePaths(ExecutablePaths* executables) {m_executables=executables;}

void NeosegExecution::setOutputPath( QString output_path )
{
   m_output_dir = new QDir(output_path);
   m_neosegExecution_path = m_output_dir->filePath( "NeosegExecution" ); 
   m_neosegExecution_dir = new QDir(m_neosegExecution_path); 
}

void NeosegExecution::defineSegmentation()
{
   QFileInfo* T2 = new QFileInfo(m_neo.T2);
   QString segmentation_name = T2->baseName() + "_EMonly_labels_" + m_suffix + ".gipl"; 
   m_segmentation = m_neosegExecution_dir->filePath(segmentation_name);
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

void NeosegExecution::createDirectory()
{
   m_output_dir->mkpath( "NeosegExecution" );
   m_neosegExecution_path = m_output_dir->filePath( "NeosegExecution" );
   m_neosegExecution_dir = new QDir(m_neosegExecution_path);
}

QString NeosegExecution::str(QString str) {return "'" + str + "'";}

QString NeosegExecution::variable(QString variable_name) {return "\" + " + variable_name + " + \"";}

QString NeosegExecution::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

   return arguments; 
}

void NeosegExecution::implementLogStdoutAndStderr(QString &script)
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

void NeosegExecution::addSubElement(QString &script, QString element, QString pythonName, QString XMLname, QString value)
{
   script += "\t" + pythonName + " = SubElement(" + element + ", '" + XMLname + "')\n";
   script += "\t" + pythonName + ".text = '" + value + "'\n";
}

void NeosegExecution::writeXMLFile(QString &script)
{   
   m_XML_path = m_neosegExecution_dir->filePath("parameters.xml");
   script += "\tparameters_path = \"" + m_XML_path + "\"\n"; 

   script += "\tsegmentationParameters = Element('SEGMENTATION-PARAMETERS')\n";

   addSubElement(script, "segmentationParameters", "suffix", "SUFFIX", m_suffix);
   addSubElement(script, "segmentationParameters", "atlasDirectory", "ATLAS-DIRECTORY", m_atlas);
   addSubElement(script, "segmentationParameters", "atlasOrientation", "ATLAS-ORIENTATION", "RAI");
   addSubElement(script, "segmentationParameters", "atlasFormat", "ATLAS-FORMAT", "NRRD");
   addSubElement(script, "segmentationParameters", "outputDirectory", "OUTPUT-DIRECTORY", m_neosegExecution_path);
   addSubElement(script, "segmentationParameters", "outputFormat", "OUTPUT-FORMAT", "NRRD");

   script += "\tT2 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement(script,"T2", "T2File", "FILE", m_neo.T2);   
   addSubElement(script,"T2", "T2Orientation", "ORIENTATION", "RAI"); 

   script += "\tT1 = SubElement(segmentationParameters, 'IMAGE')\n";
   addSubElement(script,"T1", "T1File", "FILE", m_neo.T1);   
   addSubElement(script,"T1", "T1Orientation", "ORIENTATION", "RAI"); 

   if(m_usingFA)
   {
      script += "\tFA = SubElement(segmentationParameters, 'IMAGE')\n";
      addSubElement(script,"FA", "FAFile", "FILE", m_neo.FA);   
      addSubElement(script,"FA", "FAOrientation", "ORIENTATION", "RAI"); 
   }

   if(m_usingAD)
   {
      script += "\tAD = SubElement(segmentationParameters, 'IMAGE')\n";
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
//   addSubElement(script, "segmentationParameters", "referenceImageIndex", "REFERENCE-IMAGE-INDEX", QString::number(m_parameters->getReferenceImageIndex()));
   addSubElement(script, "segmentationParameters", "referenceModality", "REFERENCE-MODALITY", m_parameters->getReferenceImage());

   script += "\tXML = xml.dom.minidom.parseString(ElementTree.tostring(segmentationParameters))\n";
   script += "\tpretty_xml_as_string = XML.toprettyxml()\n";

   script += "\tparameters = open(parameters_path, 'w')\n";
   script += "\tparameters.write(pretty_xml_as_string)\n";  
   script += "\tparameters.close()\n\n";

}

void NeosegExecution::writeWriteAffineTranformationFile(QString& script)
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

   QString T2ToTemplate_name = T2_name + "_to_" + referenceTemplate_name + "_" + m_suffix + ".affine"; 
   QString T2ToTemplate_path = m_neosegExecution_dir->filePath(T2ToTemplate_name); 

   script += "\tT2ToTemplate = " + str(T2ToTemplate_path) + "\n";
   script += "\twriteAffineTransformationFile(T2ToTemplate)\n\n";

   QString T2ToT1_name = T2_name + "_to_" + T1_name + "_" + m_suffix + ".affine"; 
   QString T2ToT1_path = m_neosegExecution_dir->filePath(T2ToT1_name); 

   script += "\tT2ToT1 = " + str(T2ToT1_path) + "\n";
   script += "\twriteAffineTransformationFile(T2ToT1)\n\n";

   if(m_usingFA)
   {
      QString FA_name = (QFileInfo(m_neo.FA)).baseName(); 

      QString T2ToFA_name = T2_name + "_to_" + FA_name + "_" + m_suffix + ".affine"; 
      QString T2ToFA_path = m_neosegExecution_dir->filePath(T2ToFA_name); 

      script += "\tT2ToFA = " + str(T2ToFA_path) + "\n";
      script += "\twriteAffineTransformationFile(T2ToFA)\n\n";
   }

   if(m_usingAD)
   {
      QString AD_name = (QFileInfo(m_neo.AD)).baseName(); 

      QString T2ToAD_name = T2_name + "_to_" + AD_name + "_" + m_suffix + ".affine"; 
      QString T2ToAD_path = m_neosegExecution_dir->filePath(T2ToAD_name); 

      script += "\tT2ToAD = " + str(T2ToAD_path) + "\n";
      script += "\twriteAffineTransformationFile(T2ToAD)\n\n";
   }

}

void NeosegExecution::runNeoseg(QString &script)
{
   QStringList argumentsList;

   argumentsList << "Neoseg" << "parameters_path";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";
}


void NeosegExecution::implementExecuteNeoseg(QString &script)
{
   writeWriteAffineTranformationFile(script);
   implementLogStdoutAndStderr(script);

   script += "def run():\n";

   script += "\t# Write XML File #\n";
   writeXMLFile(script);
   
   script += "\t# Write Affine Transformation File #\n";
   writeAffineTranformationFiles(script);

   script += "\t# Run Neoseg #\n";
   runNeoseg(script);
} 



void NeosegExecution::initializeScript(QString &script)
{
   script += "#!/usr/bin/env python\n";

   // Import the librairies needed  
   script += "import os \n";
   script += "import sys \n";
   script += "import logging \n";
   script += "import subprocess \n\n";

   script += "from xml.etree import ElementTree\n";
   script += "from xml.etree.ElementTree import Element\n";
   script += "from xml.etree.ElementTree import SubElement\n";
   script += "import xml.dom.minidom\n\n";

   script += "Neoseg = " + str(m_Neoseg) + "\n";
} 

void NeosegExecution::writeExecuteNeoseg()
{
   QString executeNeoseg;

   initializeScript(executeNeoseg);

   implementExecuteNeoseg(executeNeoseg);

   QString script_path = m_output_dir->filePath("executeNeoseg.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << executeNeoseg.toStdString() << std::endl;
   script_stream->close();
}

void NeosegExecution::update()
{   
   createDirectory();
   writeExecuteNeoseg(); 
}

QString NeosegExecution::getOutput() { return m_segmentation; }



