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
#include "Atlas.h"
#include "Neo.h"
#include "NeosegParameters.h"
#include "ExecutablePaths.h"


class NeosegExecution
{
   public:

   // Constructor
   NeosegExecution();

   // Setting Parameters
   void setNeo(Neo neo);
   void setAtlas(QString atlas);  
   void setOutputPath(QString output_path); 
   void setNeosegParameters(NeosegParameters* parameters);
   void setExecutablePaths(ExecutablePaths* executables);

   // Checking Existing Results
   void defineSegmentation();
   bool checkSegmentation();

   // Create Directory 
   void createDirectory();

   // Writing Script 
   QString variable(QString variable_name);
   QString str(QString str);
   QString listToString(QStringList argumentsList);

   // Implementing Script 
   void initializeScript( QString &script );
   void implementLogStdoutAndStderr(QString &script);
   void addSubElement(QString &script, QString element, QString pythonName, QString XMLname, QString value);
   void writeXMLFile(QString &script);
   void writeWriteAffineTranformationFile(QString& script);
   void writeAffineTranformationFiles(QString& script);
   void runNeoseg(QString &script);
   void implementExecuteNeoseg( QString &script );  
   void writeExecuteNeoseg( );


   // Updating & Getting Output 
   void update();
   QString getOutput();

   private:
   Neo         m_neo;
   QString     m_suffix;
   QString     m_atlas; 
   QString     m_segmentation; 

   bool        m_usingFA;
   bool        m_usingAD;

   QString     m_Neoseg; 

   QString     m_XML_path;

   // Parameters
   NeosegParameters* m_parameters;

   //Executables 
   ExecutablePaths* m_executables;


   // Output //
   QDir*                         m_output_dir;          
   QString                       m_neosegExecution_path;    
   QDir*                         m_neosegExecution_dir; 
};

#endif

