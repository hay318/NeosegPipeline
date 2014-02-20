#ifndef DEF_DtiRegistration
#define DEF_DtiRegistration


// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>

// Qt Librairies
#include <QString>
#include <QDir>

// My Specific Librairies
#include "Neo.h"
#include "ExecutablePaths.h"


class DtiRegistration
{
   public:

   // Constructor
   DtiRegistration(); 

   // Setting Parameters
   void setOutputPath( QString output_path ); 
   void setNeo( Neo neo );
   void setExecutablePaths(ExecutablePaths* executables);

   // Checking Existing Results
   void defineRegisteredDTI();
   bool checkRegisteredDTI();

   // Create Directory
   void createDirectory();

   // Writing Script 
   QString str(QString str);
   QString variable(QString variable_name);
   QString listToString(QStringList argumentsList);

   // Implementing Scripts 
   void initializeScript();
   void implementLogStdoutAndStderr();
   void implementLogStderr();
   void upsample(QString image); 
   void generateDTI();
   void skullStripb0();
   void skullStripDTI();
   void generateFA();
   void generateAD();
   void calculateTransformations();
   void applyTransformations();
   void implementRegisterDTI(); 
   void writeRegisterDTI();

   // Output //
   void update();
   Neo getOutput();


   private:

   // Data 
   Neo                     m_neo; 
   ExecutablePaths*        m_executables;            

   //Directories 
   QDir*    m_output_dir;
   QString  m_DTIRegistration_path;
   QDir*    m_DTIRegistration_dir;  

   // Script 
   QString                 m_script;
   
   // Output 
   Neo      m_registeredNeo;
}; 

#endif
