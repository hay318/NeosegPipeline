#ifndef DEF_Pipeline
#define DEF_Pipeline

// General Librairies
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <utility>
#include <sys/stat.h>

// Qt Librairies
#include <QDir>
#include <QStringList>
#include <QProcess>

// My Specific Librairies
#include "PipelineParameters.h"
#include "PreProcessingData.h"
#include "DtiRegistration.h"
#include "AtlasRegistration.h"
#include "AtlasGeneration.h"
#include "NeosegExecution.h"


class Pipeline
{
   public:

   // Constructor
   Pipeline();

   // Parameters
   void setPipelineParameters(PipelineParameters* parameters);

   // Process
   QProcess* getMainScriptProcess();

   // Save Configuration //
   void saveConfiguration(QString xml_path);  // to delete

   // Run Pipeline
   void runPipeline();


   private: 

   void createProcessingDirectory();
   QString createModuleDirectory(QString directory_name);

   void initializeMainScript();
   void initializeLogging();
   void defineSignalHandler();
   void writePreProcessingData();
   void writeAtlasRegistration();
   void writeDTIRegistration();   
   void writeAtlasGeneration();
   void writeNeosegExecution();
   void writeMainScript();
   void executeMainScript();
   void copySegmentations();


   PipelineParameters* m_parameters; 

   QString m_processing_name;
   QString m_processing_path;

   QString m_script;
   QString m_importingModules;
   QString m_runningModules;

   QString m_main_path;

   // QProcess
   QProcess* m_mainScriptProcess;
};

#endif


