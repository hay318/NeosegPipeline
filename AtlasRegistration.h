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
#include "Script.h"
#include "Atlas.h"
#include "Neo.h"
#include "AntsParameters.h"
#include "ExecutablePaths.h"

class AtlasRegistration : public Script  
{
   
   public: 

   // Constructor // 
   AtlasRegistration(QString module);

   // Set // 
   void setAtlasPopulation(std::vector<Atlas> atlasPopulation);  
   void setComputingSystem(QString computingSystem);
   void setNumberOfCores(int nbCores); 
   void setAntsParameters(AntsParameters* ANTSParamaters);

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
   void initializeScript(QString &script);
   void implementExecute(QString &script);

   // Register Atlas Script // 
   void implementRegisterAtlas(QString &script, bool probabilistic);
   void writeRegisterAtlas();
   void writeRegisterProbabilisticAtlas();

   // Register Atlas Population Script // 
   void implementTestRegistrationDone(QString &script);
   void defineRegisterAtlasParameters(QString &script, Atlas atlas);
   void submitRegisterAtlasJob(QString &script);
   void executeRegisterAtlasProcess(QString &script, Atlas atlas, int i);
   void implementRegisterAtlasPopulation(QString &script);
   void writeRegisterAtlasPopulation();

   // Output //
   void update();
   std::vector<Atlas> getOutput();


   private:

   // Input // 
   std::vector<Atlas>      m_atlasPopulation;
   AntsParameters*         m_parameters;
   QString                 m_computingSystem;
   int                     m_nbCores;
   
   // Output //
   std::vector<Atlas>      m_atlasPopulationRegistered;
};

#endif 
