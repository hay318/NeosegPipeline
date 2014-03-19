#include "Pipeline.h"

Pipeline::Pipeline()
{
   m_processing_name = "Processing";
}

void Pipeline::setPipelineParameters(PipelineParameters* parameters)
{
   m_parameters = parameters; 
}

QProcess* Pipeline::getMainScriptProcess()
{
   return m_mainScriptProcess;
}

void Pipeline::createProcessingDirectory()
{
   QDir output_dir(m_parameters->getOutput());
   if(!output_dir.exists(m_processing_name))
   {
      output_dir.mkdir(m_processing_name);
   } 
   m_processing_path = output_dir.filePath(m_processing_name);
}

QString Pipeline::createModuleDirectory(QString directory_name)
{
   QDir output_dir(m_parameters->getOutput());
   if(!output_dir.exists(directory_name))
   {
      output_dir.mkdir(directory_name);
   }  
   return output_dir.filePath(directory_name);
}

void Pipeline::writePreProcessingData()
{
   QString directory_name = "1.PreProcessingData";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "preProcessingData";
   PreProcessingData preProcessingData(module_name);
   
   preProcessingData.setNeo(m_parameters->getNeo());
   preProcessingData.setSkullStripping(m_parameters->getSkullStripping());  
   preProcessingData.setModuleDirectory(directory_path);
   preProcessingData.setProcessingDirectory(m_processing_path);
   preProcessingData.setExecutablePaths(m_parameters->getExecutablePaths());

   preProcessingData.update(); 
   m_importingModules += "import " + module_name + "\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setNeo(preProcessingData.getOutput());
}

void Pipeline::writeDTIRegistration()
{
   QString directory_name = "2.DTIRegistration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "DTIRegistration";
   DtiRegistration dtiRegistration(module_name);

   dtiRegistration.setNeo(m_parameters->getNeo());
   dtiRegistration.setModuleDirectory(directory_path);
   dtiRegistration.setProcessingDirectory(m_processing_path);
   dtiRegistration.setExecutablePaths(m_parameters->getExecutablePaths());

   dtiRegistration.update();
   m_importingModules += "import " + module_name + "\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setNeo(dtiRegistration.getOutput());
}

void Pipeline::writeAtlasRegistration()
{
   QString directory_name = "3.AtlasRegistration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "atlasRegistration";
   AtlasRegistration atlasRegistration(module_name); 

   atlasRegistration.setNeo(m_parameters->getNeo());
   atlasRegistration.setAtlasPopulation(m_parameters->getAtlasPopulation());
   atlasRegistration.setModuleDirectory(directory_path);
   atlasRegistration.setProcessingDirectory(m_processing_path);
   atlasRegistration.setComputingSystem(m_parameters->getComputingSystem());
   atlasRegistration.setNumberOfCores(m_parameters->getNumberOfCores());
   atlasRegistration.setAntsParameters(m_parameters->getAntsParameters());
   atlasRegistration.setExecutablePaths(m_parameters->getExecutablePaths());

   atlasRegistration.defineRegisteredAtlasPopulation();
   if(!atlasRegistration.checkRegisteredAtlasPopulation())
   {
      atlasRegistration.update();
      m_importingModules += "import " + module_name + "\n"; 
      m_runningModules += module_name + ".run()\n"; 
   }

   m_parameters->setAtlasPopulation(atlasRegistration.getOutput());
}

void Pipeline::writeAtlasGeneration()
{
   QString directory_name = "4.AtlasGeneration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "atlasGeneration";
   AtlasGeneration atlasGeneration(module_name);

   atlasGeneration.setNeo(m_parameters->getNeo());
   atlasGeneration.setAtlasPopulation(m_parameters->getAtlasPopulation());
   atlasGeneration.setModuleDirectory(directory_path);
   atlasGeneration.setProcessingDirectory(m_processing_path);
   atlasGeneration.setSmoothing(m_parameters->getSmoothing());
   atlasGeneration.setSmoothingSize(m_parameters->getSmoothingSize());
   atlasGeneration.setIncludingFA(m_parameters->getIncludingFA());
   atlasGeneration.setComputingWeights(m_parameters->getComputingWeights());
   atlasGeneration.setNeightborhoodRadius(m_parameters->getWeightsRadius());
   atlasGeneration.setExecutablePaths(m_parameters->getExecutablePaths());

   atlasGeneration.defineGeneratedAtlas();
   if(!atlasGeneration.checkGeneratedAtlas())
   {
      atlasGeneration.update();
      m_importingModules += "import " + module_name + "\n"; 
      m_runningModules += module_name + ".run()\n"; 
   }

   m_parameters->setAtlas(atlasGeneration.getOutput());
}

void Pipeline::writeNeosegExecution()
{
   QString directory_name = "5.NeosegExecution";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "neosegExecution";
   NeosegExecution neosegExecution(module_name);

   std::cout<<"using FA:"<<m_parameters->getUsingFA()<<std::endl; 
   std::cout<<"using AD:"<<m_parameters->getUsingAD()<<std::endl; 

   neosegExecution.setNeo(m_parameters->getNeo());
   neosegExecution.setModuleDirectory(directory_path);
   neosegExecution.setProcessingDirectory(m_processing_path);
   neosegExecution.setAtlas(m_parameters->getAtlas());
   neosegExecution.setUsingFA(m_parameters->getUsingFA());
   neosegExecution.setUsingAD(m_parameters->getUsingAD());
   neosegExecution.setComputing3LabelsSeg(m_parameters->getComputing3LabelsSeg());
   neosegExecution.setNeosegParameters(m_parameters->getNeosegParameters());
   neosegExecution.setExecutablePaths(m_parameters->getExecutablePaths());

   neosegExecution.update();
   m_importingModules += "import " + module_name +"\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setNeo(neosegExecution.getOutput());
}

void Pipeline::initializeMainScript()
{
   m_script = "#!/usr/bin/env python\n\n";

   m_script += "import os\n";
   m_script += "import sys\n";
   m_script += "import signal\n";
   m_script += "import logging\n";
   m_script += "import subprocess\n";
   m_script += "import shutil\n\n";
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
   QString log = output_dir->filePath((m_parameters->getNeo()).prefix + ".log"); 

   m_script += "log = \"" + log + "\"\n";
   m_script += "logFile = open(log, \"w\") \n\n";

   m_script += "logging.basicConfig(level=logging.DEBUG, filename=log)\n";

   m_script += "rootLogger = logging.getLogger()\n";
   m_script += "fileFormatter = logging.Formatter('%(message)s')\n";
   m_script += "((rootLogger.handlers)[0]).setFormatter(fileFormatter)\n\n";

   m_script += "consoleHandler = logging.StreamHandler()\n";
   m_script += "consoleHandler.setLevel(logging.INFO)\n";
   m_script += "consoleFormatter = logging.Formatter('[%(levelname)s]  %(message)s')\n";
   m_script += "consoleHandler.setFormatter(consoleFormatter)\n";

   m_script += "rootLogger.addHandler(consoleHandler)\n";
}

void Pipeline::copySegmentations()
{
   QDir* output_dir = new QDir(m_parameters->getOutput());
   Neo neo = m_parameters->getNeo(); 

   m_script += "seg4Labels_src = \"" + neo.seg4Labels + "\"\n";
   QString seg4Labels_name = neo.prefix + "-seg-4Labels.nrrd";
   QString seg4Labels_path = output_dir->filePath(seg4Labels_name);
   m_script += "seg4Labels_dest = \"" + seg4Labels_path + "\"\n";

   m_script += "shutil.copyfile(seg4Labels_src, seg4Labels_dest)\n\n"; 

   if(m_parameters->getComputing3LabelsSeg())
   {
      m_script += "seg3Labels_src = \"" + neo.seg3Labels + "\"\n";
      QString seg3Labels_name = neo.prefix + "-seg-3Labels.nrrd";
      QString seg3Labels_path = output_dir->filePath(seg3Labels_name);
      m_script += "seg3Labels_dest = \"" + seg3Labels_path + "\"\n";

      m_script += "shutil.copyfile(seg3Labels_src, seg3Labels_dest)\n\n"; 
   }
}

void Pipeline::writeMainScript()
{
   QDir* processing_dir = new QDir(m_processing_path); 
   m_main_path = processing_dir->filePath("main.py");

   std::ofstream* script_stream = new std::ofstream((m_main_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);

   m_script += m_importingModules; 
   defineSignalHandler();
   initializeLogging();

   m_script += m_runningModules; 

   copySegmentations();

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

   std::cout<<"pipeline done"<<std::endl; 
}

void Pipeline::runPipeline()
{
   createProcessingDirectory();

   m_parameters->initializeNeo(); 
   m_parameters->initializeAtlasPopulation(); 

   initializeMainScript();

   writePreProcessingData();
   writeDTIRegistration(); 

   if (m_parameters->getNewAtlas()==true)
   {
      writeAtlasRegistration();

      writeAtlasGeneration();
   }

   writeNeosegExecution();

   writeMainScript();

   executeMainScript();
}

