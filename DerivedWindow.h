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
#include "Pipeline.h"
#include "XmlWriter.h"
#include "MainScriptThread.h"
#include "ExecutablePaths.h" 


class DerivedWindow : public QMainWindow , public Ui_Window
{
	Q_OBJECT // Enable slots and signals 

   struct Executable
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
      QPushButton* reset_button; 
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

   // Initialization
   void initializeImagesMap();
   void initializeExecutablesMap();   
   void initializeParameters();
   void initializeExecutables(); 

   // Connection 


   // Atlas Population
   void checkAtlases();
   void displayAtlases();
   void checkSelectedAtlases();

   // Parameter
   void setParameters(); 
   void setExecutables();


   public slots :

   // Output
   void selectOuput();
   void enterOutput();

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

   // Parameters
   void selectParameters(); 
   void enterParameters();
   void saveParameters();  

   // Executables
   void selectExecutables(); 
   void enterExecutables(); 
   void saveExecutables(); 

   // Executables 
   void selectExecutable(QString executable_name);
   void enterExecutable(QString executable_name);
   void resetExecutable(QString executable_name);
   void resetAllExecutables(); 

   // Run Pipeline
   void runPipeline();

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
   AntsParameters* m_antsParameters; 
   NeosegParameters* m_neosegParameters; 

   // Executables 
   ExecutablePaths* m_executables; 

   std::map<QString, QLineEdit*> m_images;
   //QMap<QString, QLineEdit*> m_executables_lineEdit;

   QString m_tests_path;
   QString m_data_path;
   QString m_existingAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases; 

   bool m_parametersSet;
   bool m_executablesSet;

   MainScriptThread* m_thread; 


   QMap<QString, Image> m_images_map; 
   QMap<QString, Executable> m_executables_map; 




};

#endif
