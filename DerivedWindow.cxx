#include "DerivedWindow.h"

DerivedWindow::DerivedWindow() : Ui_Window()
{
	setupUi(this);

   m_parametersSet = false; 
   m_executablesSet = false; 

   initializeImagesMap();
   initializeExecutablesMap(); 

   //Connections

   // OutputDir //
   connect(output_button, SIGNAL(clicked()), this, SLOT(selectOuput()));
   connect(output_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterOutput()));

   // Prefix // 
   connect(prefix_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterPrefix()));

   // Suffix //
   connect(suffix_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterSuffix()));   

   // T1 // 
	connect(T1_button, SIGNAL(clicked()), this, SLOT(selectT1()));
   connect(T1_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterT1()));

   // T2 //
	connect(T2_button, SIGNAL(clicked()), this, SLOT(selectT2()));
   connect(T2_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterT2()));

   // Mask //
	connect(mask_button, SIGNAL(clicked()), this, SLOT(selectMask()));
   connect(mask_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterMask()));

   // DWI //
	connect(DWI_button, SIGNAL(clicked()), this, SLOT(selectDWI()));
   connect(DWI_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterDWI()));

   // T2 //
	connect(b0_button, SIGNAL(clicked()), this, SLOT(selectB0()));
   connect(b0_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterB0()));

   // New Or Existing Atlas //
   connect(newAtlas_radioButton, SIGNAL(clicked()), this, SLOT(selectNewOrExistingAtlas()));
   connect(existingAtlas_radioButton, SIGNAL(clicked()), this, SLOT(selectNewOrExistingAtlas()));

   // New Atlas //
   connect(atlasPopulationDirectory_button, SIGNAL(clicked()), this, SLOT(selectAtlasPopulationDirectory())); 
   connect(resetAtlasPopulationDirectory_button, SIGNAL(clicked()), this, SLOT(resetAtlasPopulationDirectory()));
   connect(atlasPopulationDirectory_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterAtlasPopulationDirectory()));
   connect(atlasPopulation_listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectAtlas(QListWidgetItem*)));

   // Old Atlas // 
   connect(existingAtlas_button, SIGNAL(clicked()), this, SLOT(selectExistingAtlas()));
   connect(existingAtlas_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterExistingAtlas()));      
   
   //Load Parameters Configuration
   connect(loadParameters_button,  SIGNAL(clicked()), this, SLOT(selectParameters()));
   connect(loadParameters_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterParameters()));

   //Load Executables Configuration
   connect(loadExecutables_button,  SIGNAL(clicked()), this, SLOT(selectExecutables()));
   connect(loadExecutables_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterExecutables()));

   connect(loadExecutables_button,  SIGNAL(clicked()), this, SLOT(selectExecutables()));
   connect(loadExecutables_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterExecutables()));

   // Save Parameters
   connect(saveParameters_button, SIGNAL(clicked()), this, SLOT(saveParameters()));

   // Save Executables
   connect(saveExecutables_button, SIGNAL(clicked()), this, SLOT(saveExecutables()));

   // SegPostProcessCLP
   connect(SegPostProcessCLP_button, SIGNAL(clicked()), this, SLOT(selectSegPostProcessCLP()));
   connect(SegPostProcessCLP_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterSegPostProcessCLP()));
   connect(resetSegPostProcessCLP_button, SIGNAL(clicked()), this, SLOT(resetSegPostProcessCLP()));

   // N4ITKBiasFieldCorrection
   connect(N4ITKBiasFieldCorrection_button, SIGNAL(clicked()), this, SLOT(selectN4ITKBiasFieldCorrection()));
   connect(N4ITKBiasFieldCorrection_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterN4ITKBiasFieldCorrection()));
   connect(resetN4ITKBiasFieldCorrection_button, SIGNAL(clicked()), this, SLOT(resetN4ITKBiasFieldCorrection()));

   // ITKTransformTools
   connect(ITKTransformTools_button, SIGNAL(clicked()), this, SLOT(selectITKTransformTools()));
   connect(ITKTransformTools_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterITKTransformTools()));
   connect(resetITKTransformTools_button, SIGNAL(clicked()), this, SLOT(resetITKTransformTools()));

   // bet2
   connect(bet2_button, SIGNAL(clicked()), this, SLOT(selectBet2()));
   connect(bet2_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterBet2()));
   connect(resetBet2_button, SIGNAL(clicked()), this, SLOT(resetBet2()));

   // dtiestim
   connect(dtiestim_button, SIGNAL(clicked()), this, SLOT(selectDtiestim()));
   connect(dtiestim_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterDtiestim()));
   connect(resetDtiestim_button, SIGNAL(clicked()), this, SLOT(resetDtiestim()));

   // dtiprocess
   connect(dtiprocess_button, SIGNAL(clicked()), this, SLOT(selectDtiprocess()));
   connect(dtiprocess_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterDtiprocess()));
   connect(resetDtiprocess_button, SIGNAL(clicked()), this, SLOT(resetDtiprocess()));

   // ANTS
   connect(ANTS_button, SIGNAL(clicked()), this, SLOT(selectANTS()));
   connect(ANTS_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterANTS()));
   connect(resetANTS_button, SIGNAL(clicked()), this, SLOT(resetANTS()));

   // ResampleVolume2
   connect(dtiestim_button, SIGNAL(clicked()), this, SLOT(selectDtiestim()));
   connect(dtiestim_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterDtiestim()));
   connect(resetDtiestim_button, SIGNAL(clicked()), this, SLOT(resetDtiestim()));

   // dtiestim
   connect(ResampleVolume2_button, SIGNAL(clicked()), this, SLOT(selectResampleVolume2()));
   connect(ResampleVolume2_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterResampleVolume2()));
   connect(resetResampleVolume2_button, SIGNAL(clicked()), this, SLOT(resetResampleVolume2()));

   // ImageMath
   connect(ImageMath_button, SIGNAL(clicked()), this, SLOT(selectImageMath()));
   connect(ImageMath_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterImageMath()));
   connect(resetImageMath_button, SIGNAL(clicked()), this, SLOT(resetImageMath()));

   // InsightSNAP
   connect(InsightSNAP_button, SIGNAL(clicked()), this, SLOT(selectInsightSNAP()));
   connect(InsightSNAP_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterInsightSNAP()));
   connect(resetInsightSNAP_button, SIGNAL(clicked()), this, SLOT(resetInsightSNAP()));

   // Reset all executables 
   connect(resetAllExecutables_button, SIGNAL(clicked()), this, SLOT(resetAllExecutables()));

   // Run Pipeline
   connect(runPipeline_button, SIGNAL(clicked()), this, SLOT(runPipeline()));

   // Stop Pipeline
   connect(stopPipeline_button, SIGNAL(clicked()), this, SLOT(stopPipeline()));

   // Display Results  
   connect(displayResults_button, SIGNAL(clicked()), this, SLOT(displayResults())); 
   
   newAtlas_radioButton->setChecked(true);
   existingAtlas_widget->hide();

   runPipeline_progressBar->setEnabled(false); 
   displayResults_button->setEnabled(false);
   stopPipeline_button->setEnabled(false);

   //QApplication::sendPostedEvents(this, QEvent::LayoutRequest);  
   //tabs->adjustSize();
   this->adjustSize();
}

void DerivedWindow::setPipelineParameters(PipelineParameters* parameters)
{  
   m_parameters = parameters;
   m_antsParameters = m_parameters->getAntsParameters(); 
   m_neosegParameters = m_parameters->getNeosegParameters(); 
   m_executables = m_parameters->getExecutablePaths();
   initializeParameters();
}

void DerivedWindow::setPipeline(Pipeline* pipeline) 
{
   m_pipeline = pipeline;
}

void DerivedWindow::setMainScriptThread(MainScriptThread* thread) 
{
   m_thread = thread;
   connect(m_thread, SIGNAL(finished()), this, SLOT(enableDisplayButton()));
}

void DerivedWindow::initializeImagesMap()
{
  m_images.insert(std::pair<QString, QLineEdit*>("T1", T1_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("T2", T2_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("mask", mask_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("DWI", DWI_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("b0", b0_lineEdit));
}

void DerivedWindow::initializeExecutablesMap()
{
   m_executables_lineEdit.insert("SegPostProcessCLP", SegPostProcessCLP_lineEdit);
   m_executables_lineEdit.insert("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection_lineEdit);
   m_executables_lineEdit.insert("ITKTransformTools", ITKTransformTools_lineEdit);
   m_executables_lineEdit.insert("bet2", bet2_lineEdit);
   m_executables_lineEdit.insert("dtiestim", dtiestim_lineEdit);
   m_executables_lineEdit.insert("dtiprocess", dtiprocess_lineEdit);
   m_executables_lineEdit.insert("ANTS", ANTS_lineEdit);
   m_executables_lineEdit.insert("ResampleVolume2", ResampleVolume2_lineEdit);
   m_executables_lineEdit.insert("ImageMath", ImageMath_lineEdit);
   m_executables_lineEdit.insert("InsightSNAP", InsightSNAP_lineEdit);
}

void DerivedWindow::selectImage(QString image)
{
	QString image_path = QFileDialog::getOpenFileName(this, "Open file", m_data_path,"Images (*.gipl *.gipl.gz *.nrrd *.nii *.nii.gz)");
   (m_images[image])->setText(image_path);
}
void DerivedWindow::enterImage(QString image)
{
   QString image_path = (m_images[image])->text();

   if(!image_path.isEmpty()) 
   {      
      if(!m_parameters->checkT1(image_path))
      {
         (m_images[image])->clear();
         QMessageBox::critical(this, image, image_path + "\ndoes not exist, enter a new file path");
      }
   }
}


//***** T1 *****//
void DerivedWindow::selectT1()
{
   selectImage("T1");
}
void DerivedWindow::enterT1()
{
   enterImage("T1");
}

//***** T2 *****//
void DerivedWindow::selectT2()
{
   selectImage("T2");
}
void DerivedWindow::enterT2()
{
   enterImage("T2");
}

//***** Mask *****//
void DerivedWindow::selectMask()
{
   selectImage("mask");
}
void DerivedWindow::enterMask()
{
   enterImage("mask");
}

//***** DWI *****//
void DerivedWindow::selectDWI()
{
   selectImage("DWI");
}
void DerivedWindow::enterDWI()
{
   enterImage("DWI");
}

//***** b0*****//
void DerivedWindow::selectB0()
{
   selectImage("b0");
}
void DerivedWindow::enterB0()
{
   enterImage("b0");
}


//***** Ouput *****//
void DerivedWindow::selectOuput()
{
   QString output = QFileDialog::getExistingDirectory (0, "Open Directory",m_tests_path , QFileDialog::ShowDirsOnly);
   output_lineEdit->setText(output);

}
void DerivedWindow::enterOutput()
{
   QString output = output_lineEdit->text();

   if(!output.isEmpty()) 
   {
      if(!m_parameters->checkOutput(output))
      {
         QMessageBox messageBox;

         messageBox.setText(output + "does not exist,");
         messageBox.setInformativeText("Do you want to create it?");
         messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
         messageBox.setDefaultButton(QMessageBox::Yes);

         QString output_path = QFileInfo(output).absoluteFilePath();
         bool result;

         int answer = messageBox.exec();
         switch (answer)
         {
            case QMessageBox::Yes:
               result = (QDir::root()).mkpath(output_path);
               if(!result)
               {
                  QMessageBox::critical(this, "Output directory", output_path + "\n can not be created,\n Please enter a new directory path");             
               }
               break;
            case QMessageBox::No:
               output_lineEdit->clear();
               break;
            case QMessageBox::Cancel:
               output_lineEdit->clear();
               break;
         }
      }
   }
}

// Prefix 
void DerivedWindow::enterPrefix()
{
   QString prefix = prefix_lineEdit->text();

   if(!prefix.isEmpty()) 
   {
      if(!m_parameters->checkPrefixSuffix(prefix))
      {
         QMessageBox::critical(this, "Prefix", prefix + " is not a valid prefix,\nIt can not contains " + m_parameters->getForbiddenCharacters() + ",\nPlease enter a new prefix");  
         prefix_lineEdit->clear();           
      }
   }
}

// Suffix
void DerivedWindow::enterSuffix()
{
   QString suffix = suffix_lineEdit->text();

   if(!suffix.isEmpty()) 
   {
      if(!m_parameters->checkPrefixSuffix(suffix))
      {
         QMessageBox::critical(this, "Suffix", suffix + " is not a valid suffix,\nIt can not contains " + m_parameters->getForbiddenCharacters() + ",\nPlease enter a new suffix");  
         suffix_lineEdit->clear();               
      }
   }
}

//***** New Atlas/Existing Atlas *****//
void DerivedWindow::selectNewOrExistingAtlas()
{
   if(newAtlas_radioButton->isChecked())
   {
      newAtlas_widget->show();
      existingAtlas_widget->hide();

      atlasPopulationDirectory_lineEdit->setText(m_parameters->getAtlasPopulationDirectory()); 
      checkAtlases();
      displayAtlases();
      m_selectedAtlases = m_goodAtlases;
      checkSelectedAtlases();
   } 

   if(existingAtlas_radioButton->isChecked())
   {
      newAtlas_widget->hide();
      existingAtlas_widget->show();
   } 

   //QApplication::sendPostedEvents(this, QEvent::LayoutRequest);  
   tabs->adjustSize();
   //this->adjustSize();
}

//***** Atlas Population Directory *****//
void DerivedWindow::selectAtlasPopulationDirectory()
{
   QString atlasPopulationDirectory = QFileDialog::getExistingDirectory (0, "Open Directory", m_parameters->getAtlasPopulationDirectory(), QFileDialog::ShowDirsOnly);
   atlasPopulationDirectory_lineEdit->setText(atlasPopulationDirectory);
   checkAtlases();
   displayAtlases();
   m_selectedAtlases = m_goodAtlases;
   checkSelectedAtlases();
}

void DerivedWindow::enterAtlasPopulationDirectory()
{
   QString atlasPopulationDirectory = atlasPopulationDirectory_lineEdit->text();

   if(!atlasPopulationDirectory.isEmpty()) 
   {
      if(!m_parameters->checkAtlasPopulationDirectory(atlasPopulationDirectory))
      {
         atlasPopulationDirectory_lineEdit->clear();
         QMessageBox::critical(this, "Output Directory", atlasPopulationDirectory + "\ndoes not exist, enter a new directory path");
      }
   }
   checkAtlases();
   displayAtlases();
   m_selectedAtlases = m_goodAtlases;
}
void DerivedWindow::resetAtlasPopulationDirectory()
{
   atlasPopulationDirectory_lineEdit->setText(m_parameters->getAtlasPopulationDirectory()); 
   checkAtlases();
   displayAtlases();
   m_selectedAtlases = m_goodAtlases;
   checkSelectedAtlases();
}


//***** Checking Atlases *****//
void DerivedWindow::checkAtlases()
{
   QDir* m_atlasPopulation_dir = new QDir(atlasPopulationDirectory_lineEdit->text());
   QStringList atlasPopulation_list = m_atlasPopulation_dir->entryList(QDir::AllDirs | QDir::NoDotAndDotDot); 

   m_goodAtlases.clear();
   m_wrongAtlases.clear();
 
   QStringList::const_iterator it;
   for (it = atlasPopulation_list.constBegin(); it != atlasPopulation_list.constEnd(); ++it) 
   {
      QString atlas_dir = m_atlasPopulation_dir->filePath(*it);
      
      if(m_parameters->checkAtlas(atlas_dir))
      {
         m_goodAtlases << *it;
      }
      else
      {
         m_wrongAtlases << *it;
      }
   } 
} 

//***** Display Atlases *****//
void DerivedWindow::displayAtlases()
{
   atlasPopulation_listWidget->clear();

   QStringList::const_iterator it;
   for (it = m_goodAtlases.constBegin(); it != m_goodAtlases.constEnd(); ++it) 
   {
        QListWidgetItem* item = new QListWidgetItem(*it, atlasPopulation_listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
   } 

   for (it = m_wrongAtlases.constBegin(); it != m_wrongAtlases.constEnd(); ++it) 
   {
        QListWidgetItem* item = new QListWidgetItem(*it, atlasPopulation_listWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
   }
}


void DerivedWindow::checkSelectedAtlases()
{
   QStringList::const_iterator it;
   for (it = m_selectedAtlases.constBegin(); it != m_selectedAtlases.constEnd(); ++it) 
   {
      QList<QListWidgetItem *> items = atlasPopulation_listWidget->findItems(*it, Qt::MatchExactly);

      QList<QListWidgetItem *>::iterator item; 
      for(item=items.begin(); item!=items.end(); ++item)
      {
         (*item)->setCheckState(Qt::Checked);
      }
   }    
}


//***** Select/Unselect Atlas *****//
void DerivedWindow::selectAtlas(QListWidgetItem* item)
{
   if(item->checkState())
   {
      m_selectedAtlases << item->text();           
   } 
  else
  {
      m_selectedAtlases.removeAt(m_selectedAtlases.indexOf(item->text())); 
  }
}


//***** Existing Atlas *****//
void DerivedWindow::selectExistingAtlas()
{
   QString existingAtlas = QFileDialog::getExistingDirectory (0, "Open Directory", m_existingAtlases_path, QFileDialog::ShowDirsOnly);

   if(m_parameters->checkExistingAtlas(existingAtlas)==false)
   {
      QMessageBox::critical(this, "Atlas Directory", existingAtlas_lineEdit->text() + "\nis not a valid atlas");
   }
   else
   {
      existingAtlas_lineEdit->setText(existingAtlas);
   }
}
void DerivedWindow::enterExistingAtlas()
{
   QString existingAtlas = existingAtlas_lineEdit->text();

   if(!existingAtlas.isEmpty()) 
   {
      if (!m_parameters->checkExistingAtlas(existingAtlas))
      {
         existingAtlas_lineEdit->clear();
         QMessageBox::critical(this, "Atlas Directory", existingAtlas + "\nis not a valid atlas");
      } 
   }
}

//***** Parameters Configuration File *****//
void DerivedWindow::selectParameters()
{
	QString parameters=QFileDialog::getOpenFileName(this, "Open file", m_data_path,"XML (*.xml)");
   loadParameters_lineEdit->setText(parameters);
}
void DerivedWindow::enterParameters()
{
   QString parameters=loadParameters_lineEdit->text();

   if(!parameters.isEmpty()) 
   {
      if(!QFile(parameters).exists())
      {
         loadParameters_lineEdit->clear();
         QMessageBox::critical(this, "Parameters Configuration File", parameters + "\ndoes not exist, enter a new file path");
      }
   }
}

//***** Executables Configuration File *****//
void DerivedWindow::selectExecutables()
{
	QString executables = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", m_data_path,"XML (*.xml)");
   loadExecutables_lineEdit->setText(executables);
}
void DerivedWindow::enterExecutables()
{
   QString executables=loadExecutables_lineEdit->text();

   if(!executables.isEmpty()) 
   {
      if(!QFile(executables).exists())
      {
         loadExecutables_lineEdit->clear();
         QMessageBox::critical(this, "Executables Configuration File", executables + "\ndoes not exist, enter a new file path");
      }
   }
}

// Executables 
void DerivedWindow::selectExecutable(QString executable)
{   
	QString executable_path = QFileDialog::getOpenFileName(this, "Select executable", m_data_path);
   if(QFileInfo(executable_path).isExecutable())
   {
      (m_executables_lineEdit[executable])->setText(executable_path);
   }
   else
   {
      QMessageBox::critical(this, executable, executable_path + "\nis not executable");      
   }
}
void DerivedWindow::enterExecutable(QString executable)
{
   QString executable_path = (m_executables_lineEdit[executable])->text();

   if(!executable_path.isEmpty()) 
   {  
      if(QFileInfo(executable_path).exists())
      {
         if(!QFileInfo(executable_path).isExecutable())
         {
            (m_executables_lineEdit[executable])->clear();
            QMessageBox::critical(this, executable, executable_path + "\nis not executable, enter a new file path");            
         }
      }

      else 
      {
         (m_executables_lineEdit[executable])->clear();
         QMessageBox::critical(this, executable, executable_path + "\ndoes not exist, enter a new file path");
      }
   }   
}
void DerivedWindow::resetExecutable(QString executable)
{
   QString defaultPath = m_executables->getDefaultExecutablePath(executable);
   (m_executables_lineEdit[executable])->setText(m_executables->getExecutablePath(executable));
}
void DerivedWindow::resetAllExecutables()
{
   resetSegPostProcessCLP(); 
   resetN4ITKBiasFieldCorrection();
   resetITKTransformTools(); 
   resetBet2(); 
   resetDtiestim(); 
   resetDtiprocess(); 
   resetANTS(); 
   resetResampleVolume2(); 
   resetImageMath(); 
   resetInsightSNAP(); 
}

// SegPostProcessCLP
void DerivedWindow::selectSegPostProcessCLP()
{
   selectExecutable("SegPostProcessCLP");
}
void DerivedWindow::enterSegPostProcessCLP()
{
   enterExecutable("SegPostProcessCLP");
}
void DerivedWindow::resetSegPostProcessCLP()
{
   resetExecutable("SegPostProcessCLP"); 
}

// N4ITKBiasFieldCorrection
void DerivedWindow::selectN4ITKBiasFieldCorrection()
{
   selectExecutable("N4ITKBiasFieldCorrection");
}
void DerivedWindow::enterN4ITKBiasFieldCorrection()
{
   enterExecutable("N4ITKBiasFieldCorrection");
}
void DerivedWindow::resetN4ITKBiasFieldCorrection()
{
   resetExecutable("N4ITKBiasFieldCorrection");
}

// ITKTransformTools
void DerivedWindow::selectITKTransformTools()
{
   selectExecutable("ITKTransformTools");
}
void DerivedWindow::enterITKTransformTools()
{
   enterExecutable("ITKTransformTools");
}
void DerivedWindow::resetITKTransformTools()
{
   resetExecutable("ITKTransformTools");
}

// bet2
void DerivedWindow::selectBet2()
{
   selectExecutable("bet2");
}
void DerivedWindow::enterBet2()
{
   enterExecutable("bet2");
}
void DerivedWindow::resetBet2()
{
   resetExecutable("bet2");
}

// dtiestim
void DerivedWindow::selectDtiestim()
{
   selectExecutable("dtiestim");
}
void DerivedWindow::enterDtiestim()
{
   enterExecutable("dtiestim");
}
void DerivedWindow::resetDtiestim()
{
   resetExecutable("dtiestim");
}

// dtiprocess
void DerivedWindow::selectDtiprocess()
{
   selectExecutable("dtiprocess");
}
void DerivedWindow::enterDtiprocess()
{
   enterExecutable("dtiprocess");
}
void DerivedWindow::resetDtiprocess()
{
   resetExecutable("dtiprocess");
}

// ANTS
void DerivedWindow::selectANTS()
{
   selectExecutable("ANTS");
}
void DerivedWindow::enterANTS()
{
   enterExecutable("ANTS");
}
void DerivedWindow::resetANTS()
{
   resetExecutable("ANTS");
}

// ResampleVolume2
void DerivedWindow::selectResampleVolume2()
{
   selectExecutable("ResampleVolume2");
}
void DerivedWindow::enterResampleVolume2()
{
   enterExecutable("ResampleVolume2");
}
void DerivedWindow::resetResampleVolume2()
{
   resetExecutable("ResampleVolume2");
}

// ImageMath
void DerivedWindow::selectImageMath()
{
   selectExecutable("ImageMath");
}
void DerivedWindow::enterImageMath()
{
   enterExecutable("ImageMath");
}
void DerivedWindow::resetImageMath()
{
   resetExecutable("ImageMath");
}

// InsightSNAP
void DerivedWindow::selectInsightSNAP()
{
   selectExecutable("InsightSNAP");
}
void DerivedWindow::enterInsightSNAP()
{
   enterExecutable("InsightSNAP");
}
void DerivedWindow::resetInsightSNAP()
{
   resetExecutable("InsightSNAP");
}

void DerivedWindow::initializeParameters()
{
   prefix_lineEdit->setText(m_parameters->getPrefix());
   suffix_lineEdit->setText(m_parameters->getSuffix());
   output_lineEdit->setText(m_parameters->getOutput());
   T1_lineEdit->setText(m_parameters->getT1());  
   T2_lineEdit->setText(m_parameters->getT2());
   mask_lineEdit->setText(m_parameters->getMask());
   DWI_lineEdit->setText(m_parameters->getDWI());
   b0_lineEdit->setText(m_parameters->getb0());
   
   if(m_parameters->getNewAtlas())
   {
      newAtlas_radioButton->setChecked(true);
      existingAtlas_radioButton->setChecked(false);
   } 
   else
   {
      newAtlas_radioButton->setChecked(false);
      existingAtlas_radioButton->setChecked(true);
   } 

   atlasPopulationDirectory_lineEdit->setText(m_parameters->getAtlasPopulationDirectory()); 
   existingAtlas_lineEdit->setText(m_parameters->getAtlas()); 
   checkAtlases();  
   displayAtlases();
  
   if(!(m_parameters->getSelectedAtlases()).empty())
   {
      m_selectedAtlases=m_parameters->getSelectedAtlases();
   }
   else
   {
      m_selectedAtlases=m_goodAtlases;
   }

   checkSelectedAtlases(); 

   smoothing_comboBox->insertItems(0, m_parameters->getSmoothingValues()); 
   smoothing_comboBox->setCurrentIndex(m_parameters->getSmoothingIndex()); 
   smoothingSize_spinBox->setValue(m_parameters->getSmoothingSize());

   computingWeights_checkBox->setChecked(m_parameters->getComputingWeights()); 
   weightsModality_comboBox->insertItems(0,m_parameters->getWeightsModalityValues());
   weightsModality_comboBox->setCurrentIndex(m_parameters->getWeightsModalityIndex());
   weightsRadius_spinBox->setValue(m_parameters->getWeightsRadius());    
   weightsRadiusUnit_comboBox->insertItems(0, m_parameters->getWeightsRadiusUnitValues()); 
   weightsRadiusUnit_comboBox->setCurrentIndex(m_parameters->getWeightsRadiusUnitIndex());
  
   includingFA_checkBox->setChecked(m_parameters->getIncludingFA()); 
   FAWeight_spinBox->setValue(m_parameters->getFAWeight());
   FASmoothingSize_spinBox->setValue(m_parameters->getFASmoothingSize());

   overwriting_checkBox->setChecked(m_parameters->getOverwriting()); 
   cleaningUp_checkBox->setChecked(m_parameters->getCleaningUp()); 
   stoppingIfError_checkBox->setChecked(m_parameters->getStoppingIfError());

   computingSystem_comboBox->insertItems(0, m_parameters->getComputingSystemValues()); 
   computingSystem_comboBox->setCurrentIndex(m_parameters->getComputingSystemIndex());
   numberOfCores_spinBox->setValue(m_parameters->getNumberOfCores());   

   // ANTS
   imageMetric1_comboBox->insertItems(0, m_antsParameters->getImageMetricValues()); 
   imageMetric1_comboBox->setCurrentIndex(m_antsParameters->getImageMetric1Index()); 
   weight1_spinBox->setValue(m_antsParameters->getWeight1());
   radius1_spinBox->setValue(m_antsParameters->getRadius1());

   imageMetric2_comboBox->insertItems(0, m_antsParameters->getImageMetricValues()); 
   imageMetric2_comboBox->setCurrentIndex(m_antsParameters->getImageMetric2Index()); 
   weight2_spinBox->setValue(m_antsParameters->getWeight2());
   radius2_spinBox->setValue(m_antsParameters->getRadius2());
   
   iterationsJ_spinBox->setValue(m_antsParameters->getIterationsJ());   
   iterationsK_spinBox->setValue(m_antsParameters->getIterationsK());
   iterationsL_spinBox->setValue(m_antsParameters->getIterationsL());

   transformationType_comboBox->insertItems(0, m_antsParameters->getTransformationTypeValues());
   transformationType_comboBox->setCurrentIndex(m_antsParameters->getTransformationTypeIndex());
   gradientStepLength_spinBox->setValue(m_antsParameters->getGradientStepLength());
   numberOfTimeSteps_spinBox->setValue(m_antsParameters->getNumberOfTimeSteps());
   deltaTime_spinBox->setValue(m_antsParameters->getDeltaTime());

   regularizationType_comboBox->insertItems(0, m_antsParameters->getRegularizationTypeValues());
   regularizationType_comboBox->setCurrentIndex(m_antsParameters->getRegularizationTypeIndex());
   gradientFieldSigma_spinBox->setValue(m_antsParameters->getGradientFieldSigma());
   deformationFieldSigma_spinBox->setValue(m_antsParameters->getDeformationFieldSigma());


   //Neoseg 
   referenceModality_comboBox->insertItems(0, m_neosegParameters->getReferenceImageValues());
   referenceModality_comboBox->setCurrentIndex(m_neosegParameters->getReferenceImageIndex());

   usingFA_checkBox->setChecked(m_neosegParameters->getUsingFA()); 
   usingAD_checkBox->setChecked(m_neosegParameters->getUsingAD()); 

   filterMethod_comboBox->insertItems(0, m_neosegParameters->getFilterMethodValues()); 
   filterMethod_comboBox->setCurrentIndex(m_neosegParameters->getFilterMethodIndex()); 
   numberOfIterations_spinBox->setValue(m_neosegParameters->getNumberOfIterations()); 
   timeStep_spinBox->setValue(m_neosegParameters->getTimeStep()); 

   priorThreshold_spinBox->setValue(m_neosegParameters->getPriorThreshold()); 
   maxDegree_spinBox->setValue(m_neosegParameters->getMaxBiasDegree());

   prior1_spinBox->setValue(m_neosegParameters->getPrior1());
   prior2_spinBox->setValue(m_neosegParameters->getPrior2());
   prior3_spinBox->setValue(m_neosegParameters->getPrior3());
   prior4_spinBox->setValue(m_neosegParameters->getPrior4());
   prior5_spinBox->setValue(m_neosegParameters->getPrior5());

   refinement_checkBox->setChecked(m_neosegParameters->getRefinement());
   initialParzenKernelWidth_spinBox->setValue(m_neosegParameters->getInitialParzenKernelWidth());  

   // Executables 
   SegPostProcessCLP_lineEdit->setText(m_executables->getExecutablePath("SegPostProcessCLP"));
   N4ITKBiasFieldCorrection_lineEdit->setText(m_executables->getExecutablePath("N4ITKBiasFieldCorrection"));
   ITKTransformTools_lineEdit->setText(m_executables->getExecutablePath("ITKTransformTools"));
   bet2_lineEdit->setText(m_executables->getExecutablePath("bet2"));
   dtiestim_lineEdit->setText(m_executables->getExecutablePath("dtiestim"));
   dtiprocess_lineEdit->setText(m_executables->getExecutablePath("dtiprocess"));
   ANTS_lineEdit->setText(m_executables->getExecutablePath("ANTS"));
   ResampleVolume2_lineEdit->setText(m_executables->getExecutablePath("ResampleVolume2"));
   ImageMath_lineEdit->setText(m_executables->getExecutablePath("ImageMath"));
   InsightSNAP_lineEdit->setText(m_executables->getExecutablePath("InsightSNAP"));
}


void DerivedWindow::setParameters()
{
   // Input
   if(!(T1_lineEdit->text()).isEmpty())
   {
      m_parameters->setT1(T1_lineEdit->text());
   }
   
   if(!(T2_lineEdit->text()).isEmpty())
   {
      m_parameters->setT2(T2_lineEdit->text());
   }

   if(!(mask_lineEdit->text()).isEmpty())
   {
      m_parameters->setMask(mask_lineEdit->text());
   }   

   if(!(DWI_lineEdit->text()).isEmpty())
   {
      m_parameters->setDWI(DWI_lineEdit->text());
   }  

   if(!(b0_lineEdit->text()).isEmpty())
   {
      m_parameters->setb0(b0_lineEdit->text());
   }  


   // Output
   if(!(prefix_lineEdit->text()).isEmpty())
   {
      m_parameters->setPrefix(prefix_lineEdit->text()); 
   }

   if(!(suffix_lineEdit->text()).isEmpty())
   {
      m_parameters->setSuffix(suffix_lineEdit->text()); 
   }

   if(!(output_lineEdit->text()).isEmpty())
   {
      m_parameters->setOutput(output_lineEdit->text()); 
   }


   //Atlas Population 
   if (newAtlas_radioButton->isChecked()) 
   {
      m_parameters->setNewAtlas(true);

      // Atlas Population
      m_parameters->setAtlasPopulationDirectory(atlasPopulationDirectory_lineEdit->text());
      m_parameters->setSelectedAtlases(m_selectedAtlases);

      // Smoothing
      m_parameters->setSmoothing(smoothing_comboBox->currentText());
      m_parameters->setSmoothingSize(smoothingSize_spinBox->value());

      // Computing Weights
      m_parameters->setComputingWeights(computingWeights_checkBox->isChecked());
      m_parameters->setWeightsRadius(weightsRadius_spinBox->value());
      m_parameters->setWeightsRadiusUnit(weightsRadiusUnit_comboBox->currentText()); 
      
      // Including FA
      m_parameters->setIncludingFA(includingFA_checkBox->isChecked()); 
      m_parameters->setFAWeight(FAWeight_spinBox->value()); 
      m_parameters->setFASmoothingSize(FASmoothingSize_spinBox->value()); 
   }

   else 
   {
      m_parameters->setNewAtlas(false);  
      m_parameters->setAtlas(existingAtlas_lineEdit->text());    
   }

   // Computation
   m_parameters->setOverwriting(overwriting_checkBox->isChecked());
   m_parameters->setCleaningUp(cleaningUp_checkBox->isChecked());
   m_parameters->setStoppingIfError(stoppingIfError_checkBox->isChecked()); 
   m_parameters->setComputingSystem(computingSystem_comboBox->currentText());
   m_parameters->setNumberOfCores(numberOfCores_spinBox->value());
   m_parameters->setDebug(debug_checkBox->isChecked());   

   
   //ANTS parameters 
   m_antsParameters->setImageMetric1(imageMetric1_comboBox->currentText());
   m_antsParameters->setWeight1(weight1_spinBox->value());
   m_antsParameters->setRadius1(radius1_spinBox->value());

   m_antsParameters->setImageMetric2(imageMetric2_comboBox->currentText());
   m_antsParameters->setWeight2(weight2_spinBox->value());
   m_antsParameters->setRadius2(radius2_spinBox->value());

   m_antsParameters->setIterationsJ(iterationsJ_spinBox->value());
   m_antsParameters->setIterationsK(iterationsK_spinBox->value());
   m_antsParameters->setIterationsL(iterationsL_spinBox->value());

   m_antsParameters->setTransformationType(transformationType_comboBox->currentText());
   m_antsParameters->setGradientStepLength(gradientStepLength_spinBox->value());
   m_antsParameters->setNumberOfTimeSteps(numberOfTimeSteps_spinBox->value());
   m_antsParameters->setDeltaTime(deltaTime_spinBox->value());

   m_antsParameters->setRegularizationType(regularizationType_comboBox->currentText());  
   m_antsParameters->setGradientFieldSigma(gradientFieldSigma_spinBox->value());
   m_antsParameters->setDeformationFieldSigma(deformationFieldSigma_spinBox->value());

   m_antsParameters->setUsingMask(usingMask_checkBox->isChecked());
   m_antsParameters->setUsingSmoothedMask(usingSmoothedMask_checkBox->isChecked());


   // Neoseg parameters 
   m_neosegParameters->setReferenceImage(referenceModality_comboBox->currentText()); 
   m_neosegParameters->setUsingFA(usingFA_checkBox->isChecked());
   m_neosegParameters->setUsingAD(usingAD_checkBox->isChecked());

   m_neosegParameters->setFilterMethod(filterMethod_comboBox->currentText()); 
   m_neosegParameters->setNumberOfIterations(numberOfIterations_spinBox->value());
   m_neosegParameters->setTimeStep(timeStep_spinBox->value()); 

   m_neosegParameters->setPriorThreshold(priorThreshold_spinBox->value()); 
   m_neosegParameters->setMaxBiasDegree(maxDegree_spinBox->value()); 
   m_neosegParameters->setPrior1(prior1_spinBox->value()); 
   m_neosegParameters->setPrior2(prior2_spinBox->value()); 
   m_neosegParameters->setPrior3(prior3_spinBox->value()); 
   m_neosegParameters->setPrior4(prior4_spinBox->value()); 
   m_neosegParameters->setPrior5(prior5_spinBox->value()); 

   m_neosegParameters->setRefinement(refinement_checkBox->isChecked());
   m_neosegParameters->setInitialParzenKernelWidth(initialParzenKernelWidth_spinBox->value());

   m_parameters->setComputing3LabelsSeg(computing3LabelsSeg_checkBox->isChecked());

   // Executables
   m_executables->setExecutablePath("SegPostProcessCLP", SegPostProcessCLP_lineEdit->text()); 
   m_executables->setExecutablePath("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection_lineEdit->text()); 
   m_executables->setExecutablePath("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection_lineEdit->text()); 
   m_executables->setExecutablePath("bet2", bet2_lineEdit->text()); 
   m_executables->setExecutablePath("dtiestim", dtiestim_lineEdit->text()); 
   m_executables->setExecutablePath("dtiprocess", dtiprocess_lineEdit->text()); 
   m_executables->setExecutablePath("ANTS", ANTS_lineEdit->text()); 
   m_executables->setExecutablePath("ResampleVolume2", ResampleVolume2_lineEdit->text()); 
   m_executables->setExecutablePath("ImageMath", ImageMath_lineEdit->text()); 
   m_executables->setExecutablePath("InsightSNAP", InsightSNAP_lineEdit->text()); 

   m_parametersSet = true;
}


void DerivedWindow::setExecutables()
{
   m_executablesSet = true;
}

void DerivedWindow::saveParameters()
{
   if(!m_parametersSet)
   {
      setParameters();
   }

   QString parameters_path = QFileDialog::getSaveFileName(this, "Save file", "parameters.xml", "XML files (*.xml)");

   XmlWriter* parameters = new::XmlWriter();
   parameters->setPipelineParameters(m_parameters);  
   parameters->writeParametersConfiguration(parameters_path);
}

void DerivedWindow::saveExecutables()
{
   if(!m_executablesSet)
   {
      setExecutables();
   }

   QString executables_path = QFileDialog::getSaveFileName(this, "Save file", "executables.xml", "XML files (*.xml)");

   XmlWriter* executables = new::XmlWriter();
   executables->setPipelineParameters(m_parameters);  
   executables->writeExecutablesConfiguration(executables_path);
}

void DerivedWindow::runPipeline()
{
   if(!m_parametersSet)
   {
      setParameters();
   }

   m_thread->setPipeline(m_pipeline);
   m_thread->start();

   runPipeline_button->setEnabled(false);

   runPipeline_progressBar->setEnabled(true);
   stopPipeline_button->setEnabled(true); 

}

void DerivedWindow::stopPipeline()
{
   m_thread->terminate(); 
   runPipeline_button->setEnabled(true);
   runPipeline_progressBar->setEnabled(false);
   stopPipeline_button->setEnabled(false); 
}

void DerivedWindow::enableDisplayButton()
{
   displayResults_button->setEnabled(true);   
}

void DerivedWindow::displayResults()
{
   QProcess insightSNAP_process;

   QString command = m_executables->getExecutablePath("InsightSNAP") + " -g " + m_parameters->getT1() + " -s " + m_parameters->getSegmentation(); 

   std::cout<<command.toStdString()<<std::endl; 

   insightSNAP_process.start(command);

   while (!insightSNAP_process.waitForFinished())
   {
   }   
}

void DerivedWindow::closeEvent(QCloseEvent *event)
{
   if(m_thread->isRunning())
   {
      QMessageBox messageBox;

      messageBox.setText("The pipeline is still running,");
      messageBox.setInformativeText("Do you want to terminate it?");
      messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      messageBox.setDefaultButton(QMessageBox::No);

      int answer = messageBox.exec();
      switch (answer)
      {
         case QMessageBox::Yes:
            m_thread->terminate(); 
            m_thread->wait();
            QMainWindow::closeEvent(event);
            break;
         case QMessageBox::No:
            this->hide();
            break;
         case QMessageBox::Cancel:
            messageBox.close();
            break;
      }
   }
}




