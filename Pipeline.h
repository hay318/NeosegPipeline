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
   PipelineParameters* getParameters();

   // Process
   QProcess* getMainScriptProcess();

   // Save Configuration //
   void saveConfiguration(QString xml_path);  // to delete

   // Run Pipeline
   void runPipeline();


   private: 

   void initializeMainScript();
   void initializeLogging();
   void defineSignalHandler();
   void runPreProcessingData();
   void runAtlasRegistration();
   void runDTIRegistration();   
   void runAtlasGeneration();
   void runNeosegExecution();
   void writeMainScript();
   void executeMainScript();


   PipelineParameters* m_parameters; 

   QString m_script;
   QString m_importingModules;
   QString m_runningModules;

   QString m_main_path;

   // QProcess
   QProcess* m_mainScriptProcess;
};

#endif


