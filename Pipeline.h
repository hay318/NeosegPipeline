#ifndef DEF_Pipeline
#define DEF_Pipeline

// General Librairies
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>

// Qt Librairies
#include <QStringList>
#include <QDir>
#include <QFile>

// My Specific Librairies
#include "PreProcessingData.h"
#include "AtlasRegistration.h"
//#include "AtlasGeneration.h"
//#include "NeosegExecution.h"
#include "Atlas.h"
#include "Neo.h"


class Pipeline
{
   public:
   Pipeline();
   void runPreProcessingData();
   void runAtlasRegistration();
   void runAtlasGeneration();
   void runNeosegExexution();

   void runPipeline();

   void setT1 (QString T1_path);
   void setT2 (QString T2_path);
   void setOutput (QString output_path);
   void setMask (bool mask);
   void setMask (QString mask);
   void setOldAtlas (QString oldAtlas);

   void setSkullStripping (bool skullStripping); 
   void setAtlasPopulation (QString atlasPopulation_path, QStringList selectedAtlases);
   bool checkAtlas(QString atlasDir);
   bool checkAtlasPopulation(QString atlasDir);
   QFileInfoList find(QDir* dir, QString name);

   void setSmoothing (QString smoothing);
   void setSmoothingSize (double smoothingSize);

   void setOverwriting(bool overwriting);
   void setCleaningUp(bool cleaningUp);

   void setComputingSystem(QString computingSystem);
   void setNbCores(int nbCores);

   private: 
   QString m_output_path;
   Neo m_neo;
   QString m_mask_path;

   bool m_newAtlas;

   QString m_oldAtlas;
   bool m_skullStripping;
   bool m_mask;

   QString m_smoothing;
   double m_smoothingSize; 

   bool m_overwriting;
   bool m_cleaningUp;

   QString m_computingSystem;
   int m_nbCores;

   
   std::vector<Atlas> m_atlasPopulation;


   // Pre-Processing Data
   std::string m_PreProcessingData_dir;
   PreProcessingData* m_preProcessingData;

   // Atlas Registration
   std::string m_AtlasRegistration_dir;
   AtlasRegistration* m_atlasRegistration;

   // Atlas Generation
   std::string m_AtlasGeneration_dir;
   //AtlasGeneration* atlasGeneration;

   // Neoseg Execution
   std::string m_NeosegExecution_dir;
   //NeosegExecution* neosegExecution;

   QString m_script;

};

#endif


