#ifndef DEF_PreProcessingData
#define DEF_PreProcessingData

// General Librairies
#include <iostream>
#include <fstream>

// Qt Librairies
#include <QString>
#include <QDir>

// My specific librairies
#include "Neo.h"
#include "ExecutablePaths.h"

class PreProcessingData
{
   public:

   // Constructor 
   PreProcessingData();

   // Setting Parameters
   void setNeo( Neo neo );
   void setSkullStripping(bool skullStripping);
   void setOutputPath( QString output_path );
   void setExecutablePaths(ExecutablePaths* executables);

   // Checking Existing Results
   void definePreProcessedData();
   bool checkPreProcessedData();

   // Create Directory 
   void createDirectory();

   // Writing Script 
   QString variable(QString variable_name);
   QString str(QString str);
   QString listToString(QStringList argumentsList);

   // Implementing Script 
   void initializeScript(QString &script);
   void implementLogStdoutAndStderr(QString &script);
   void implementLogStderr(QString &script);
   void closeMask(QString &script);
   void skullStripImages(QString &script);
   void correctImages(QString &script);
   void implementPreProcessData(QString &script);
   void writePreProcessData();

   // Updating & Getting Output 
   void update();
   Neo getOutput();


   private: 
   
   // Data
   Neo m_neo;
   bool m_skullStripping;

   //Executables 
   ExecutablePaths* m_executables;

   // Directories
   QDir* m_output_dir; 
   QDir* m_preProcessingData_dir; 

   // Script 
   QString m_script;

   // Suffix
   QString m_skullStripping_suffix;
   QString m_correcting_suffix;
   QString m_closing_suffix;

   // Output
   Neo m_preProcessedNeo;
};

#endif
