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
   void selectExecutable(QString executable);
   void enterExecutable(QString executable);
   void resetExecutable(QString executable);
   void resetAllExecutables(); 

   // SegPostProcessCLP
   void selectSegPostProcessCLP();
   void enterSegPostProcessCLP(); 
   void resetSegPostProcessCLP(); 

   // N4ITKBiasFieldCorrection
   void selectN4ITKBiasFieldCorrection();
   void enterN4ITKBiasFieldCorrection(); 
   void resetN4ITKBiasFieldCorrection();

   // ITKTransformTools
   void selectITKTransformTools();
   void enterITKTransformTools(); 
   void resetITKTransformTools(); 

   // bet2
   void selectBet2();
   void enterBet2(); 
   void resetBet2(); 

   // dtiestim
   void selectDtiestim();
   void enterDtiestim(); 
   void resetDtiestim(); 

   // dtiprocess
   void selectDtiprocess();
   void enterDtiprocess();
   void resetDtiprocess(); 

   // ANTS
   void selectANTS();
   void enterANTS();
   void resetANTS();
 
   // ResampleVolume2
   void selectResampleVolume2();
   void enterResampleVolume2(); 
   void resetResampleVolume2(); 

   // ImageMath
   void selectImageMath();
   void enterImageMath(); 
   void resetImageMath(); 

   // InsightSNAP
   void selectInsightSNAP();
   void enterInsightSNAP(); 
   void resetInsightSNAP(); 

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
   QMap<QString, QLineEdit*> m_executables_lineEdit;

   QString m_tests_path;
   QString m_data_path;
   QString m_existingAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases; 

   bool m_parametersSet;
   bool m_executablesSet;

   MainScriptThread* m_thread; 
};

#endif
