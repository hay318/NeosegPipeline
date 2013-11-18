#ifndef DEF_DerivedWindow
#define DEF_DerivedWindow

// General Librairies
#include <iostream>
#include <string>
#include <cstring>

// Qt Librairies
#include <QWidget>
#include <QtGui>  //MessageBox

// My Specific Librairies
#include "ui_Window.h"
#include "Pipeline.h"


class DerivedWindow : public QMainWindow , public Ui_Window
{
	Q_OBJECT // Enable slots and signals 

   public:
   DerivedWindow();

   public slots :

   // Data //
   void selectOuputDir();
   void enterOutputDir();
   void selectT1();
   void enterT1();
   void selectT2();
   void enterT2();
   void selectSkullStripping(int state);
   void mask();
   void selectMask();
   void enterMask();
   void selectOldAtlas();
   void enterOldAtlas();

   // Atlas //
   void selectAtlas();
   void checkAtlases();
   void displayAtlasPopulation();
   void changeAtlasesDir();
   void selectAtlases(QListWidgetItem* item);



   void runPipeline();


   private :
   Ui_Window ui;
   Pipeline* m_pipeline;
   QString m_tests_path;
   QString m_data_path;
   QString m_atlasPopulation_path;
   QString m_oldAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases; 
};

#endif
