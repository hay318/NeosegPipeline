#ifndef DEF_AtlasRegistration
#define DEF_AtlasRegistration

// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

// Qt Librairies
#include <QString>
#include <QDir>

// My Specific Librairies
#include "Atlas.h"
#include "Neo.h"


class AtlasRegistration
{
   public: 

   // Constructor //
   AtlasRegistration(QString output_path, QString computingSystem);

   // Checking results //
   Atlas defineOutput_Atlas(Atlas atlas);
   bool checkExistingResults_Atlas(Atlas atlas);
   bool checkExistingResults(std::vector<Atlas> atlasPopulation);

   // Creating directories //
   void createDirectory();
   void createAtlasDirectories(std::vector<Atlas> atlasPopulation);

   // Writting the script //
   QString str(QString str);
   QString variable(QString variable_name);

   // Script //
   void initializeScript(QString &script);

   void implementRegisterAtlas(QString &script, Neo neo);
   void writeRegisterAtlas(Neo neo);

   void implementTestRegistrationDone(QString &script);
   void defineRegisterAtlasParameters(QString &script, Atlas atlas);
   void submitRegisterAtlasJob(QString &script);
   void executeRegisterAtlasProcess(QString &script, int i);
   void implementRegisterAtlasPopulation(QString &script, std::vector<Atlas> atlasPopulation);
   void writeRegisterAtlasPopulation(std::vector<Atlas> atlasPopulation);

   // Output //
   void updateAtlasPopulation(std::vector<Atlas> &atlasPopulation);

   private:

   QString m_computingSystem;

   QString m_ANTS;
   QString m_ResampleVolume2; 

   QDir* m_output_dir;
   QDir* m_atlasRegistration_dir; 
};

#endif 
