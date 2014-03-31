#ifndef DEF_PipelineParameters
#define DEF_PipelineParameters

// General Librairies // 
#include <iostream>
#include <vector>

// Qt Librairies // 
#include <QString>
#include <QDir>
#include <QFile>

// My Specific Librairies //
#include "Neo.h"
#include "Atlas.h"
#include "AntsParameters.h" 
#include "NeosegParameters.h" 
#include "ExecutablePaths.h" 


class PipelineParameters
{
   public:

   PipelineParameters();

   bool isBoolean(int value);
   bool isBetween(int value, int min, int max);
   bool isBetween(double value, double min, double max);
   bool isIn(QString item, QStringList list);
   QFileInfoList find(QDir* dir, QString name);

   // Current Directory
   void setProgramPath(QString programPath); 
   QString getProgramPath();

   bool checkPrefixSuffix(QString string); 
   QString getForbiddenCharacters(); 

   // Prefix
   void setPrefix(QString prefix);
   QString getPrefix();

   // Suffix
   void setSuffix(QString suffix);
   QString getSuffix();

   // Output
   bool checkOutput(QString output);
   void setOutput(QString output);
   QString getOutput();

   // T1
   bool checkT1(QString T1);
   void setT1(QString T1);
   QString getT1(); 

   // T2
   bool checkT2(QString T2);
   void setT2(QString T2);
   QString getT2(); 

   // Mask 
   bool checkMask(QString mask);
   void setMask(QString Mask);
   QString getMask(); 

   // DWI 
   bool checkDWI(QString DWI);
   void setDWI(QString DWI);
   QString getDWI(); 

   // b0 
   bool checkb0(QString b0);
   void setb0(QString b0);
   QString getb0(); 

   // Neo
   void initializeNeo();
   void setNeo(Neo neo); 
   Neo getNeo(); 

   // New Atlas    
   void setNewAtlas(bool newAtlas);
   bool getNewAtlas();

   // Atlas
   bool checkExistingAtlas(QString atlas);
   void setAtlas(QString atlas);
   QString getAtlas(); 

   // Atlas Population Directory 
   bool checkAtlasPopulationDirectory(QString atlasPopulationDirectory);
   void setAtlasPopulationDirectory(QString atlasPopulationDirectory);
   QString getAtlasPopulationDirectory(); 

   // Selected Atlases
   void setSelectedAtlases(QStringList selectedAtlases);
   QStringList getSelectedAtlases(); 

   // Atlas Population 
   std::map<QString,QFileInfoList> findAtlasFiles(QString atlas);
   bool checkAtlas(QString atlas);
   void initializeAtlasPopulation();
   void setAtlasPopulation(std::vector<Atlas> m_atlasPopulation);
   std::vector<Atlas> getAtlasPopulation();   

   // Smoothing 
   bool checkSmoothing(QString smoothing);
   QString getSmoothingFlag(QString regularizationType);
   void setSmoothing(QString smoothing);
   QString getSmoothing();
   int getSmoothingIndex();
   QStringList getSmoothingValues(); 

   // Smoothing Size
   bool checkSmoothingSize(double smoothingSize);
   void setSmoothingSize(double smoothingSize);
   double getSmoothingSize();

   // Computing Weights
   void setComputingWeights(int computingWeights);
   bool getComputingWeights();

   // Weights Modality 
   bool checkWeightsModality(QString weightsModality);
   void setWeightsModality(QString weightsModality);
   QString getWeightsModality();
   int getWeightsModalityIndex();
   QStringList getWeightsModalityValues(); 

   // Weights Radius 
   bool checkWeightsRadius(double weightsRadius);
   void setWeightsRadius(double weightsRadius);
   double getWeightsRadius();

   // Weights Radius Unit
   bool checkWeightsRadiusUnit(QString radiusUnit);
   void setWeightsRadiusUnit(QString radiusUnit);
   QString getWeightsRadiusUnit();
   int getWeightsRadiusUnitIndex();
   QStringList getWeightsRadiusUnitValues();

   // Including FA
   void setIncludingFA(bool includingFA);
   bool getIncludingFA();

   // FA Weight
   void setFAWeight(double FAWeight);
   double getFAWeight(); 

   // FA Smoothing Size
   void setFASmoothingSize(double FASmoothingSize);
   double getFASmoothingSize(); 

   // Computing 3-Labels Segmentation
   void setComputing3LabelsSeg(bool computing3LabelsSeg);
   bool getComputing3LabelsSeg();   

   // Overwriting
   void setOverwriting(bool overwriting);
   bool getOverwriting();

   // Cleaning-Up 
   void setCleaningUp(bool cleaningUp);
   bool getCleaningUp(); 

   // Stopping If Error
   void setStoppingIfError(bool stoppingIfError); 
   bool getStoppingIfError();  

   // Computing System 
   bool checkComputingSystem(QString computingSystem);
   void setComputingSystem(QString computingSystem);
   QString getComputingSystem(); 
   int getComputingSystemIndex(); 
   QStringList getComputingSystemValues(); 

   // Number Of Cores 
   bool checkNumberOfCores(int NumberOfCores);
   void setNumberOfCores(int NumberOfCores); 
   int getNumberOfCores();

   // Debug 
   void setDebug(bool debug);
   bool getDebug();

   // ANTS Parameters 
   AntsParameters* getAntsParameters();  

   // Neoseg Parameters
   NeosegParameters* getNeosegParameters(); 

   // Executable Paths 
   ExecutablePaths* getExecutablePaths(); 

   // Segmentation
   QString getSegmentation(); 


   private:

   QString m_programPath; 
   
   QStringList m_forbiddenCharacters; 

   // Data
   QString m_prefix;
   QString m_suffix;
   QString m_output;
   QString m_T1; 
   QString m_T2;
   QString m_mask; 
   QString m_DWI; 
   QString m_b0; 
   bool m_skullStripping; 

   Neo m_neo; 

   // New Atlas 
   bool m_newAtlas_default;
   bool m_newAtlas;

   // Atlas
   QString m_atlas; 

   // Atlas Population Directory
   QString m_atlasPopulationDirectory_default; 
   QString m_atlasPopulationDirectory; 

   // Atlas Population
   QStringList m_selectedAtlases; 
   std::vector<Atlas> m_atlasPopulation; 

   // Smoothing 
   QStringList m_smoothing_values;
   QString m_smoothing_default;
   QString m_smoothing;

   // Smoothing Size
   double m_smoothingSize_min;
   double m_smoothingSize_max;
   double m_smoothingSize_default;
   double m_smoothingSize;

   // Computing Weights
   bool m_computingWeights_default;
   bool m_computingWeights;

   // Weights Modality
   QStringList m_weightsModality_values;
   QString m_weightsModality_default;
   QString m_weightsModality;

   // Weights Radius
   double m_weightsRadius_min;
   double m_weightsRadius_max;
   double m_weightsRadius_default;
   double m_weightsRadius;

   // Weights Radius Unit
   QStringList m_weightsRadiusUnit_values; 
   QString m_weightsRadiusUnit_default; 
   QString m_weightsRadiusUnit; 

   // Including FA
   bool m_includingFA_default; 
   bool m_includingFA;    

   // FA weight
   double m_FAWeight_default; 
   double m_FAWeight; 

   // FA Smoothing Size 
   double m_FASmoothingSize_default;
   double m_FASmoothingSize;
    
   
   // Computing 3-labels segmentation
   bool m_computing3LabelsSeg_default; 
   bool m_computing3LabelsSeg;

   // Overwriting 
   bool m_overwriting_default;
   bool m_overwriting;

   // Cleaning-Up
   bool m_cleaningUp_default;
   bool m_cleaningUp;

   // Stopping if error 
   bool m_stoppingIfError_default; 
   bool m_stoppingIfError; 

   // Computing System
   QStringList m_computingSystem_values;
   QString m_computingSystem_default;
   QString m_computingSystem;

   // Number Of Cores
   int m_numberOfCores_min;
   int m_numberOfCores_max;
   int m_numberOfCores_default;
   int m_numberOfCores;

   // Debug 
   bool m_debug_default; 
   bool m_debug; 
   
   // ANTS Parameters 
   AntsParameters* m_antsParameters;

   // Neoseg Parameters
   NeosegParameters* m_neosegParameters;

   // Executable Paths 
   ExecutablePaths* m_executablePaths;
};


#endif
