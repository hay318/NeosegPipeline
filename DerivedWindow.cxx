#include "DerivedWindow.h"

DerivedWindow::DerivedWindow() : Ui_Window(), m_pipeline (NULL) 
{
	setupUi(this);

   m_pipeline = new Pipeline();

   m_tests_path = "/home/cherel/neosegPipeline";
   m_data_path = "/home/cherel/neosegPipeline/data";
   m_atlasPopulation_path = "/work/mcherel/project/atlasPopulation/atlases";
   m_oldAtlases_path = "/work/mcherel/project/neoseg/atlas";

   //std::string output_Dir="/work/mcherel/project/neosegPipeline/test"

   //Connections

   // OutputDir //
   connect(browserOutputDir_button, SIGNAL(clicked()), this, SLOT(selectOuputDir()));
   connect(outputDir_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterOutputDir()));

   // T1 // 
	connect(browserT1_button, SIGNAL(clicked()), this, SLOT(selectT1()));
   connect(T1_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterT1()));

   // T2 //
	connect(browserT2_button, SIGNAL(clicked()), this, SLOT(selectT2()));
   connect(T2_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterT2()));

   // Skull-Stripping //
   connect(skullStripping_checkbox, SIGNAL(stateChanged(int)), this, SLOT(selectSkullStripping(int)));
   connect(mask_radioButton, SIGNAL(clicked()), this, SLOT(mask()));
   connect(noMask_radioButton, SIGNAL(clicked()), this, SLOT(mask()));   
	connect(browserMask_button, SIGNAL(clicked()), this, SLOT(selectMask()));
   connect(mask_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterMask()));

   // Atlas //
   connect(newAtlas_radioButton, SIGNAL(clicked()), this, SLOT(selectAtlas()));
   connect(oldAtlas_radioButton, SIGNAL(clicked()), this, SLOT(selectAtlas()));

   // New Atlas //
   connect(browserAtlasesDir_button, SIGNAL(clicked()), this, SLOT(changeAtlasesDir())); 
   connect(atlasPopulation_listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectAtlases(QListWidgetItem*)));

   // Old Atlas // 
   connect(browserOldAtlas_button, SIGNAL(clicked()), this, SLOT(selectOldAtlas()));
   connect(oldAtlas_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterOldAtlas()));      
   

   // Run Pipeline //
   connect(runPipeline_button, SIGNAL(clicked()), this, SLOT(runPipeline()));


   oldSeg_widget->hide();
   newAtlas_widget->hide();
   oldAtlas_widget->hide();
   runPipeline_progressBar->hide(); 
   displayResults_button->setEnabled(false);


   //QApplication::sendPostedEvents(this, QEvent::LayoutRequest);  
   //tabs->adjustSize();
   this->adjustSize();

   browserMask_button->hide();
   mask_lineEdit->hide();
}

//// Ouput Dir ////
void DerivedWindow::selectOuputDir()
{
   QString outputDir = QFileDialog::getExistingDirectory (0, "Open Directory",m_tests_path , QFileDialog::ShowDirsOnly);
   m_pipeline->setOutput (outputDir);
   outputDir_lineEdit->setText(outputDir);

}
void DerivedWindow::enterOutputDir()
{
   if(!(outputDir_lineEdit->text()).isEmpty()) 
   {
      QString output_path = outputDir_lineEdit->text();
      QDir* outputDir = new QDir (output_path); 

      if(!outputDir->exists())
      {
         outputDir_lineEdit->clear();
         QMessageBox::critical(this, "Output Directory", output_path + "\ndoes not exist, enter a new directory path");
      }
      else
      {
         m_pipeline->setOutput(output_path);
      }
   }
}


//*********** T1 ***********//
void DerivedWindow::selectT1()
{
	QString T1 = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", m_data_path,"Images (*.gipl *.gipl.gz *.nrrd *.nii *.nii.gz)");
   m_pipeline->setT1 (T1);
   T1_lineEdit->setText(T1);
}

void DerivedWindow::enterT1()
{
   if(!(T1_lineEdit->text()).isEmpty()) 
   {
      QString T1_path = T1_lineEdit->text();
      QFile* T1 = new QFile (T1_path); 
      
      if(!T1->exists())
      {
         T1_lineEdit->clear();
         QMessageBox::critical(this, "T1 File", T1_path + "\ndoes not exist, enter a new file path");
      }
      else
      {
         m_pipeline->setT1(T1_path);
      }
   }
}


//*********** T2 ***********//
void DerivedWindow::selectT2()
{
	QString T2 = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", m_data_path,"Images (*.gipl *.gipl.gz *.nrrd *.nii *.nii.gz)");
   m_pipeline->setT2 (T2);
   T2_lineEdit->setText(T2);
}

void DerivedWindow::enterT2()
{
   if(!(T2_lineEdit->text()).isEmpty()) 
   {
      QString T2_path = T2_lineEdit->text();
      QFile* T2 = new QFile (T2_path); 

      if(!T2->exists())
      {
         T2_lineEdit->clear();
         QMessageBox::critical(this, "T2 File", T2_path + "\ndoes not exist, enter a new file path");
      }
      else
      {
         m_pipeline->setT2(T2_path);
      }
   }
}



//*********** Skull-Stripping ***********//
void DerivedWindow::selectSkullStripping(int state)
{
   if(skullStripping_checkbox->isChecked())
   {      
      oldSeg_widget->hide(); 
      m_pipeline->setSkullStripping(true);
   }
   
   else
   {
      oldSeg_widget->show(); 
      m_pipeline->setSkullStripping(false);
   }

   QApplication::sendPostedEvents(this, QEvent::LayoutRequest); 
   this->adjustSize();
}

void DerivedWindow::mask()
{
   if(mask_radioButton->isChecked())
   {
      mask_lineEdit->show();
      browserMask_button->show();
      m_pipeline->setMask(true);
   } 

   if(noMask_radioButton->isChecked())
   {
      mask_lineEdit->hide();
      browserMask_button->hide();
      m_pipeline->setMask(false);
   } 

}

void DerivedWindow::selectMask()
{
   QString mask = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", m_data_path,"Images (*.gipl *.gipl.gz *.nrrd *.nii *.nii.gz)");
   m_pipeline->setMask (mask);
   mask_lineEdit->setText(mask);
}
void DerivedWindow::enterMask()
{
   QFile* mask = new QFile (mask_lineEdit->text()); 

   if(!(mask_lineEdit->text()).isEmpty()) 
   {
      if(!mask->exists())
      {
         mask_lineEdit->clear();
         QMessageBox::critical(this, "Mask File", mask_lineEdit->text() + "\ndoes not exist, enter a new file path");
      }
      else
      {
         m_pipeline->setMask(mask_lineEdit->text());
      }
   }
}

//*********** Atlas ***********//
void DerivedWindow::selectAtlas()
{
   if(newAtlas_radioButton->isChecked())
   {
      //pipeline->setAtlas(newAtlas);
      newAtlas_widget->show();
      oldAtlas_widget->hide();

      atlasPopulation_lineEdit->setText(m_atlasPopulation_path); 
      checkAtlases();
      displayAtlasPopulation();
      m_selectedAtlases = m_goodAtlases;
   } 

   if(oldAtlas_radioButton->isChecked())
   {
      //pipeline->setAtlas(oldAtlas);
      newAtlas_widget->hide();
      oldAtlas_widget->show();
   } 

   //QApplication::sendPostedEvents(this, QEvent::LayoutRequest);  
   tabs->adjustSize();
   //this->adjustSize();
}

//*********** New Atlas ***********//
void DerivedWindow::checkAtlases()
{
   QDir* m_atlasPopulation_dir = new QDir(m_atlasPopulation_path);
   QStringList atlasPopulation_list = m_atlasPopulation_dir->entryList(QDir::AllDirs | QDir::NoDotAndDotDot); 

   m_goodAtlases.clear();
   m_wrongAtlases.clear();
 
   QStringList::const_iterator it;
   for (it = atlasPopulation_list.constBegin(); it != atlasPopulation_list.constEnd(); ++it) 
   {
      QString atlas_dir = m_atlasPopulation_path + "/" + *it;
      
      if(m_pipeline->checkAtlasPopulation(atlas_dir)) {m_goodAtlases << *it;}
      else {m_wrongAtlases << *it;}
   } 
} 


void DerivedWindow::selectAtlases(QListWidgetItem* item)
{
   if(item->checkState())
   {
      // Add the atlas to the m_selectedAtlases list
      m_selectedAtlases << item->text();           
   } 
  else
  {
      // Remove the atlas from the m_selectedAtlases list
      m_selectedAtlases.removeAt(m_selectedAtlases.indexOf(item->text())); 
  }
}

void DerivedWindow::displayAtlasPopulation()
{
   atlasPopulation_listWidget->clear();

   QStringList::const_iterator it;
   for (it = m_goodAtlases.constBegin(); it != m_goodAtlases.constEnd(); ++it) 
   {
        QListWidgetItem* item = new QListWidgetItem(*it, atlasPopulation_listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Checked);
   } 

   for (it = m_wrongAtlases.constBegin(); it != m_wrongAtlases.constEnd(); ++it) 
   {
        QListWidgetItem* item = new QListWidgetItem(*it, atlasPopulation_listWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
   }
}

void DerivedWindow::changeAtlasesDir()
{
   m_atlasPopulation_path = QFileDialog::getExistingDirectory (0, "Open Directory",m_atlasPopulation_path , QFileDialog::ShowDirsOnly);
   atlasPopulation_lineEdit->setText(m_atlasPopulation_path); 
   checkAtlases();
   displayAtlasPopulation();
   m_selectedAtlases = m_goodAtlases;
}

//// Old Atlas ////
void DerivedWindow::selectOldAtlas()
{
   QString oldAtlas_path;

   do
   {
      oldAtlas_path = QFileDialog::getExistingDirectory (0, "Open Directory",m_oldAtlases_path , QFileDialog::ShowDirsOnly);
      oldAtlas_lineEdit->setText(oldAtlas_path);

      if((m_pipeline->checkAtlas(oldAtlas_path)==false) && (!oldAtlas_path.isEmpty())){QMessageBox::critical(this, "Atlas Directory", oldAtlas_lineEdit->text() + "\nis not a valid atlas");}

   } while((m_pipeline->checkAtlas(oldAtlas_path)==false) && (!oldAtlas_path.isEmpty()));

   //m_pipeline->setOldAtlas (oldAtlas_path.toStdString());

}
void DerivedWindow::enterOldAtlas()
{
   QString oldAtlas_path = oldAtlas_lineEdit->text();
   QDir* oldAtlas = new QDir (oldAtlas_path); 

   if(!(oldAtlas_lineEdit->text()).isEmpty()) 
   {
      if(oldAtlas->exists())
      {
         if (m_pipeline->checkAtlas(oldAtlas_path))
         {
            m_pipeline->setOldAtlas(outputDir_lineEdit->text());
         } 
         else
         {
            oldAtlas_lineEdit->clear();
            QMessageBox::critical(this, "Atlas Directory", oldAtlas_path + "\nis not a valid atlas");
         } 
      }
      else
      {
         oldAtlas_lineEdit->clear();
         QMessageBox::critical(this, "Atlas Directory", oldAtlas_path + "\ndoes not exist, enter a new directory path");
      }
   }
}

void DerivedWindow::runPipeline()
{
   runPipeline_progressBar->show();

   //Atlas Population 
   if (newAtlas_radioButton->isChecked()) 
   {
      m_pipeline->setAtlasPopulation(m_atlasPopulation_path, m_selectedAtlases);
   }

   //Smoothing 
   m_pipeline->setSmoothing(smoothing_comboBox->currentText());

   //Smoothing Size
   m_pipeline->setSmoothingSize(smoothingSize_doubleSpinBox->value());

   //Overwriting
   m_pipeline->setOverwriting(overwriting_checkBox->isChecked());
   
   //Cleaning-Up
   m_pipeline->setCleaningUp(cleaningUp_checkBox->isChecked());

   //Computing System
   m_pipeline->setComputingSystem(computingSystem_comboBox->currentText());

   //Nb of cores
   m_pipeline->setNbCores(nbCores_spinBox->value());

   m_pipeline->runPipeline();
}

