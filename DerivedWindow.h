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

// My Specific Librairies
#include "ui_Window.h"
#include "Pipeline.h"
#include "XmlWriter.h"
#include "MainScriptThread.h"
#include "ExecutablePaths.h" 

class DerivedWindow : public QMainWindow , public Ui_Window
{
	Q_OBJECT // Enable slots and signals 

   public:

   // Constructor
   DerivedWindow();

   // Set
   void setPipeline(Pipeline* pipeline);
   void setPipelineParameters(PipelineParameters* parameters);
   void setMainScriptThread(MainScriptThread* thread);

   // Initialization
   void initializeImagesMap(); 
   void initializeParameters();
   void initializeExecutables(); 

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


   // Images
   void selectImage(QString image); 
   void enterImage(QString image);

   // T1
   void selectT1();
   void enterT1();

   // T2
   void selectT2();
   void enterT2();

   // Mask 
   void selectMask();
   void enterMask();

   // DWI
   void selectDWI();
   void enterDWI();

   // b0
   void selectB0();
   void enterB0();

   // New or Existing Atlas
   void selectNewOrExistingAtlas();

   // New Atlas
   void selectAtlasPopulationDirectory();
   void enterAtlasPopulationDirectory();
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

   // Run Pipeline
   void runPipeline();

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

   QString m_tests_path;
   QString m_data_path;
   QString m_atlasPopulation_path;
   QString m_existingAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases; 

   bool m_parametersSet;
   bool m_executablesSet;

   MainScriptThread* m_thread; 
};

#endif
