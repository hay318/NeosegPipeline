#include "AtlasGeneration.h"

AtlasGeneration::AtlasGeneration(QString module) : Script(module)
{
   m_white.name="white";
   m_gray.name="gray";
   m_csf.name="csf";
   m_rest.name="rest";
   m_templateT1.name="templateT1";
   m_templateT2.name="templateT2";
}

void AtlasGeneration::setAtlasPopulation(std::vector<Atlas> atlasPopulation)
{
   m_atlasPopulation = atlasPopulation;
}
void AtlasGeneration::setSmoothing(QString smoothing)
{
   m_smoothing = smoothing;
}
void AtlasGeneration::setSmoothingSize(double smoothingSize)
{
   m_smoothingSize = smoothingSize;
}
void AtlasGeneration::setComputingWeights(bool computingWeights)
{
   m_computingWeights = computingWeights;
}
void AtlasGeneration::setNeightborhoodRadius(double neightborhoodRadius)
{
   m_neightborhoodRadius = neightborhoodRadius;
}
void AtlasGeneration::setIncludingFA(bool includingFA)
{
   m_includingFA = includingFA;
}

void AtlasGeneration::defineGeneratedAtlas()
{
    m_white.finalImage = m_module_dir->filePath("white.nrrd"); 
    m_gray.finalImage = m_module_dir->filePath("gray.nrrd"); 
    m_csf.finalImage = m_module_dir->filePath("csf.nrrd"); 
    m_rest.finalImage = m_module_dir->filePath("rest.nrrd"); 
    m_templateT1.finalImage = m_module_dir->filePath("templateT1.nrrd"); 
    m_templateT2.finalImage = m_module_dir->filePath("templateT2.nrrd");    
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
   m_module_dir->mkpath( "temp/templates/templateT1" );
   QString templateT1 = m_module_dir->filePath( "temp/templates/templateT1" );
   m_templateT1.dir = new QDir( templateT1 );  
 
   m_module_dir->mkpath( "temp/templates/templateT2" );
   QString templateT2 = m_module_dir->filePath( "temp/templates/templateT2" );
   m_templateT2.dir = new QDir( templateT2 );  

   m_module_dir->mkpath( "temp/priorProbabilities/white" );
   QString white_path = m_module_dir->filePath( "temp/priorProbabilities/white" );
   m_white.dir = new QDir( white_path );  
 
   m_module_dir->mkpath( "temp/priorProbabilities/gray" );
   QString gray_path = m_module_dir->filePath( "temp/priorProbabilities/gray" );
   m_gray.dir = new QDir( gray_path );  
 
   m_module_dir->mkpath( "temp/priorProbabilities/csf" );
   QString csf_path = m_module_dir->filePath( "temp/priorProbabilities/csf" );
   m_csf.dir = new QDir( csf_path );  
 
   m_module_dir->mkpath( "temp/priorProbabilities/rest" );
   QString rest_path = m_module_dir->filePath( "temp/priorProbabilities/rest" );
   m_rest.dir = new QDir( rest_path ); 

   m_module_dir -> mkpath("temp/priorProbabilities");
   QString priorProbabilities_path = m_module_dir->filePath("temp/priorProbabilities");
   m_priorProbabilities_dir = new QDir(priorProbabilities_path);  
}

void AtlasGeneration::initializeScript(QString &script)
{
   definePython(script);
   importGeneralModules(script);
   importXmlModules(script);
  
   defineExecutable(script, "ImageMath");
   defineExecutable(script, "ResampleVolume2");
   defineExecutable(script, "unu");
   defineExecutable(script, "SpreadFA");

   m_script += "runningProcess = None\n\n";  
} 

QString AtlasGeneration::getImage(Atlas atlas, QString name)
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

void AtlasGeneration::generateTemplate(QString &script, TemplateImage templateImage)
{
   QStringList argumentsList;

   m_script += "\taverage = '" + templateImage.finalImage + "'\n\n";

   //m_script += "\tif checkFileExistence(parameters_path)==False:\n";   

   QString ref = getImage(m_atlasPopulation[1], templateImage.name);
   m_script += "\tref = '" + ref + "'\n\n"; 

   argumentsList << "ImageMath" << "ref" << "'-avg'";

   int i=1;
   std::vector<Atlas>::iterator it;
   for (it=m_atlasPopulation.begin()+1; it!=m_atlasPopulation.end(); ++it)
   {
      QString atlas = "atlas_" + QString::number(i);
      m_script += "\t" + atlas + " = '" + getImage(*it, templateImage.name) + "'\n"; 
      argumentsList << atlas; 
      i++; 
   }

   argumentsList << "'-outfile'" << "average";
   m_script += "\tlogging.info('- Averaging all the images...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";
}

void AtlasGeneration::generateWeightedAveragedLabels(QString &script)
{
   QStringList argumentsList;

   QString WeightedLabelsAverage = "/work/mcherel/project/neosegPipeline/weightedLabelsAverage/bin/WeightedLabelsAverage";
   m_script += "\tWeightedLabelsAverage = '" + WeightedLabelsAverage + "'\n\n";

   QString m_XML_path = m_module_dir->filePath("parameters.xml");
   m_script += "\tparameters_path = \"" + m_XML_path + "\"\n"; 

   m_white.image = m_white.dir->filePath(m_white.name + "_average.nrrd");
   m_gray.image = m_gray.dir->filePath(m_gray.name + "_average.nrrd");
   m_csf.image = m_csf.dir->filePath(m_csf.name + "_average.nrrd");

   m_script += "\tif checkFileExistence(parameters_path)==False:\n";   

   m_script += "\t\tparameters = Element('WEIGHTED-AVERAGED-LABELS-PARAMETERS')\n";
   addSubElement(script, "parameters", "Input", "INPUT", m_neo.T1);
   m_script += "\t\tatlasPopulation = SubElement(parameters, 'ATLAS-POPULATION')\n";

   std::vector<Atlas>::iterator it;
   Atlas atlas;

   for (it=m_atlasPopulation.begin(); it!=m_atlasPopulation.end(); ++it)
   {
      atlas = *it;

      if(atlas.probabilistic)
      {
         m_script += "\t\tatlas = SubElement(atlasPopulation, 'PROBABILISTIC-ATLAS')\n";
         addSubElement(script, "atlas", "image", "IMAGE", (*it).T2); 
         addSubElement(script, "atlas", "seg", "WHITE", (*it).white);
         addSubElement(script, "atlas", "seg", "GRAY", (*it).gray);
         addSubElement(script, "atlas", "seg", "CSF", (*it).csf);
      }

      else
      {
         m_script += "\t\tatlas = SubElement(atlasPopulation, 'ATLAS')\n";
         addSubElement(script, "atlas", "image", "IMAGE", (*it).T2); 
         addSubElement(script, "atlas", "seg", "SEG", (*it).seg);
      }
   }
   
   m_script += "\t\tweights = SubElement(parameters, 'WEIGHTS')\n";
   addSubElement(script, "weights", "computingWeights", "COMPUTING-WEIGHTS", QString::number(m_computingWeights));
   addSubElement(script, "weights", "neightborhoodRadius", "NEIGHTBORHOOD-RADIUS", QString::number(m_neightborhoodRadius));

   m_script += "\t\toutputs = SubElement(parameters, 'OUTPUTS')\n";
   addSubElement(script, "outputs", "white", "WHITE-AVERAGE", m_white.image);
   addSubElement(script, "outputs", "gray", "GRAY-AVERAGE", m_gray.image);
   addSubElement(script, "outputs", "csf", "CSF-AVERAGE", m_csf.image);

   m_script += "\t\tXML = xml.dom.minidom.parseString(ElementTree.tostring(parameters))\n";
   m_script += "\t\tpretty_xml_as_string = XML.toprettyxml()\n";

   m_script += "\t\tparameters = open(parameters_path, 'w')\n";
   m_script += "\t\tparameters.write(pretty_xml_as_string)\n";  
   m_script += "\t\tparameters.close()\n";
   
   m_script += "\twhite_average = '" + m_white.image + "'\n";
   m_script += "\tgray_average = '" + m_gray.image + "'\n";
   m_script += "\tcsf_average = '" + m_csf.image + "'\n";

   m_script += "\tif checkFileExistence(white_average)==False or checkFileExistence(gray_average)==False or checkFileExistence(csf_average)==False:\n";   
   argumentsList << "WeightedLabelsAverage" << "parameters_path";
   m_script += "\t\targs = " + listToString(argumentsList) + "\n"; 
   m_script += "\t\texecute(args)\n";
}

void AtlasGeneration::extractWMFromFA(QString &script)
{
   QStringList argumentsList;

   m_white.dir->mkpath("FA");
   QString FA_path = m_white.dir->filePath("FA");
   QDir* FA_dir = new QDir(FA_path);  

   m_script += "\tFA = '" + m_neo.FA + "'\n\n";

   QString rescaledFA = FA_dir->filePath("FA-rescaled.nrrd");
   m_script += "\trescaledFA = '" + rescaledFA + "'\n";
 
   argumentsList.clear();
   argumentsList << "ImageMath" << "FA" << "'-rescale'"<< "'0,255'" << "'-outfile'" << "rescaledFA";
   m_script += "\tlogging.info('- Rescaling the FA between 0 and 255...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";

   QString spreadFA = FA_dir->filePath("FA-spread.nrrd");
   m_script += "\tspreadFA = '" + spreadFA + "'\n";
 
   argumentsList.clear();
   argumentsList << "SpreadFA" << "rescaledFA" << "spreadFA";
   m_script += "\tlogging.info('- Spreading the FA...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";

   QString smoothedFA = FA_dir->filePath("FA-smoothed.nrrd");
   m_script += "\tsmoothedFA = '" + smoothedFA + "'\n";
 
   argumentsList.clear();
   argumentsList << "ImageMath" << "spreadFA" << "'-smooth'"<< "'-gauss'" << "'-size'" << "'1'" << "'-type'" << "'float'" << "'-outfile'" << "smoothedFA";
   m_script += "\tlogging.info('- Smoothing the FA (type=gauss, size=1)...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";


   m_script += "\tmask = '" + m_neo.mask + "'\n";    

   QString erodedMask = FA_dir->filePath("erodedMask.nrrd");
   m_script += "\terodedMask = '" + erodedMask + "'\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "mask" << "'-erode'"<< "'2,1'" << "'-outfile'" << "erodedMask";
   m_script += "\tlogging.info('- Eroding the brain mask (radius=2)... ')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";


   QString maskedFA = FA_dir->filePath("FA-masked.nrrd");
   m_script += "\tmaskedFA = '" + maskedFA + "'\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "smoothedFA" << "'-mask'"<< "erodedMask" << "'-type'" << "'float'" << "'-outfile'" << "maskedFA";
   m_script += "\tlogging.info('- Masking the FA with the eroded brain mask...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";

   m_script += "\twhite_average = '" + m_white.image + "'\n";

   QString FAMultipliedByWeight = FA_dir->filePath("FAMultipliedByWeight.nrrd");
   m_script += "\tFAMultipliedByWeight = '" + FAMultipliedByWeight + "'\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "maskedFA" << "'-constOper'" << "'2,1.5'" << "'-type'" << "'float'" << "'-outfile'" << "FAMultipliedByWeight";
   m_script += "\tlogging.info('- Multiplying the FA by 1.5...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";

   QString whitePlusFA = m_white.dir->filePath("whitePlusFA.nrrd");
   m_script += "\twhitePlusFA = '" + whitePlusFA + "'\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << "white_average" << "'-add'" << "FAMultipliedByWeight" << "'-type'" << "'float'" << "'-outfile'" << "whitePlusFA";
   m_script += "\tlogging.info('- Adding the FA to the white average...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";

   m_white.image = whitePlusFA;
}

void AtlasGeneration::generatePriorProbability(QString &script, PriorProbability& priorProbability)
{
   QStringList argumentsList;

   QString smoothing = "'-" + m_smoothing + "'";
   QString smoothingSize = "'" + QString::number(m_smoothingSize) + "'";

   m_script += "\t" + priorProbability.name + "_average = '" + priorProbability.image + "'\n\n";

   QString probability = priorProbability.dir->filePath( priorProbability.name + "_probability.nrrd" );
   m_script += "\t" + priorProbability.name + "_probability = '" + probability + "'\n";

   argumentsList << "ImageMath" << priorProbability.name + "_average" << "'-smooth'" << smoothing << "'-size'" << smoothingSize << "'-type'" << "'float'" << "'-outfile'" << priorProbability.name + "_probability";
   m_script += "\tlogging.info('- Smoothing the " + priorProbability.name + " average (type=" + m_smoothing + ", size=" + QString::number(m_smoothingSize) + ")...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";

   QString maskedProbability = priorProbability.dir->filePath(priorProbability.name + "_maskedProbability.nrrd");
   m_script += "\t" + priorProbability.name + "_maskedProbability = '" + maskedProbability + "'\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << priorProbability.name + "_probability" << "'-mask'"<< "mask" << "'-type'" << "'float'" << "'-outfile'" << priorProbability.name + "_maskedProbability";
   m_script += "\tlogging.info('- Masking the " + priorProbability.name + " probability with the brain mask...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";
}

void AtlasGeneration::preNormalizePriorProbability(QString &script, PriorProbability priorProbability)
{
   QStringList argumentsList;

   argumentsList << "unu" << "'2op'" << "'/'" << priorProbability.name + "_maskedProbability" << "sumProbabilities" << "'-t'" << "'float'";
   m_script += "\tlogging.info('- Dividing the probability by the sum of the probabilities...')\n";
   m_script += "\targs1 = " + listToString(argumentsList) + "\n";

   argumentsList.clear();
   argumentsList << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << priorProbability.name + "_preNormalizedProbability";
   m_script += "\targs2 = " + listToString(argumentsList) + "\n";

   m_script += "\texecutePipe(args1, args2)\n";

   argumentsList.clear();
   argumentsList << "ImageMath" << priorProbability.name + "_preNormalizedProbability" << "'-constOper'"<< "'2,255'" << "'-outfile'" << priorProbability.name + "_preNormalizedProbability";
   m_script += "\tlogging.info('- Multiplying the probability by 255...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";
}

void AtlasGeneration::computeSumProbabilities(QString &script)
{
   QStringList argumentsList;

   QString sumProbabilities = m_priorProbabilities_dir->filePath("sumProbabilities.nrrd");
   m_script += "\tsumProbabilities = '" + sumProbabilities + "'\n\n";

   QString preNormalizedWhite = m_white.dir->filePath("white_preNormalizedProbability.nrrd"); 
   QString preNormalizedGray = m_gray.dir->filePath("gray_preNormalizedProbability.nrrd"); 
   QString preNormalizedCsf = m_csf.dir->filePath("csf_preNormalizedProbability.nrrd"); 

   m_script += "\twhite_preNormalizedProbability = '" + preNormalizedWhite + "'\n";
   m_script += "\tgray_preNormalizedProbability = '" + preNormalizedGray + "'\n";
   m_script += "\tcsf_preNormalizedProbability = '" + preNormalizedCsf + "'\n\n";

   argumentsList << "unu" << "'3op'" << "'+'" << "white_maskedProbability" << "gray_maskedProbability" << "csf_maskedProbability" << "'-t'" << "'float'";
   m_script += "\targs1 = " + listToString(argumentsList) + "\n";

   argumentsList.clear();
   argumentsList << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << "sumProbabilities";
   m_script += "\targs2 = " + listToString(argumentsList) + "\n";

   m_script += "\tlogging.info('- Computing the sum of the probabilities...')\n";
   m_script += "\texecutePipe(args1, args2)\n";

   argumentsList.clear();
   argumentsList << "unu" << "'2op'" << "'if'" << "sumProbabilities" << "'1'" << "'-t'" << "'float'";
   m_script += "\targs1 = " + listToString(argumentsList) + "\n";

   argumentsList.clear();
   argumentsList << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << "sumProbabilities";
   m_script += "\targs2 = " + listToString(argumentsList) + "\n";

   m_script += "\tlogging.info('- Replacing 0 by 1 in the sum(to be able to divide)...')\n";
   m_script += "\texecutePipe(args1, args2)\n\n";
}

void AtlasGeneration::computeRest(QString &script)
{
   QStringList argumentsList;

   QString priorProbabilities = m_white.image + "," + m_gray.image + "," + m_csf.image; 
   
   m_script += "\ttemplateT1 = '" + m_templateT1.finalImage + "'\n"; 
   m_script += "\tpriorProbabilities = white_preNormalizedProbability + \",\" + gray_preNormalizedProbability + \",\" + csf_preNormalizedProbability\n";

   QString outbase = m_rest.dir->filePath("label"); 
   m_script += "\toutbase = '" + outbase + "'\n";

   argumentsList << "ImageMath" << "templateT1" << "'-normalizeEMS'"<< "'3'" << "'-EMSfile'" << "priorProbabilities" << "'-type'" << "'float'" << "'-extension'" << "'.nrrd'" << "'-outbase'" << "outbase";
   m_script += "\tlogging.info('- Computing the rest probability...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";

   m_white.image = m_rest.dir->filePath("label1_normalized.nrrd");
   m_gray.image = m_rest.dir->filePath("label2_normalized.nrrd");
   m_csf.image = m_rest.dir->filePath("label3_normalized.nrrd");
   m_rest.image = m_rest.dir->filePath("label4_normalized.nrrd");

   m_script += "\twhite_normalizedProbability = '" + m_white.image + "'\n";
   m_script += "\tgray_normalizedProbability = '" + m_gray.image + "'\n";
   m_script += "\tcsf_normalizedProbability = '" + m_csf.image + "'\n";
   m_script += "\trest_normalizedProbability = '" + m_rest.image + "'\n\n";

   m_script += "\t# Mask Rest Probability #\n";

   m_script += "\tbrainMask = '" + m_neo.mask + "'\n";

   QString dilatedMask = m_priorProbabilities_dir->filePath("dilatedMask.nrrd");
   m_script += "\tdilatedMask = '" + dilatedMask + "'\n";
   argumentsList.clear();

   argumentsList << "ImageMath" << "brainMask" << "'-dilate'"<< "'5,1'" << "'-outfile'" << "dilatedMask";
   m_script += "\tlogging.info('- Dilating the brain mask (radius=5)...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";

   m_script += "\trest = '" + m_rest.image + "'\n"; 
   QString restMasked = m_rest.dir->filePath("restMasked.nrrd");
   m_script += "\trest_maskedProbability = '" + restMasked + "'\n";
 
   argumentsList.clear();
   argumentsList << "ImageMath" << "rest" << "'-mask'"<< "dilatedMask" << "'-outfile'" << "rest_maskedProbability";
   m_script += "\tlogging.info('- Masking the rest with the dilated mask...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";
}

void AtlasGeneration::copyFinalPriorProbability(QString &script, PriorProbability& priorProbability)
{
   QStringList argumentsList;

   m_script += "\t" + priorProbability.name + "_finalProbability = '" + priorProbability.finalImage + "'\n";
  
   if(priorProbability.name == "rest")
   {
      argumentsList << "ResampleVolume2" << priorProbability.name + "_maskedProbability" << priorProbability.name + "_finalProbability";            
   }
   else
   {
      argumentsList << "ResampleVolume2" << priorProbability.name + "_normalizedProbability" << priorProbability.name + "_finalProbability";
   }

   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";
} 

void AtlasGeneration::implementRun(QString &script)
{
   m_script += "def run():\n";

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   m_script += "\tlogging.debug('')\n";
   m_script += "\tlogging.info('=== Atlas Generation ===')\n";
   m_script += "\tlogging.debug('')\n";

   m_script += "\tmask = '" + m_neo.mask + "'\n";  

   m_script += "\t### TEMPLATES ###\n";

   m_script += "\t# Compute Template T1 #\n";
   m_script += "\tlogging.info('Computing template T1')\n";
   m_script += "\tlogging.debug('')\n";
   generateTemplate(script, m_templateT1);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Compute Template T2 #\n";
   m_script += "\tlogging.info('Computing template T2')\n";
   m_script += "\tlogging.debug('')\n";
   generateTemplate(script, m_templateT2);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t### PRIOR PROBABILITIES ###\n";

   m_script += "\t# Compute Tissue Weighted Averages #\n";
   m_script += "\tlogging.info('Computing weighted labels averages...')\n";
   m_script += "\tlogging.debug('')\n";
   generateWeightedAveragedLabels(script);
   m_script += "\tlogging.debug('')\n";

   if(m_includingFA)
   {
      m_script += "\t# Extract WM From FA #\n";
      m_script += "\tlogging.info('Extracting and adding the FA to the white average :')\n";
      m_script += "\tlogging.debug('')\n";
      extractWMFromFA(script);
      m_script += "\tlogging.debug('')\n";
   }

   m_script += "\t# Compute White Probability #\n";
   m_script += "\tlogging.info('Computing white probability :')\n";
   m_script += "\tlogging.debug('')\n";
   generatePriorProbability(script, m_white);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Compute Gray Probability #\n";
   m_script += "\tlogging.info('Computing gray probability :')\n";
   m_script += "\tlogging.debug('')\n";
   generatePriorProbability(script, m_gray);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Compute CSF Probability #\n";  
   m_script += "\tlogging.info('Computing csf probability :')\n";
   m_script += "\tlogging.debug('')\n"; 
   generatePriorProbability(script, m_csf);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Compute Sum Of Probabilities #\n";
   m_script += "\tlogging.info('Computing the sum of the probabilities :')\n"; 
   m_script += "\tlogging.debug('')\n";
   computeSumProbabilities(script);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Prenormalize White Probability #\n";
   m_script += "\tlogging.info('Prenormalizing white probability :')\n";
   m_script += "\tlogging.debug('')\n";
   preNormalizePriorProbability(script, m_white);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Prenormalize Gray Probability #\n";
   m_script += "\tlogging.info('Prenormalizing gray probability :')\n";
   m_script += "\tlogging.debug('')\n";
   preNormalizePriorProbability(script, m_gray);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Prenormalize CSF Probability #\n";
   m_script += "\tlogging.info('Prenormalizing CSF probability :')\n";
   m_script += "\tlogging.debug('')\n";
   preNormalizePriorProbability(script, m_csf);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Compute Rest Probability #\n"; 
   m_script += "\tlogging.info('Computing rest probability :')\n"; 
   m_script += "\tlogging.debug('')\n";
   computeRest(script); 
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Copy White Probability #\n";
   m_script += "\tlogging.info('Copying final white probability...')\n";
   m_script += "\tlogging.debug('')\n"; 
   copyFinalPriorProbability(script, m_white);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Copy Gray Probability #\n";
   m_script += "\tlogging.info('Copying final gray probability...')\n"; 
   copyFinalPriorProbability(script, m_gray);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Copy CSF Probability #\n";
   m_script += "\tlogging.info('Copying final csf probability...')\n"; 
   copyFinalPriorProbability(script, m_csf);
   m_script += "\tlogging.debug('')\n";

   m_script += "\t# Copy Rest Probability #\n";
   m_script += "\tlogging.info('Copying final rest probability...')\n"; 
   copyFinalPriorProbability(script, m_rest);
   m_script += "\tlogging.debug('')\n";

   m_script += "\tlogging.info('')\n";
}

void AtlasGeneration::update()
{
   createDirectories(); 

   initializeScript(m_script);

   implementStop(m_script);
   implementCheckFileExistence(m_script);   
   implementExecute(m_script);
   implementExecutePipe(m_script);
   implementRun(m_script);

   writeScript(m_script);
}

QString AtlasGeneration::getOutput()
{
   return m_module_path;
}

