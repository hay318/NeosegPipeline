#ifndef DEF_AtlasGeneration
#define DEF_AtlasGeneration

// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

// Qt Librairies
#include <QString>
#include <QDir>
#include "Atlas.h"
#include "PriorProbability.h"
#include "TemplateImage.h"
#include "Neo.h"
#include "ExecutablePaths.h"


class AtlasGeneration
{
   public:

   // Constructor
   AtlasGeneration();

   // Setting Parameters
   void setNeo(Neo neo);
   void setAtlasPopulation(std::vector<Atlas> atlasPopulation);  
   void setOutputPath(QString output_path); 
   void setComputingWeights(bool computingWeights);
   void setNeightborhoodRadius(double neightborhoodRadius);
   void setSmoothing(QString smoothing);
   void setSmoothingSize(double smoothingSize);
   void setExecutablePaths(ExecutablePaths* executables);

   // Checking Existing Results
   void defineGeneratedAtlas();
   bool checkGeneratedAtlas();

   // Create Directories 
   void createDirectories();

   // Writing Script 
   QString variable(QString variable_name);
   QString str(QString str);
   QString listToString(QStringList argumentsList);

   void initializeScript(QString& script);
   void implementLogStdoutAndStderr(QString &script);
   void implementLogStderr(QString &script);

   void implementGenerateAtlas(QString& script);  
   void writeGenerateAtlas();

   // Implementation 
   QString getImage(Atlas atlas, QString name);
   void generateTemplate(QString& script, TemplateImage templateImage);
   void addSubElement(QString& script, QString element, QString pythonName, QString XMLname, QString value);
   void extractWMFromFA(QString &script);
   void generateWeightedAveragedLabels(QString& script);
   void generatePriorProbability(QString& script, PriorProbability& priorProbability);
   void preNormalizePriorProbability(QString &script, PriorProbability priorProbability);
   void preNormalizePriorProbabilities(QString &script);
   void computeRest(QString& script);
   void copyFinalPriorProbability(QString& script, PriorProbability& probability);

   // Updating & Getting Output 
   void update();
   QString getOutput();


   private:
   Neo m_neo;
   std::vector<Atlas>            m_atlasPopulation;

   //Executables 
   ExecutablePaths* m_executables;

   TemplateImage                 m_templateT1; 
   TemplateImage                 m_templateT2;
   PriorProbability              m_white;
   PriorProbability              m_gray;
   PriorProbability              m_csf;
   PriorProbability              m_rest;

   QString                       m_smoothing; 
   double                        m_smoothingSize;

   bool                          m_computingWeights;
   double                        m_neightborhoodRadius; 

   // Output //
   QString                       m_output_path;
   QDir*                         m_output_dir;

   QString                       m_atlasGeneration_path;               
   QDir*                         m_atlasGeneration_dir; 

   QDir*                         m_priorProbabilities_dir; 
          
   QString                       ImageMath;
   QString                       ResampleVolume2;   


};

#endif


