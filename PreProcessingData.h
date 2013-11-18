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

class PreProcessingData
{
   public:
   PreProcessingData(QString output_path);
   QString getOutputPath(QString input_path, QString suffix);
   bool checkExistingResults(Neo neo);
   void createDirectory();
   void createScript();
   QString runNeoseg(QString image);
   QString skullStripImage (QString image_path, QString mask_path);
   Neo skullStripNeo (Neo neo, QString mask_path);


   private: 
   std::ofstream* m_script_stream;
   QString m_script;
   QString m_output_path;
   QDir* m_preProcessingData_dir; 

   QString m_skullStripping_suffix;

};

#endif
