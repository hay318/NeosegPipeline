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
   void setNeightborhoodRadiusUnit(QString neightborhoodRadiusUnit);
   void setIncludingFA(bool includingFA);
   void setFAShift(double FAShift);
   void setFASigmaScale(double FASigmaScale);
   void setFAWeight(double FAWeight);
   void setFASmoothingSize(double FASmoothingSize); 
   void setSmoothing(QString smoothing);
   void setSmoothingSize(double smoothingSize);

   // Checking Existing Results
   void defineGeneratedAtlas();

   // Creating Directories 
   void createDirectories();

   void initializeScript();
   void implementRun();  

   // Implementation 
   QString getImage(Atlas atlas, QString name);
   void generateTemplate(TemplateImage& templateImage);
   void extractWMFromFA();
   void generateWeightedAveragedLabels();
   void generatePriorProbability(PriorProbability& priorProbability);
   void computeRest();
   void copyFinalPriorProbability(PriorProbability& probability);

   // Updating & Getting Output 
   void update();
   QString getOutput();

   void cleanUp();

   void setUseT1(bool uset1){
       m_UseT1 = uset1;
   }

   void setABCPipelineModeOn(){
      m_ABCPipelineModeOn = true;
   }

   //Specific functions that generate the ABC scripts
   void generatePriorsProbability();
   void computeRestABC();

private:

   // Input 
   std::vector<Atlas>   m_atlasPopulation;
   QString              m_smoothing; 
   double               m_smoothingSize;
   bool                 m_computingWeights;
   QString              m_neightborhoodRadiusUnit; 
   bool                 m_includingFA;
   double               m_FAShift; 
   double               m_FASigmaScale; 
   double               m_FAWeight; 
   double               m_FASmoothingSize; 
   double               m_neightborhoodRadius;
   bool                 m_UseT1;
   bool                 m_ABCPipelineModeOn;

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


