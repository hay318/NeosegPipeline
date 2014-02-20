#include "AtlasGeneration.h"



AtlasGeneration::AtlasGeneration()
{
   m_white.name="white";
   m_gray.name="gray";
   m_csf.name="csf";
   m_rest.name="rest";
   m_templateT1.name="templateT1";
   m_templateT2.name="templateT2";
}

void AtlasGeneration::setNeo(Neo neo) {m_neo=neo;}

void AtlasGeneration::setAtlasPopulation(std::vector<Atlas> atlasPopulation) {m_atlasPopulation=atlasPopulation;}

void AtlasGeneration::setOutputPath( QString outputPath ) 
{
   m_output_dir = new QDir(outputPath); 

   m_atlasGeneration_path = m_output_dir->filePath("AtlasGeneration");
   m_atlasGeneration_dir = new QDir(m_atlasGeneration_path);
}

void AtlasGeneration::setSmoothing(QString smoothing) {m_smoothing=smoothing;}

void AtlasGeneration::setSmoothingSize(double smoothingSize) {m_smoothingSize=smoothingSize;}

void AtlasGeneration::setComputingWeights(bool computingWeights) {m_computingWeights=computingWeights;}

void AtlasGeneration::setNeightborhoodRadius(double neightborhoodRadius) {m_neightborhoodRadius=neightborhoodRadius;}

void AtlasGeneration::setExecutablePaths(ExecutablePaths* executables) {m_executables=executables;}


void AtlasGeneration::defineGeneratedAtlas()
{
    m_white.finalImage = m_atlasGeneration_dir->filePath("white.nrrd"); 
    m_gray.finalImage = m_atlasGeneration_dir->filePath("gray.nrrd"); 
    m_csf.finalImage = m_atlasGeneration_dir->filePath("csf.nrrd"); 
    m_rest.finalImage = m_atlasGeneration_dir->filePath("rest.nrrd"); 
    m_templateT1.finalImage = m_atlasGeneration_dir->filePath("templateT1.nrrd"); 
    m_templateT2.finalImage = m_atlasGeneration_dir->filePath("templateT2.nrrd");    
}

bool AtlasGeneration::checkGeneratedAtlas()
{
   QFile* white_file = new QFile( m_white.finalImage );
   QFile* gray_file = new QFile( m_gray.finalImage );
   QFile* csf_file = new QFile( m_csf.finalImage );
   QFile* rest_file = new QFile( m_rest.finalImage );
   QFile* templateT1_file = new QFile( m_templateT1.finalImage );
   QFile* templateT2_file = new QFile( m_templateT2.finalImage ); 

   if ( white_file->exists() && gray_file->exists() && csf_file->exists() && rest_file->exists() && templateT1_file->exists() && templateT2_file->exists() )
   {
      return true;
   } 
   else
   {
      return false; 
   }
}


void AtlasGeneration::createDirectories()
{
   m_output_dir->mkpath("AtlasGeneration");
   
   m_atlasGeneration_dir->mkpath( "temp/templates/templateT1" );
   QString templateT1 = m_atlasGeneration_dir->filePath( "temp/templates/templateT1" );
   m_templateT1.dir = new QDir( templateT1 );  
 
   m_atlasGeneration_dir->mkpath( "temp/templates/templateT2" );
   QString templateT2 = m_atlasGeneration_dir->filePath( "temp/templates/templateT2" );
   m_templateT2.dir = new QDir( templateT2 );  

   m_atlasGeneration_dir->mkpath( "temp/priorProbabilities/white" );
   QString white_path = m_atlasGeneration_dir->filePath( "temp/priorProbabilities/white" );
   m_white.dir = new QDir( white_path );  
 
   m_atlasGeneration_dir->mkpath( "temp/priorProbabilities/gray" );
   QString gray_path = m_atlasGeneration_dir->filePath( "temp/priorProbabilities/gray" );
   m_gray.dir = new QDir( gray_path );  
 
   m_atlasGeneration_dir->mkpath( "temp/priorProbabilities/csf" );
   QString csf_path = m_atlasGeneration_dir->filePath( "temp/priorProbabilities/csf" );
   m_csf.dir = new QDir( csf_path );  
 
   m_atlasGeneration_dir->mkpath( "temp/priorProbabilities/rest" );
   QString rest_path = m_atlasGeneration_dir->filePath( "temp/priorProbabilities/rest" );
   m_rest.dir = new QDir( rest_path ); 

   m_atlasGeneration_dir -> mkpath("temp/priorProbabilities");
   QString priorProbabilities_path = m_atlasGeneration_dir->filePath("temp/priorProbabilities");
   m_priorProbabilities_dir = new QDir(priorProbabilities_path);  
}

void AtlasGeneration::initializeScript(QString &script)
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

   script += "ImageMath = " + str(m_executables->getExecutablePath("ImageMath")) + "\n";
   script += "ResampleVolume2 = " + str(m_executables->getExecutablePath("ResampleVolume2")) + "\n";
   script += "unu = " + str(m_executables->getExecutablePath("unu")) + "\n";
   script += "SpreadFA = '/work/mcherel/project/neosegPipeline/spreadFA/bin/SpreadFA'\n";
   script += "log = '/work/mcherel/project/DTI/atlases/T0287-1-1/neosegPipeline/test.log'\n\n";

} 

QString AtlasGeneration::getImage( Atlas atlas, QString name )
{
   if( name == "templateT1" )
   {
      return atlas.T1;
   }
  
   if( name == "templateT2" )
   {
      return atlas.T2;
   }

   return NULL;

}

QString AtlasGeneration::variable(QString variable_name) {return "\" + " + variable_name + " + \"";}

QString AtlasGeneration::str(QString str) {return "\"" + str + "\"";}

QString AtlasGeneration::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

   return arguments; 
}

void AtlasGeneration::implementLogStdoutAndStderr(QString &script)
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

void AtlasGeneration::implementLogStderr(QString &script)
{
   script += "def logStderr(runningProcess):\n";

   script += "\twhile True:\n";
   script += "\t\tline = runningProcess.stderr.readline()\n";
   script += "\t\tif line:\n";
   script += "\t\t\tlogging.error(line.rstrip())\n";
   script += "\t\tif not line: break\n";

   script += "\trunningProcess.wait()\n\n";
}

void AtlasGeneration::generateTemplate(QString &script, TemplateImage templateImage)
{
   QStringList argumentsList;

   QString ref = getImage(m_atlasPopulation[1], templateImage.name);
   script += "\tref = " + str(ref) + "\n\n";

   argumentsList << "ImageMath" << "ref" << "'-avg'";

   int i=1;
   std::vector<Atlas>::iterator it;
   for (it=m_atlasPopulation.begin()+1; it!=m_atlasPopulation.end(); ++it)
   {
      QString atlas = "atlas_" + QString::number(i);
      script += "\t" + atlas + " = " + str(getImage(*it, templateImage.name)) + "\n"; 
      argumentsList << atlas; 
      i++; 
   }

   script += "\taverage = " + str(templateImage.finalImage) + "\n\n";

   argumentsList << "'-outfile'" << "average";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   script += "\tlogStderr(runningProcess)\n\n";
}


void AtlasGeneration::addSubElement(QString &script, QString element, QString pythonName, QString XMLname, QString value)
{
   script += "\t" + pythonName + " = SubElement(" + element + ", '" + XMLname + "')\n";
   script += "\t" + pythonName + ".text = '" + value + "'\n";
}


void AtlasGeneration::generateWeightedAveragedLabels( QString &script )
{
   QStringList argumentsList;

   QString WeightedLabelsAverage = "/work/mcherel/project/neosegPipeline/weightedLabelsAverage/bin/WeightedLabelsAverage";
   script += "\tWeightedLabelsAverage = " + str(WeightedLabelsAverage) + "\n\n";

   QString m_XML_path = m_atlasGeneration_dir->filePath("parameters.xml");
   script += "\tparameters_path = \"" + m_XML_path + "\"\n"; 

   m_white.image = m_white.dir->filePath( m_white.name + "_average.nrrd" );
   m_gray.image = m_gray.dir->filePath( m_gray.name + "_average.nrrd" );
   m_csf.image = m_csf.dir->filePath( m_csf.name + "_average.nrrd" );

   script += "\tparameters = Element('WEIGHTED-AVERAGED-LABELS-PARAMETERS')\n";
   addSubElement(script, "parameters", "input", "INPUT", m_neo.T1);
   script += "\tatlasPopulation = SubElement(parameters, 'ATLAS-POPULATION')\n";

   std::vector<Atlas>::iterator it;
   Atlas atlas;

   for (it=m_atlasPopulation.begin(); it!=m_atlasPopulation.end(); ++it)
   {
      atlas = *it;

      if(atlas.probabilistic)
      {
         script += "\tatlas = SubElement(atlasPopulation, 'PROBABILISTIC-ATLAS')\n";
         addSubElement(script, "atlas", "image", "IMAGE", (*it).T2); 
         addSubElement(script, "atlas", "seg", "WHITE", (*it).white);
         addSubElement(script, "atlas", "seg", "GRAY", (*it).gray);
         addSubElement(script, "atlas", "seg", "CSF", (*it).csf);
      }

      else
      {
         script += "\tatlas = SubElement(atlasPopulation, 'ATLAS')\n";
         addSubElement(script, "atlas", "image", "IMAGE", (*it).T2); 
         addSubElement(script, "atlas", "seg", "SEG", (*it).seg);
      }
   }
   
   script += "\tweights = SubElement(parameters, 'WEIGHTS')\n";
   addSubElement(script, "weights", "computingWeights", "COMPUTING-WEIGHTS", QString::number(m_computingWeights));
   addSubElement(script, "weights", "neightborhoodRadius", "NEIGHTBORHOOD-RADIUS", QString::number(m_neightborhoodRadius));

   script += "\toutputs = SubElement(parameters, 'OUTPUTS')\n";
   addSubElement(script, "outputs", "white", "WHITE-AVERAGE", m_white.image);
   addSubElement(script, "outputs", "gray", "GRAY-AVERAGE", m_gray.image);
   addSubElement(script, "outputs", "csf", "CSF-AVERAGE", m_csf.image);

   script += "\tXML = xml.dom.minidom.parseString(ElementTree.tostring(parameters))\n";
   script += "\tpretty_xml_as_string = XML.toprettyxml()\n";

   script += "\tparameters = open(parameters_path, 'w')\n";
   script += "\tparameters.write(pretty_xml_as_string)\n";  
   script += "\tparameters.close()\n";
   
   argumentsList << "WeightedLabelsAverage" << "parameters_path";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";
}

void AtlasGeneration::extractWMFromFA(QString &script)
{
   QStringList argumentsList;

   m_white.dir->mkpath("FA");
   QString FA_path = m_white.dir->filePath("FA");
   QDir* FA_dir = new QDir(FA_path);  

   script += "\tFA = " + str(m_neo.FA) + "\n\n";


   QString rescaledFA = FA_dir->filePath("FA-rescaled.nrrd");
   script += "\trescaledFA = " + str(rescaledFA) + "\n";
 
   argumentsList.clear();
   argumentsList << "ImageMath" << "FA" << "'-rescale'"<< "'0,255'" << "'-outfile'" << "rescaledFA";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   script += "\tlogStderr(runningProcess)\n\n";

   QString spreadFA = FA_dir->filePath("FA-spread.nrrd");
   script += "\tspreadFA = " + str(spreadFA) + "\n";
 
   argumentsList.clear();
   argumentsList << "SpreadFA" << "rescaledFA" << "spreadFA";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   script += "\tlogStderr(runningProcess)\n\n";

   QString smoothedFA = FA_dir->filePath("FA-smoothed.nrrd");
   script += "\tsmoothedFA = " + str(smoothedFA) + "\n";
 
   argumentsList.clear();
   argumentsList << "ImageMath" << "spreadFA" << "'-smooth'"<< "'-gauss'" << "'-size'" << "'1'" << "'-type'" << "'float'" << "'-outfile'" << "smoothedFA";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";


   script += "\tmask = " + str(m_neo.mask) + "\n";    

   QString erodedMask = FA_dir->filePath("erodedMask.nrrd");
   script += "\terodedMask = " + str(erodedMask) + "\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "mask" << "'-erode'"<< "'2,1'" << "'-outfile'" << "erodedMask";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   script += "\tlogStderr(runningProcess)\n\n";


   QString maskedFA = FA_dir->filePath("FA-masked.nrrd");
   script += "\tmaskedFA = " + str(maskedFA) + "\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "smoothedFA" << "'-mask'"<< "erodedMask" << "'-type'" << "'float'" << "'-outfile'" << "maskedFA";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   script += "\twhite_average = " + str(m_white.image) + "\n";

   QString FAMultipliedByWeight = FA_dir->filePath("FAMultipliedByWeight.nrrd");
   script += "\tFAMultipliedByWeight = " + str(FAMultipliedByWeight) + "\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "maskedFA" << "'-constOper'" << "'2,1.5'" << "'-type'" << "'float'" << "'-outfile'" << "FAMultipliedByWeight";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   QString whitePlusFA = m_white.dir->filePath("whitePlusFA.nrrd");
   script += "\twhitePlusFA = " + str(whitePlusFA) + "\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "white_average" << "'-add'" << "FAMultipliedByWeight" << "'-type'" << "'float'" << "'-outfile'" << "whitePlusFA";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   m_white.image = whitePlusFA;
}

void AtlasGeneration::generatePriorProbability(QString &script, PriorProbability& priorProbability)
{
   QStringList argumentsList;

   QString smoothing = "'-" + m_smoothing + "'";
   QString smoothingSize = "'" + QString::number(m_smoothingSize) + "'";

   script += "\t" + priorProbability.name + "_average = " + str(priorProbability.image) + "\n\n";

   QString probability = priorProbability.dir->filePath( priorProbability.name + "_probability.nrrd" );
   script += "\t" + priorProbability.name + "_probability = " + str(probability) + "\n";

   argumentsList << "ImageMath" << priorProbability.name + "_average" << "'-smooth'" << smoothing << "'-size'" << smoothingSize << "'-type'" << "'float'" << "'-outfile'" << priorProbability.name + "_probability";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE) \n";
   script += "\tlogStdoutAndStderr(runningProcess)\n\n";

   QString maskedProbability = priorProbability.dir->filePath( priorProbability.name + "_maskedProbability.nrrd" );
   script += "\t" + priorProbability.name + "_maskedProbability = " + str(maskedProbability) + "\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << priorProbability.name + "_probability" << "'-mask'"<< "mask" << "'-type'" << "'float'" << "'-outfile'" << priorProbability.name + "_maskedProbability";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";
   
   priorProbability.image = maskedProbability;
}

void AtlasGeneration::preNormalizePriorProbability(QString &script, PriorProbability priorProbability)
{
   QStringList argumentsList;

   argumentsList << "unu" << "'2op'" << "'/'" << priorProbability.name + "_maskedProbability" << "sumProbabilities" << "'-t'" << "'float'" << "'-o'" << priorProbability.name + "_preNormalizedProbability";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << priorProbability.name + "_preNormalizedProbability" << "'-constOper'"<< "'2,255'" << "'-outfile'" << priorProbability.name + "_preNormalizedProbability";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";
}


void AtlasGeneration::preNormalizePriorProbabilities(QString &script)
{
   QStringList argumentsList;

   QString sumProbabilities = m_priorProbabilities_dir->filePath("sumProbabilities.nrrd");
   script += "\tsumProbabilities = " + str(sumProbabilities) + "\n\n";

   QString preNormalizedWhite = m_white.dir->filePath("white_preNormalizedProbability.nrrd"); 
   QString preNormalizedGray = m_gray.dir->filePath("gray_preNormalizedProbability.nrrd"); 
   QString preNormalizedCsf = m_csf.dir->filePath("csf_preNormalizedProbability.nrrd"); 

   script += "\twhite_preNormalizedProbability = " + str(preNormalizedWhite) + "\n";
   script += "\tgray_preNormalizedProbability = " + str(preNormalizedGray) + "\n";
   script += "\tcsf_preNormalizedProbability = " + str(preNormalizedCsf) + "\n\n";

   argumentsList << "unu" << "'3op'" << "'+'" << "white_maskedProbability" << "gray_maskedProbability" << "csf_maskedProbability" << "'-t'" << "'float'" << "'-o'" << "sumProbabilities";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   argumentsList.clear();
   argumentsList << "unu" << "'2op'" << "'if'" << "sumProbabilities" << "'1'" << "'-t'" << "'float'" << "'-o'" << "sumProbabilities";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   preNormalizePriorProbability(script, m_white);
   preNormalizePriorProbability(script, m_gray);
   preNormalizePriorProbability(script, m_csf);
}

void AtlasGeneration::computeRest(QString &script)
{
   QStringList argumentsList;

   QString priorProbabilities = m_white.image + "," + m_gray.image + "," + m_csf.image; 
   
   script += "\ttemplateT1 = " + str(m_templateT1.finalImage) + "\n"; 
   script += "\tpriorProbabilities = white_preNormalizedProbability + \",\" + gray_preNormalizedProbability + \",\" + csf_preNormalizedProbability\n";

   QString outbase = m_rest.dir->filePath("label"); 
   script += "\toutbase = " + str(outbase) + "\n";

   argumentsList << "ImageMath" << "templateT1" << "'-normalizeEMS'"<< "'3'" << "'-EMSfile'" << "priorProbabilities" << "'-type'" << "'float'" << "'-extension'" << "'.nrrd'" << "'-outbase'" << "outbase";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   m_white.image = m_rest.dir->filePath("label1_normalized.nrrd");
   m_gray.image = m_rest.dir->filePath("label2_normalized.nrrd");
   m_csf.image = m_rest.dir->filePath("label3_normalized.nrrd");
   m_rest.image = m_rest.dir->filePath("label4_normalized.nrrd");

   script += "\twhite_normalizedProbability = " + str(m_white.image) + "\n";
   script += "\tgray_normalizedProbability = " + str(m_gray.image) + "\n";
   script += "\tcsf_normalizedProbability = " + str(m_csf.image) + "\n";
   script += "\trest_normalizedProbability = " + str(m_rest.image) + "\n\n";

   script += "\t# Mask Rest Probability #\n";

   script += "\tbrainMask = " + str(m_neo.mask) + "\n";

   QString dilatedMask = m_priorProbabilities_dir->filePath("dilatedMask.nrrd");
   script += "\tdilatedMask = " + str(dilatedMask) + "\n";
   argumentsList.clear();

   argumentsList << "ImageMath" << "brainMask" << "'-dilate'"<< "'5,1'" << "'-outfile'" << "dilatedMask";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

   script += "\trest = " + str(m_rest.image) + "\n"; 
   QString restMasked = m_rest.dir->filePath("restMasked.nrrd");
   script += "\trestMasked = " + str(restMasked) + "\n";
 
   argumentsList.clear();
   argumentsList << "ImageMath" << "rest" << "'-mask'"<< "dilatedMask" << "'-outfile'" << "restMasked";
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";

}

void AtlasGeneration::copyFinalPriorProbability(QString &script, PriorProbability& priorProbability)
{
   QStringList argumentsList;

   script += "\t" + priorProbability.name + "_finalProbability = " + str(priorProbability.finalImage) + "\n";
  
   argumentsList << "ResampleVolume2" << priorProbability.name + "_normalizedProbability" << priorProbability.name + "_finalProbability" ;
   script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE) \n";
   script += "\tlogStderr(runningProcess)\n\n";
} 

void AtlasGeneration::implementGenerateAtlas(QString &script)
{
   script += "def run():\n";

   script += "\t### TEMPLATES ###\n";

   script += "\t# Compute Template T1 #\n";
   generateTemplate(script, m_templateT1);

   script += "\t# Compute Template T2 #\n";
   generateTemplate(script, m_templateT2);


   script += "\t### PRIOR PROBABILITIES ###\n";

   script += "\t# Compute Tissue Weighted Averages #\n";
   generateWeightedAveragedLabels(script);

   script += "\t# Extract WM From FA #\n";
   extractWMFromFA(script);

   script += "\t# Compute White Probability #\n";
   generatePriorProbability(script, m_white);

   script += "\t# Compute Gray Probability #\n";
   generatePriorProbability(script, m_gray);

   script += "\t# Compute CSF Probability #\n";   
   generatePriorProbability(script, m_csf);

   script += "\t# Pre-Normalize Prior Probabilities #\n";
   preNormalizePriorProbabilities(script);

   script += "\t# Compute Rest Probability #\n"; 
   computeRest(script); 

   script += "\t# Copy White Probability #\n";
   copyFinalPriorProbability(script, m_white);

   script += "\t# Copy Gray Probability #\n";
   copyFinalPriorProbability(script, m_gray);

   script += "\t# Copy CSF Probability #\n";
   copyFinalPriorProbability(script, m_csf);

   script += "\t# Copy Rest Probability #\n";
   copyFinalPriorProbability(script, m_rest);
}

void AtlasGeneration::writeGenerateAtlas()
{
   QString generateAtlas;

   initializeScript(generateAtlas);
   
   implementLogStdoutAndStderr(generateAtlas); 
   implementLogStderr(generateAtlas); 

   implementGenerateAtlas(generateAtlas);

   QString script_path = m_output_dir->filePath("generateAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << generateAtlas.toStdString() << std::endl;

   script_stream->close();
}

void AtlasGeneration::update()
{
   createDirectories();
   writeGenerateAtlas();
}

QString AtlasGeneration::getOutput() { return m_atlasGeneration_path; }

