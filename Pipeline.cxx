#include "Pipeline.h"

Pipeline::Pipeline()
{
   m_parameters=new PipelineParameters(); 
}

PipelineParameters* Pipeline::getParameters() {return m_parameters;}

QProcess* Pipeline::getMainScriptProcess() {return m_mainScriptProcess;}

void Pipeline::runPreProcessingData()
{
   PreProcessingData preProcessingData;
   
   preProcessingData.setNeo(m_parameters->getNeo());
   preProcessingData.setSkullStripping(m_parameters->getSkullStripping());
   preProcessingData.setOutputPath(m_parameters->getOutput());
   preProcessingData.setExecutablePaths(m_parameters->getExecutablePaths());

   preProcessingData.definePreProcessedData(); 
   if(!preProcessingData.checkPreProcessedData())
   {
      preProcessingData.update(); 
      m_importingModules += "import preProcessData\n"; 
      m_runningModules += "preProcessData.run()\n"; 
   }

   m_parameters->setNeo(preProcessingData.getOutput());
}


void Pipeline::runAtlasRegistration()
{
   AtlasRegistration atlasRegistration;

   atlasRegistration.setNeo(m_parameters->getNeo());
   atlasRegistration.setAtlasPopulation(m_parameters->getAtlasPopulation());
   atlasRegistration.setOutputPath(m_parameters->getOutput());
   atlasRegistration.setComputingSystem(m_parameters->getComputingSystem());
   atlasRegistration.setNumberOfCores(m_parameters->getNumberOfCores());
   atlasRegistration.setAntsParameters(m_parameters->getAntsParameters());
   atlasRegistration.setExecutablePaths(m_parameters->getExecutablePaths());

   atlasRegistration.defineRegisteredAtlasPopulation();
   if(!atlasRegistration.checkRegisteredAtlasPopulation())
   {
      atlasRegistration.update();
      m_importingModules += "import registerAtlas\n"; 
      m_runningModules += "registerAtlas.run()\n"; 
   }

   m_parameters->setAtlasPopulation(atlasRegistration.getOutput());
}

void Pipeline::runDTIRegistration()
{
   DtiRegistration dtiRegistration;

   dtiRegistration.setNeo(m_parameters->getNeo());
   dtiRegistration.setOutputPath(m_parameters->getOutput());
   dtiRegistration.setExecutablePaths(m_parameters->getExecutablePaths());

   dtiRegistration.defineRegisteredDTI();
   if(!dtiRegistration.checkRegisteredDTI())
   {
      dtiRegistration.update();
      m_importingModules += "import registerDTI\n"; 
      m_runningModules += "registerDTI.run()\n"; 
   }

   m_parameters->setNeo(dtiRegistration.getOutput());
}

void Pipeline::runAtlasGeneration()
{
   AtlasGeneration atlasGeneration;

   atlasGeneration.setNeo(m_parameters->getNeo());
   atlasGeneration.setAtlasPopulation(m_parameters->getAtlasPopulation());
   atlasGeneration.setOutputPath(m_parameters->getOutput());
   atlasGeneration.setSmoothing(m_parameters->getSmoothing());
   atlasGeneration.setSmoothingSize(m_parameters->getSmoothingSize());
   atlasGeneration.setComputingWeights(m_parameters->getComputingWeights());
   atlasGeneration.setNeightborhoodRadius(m_parameters->getWeightsRadius());
   atlasGeneration.setExecutablePaths(m_parameters->getExecutablePaths());

   atlasGeneration.defineGeneratedAtlas();
   if(!atlasGeneration.checkGeneratedAtlas())
   {
      atlasGeneration.update();
      m_importingModules += "import generateAtlas\n"; 
      m_runningModules += "generateAtlas.run()\n"; 
   }

   m_parameters->setAtlas(atlasGeneration.getOutput());
}

void Pipeline::runNeosegExecution()
{
   NeosegExecution neosegExecution;

   neosegExecution.setNeo(m_parameters->getNeo());
   neosegExecution.setOutputPath(m_parameters->getOutput());
   neosegExecution.setAtlas(m_parameters->getAtlas());
   neosegExecution.setNeosegParameters(m_parameters->getNeosegParameters());
   neosegExecution.setExecutablePaths(m_parameters->getExecutablePaths());

   neosegExecution.defineSegmentation();
   if(!neosegExecution.checkSegmentation())
   {
      neosegExecution.update();
      m_importingModules += "import executeNeoseg\n"; 
      m_runningModules += "executeNeoseg.run()\n"; 
   }

   m_parameters->setSegmentation(neosegExecution.getOutput());
}

void Pipeline::initializeMainScript()
{
   m_script = "#!/usr/bin/env python\n\n";

   m_script += "import os\n";
   m_script += "import sys\n";
   m_script += "import signal\n";
   m_script += "import logging\n";
   m_script += "import subprocess\n\n";
}

void Pipeline::defineSignalHandler()
{
   m_script += "def signal_handler(signal, frame):\n";
   m_script += "\tprint '***************You pressed Ctrl+C!******************'\n";
   m_script += "\tif runningProcess.poll!=1:\n";
   m_script += "\t\trunningProcess.terminate()\n";
   m_script += "\tsys.exit(0)\n\n";

   m_script += "signal.signal(signal.SIGINT, signal_handler)\n";
   m_script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";
}

void Pipeline::initializeLogging()
{
   QDir* output_dir = new QDir(m_parameters->getOutput());
   QString log = output_dir->filePath("pipeline.log"); 

   m_script += "log = \"" + log + "\"\n";
   m_script += "logFile = open(log, \"w\") \n\n";

   m_script += "logging.basicConfig(level=logging.DEBUG)\n";
   m_script += "rootLogger = logging.getLogger()\n";

   m_script += "logFormatter = logging.Formatter('[%(levelname)s]  %(message)s')\n";

   m_script += "fileHandler = logging.FileHandler(log)\n";
   m_script += "fileHandler.setLevel(logging.DEBUG)\n";
   m_script += "fileHandler.setFormatter(logFormatter)\n";

   m_script += "rootLogger.addHandler(fileHandler)\n";
   m_script += "((rootLogger.handlers)[0]).setFormatter(logFormatter)\n\n";
}

void Pipeline::writeMainScript()
{
   QDir* output_dir = new QDir(m_parameters->getOutput()); 
   m_main_path = output_dir->filePath("main.py");

   std::ofstream* script_stream = new std::ofstream((m_main_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);

   m_script += m_importingModules; 
   defineSignalHandler();
   initializeLogging();
   m_script += m_runningModules; 

   *script_stream << m_script.toStdString() << std::endl; 
   
   script_stream->close();

   chmod((m_main_path.toStdString()).c_str(), 0755);
}

void Pipeline::executeMainScript()
{
   QString command; 

   if (!(m_parameters->getComputingSystem()).compare("local", Qt::CaseInsensitive))
   {
      command = m_main_path;
   }

   if (!(m_parameters->getComputingSystem()).compare("killdevil", Qt::CaseInsensitive))
   {
      command = "bsub -n 1 python " +  m_main_path;      
   }   

   m_mainScriptProcess = new QProcess();
   m_mainScriptProcess->setProcessChannelMode(QProcess::ForwardedChannels);
   m_mainScriptProcess->start(command);

   while (!m_mainScriptProcess->waitForFinished())
   {
   }

   std::cout<<"QProcess finished!"<<std::endl;
}


void Pipeline::runPipeline()
{
   m_parameters->initializeNeo(); 
   m_parameters->initializeAtlasPopulation(); 

   initializeMainScript();

   runPreProcessingData();

   runDTIRegistration(); 

   if (m_parameters->getNewAtlas()==true)
   {
      runAtlasRegistration();

      runAtlasGeneration();
   }

   runNeosegExecution();

   writeMainScript();
   executeMainScript();
}

