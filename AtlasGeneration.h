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


#include "Script.h"
#include "Atlas.h"
#include "PriorProbability.h"
#include "TemplateImage.h"
#include "Neo.h"
#include "ExecutablePaths.h"


class AtlasGeneration : public Script
{
   public:

   // Constructor
   AtlasGeneration(QString module);

   // Setting Parameters
   void setAtlasPopulation(std::vector<Atlas> atlasPopulation);  
   void setComputingWeights(bool computingWeights);
   void setNeightborhoodRadius(double neightborhoodRadius);
   void setIncludingFA(bool includingFA);
   void setSmoothing(QString smoothing);
   void setSmoothingSize(double smoothingSize);

   // Checking Existing Results
   void defineGeneratedAtlas();

   // Creating Directories 
   void createDirectories();

   void initializeScript(QString& script);
   void implementRun(QString& script);  

   // Implementation 
   QString getImage(Atlas atlas, QString name);
   void generateTemplate(QString& script, TemplateImage& templateImage);
   void extractWMFromFA(QString &script);
   void generateWeightedAveragedLabels(QString& script);
   void generatePriorProbability(QString& script, PriorProbability& priorProbability);
   void preNormalizePriorProbability(QString &script, PriorProbability& priorProbability);
   void computeSumProbabilities(QString &script);
   void computeRest(QString& script);
   void copyFinalPriorProbability(QString& script, PriorProbability& probability);

   // Updating & Getting Output 
   void update();
   QString getOutput();


   private:

   // Input 
   std::vector<Atlas>   m_atlasPopulation;
   QString              m_smoothing; 
   double               m_smoothingSize;
   bool                 m_computingWeights;
   bool                 m_includingFA;
   double               m_neightborhoodRadius; 

   // Directories
   QDir*                m_priorProbabilities_dir; 

   // Output //
   TemplateImage        m_templateT1; 
   TemplateImage        m_templateT2;
   PriorProbability     m_white;
   PriorProbability     m_gray;
   PriorProbability     m_csf;
   PriorProbability     m_rest;

};

#endif


