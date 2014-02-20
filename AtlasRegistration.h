#ifndef DEF_AtlasRegistration
#define DEF_AtlasRegistration

// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <utility>

// Qt Librairies
#include <QString>
#include <QDir>

// My Specific Librairies
#include "Atlas.h"
#include "Neo.h"
#include "AntsParameters.h"
#include "ExecutablePaths.h"

class AtlasRegistration
{
   
   public: 

   // Set // 
   void setNeo(Neo neo);
   void setAtlasPopulation(std::vector<Atlas> atlasPopulation);  
   void setOutputPath(QString output_path); 
   void setComputingSystem(QString computingSystem);
   void setNumberOfCores(int nbCores); 
   void setAntsParameters(AntsParameters* ANTSParamaters);
   void setExecutablePaths(ExecutablePaths* executables);

   // Define Outputs //
   Atlas defineRegisteredAtlas(Atlas atlas);
   void defineRegisteredAtlasPopulation();
 
   // Checking results //
   bool checkRegisteredAtlas(Atlas atlas);
   bool checkRegisteredAtlasPopulation();

   // Creating directories //
   void createDirectory();
   void createAtlasDirectories();

   // Scripts //
   QString str(QString str);
   QString variable(QString variable_name);
   QString listToString(QStringList argumentsList);
   void initializeScript(QString &script);

   // Register Atlas Script // 
   void implementRegisterAtlas(QString &script, bool probabilistic);
   void writeRegisterAtlas();
   void writeRegisterProbabilisticAtlas();

   // Register Atlas Population Script // 
   void implementTestRegistrationDone(QString &script);
   void defineRegisterAtlasParameters(QString &script, Atlas atlas);
   void submitRegisterAtlasJob(QString &script);
   void executeRegisterAtlasProcess(QString &script, int i, bool probabilistic);
   void implementRegisterAtlasPopulation(QString &script);
   void writeRegisterAtlasPopulation();

   // Output //
   void update();
   std::vector<Atlas> getOutput();


   private:

   // Input // 
   Neo                     m_neo;
   std::vector<Atlas>      m_atlasPopulation;
   AntsParameters*         m_parameters;
   ExecutablePaths*        m_executables; 
   QString                 m_computingSystem;
   int                     m_nbCores;
   
   // Directories
   QDir*                   m_output_dir;
   QDir*                   m_atlasRegistration_dir; 

   // Output //

   std::vector<Atlas>      m_atlasPopulationRegistered;
};

#endif 
