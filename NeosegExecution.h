#ifndef DEF_NeosegExecution
#define DEF_NeosegExecution

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
#include "Neo.h"
#include "NeosegParameters.h"
#include "ExecutablePaths.h"


class NeosegExecution : public Script 
{
   public:

   // Constructor
   NeosegExecution(QString module);

   // Setting Parameters
   void setAtlas(QString atlas);  
   void setComputing3LabelsSeg(bool computing3LabelsSeg);
   void setNeosegParameters(NeosegParameters* parameters);

   // Checking Existing Results
   void defineSegmentation();
   bool checkSegmentation();

   // Implementing Script 
   void initializeScript();
   void writeXMLFile();
   void implementWriteAffineTranformationFile();
   void writeAffineTranformationFiles();
   void runNeoseg();
   void mergeWhiteMatters();
   void implementRun();  


   // Updating & Getting Output 
   void update();
   Neo getOutput();


   private:

   // Parameters
   QString              m_atlas; 
   bool                 m_computing3LabelsSeg;
   NeosegParameters*    m_parameters;

   // Outout 
   QString              m_segmentation;
};

#endif

