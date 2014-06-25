#ifndef DEF_PreProcessingData
#define DEF_PreProcessingData

// General Librairies
#include <iostream>
#include <fstream>

// Qt Librairies
#include <QString>
#include <QDir>

// My specific librairies
#include "Script.h"
#include "Neo.h"
#include "ExecutablePaths.h"
#include "MinMax.h"


class PreProcessingData : public Script 
{
   public:

   // Constructor 
   PreProcessingData(QString module);

   // Set 
   void setAddingExtraCSF(bool addingExtraCSF); 

   // Implementing Script 
   void initializeScript();
   QString skullStripImage(QString image);
   QString correctImage(QString image);
   QString dilateMask();
   QString addExtraCSF();
   void implementRun();

   // Updating & Getting Output 
   void update();
   Neo getOutput();

   private: 

   // Paraneters
   bool              m_addingExtraCSF; 

   // Suffix
   QString           m_skullStripping_suffix;
   QString           m_converting_suffix;
   QString           m_correcting_suffix;
   QString           m_rescaling_suffix;
   QString           m_closing_suffix;
};

#endif
