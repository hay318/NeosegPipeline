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
   void setNewAtlas(bool newAtlas); 
   void setAtlas(QString atlas);  
   void setUsingFA(bool usingFA);  
   void setUsingAD(bool usingAD);  
   void setNeosegParameters(NeosegParameters* parameters);
   void setComputing3LabelsSeg(bool computing3LabelsSeg);
   void setReassigningWhiteMatter(bool reassigningWhiteMatter);
   void setSizeThreshold(int sizeThreshold);

   // Implementing Script 
   void initializeScript();
   void writeXMLFile();
   void implementWriteAffineTranformationFile();
   void writeAffineTranformationFiles();
   void runNeoseg();
   void mergeWhiteMatters();
   void reassignWhiteMatter(); 
   void implementRun();  


   // Updating & Getting Output 
   void update();
   Neo getOutput();


   private:

   // Atlas
   bool m_newAtlas;
   QString m_atlas; 

   // Using FA
   bool m_usingFA; 

   // Using AD
   bool m_usingAD; 

   // Computing 3-labels segmentation 
   bool m_computing3LabelsSeg;
   
   // Reassigning White Matter 
   bool m_reassigningWhiteMatter; 
   int m_sizeThreshold; 

   // Neoseg Parameters
   NeosegParameters* m_parameters;

   // Outout 
   QString m_segmentation;
};

#endif

