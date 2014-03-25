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

class PreProcessingData : public Script 
{
   public:

   // Constructor 
   PreProcessingData(QString module);

   // Implementing Script 
   void initializeScript(QString &script);
   QString skullStripImage(QString &script, QString image);
   QString correctImage(QString &script, QString image);
   void implementRun(QString &script);

   // Updating & Getting Output 
   void update();
   Neo getOutput();


   private: 

   // Suffix
   QString           m_skullStripping_suffix;
   QString           m_converting_suffix;
   QString           m_correcting_suffix;
   QString           m_rescaling_suffix;
   QString           m_closing_suffix;
};

#endif
