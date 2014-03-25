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

   QString WeightedLabelsAverage = "/work/mcherel/project/neosegPipeline/weightedLabelsAverage/bin/WeightedLabelsAverage";
   m_script += "WeightedLabelsAverage = '" + WeightedLabelsAverage + "'\n\n";

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

void AtlasGeneration::generateTemplate(QString &script, TemplateImage& templateImage)
{
   QString ref = getImage(m_atlasPopulation[1], templateImage.name);
   m_inputs.insert("ref", ref);

   m_log = "- Averaging all the " + templateImage.name;
   m_argumentsList << "ImageMath" << "ref" << "'-avg'";

   int i=1;
   std::vector<Atlas>::iterator it;
   for (it=m_atlasPopulation.begin()+1; it!=m_atlasPopulation.end(); ++it)
   {
      QString atlas = "atlas_" + QString::number(i);
      m_inputs.insert(atlas, getImage(*it, templateImage.name));
      m_argumentsList << atlas; 
      i++; 
   }

   QString average = m_module_dir->filePath(templateImage.name + ".nrrd"); 
   templateImage.output = templateImage.name;
   m_outputs.insert(templateImage.output, average);

   m_argumentsList << "'-outfile'" << templateImage.output;

   execute(script);
}

void AtlasGeneration::generateWeightedAveragedLabels(QString &script)
{
   QStringList argumentsList;

   m_white.output = m_white.name + "_average";
   m_gray.output = m_gray.name + "_average";
   m_csf.output = m_csf.name + "_average";

   QString whiteAverage = m_white.dir->filePath(m_white.name + "-average" + m_suffix + ".nrrd");
   QString grayAverage = m_gray.dir->filePath(m_gray.name + "-average" + m_suffix + ".nrrd");
   QString csfAverage = m_csf.dir->filePath(m_csf.name + "-average" + m_suffix + ".nrrd");

   m_script += "\t" + m_white.output + " = '" + whiteAverage + "'\n";
   m_script += "\t" + m_gray.output + " = '" + grayAverage + "'\n";
   m_script += "\t" + m_csf.output + " = '" + csfAverage + "'\n";

   QString m_XML_path = m_module_dir->filePath("parameters.xml");
   m_script += "\tparameters_path = \"" + m_XML_path + "\"\n"; 

   m_script += "\tif checkFileExistence(parameters_path)==False:\n";   
   m_script += "\t\tlogging.info('- Writing the XML file...')\n";
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
   addSubElement(script, "outputs", "white", "WHITE-AVERAGE", whiteAverage);
   addSubElement(script, "outputs", "gray", "GRAY-AVERAGE", grayAverage);
   addSubElement(script, "outputs", "csf", "CSF-AVERAGE", csfAverage);

   m_script += "\t\tXML = xml.dom.minidom.parseString(ElementTree.tostring(parameters))\n";
   m_script += "\t\tpretty_xml_as_string = XML.toprettyxml()\n";

   m_script += "\t\tparameters = open(parameters_path, 'w')\n";
   m_script += "\t\tparameters.write(pretty_xml_as_string)\n";  
   m_script += "\t\tparameters.close()\n";
   
   m_script += "\telse:\n";
   m_script += "\t\tlogging.info('- Writing the XML file : Done ')\n";


   m_log = "- Computing weighted labels averages";
   m_outputs.insert(m_white.output, whiteAverage);
   m_outputs.insert(m_gray.output, grayAverage);
   m_outputs.insert(m_csf.output, csfAverage);
   m_argumentsList << "WeightedLabelsAverage" << "parameters_path";
   execute(script); 
}

void AtlasGeneration::extractWMFromFA(QString &script)
{
   m_white.dir->mkpath("FA");
   QString FA_path = m_white.dir->filePath("FA");
   QDir* FA_dir = new QDir(FA_path);  

   m_script += "\tFA = '" + m_neo.FA + "'\n\n";

   // Rescaling FA
   QString rescaledFA = FA_dir->filePath(m_prefix + "FA" + "-rescaled" + m_suffix + ".nrrd");
   m_outputs.insert("rescaledFA", rescaledFA);
 
   m_argumentsList << "ImageMath" << "FA" << "'-rescale'"<< "'0,255'" << "'-outfile'" << "rescaledFA";
   m_log = "- Rescaling the FA between 0 and 255";
   execute(script);

   // Spreading FA
   QString spreadFA = FA_dir->filePath(m_prefix + "FA" + "-spread" + m_suffix + ".nrrd");
   m_outputs.insert("spreadFA", spreadFA);
 
   m_argumentsList << "SpreadFA" << "rescaledFA" << "spreadFA";
   m_log = "- Spreading the FA...";
   execute(script);

   // Smoothing FA
   QString smoothedFA = FA_dir->filePath(m_prefix + "FA" + "-smoothed" + m_suffix + ".nrrd");
   m_outputs.insert("smoothedFA", smoothedFA);
 
   m_argumentsList << "ImageMath" << "spreadFA" << "'-smooth'"<< "'-gauss'" << "'-size'" << "'1'" << "'-type'" << "'float'" << "'-outfile'" << "smoothedFA";
   m_log = "- Smoothing the FA (type=gauss, size=1)";
   execute(script);

   // Eroding brain mask
   m_script += "\tmask = '" + m_neo.mask + "'\n"; 

   QString erodedMask = FA_dir->filePath(m_prefix + "mask" + "-eroded" + m_suffix + ".nrrd");
   m_outputs.insert("erodedMask", erodedMask);
   m_argumentsList << "ImageMath" << "mask" << "'-erode'"<< "'2,1'" << "'-outfile'" << "erodedMask";
   m_log = "- Eroding the brain mask (radius=2)";
   execute(script);

   // Masking FA
   QString maskedFA = FA_dir->filePath(m_prefix + "FA" + "-masked" + m_suffix + ".nrrd");
   m_outputs.insert("maskedFA", maskedFA);
   m_argumentsList << "ImageMath" << "smoothedFA" << "'-mask'"<< "erodedMask" << "'-type'" << "'float'" << "'-outfile'" << "maskedFA";
   m_log = "- Masking the FA with the eroded brain mask"; 
   execute(script);
  
   // Multiplying FA 
   QString weightedFA = FA_dir->filePath(m_prefix + "FA" + "-weighted" + m_suffix + ".nrrd");
   m_outputs.insert("weightedFA", weightedFA);
   m_argumentsList << "ImageMath" << "maskedFA" << "'-constOper'" << "'2,1.5'" << "'-type'" << "'float'" << "'-outfile'" << "weightedFA";
   m_log = "- Multiplying the FA by 1.5";  
   execute(script); 

   // Adding FA
   m_white.input = m_white.output; 
   m_white.output = "whitePlusFA";

   QString whitePlusFA = m_white.dir->filePath(m_prefix + "white" + "-plusFA" + m_suffix + ".nrrd");
   m_outputs.insert(m_white.output, whitePlusFA);
   m_argumentsList << "ImageMath" << m_white.input << "'-add'" << "weightedFA" << "'-type'" << "'float'" << "'-outfile'" << m_white.output;
   m_log = "- Adding the FA to the white average...";
   execute(script);
}

void AtlasGeneration::generatePriorProbability(QString &script, PriorProbability& priorProbability)
{
   // Smoothing probability
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_probability";
   QString probability = priorProbability.dir->filePath(priorProbability.name + "-probability" + m_suffix + ".nrrd");

   QString smoothing = "'-" + m_smoothing + "'";
   QString smoothingSize = "'" + QString::number(m_smoothingSize) + "'";

   m_log = "- Smoothing the " + priorProbability.name + " average (type=" + m_smoothing + ", size=" + QString::number(m_smoothingSize);
   m_argumentsList << "ImageMath" << priorProbability.input << "'-smooth'" << smoothing << "'-size'" << smoothingSize << "'-type'" << "'float'" << "'-outfile'" << priorProbability.output;
   m_outputs.insert(priorProbability.output, probability); 
   execute(script);

   // Masking probability
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_maskedProbability"; 
   QString maskedProbability = priorProbability.dir->filePath(priorProbability.name + "-maskedProbability" + m_suffix + ".nrrd");

   m_log = "- Masking the " + priorProbability.name + " probability with the brain mask";
   m_outputs.insert(priorProbability.output, maskedProbability);
   m_argumentsList << "ImageMath" << priorProbability.input << "'-mask'"<< "mask" << "'-type'" << "'float'" << "'-outfile'" << priorProbability.output;
   execute(script);
}

void AtlasGeneration::preNormalizePriorProbability(QString &script, PriorProbability& priorProbability)
{
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_preNormalizedProbability";
   QString preNormalizedProbability = priorProbability.dir->filePath(priorProbability.name + "-preNormalizedProbability" + m_suffix + ".nrrd");

   m_log = "- Dividing the probability by the sum of the probabilities";
   m_outputs.insert(priorProbability.output, preNormalizedProbability); 
   m_argumentsList_1 << "unu" << "'2op'" << "'/'" << priorProbability.input << "sumProbabilities" << "'-t'" << "'float'";
   m_argumentsList_2 << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << priorProbability.output;
   executePipe(script);

   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_rescaledProbability";
   QString rescaledProbability = priorProbability.dir->filePath(priorProbability.name + "-rescaledProbability" + m_suffix + ".nrrd");

   m_log = "- Multiplying the probability by 255";
   m_outputs.insert(priorProbability.output, rescaledProbability);
   m_argumentsList << "ImageMath" << priorProbability.input << "'-constOper'"<< "'2,255'" << "'-outfile'" << priorProbability.output;
   execute(script); 
}

void AtlasGeneration::computeSumProbabilities(QString &script)
{
   QString sumProbabilities = m_priorProbabilities_dir->filePath("sumProbabilities" + m_suffix + ".nrrd");
   m_outputs.insert("sumProbabilities", sumProbabilities);

   m_log = "- Computing the sum of the probabilities"; 
   m_argumentsList_1 << "unu" << "'3op'" << "'+'" << "white_maskedProbability" << "gray_maskedProbability" << "csf_maskedProbability" << "'-t'" << "'float'";
   m_argumentsList_2 << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << "sumProbabilities";
   executePipe(script);

   m_log = "- Replacing 0 by 1 in the sum(to be able to divide)";
   m_argumentsList_1 << "unu" << "'2op'" << "'if'" << "sumProbabilities" << "'1'" << "'-t'" << "'float'";
   m_argumentsList_2 << "unu" << "'save'" << "'-e'" << "'gzip'" << "'-f'" << "'nrrd'" << "'-o'" << "sumProbabilities";
   executePipe(script); 
}

void AtlasGeneration::computeRest(QString &script)
{
   QStringList argumentsList;

   QString outbase = m_rest.dir->filePath("label"); 
   m_inputs.insert("outbase", outbase);

   m_white.input = m_white.output;
   m_white.output = "white_normalizedProbability";
   QString label1 = m_rest.dir->filePath("label1_normalized.nrrd");
   m_outputs.insert(m_white.output, label1); 

   m_gray.input = m_gray.output;
   m_gray.output = "gray_normalizedProbability";
   QString label2 = m_rest.dir->filePath("label2_normalized.nrrd");
   m_outputs.insert(m_gray.output, label2); 

   m_csf.input = m_csf.output;
   m_csf.output = "csf_normalizedProbability";
   QString label3 = m_rest.dir->filePath("label3_normalized.nrrd");
   m_outputs.insert(m_csf.output, label3); 

   m_rest.output = "rest_normalizedProbability";
   QString label4 = m_rest.dir->filePath("label4_normalized.nrrd");
   m_outputs.insert(m_rest.output, label4); 

   QString priorProbabilities = "' + " + m_white.input + " + ',' + " + m_gray.input + " + ',' + " + m_csf.input + " + '";
   m_inputs.insert("priorProbabilities", priorProbabilities);   

   m_argumentsList << "ImageMath" << m_templateT1.output << "'-normalizeEMS'"<< "'3'" << "'-EMSfile'" << "priorProbabilities" << "'-type'" << "'float'" << "'-extension'" << "'.nrrd'" << "'-outbase'" << "outbase";
   m_log = "- Computing the rest probability";

   execute(script);

   // Dilating brain mask 
   QString dilatedMask = m_priorProbabilities_dir->filePath(m_prefix + "mask" + "-dilated" + m_suffix + ".nrrd");

   m_inputs.insert("brainMask", m_neo.mask);
   m_outputs.insert("dilatedMask", dilatedMask);
   m_log = "- Dilating the brain mask (radius=5)";
   m_argumentsList << "ImageMath" << "brainMask" << "'-dilate'"<< "'5,1'" << "'-outfile'" << "dilatedMask";

   execute(script); 

   // Masking rest
   m_rest.input = m_rest.output; 
   m_rest.output = "rest_maskedProbability"; 

   QString restMasked = m_rest.dir->filePath("rest-masked" + m_suffix + ".nrrd");

   m_outputs.insert(m_rest.output, restMasked);
   m_argumentsList << "ImageMath" << m_rest.input << "'-mask'"<< "dilatedMask" << "'-outfile'" << m_rest.output;
   m_log = "- Masking the rest with the dilated mask";
   execute(script);
}

void AtlasGeneration::copyFinalPriorProbability(QString &script, PriorProbability& priorProbability)
{
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_finalProbability"; 

   QString finalProbability = m_module_dir->filePath(priorProbability.name + ".nrrd");  
   m_outputs.insert(priorProbability.output, finalProbability); 

   m_argumentsList << "ResampleVolume2" << priorProbability.input << priorProbability.output;
   execute(script); 
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
   generateTemplate(script, m_templateT1);

   m_script += "\t# Compute Template T2 #\n";
   m_script += "\tlogging.info('Computing template T2')\n";
   generateTemplate(script, m_templateT2);

   m_script += "\t### PRIOR PROBABILITIES ###\n";

   m_script += "\t# Compute Tissue Weighted Averages #\n";
   m_script += "\tlogging.info('Computing weighted labels averages...')\n";
   generateWeightedAveragedLabels(script);

   if(m_includingFA)
   {
      m_script += "\t# Extract WM From FA #\n";
      m_script += "\tlogging.info('Extracting and adding the FA to the white average :')\n";
      extractWMFromFA(script);
   }

   m_script += "\t# Compute White Probability #\n";
   m_script += "\tlogging.info('Computing white probability :')\n";
   generatePriorProbability(script, m_white);

   m_script += "\t# Compute Gray Probability #\n";
   m_script += "\tlogging.info('Computing gray probability :')\n";
   generatePriorProbability(script, m_gray);

   m_script += "\t# Compute CSF Probability #\n";  
   m_script += "\tlogging.info('Computing csf probability :')\n";
   generatePriorProbability(script, m_csf);

   m_script += "\t# Compute Sum Of Probabilities #\n";
   m_script += "\tlogging.info('Computing the sum of the probabilities :')\n"; 
   computeSumProbabilities(script);

   m_script += "\t# Prenormalize White Probability #\n";
   m_script += "\tlogging.info('Prenormalizing white probability :')\n";
   preNormalizePriorProbability(script, m_white);

   m_script += "\t# Prenormalize Gray Probability #\n";
   m_script += "\tlogging.info('Prenormalizing gray probability :')\n";
   preNormalizePriorProbability(script, m_gray);

   m_script += "\t# Prenormalize CSF Probability #\n";
   m_script += "\tlogging.info('Prenormalizing CSF probability :')\n";
   preNormalizePriorProbability(script, m_csf);

   m_script += "\t# Compute Rest Probability #\n"; 
   m_script += "\tlogging.info('Computing rest probability :')\n"; 
   computeRest(script); 

   m_script += "\t# Copy White Probability #\n";
   m_script += "\tlogging.info('Copying final white probability...')\n";
   copyFinalPriorProbability(script, m_white);

   m_script += "\t# Copy Gray Probability #\n";
   m_script += "\tlogging.info('Copying final gray probability...')\n"; 
   copyFinalPriorProbability(script, m_gray);

   m_script += "\t# Copy CSF Probability #\n";
   m_script += "\tlogging.info('Copying final csf probability...')\n"; 
   copyFinalPriorProbability(script, m_csf);

   m_script += "\t# Copy Rest Probability #\n";
   m_script += "\tlogging.info('Copying final rest probability...')\n"; 
   copyFinalPriorProbability(script, m_rest);

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

