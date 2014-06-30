#ifndef DEF_DerivedWindow
#define DEF_DerivedWindow

// General Librairies
#include <iostream>
#include <string>
#include <cstring>
#include <map>

// Qt Librairies
#include <QWidget>
#include <QtGui>  //MessageBox
#include <QMap>

// My Specific Librairies
#include "ui_Window.h"
#include "ui_About.h"
#include "Pipeline.h"
#include "XmlReader.h"
#include "XmlWriter.h"
#include "MainScriptThread.h"
#include "ExecutablePaths.h" 
#include "LibraryPaths.h"
#include "About.h" 


#ifndef NEOSEGPIPELINE_VERSION
#define NEOSEGPIPELINE_VERSION "unknown"
#endif

class DerivedWindow : public QMainWindow , public Ui_Window
{
	Q_OBJECT // Enable slots and signals 

   struct Executable
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
      QPushButton* reset_button; 
   };

   struct Library
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
   };

   struct Image
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
   }; 

   public:

   // Constructor
   DerivedWindow();

   // Set
   void setPipeline(Pipeline* pipeline);
   void setPipelineParameters(PipelineParameters* parameters);
   void setMainScriptThread(MainScriptThread* thread);

   // Print XML Errors
   void printErrors(QString errors);

   // Initialization
   void initializeImagesMap();
   void initializeExecutablesMap();
   void initializeLibrariesMap();   
   void initializeParameters();
   void initializeExecutables(); 

   // Connection 

   // Atlas Population
   void checkAtlases();
   void displayAtlases();
   void checkSelectedAtlases();

   // Parameter
   void setData();
   void setParameters(); 
   void setExecutables();


   public slots :

   // About 
   void about(); 

   // Output
   void selectOuput();
   void enterOutput();
   void createOutput(QString output);

   // Prefix 
   void enterPrefix(); 

   // Suffix
   void enterSuffix();

   // Images
   void selectImage(QString image); 
   void enterImage(QString image);

   // New or Existing Atlas
   void selectNewOrExistingAtlas();

   // New Atlas
   void selectAtlasPopulationDirectory();
   void enterAtlasPopulationDirectory();
   void resetAtlasPopulationDirectory();
   void selectAtlas(QListWidgetItem* item);

   // Existing Atlas
   void selectExistingAtlas();
   void enterExistingAtlas();

   // Computing System
   void changeComputingSystem(int index);

   // Parameters
   void selectParameters(); 
   void saveParameters();  

   // Executables
   void selectExecutables(); 
   void saveExecutables(); 

   // Executables 
   void selectExecutable(QString executable_name);
   void enterExecutable(QString executable_name);
   void resetExecutable(QString executable_name);
   void resetAllExecutables(); 

   // Executables 
   void selectLibrary(QString library_name);
   void enterLibrary(QString library_name);

   // Run Pipeline
   void runPipeline();

   // Logging
   void initializePipelineLogging();
   void initializeRegistrationsLogging();

   void printPipelineLog();    
   void printRegistrationLog(QString file_name); 

   void selectLog();
   void changeExecutionPlainTextEdit();

   // Enable widgets 
   void setParametersWidgetEnabled(bool enabled);
   
   // Stop Pipeline
   void stopPipeline(); 

   // Display Results
   void enableDisplayButton();
   void displayResults(); 

   // Exit Application 
   void closeEvent(QCloseEvent *event);

   private :
   
   // Window
   Ui_Window ui;

   // Pipeline 
   Pipeline* m_pipeline;

   // Parameters
   PipelineParameters* m_parameters;
   AntsParameters* m_antsParameters_DTI; 
   AntsParameters* m_antsParameters_atlas; 
   NeosegParameters* m_neosegParameters; 

   // Executables 
   ExecutablePaths* m_executables; 

   // Libraries
   LibraryPaths* m_libraries;

   QString m_tests_path;
   QString m_data_path;
   QString m_existingAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases; 

   bool m_parametersSet;
   bool m_executablesSet;
   bool m_pipelineWriten; 

   MainScriptThread* m_thread; 

   QMap<QString, Image> m_images_map; 
   QMap<QString, Executable> m_executables_map; 
   QMap<QString, Library> m_libraries_map; 

   struct Logging
   {
      QTextStream* textStream;
      QPlainTextEdit* plainTextEdit; 
   };

   QTextStream* m_log_textStream; 

   QMap<QString, Logging> m_registrationLoggings; 

   About* m_about; 
   

};

#endif
