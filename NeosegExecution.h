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
   void setUsingFA(bool usingFA);
   void setUsingAD(bool usingAD);
   void setComputing3LabelsSeg(bool computing3LabelsSeg);
   void setNeosegParameters(NeosegParameters* parameters);

   // Checking Existing Results
   void defineSegmentation();
   bool checkSegmentation();

   // Implementing Script 
   void initializeScript(QString &script);
   void writeXMLFile(QString &script);
   void implementWriteAffineTranformationFile(QString& script);
   void writeAffineTranformationFiles(QString& script);
   void runNeoseg(QString &script);
   void mergeWhiteMatters(QString &script);
   void implementRun(QString &script);  


   // Updating & Getting Output 
   void update();
   Neo getOutput();


   private:

   // Parameters
   QString              m_atlas; 
   bool                 m_usingFA;
   bool                 m_usingAD;
   bool                 m_computing3LabelsSeg;
   NeosegParameters*    m_parameters;

   // Outout 
   QString              m_segmentation;
};

#endif

