#include "DerivedWindow.h"

DerivedWindow::DerivedWindow() : Ui_Window()
{
	setupUi(this);

   m_parametersSet = false; 
   m_executablesSet = false; 
   m_pipelineWriten = false; 

   // Parameters Configuration 
   connect(loadParametersConfiguration_action, SIGNAL(triggered()), this, SLOT(selectParameters()));
   connect(saveParametersConfiguration_action, SIGNAL(triggered()), this, SLOT(saveParameters()));

   // Executables Configuration
   connect(loadSoftwaresConfiguration_action, SIGNAL(triggered()), this, SLOT(selectExecutables()));
   connect(saveSoftwaresConfiguration_action, SIGNAL(triggered()), this, SLOT(saveExecutables()));

   // Quit
   connect(quit_action, SIGNAL(triggered()), this, SLOT(close())); 

   // Help
   connect(about_action, SIGNAL(triggered()), this, SLOT(about())); 

   // Toolbar 
   QWidget* spacer = new QWidget();
   spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   toolBar->addWidget(spacer);
   toolBar->addAction(about_action);

   // Select Image Signal Mapper
   QSignalMapper* selectImage_signalMapper = new QSignalMapper(this);
   connect(selectImage_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectImage(QString)));

   // Enter Image Signal Mapper
   QSignalMapper* enterImage_signalMapper = new QSignalMapper(this);
   connect(enterImage_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterImage(QString)));

   // Connect Images Signal/Slot
   initializeImagesMap();
   QMap<QString, Image>::iterator it_images; 
   for(it_images = m_images_map.begin(); it_images != m_images_map.end(); ++it_images)
   {
      QString name = it_images.key(); 
      Image image = it_images.value();  

      selectImage_signalMapper->setMapping(image.select_button, name);
      connect(image.select_button, SIGNAL(clicked()), selectImage_signalMapper, SLOT(map())); 

      enterImage_signalMapper->setMapping(image.enter_lineEdit, name);
      connect(image.enter_lineEdit, SIGNAL(editingFinished()), enterImage_signalMapper, SLOT(map())); 
   }

   // Select Executable Signal Mapper
   QSignalMapper* selectExecutable_signalMapper = new QSignalMapper(this);
   connect(selectExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectExecutable(QString)));

   // Enter Executable Signal Mapper
   QSignalMapper* enterExecutable_signalMapper = new QSignalMapper(this);
   connect(enterExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterExecutable(QString)));

   // Reset Executable Signal Mapper
   QSignalMapper* resetExecutable_signalMapper = new QSignalMapper(this);
   connect(resetExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(resetExecutable(QString)));

   // Connect Executables Signal/Slot
   initializeExecutablesMap(); 
   QMap<QString, Executable>::iterator it_exec; 
   for(it_exec = m_executables_map.begin(); it_exec != m_executables_map.end(); ++it_exec)
   {
      QString name = it_exec.key(); 
      Executable executable = it_exec.value();  

      selectExecutable_signalMapper->setMapping(executable.select_button, name);
      connect(executable.select_button, SIGNAL(clicked()), selectExecutable_signalMapper, SLOT(map())); 

      enterExecutable_signalMapper->setMapping(executable.enter_lineEdit, name);
      connect(executable.enter_lineEdit, SIGNAL(editingFinished()), enterExecutable_signalMapper, SLOT(map())); 

      resetExecutable_signalMapper->setMapping(executable.reset_button, name);
      connect(executable.reset_button, SIGNAL(clicked()), resetExecutable_signalMapper, SLOT(map()));      
   }

   // OutputDir //
   connect(output_button, SIGNAL(clicked()), this, SLOT(selectOuput()));
   connect(output_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterOutput()));

   // Prefix // 
   connect(prefix_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterPrefix()));

   // Suffix //
   connect(suffix_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterSuffix()));   

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
   
   // Reset all executables 
   connect(resetAllExecutables_button, SIGNAL(clicked()), this, SLOT(resetAllExecutables()));

   // Run Pipeline
   connect(runPipeline_button, SIGNAL(clicked()), this, SLOT(runPipeline()));

   // Logging 
   connect(pipeline_radioButton, SIGNAL(clicked()), this, SLOT(selectLog())); 
   connect(registrations_radioButton, SIGNAL(clicked()), this, SLOT(selectLog())); 

   // Stop Pipeline
   connect(stopPipeline_button, SIGNAL(clicked()), this, SLOT(stopPipeline()));

   // Display Results  
   connect(displayResults_button, SIGNAL(clicked()), this, SLOT(displayResults())); 
   

   stopPipeline_action->setEnabled(false);

   newAtlas_radioButton->setChecked(true);
   existingAtlas_widget->hide();

   runPipeline_progressBar->hide(); 

   displayResults_button->setEnabled(false);
   displayResults_action->setEnabled(false);

   stopPipeline_button->setEnabled(false);

   pipeline_radioButton->setEnabled(false);  
   registrations_radioButton->setEnabled(false);
   registrations_comboBox->setEnabled(false); 

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

void DerivedWindow::printErrors(QString errors)
{
   QMessageBox::critical(this, "Errors in XML file(s)", errors);   
}

void DerivedWindow::initializeImagesMap()
{
   Image T1 = {T1_button, T1_lineEdit}; 
   m_images_map.insert("T1", T1);

   Image T2 = {T2_button, T2_lineEdit}; 
   m_images_map.insert("T2", T2);

   Image mask = {mask_button, mask_lineEdit}; 
   m_images_map.insert("mask", mask);

   Image b0 = {b0_button, b0_lineEdit}; 
   m_images_map.insert("b0", b0);

   Image DWI = {DWI_button, DWI_lineEdit}; 
   m_images_map.insert("DWI", DWI);
}

void DerivedWindow::initializeExecutablesMap()
{
   Executable SegPostProcessCLP = {SegPostProcessCLP_button, SegPostProcessCLP_lineEdit, resetSegPostProcessCLP_button};
   m_executables_map.insert("SegPostProcessCLP", SegPostProcessCLP);

   Executable N4ITKBiasFieldCorrection = {N4ITKBiasFieldCorrection_button, N4ITKBiasFieldCorrection_lineEdit, resetN4ITKBiasFieldCorrection_button};
   m_executables_map.insert("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection);

   Executable ITKTransformTools = {ITKTransformTools_button, ITKTransformTools_lineEdit, resetITKTransformTools_button};
   m_executables_map.insert("ITKTransformTools", ITKTransformTools);

   Executable bet2 = {bet2_button, bet2_lineEdit, resetBet2_button};
   m_executables_map.insert("bet2", bet2);

   Executable dtiestim = {dtiestim_button, dtiestim_lineEdit, resetDtiestim_button};
   m_executables_map.insert("dtiestim", dtiestim);

   Executable dtiprocess = {dtiprocess_button, dtiprocess_lineEdit, resetDtiprocess_button};
   m_executables_map.insert("dtiprocess", dtiprocess);

   Executable ANTS = {ANTS_button, ANTS_lineEdit, resetANTS_button};
   m_executables_map.insert("ANTS", ANTS);

   Executable ResampleVolume2 = {ResampleVolume2_button, ResampleVolume2_lineEdit, resetResampleVolume2_button};
   m_executables_map.insert("ResampleVolume2", ResampleVolume2);

   Executable ImageMath = {ImageMath_button, ImageMath_lineEdit, resetImageMath_button};
   m_executables_map.insert("ImageMath", ImageMath);

   Executable InsightSNAP = {InsightSNAP_button, InsightSNAP_lineEdit, resetInsightSNAP_button};
   m_executables_map.insert("InsightSNAP", InsightSNAP);
}

void DerivedWindow::about()
{
   m_about = new::About(); 
   m_about->show();    
}


void DerivedWindow::selectImage(QString image_name)
{
   Image image = m_images_map[image_name]; 

   QString image_path = (image.enter_lineEdit)->text();

   QString dir_path = ""; 
   if(!(image_path.isEmpty()))
   {
      dir_path = (QFileInfo(image_path).dir()).absolutePath(); 
   }

	image_path = QFileDialog::getOpenFileName(this, "Open file", dir_path,"Images (*.gipl *.gipl.gz *.nrrd *.nii *.nii.gz)");

   if(!image_path.isEmpty())
   {
      (image.enter_lineEdit)->setText(image_path);
   }
}
void DerivedWindow::enterImage(QString image_name)
{
   Image image = m_images_map[image_name];

   QString image_path = (image.enter_lineEdit)->text();

   if(!image_path.isEmpty()) 
   {    
      if(QFileInfo(image_path).exists())
      {
         if(!QFileInfo(image_path).isFile())
         {
            (image.enter_lineEdit)->clear(); 
            QMessageBox::critical(this, image_name, image_path + " is not a file,\nPlease enter a new file path");
         }
      }
      else
      {
         (image.enter_lineEdit)->clear(); 
         QMessageBox::critical(this, image_name, image_path + " does not exist,\nPlease enter a new file path");

      }
   }
}

//***** Ouput *****//
void DerivedWindow::selectOuput()
{
   QString output = QFileDialog::getExistingDirectory (this, "Open Directory",m_tests_path , QFileDialog::ShowDirsOnly);
   if(!(output.isEmpty()))
   {
      output_lineEdit->setText(output);
   }
}
void DerivedWindow::enterOutput()
{
   QString output = output_lineEdit->text();

   if(!output.isEmpty()) 
   {
      if(!m_parameters->checkOutput(output))
      {
         createOutput(output); 
      }
   }
}

void DerivedWindow::createOutput(QString output)
{
   QMessageBox messageBox(this);

   messageBox.setText(output + " does not exist,");
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
         //output_lineEdit->clear();
         break;
      case QMessageBox::Cancel:
         //output_lineEdit->clear();
         break;
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
   QString atlasPopulationDirectory = QFileDialog::getExistingDirectory (this, "Open Directory", atlasPopulationDirectory_lineEdit->text(), QFileDialog::ShowDirsOnly);
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
	QString parameters = QFileDialog::getOpenFileName(this, "Open file", m_data_path,"XML (*.xml)");
   if(!parameters.isEmpty())
   {
      XmlReader xmlReader;
      xmlReader.setPipelineParameters(m_parameters);
      QString parametersErrors = xmlReader.readParametersConfigurationFile(parameters); 

      parametersErrors = "Errors in the parameters configuration :\n" + parametersErrors; 
      parametersErrors += "\n";
      parametersErrors += "All the parameters that are nor valid, are left to their default value\n";
      QMessageBox::critical(this, "Errors in XML file", parametersErrors);   
   }
}


//***** Executables Configuration File *****//
void DerivedWindow::selectExecutables()
{
	QString executables = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", m_data_path,"XML (*.xml)");
   if(!executables.isEmpty())
   {
      XmlReader xmlReader;
      xmlReader.setPipelineParameters(m_parameters);
      QString executablesErrors = xmlReader.readParametersConfigurationFile(executables); 

      executablesErrors = "Errors in the parameters configuration :\n" + executablesErrors; 
      executablesErrors += "\n";
      executablesErrors += "All the parameters that are nor valid, are left to their default value\n";
      QMessageBox::critical(this, "Errors in XML file", executablesErrors);  
   }
}


// Executables 
void DerivedWindow::selectExecutable(QString executable_name)
{   
   Executable executable = m_executables_map[executable_name];

   QString executable_path = (executable.enter_lineEdit)->text(); 
   QString dir_path = ""; 

   if(!(executable_path.isEmpty()))
   {
      dir_path = (QFileInfo(executable_path).dir()).absolutePath(); 
   }

	executable_path = QFileDialog::getOpenFileName(this, "Select executable", dir_path);
   if(!executable_path.isEmpty())
   {
      if(QFileInfo(executable_path).isExecutable())
      {
         (executable.enter_lineEdit)->setText(executable_path);
      }
      else
      {
         QMessageBox::critical(this, executable_name, executable_path + "\nis not executable");      
      }
   }
}
void DerivedWindow::enterExecutable(QString executable_name)
{
   Executable executable = m_executables_map[executable_name];   

   QString executable_path = (executable.enter_lineEdit)->text();

   if(!executable_path.isEmpty()) 
   {  
      if(QFileInfo(executable_path).exists())
      {
         if(!QFileInfo(executable_path).isExecutable())
         {
            (executable.enter_lineEdit)->clear();
            QMessageBox::critical(this, executable_name, executable_path + "\nis not executable, enter a new executable path");            
         }
      }

      else 
      {
         (executable.enter_lineEdit)->clear();
         QMessageBox::critical(this, executable_name, executable_path + "\ndoes not exist, enter a new file path");
      }
   }   
}
void DerivedWindow::resetExecutable(QString executable_name)
{
   Executable executable = m_executables_map[executable_name]; 
   (executable.enter_lineEdit)->setText(m_executables->getExecutablePath(executable_name));
}
void DerivedWindow::resetAllExecutables()
{
   resetExecutable("SegPostProcessCLP"); 
   resetExecutable("N4ITKBiasFieldCorrection"); 
   resetExecutable("ITKTransformTools"); 
   resetExecutable("bet2"); 
   resetExecutable("dtiestim"); 
   resetExecutable("dtiprocess"); 
   resetExecutable("ANTS"); 
   resetExecutable("ResampleVolume2"); 
   resetExecutable("ImageMath"); 
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
      m_selectedAtlases = m_parameters->getSelectedAtlases();
   }
   else
   {
      m_selectedAtlases = m_goodAtlases;
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

   usingFA_checkBox->setChecked(m_parameters->getUsingFA()); 
   usingAD_checkBox->setChecked(m_parameters->getUsingAD()); 

   reassigningWhite_checkBox->setChecked(m_parameters->getReassigningWhiteMatter());
   whiteThreshold_spinBox->setValue(m_parameters->getSizeThreshold()); 

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

   usingMask_checkBox->setChecked(m_antsParameters->getUsingMask());
   usingSmoothedMask_checkBox->setChecked(m_antsParameters->getUsingSmoothedMask());

   //Neoseg 
   referenceModality_comboBox->insertItems(0, m_neosegParameters->getReferenceImageValues());
   referenceModality_comboBox->setCurrentIndex(m_neosegParameters->getReferenceImageIndex());

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


void DerivedWindow::setData()
{
   // Input
   m_parameters->setT1(T1_lineEdit->text());
   m_parameters->setT2(T2_lineEdit->text());
   m_parameters->setMask(mask_lineEdit->text());
   m_parameters->setDWI(DWI_lineEdit->text());
   m_parameters->setb0(b0_lineEdit->text());
 
   // Output
   m_parameters->setPrefix(prefix_lineEdit->text()); 
   m_parameters->setSuffix(suffix_lineEdit->text()); 
   if(!m_parameters->checkOutput(output_lineEdit->text()))
   {
      createOutput(output_lineEdit->text()); 
   }
   m_parameters->setOutput(output_lineEdit->text()); 
}

void DerivedWindow::setParameters()
{
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

      m_parameters->setUsingFA(usingFA_checkBox->isChecked());
      m_parameters->setUsingAD(usingAD_checkBox->isChecked());

      // Reassigning White Matter
      m_parameters->setReassigningWhiteMatter(reassigningWhite_checkBox->isChecked()); 
      m_parameters->setSizeThreshold(whiteThreshold_spinBox->value());
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
   m_parametersSet = true;
}

void DerivedWindow::setExecutables()
{
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
   m_executablesSet = true;
}

void DerivedWindow::saveParameters()
{
   setParameters();

   QString parameters_path = QFileDialog::getSaveFileName(this, "Save file", "parameters.xml", "XML files (*.xml)");

   XmlWriter* parameters = new::XmlWriter();
   parameters->setPipelineParameters(m_parameters);  
   parameters->writeParametersConfiguration(parameters_path);
}

void DerivedWindow::saveExecutables()
{
   setExecutables();

   QString executables_path = QFileDialog::getSaveFileName(this, "Save file", "executables.xml", "XML files (*.xml)");

   XmlWriter* executables = new::XmlWriter();
   executables->setPipelineParameters(m_parameters);  
   executables->writeExecutablesConfiguration(executables_path);
}

void DerivedWindow::setParametersWidgetEnabled(bool enabled)
{
   // Actions 
   loadParametersConfiguration_action->setEnabled(enabled); 
   saveParametersConfiguration_action->setEnabled(enabled); 
   loadSoftwaresConfiguration_action->setEnabled(enabled); 
   saveSoftwaresConfiguration_action->setEnabled(enabled); 
   runPipeline_action->setEnabled(enabled); 
   
   // Data 
   inputs_groupBox->setEnabled(enabled);
   outputs_groupBox->setEnabled(enabled);

   // Atlas
   newAtlas_radioButton->setEnabled(enabled);
   existingAtlas_radioButton->setEnabled(enabled);
   existingAtlas_groupBox->setEnabled(enabled);
   atlasPopulation_groupBox->setEnabled(enabled); 

   // Atlas Registration
   numberOfRegistrations_groupBox->setEnabled(enabled);
   ANTSParameters_groupBox->setEnabled(enabled);

   // Atlas Generation
   computingCCWeights_groupBox->setEnabled(enabled);
   includingFA_groupBox->setEnabled(enabled);
   smoothingProbabilities_groupBox->setEnabled(enabled);

   // Neoseg 
   neosegModalities_groupBox->setEnabled(enabled);
   neosegParameters_groupBox->setEnabled(enabled);
   mergedSegmentation_groupBox->setEnabled(enabled);

   // Softwares
   QMap<QString, Executable>::iterator it_exec; 
   for(it_exec = m_executables_map.begin(); it_exec != m_executables_map.end(); ++it_exec)
   {
      QString name = it_exec.key(); 
      Executable executable = it_exec.value();  

      executable.select_button->setEnabled(enabled); 
      executable.enter_lineEdit->setEnabled(enabled);
      executable.reset_button->setEnabled(enabled);   
   }

   resetAllExecutables_button->setEnabled(enabled);   

   // Computing 
   overwriting_checkBox->setEnabled(enabled);
   stoppingIfError_checkBox->setEnabled(enabled);
   cleaningUp_checkBox->setEnabled(enabled);
   computingSystem_label->setEnabled(enabled);
   computingSystem_comboBox->setEnabled(enabled);   
} 

void DerivedWindow::runPipeline()
{
   setData();
   setParameters();
   setExecutables();

   QString imagesErrors = m_parameters->checkImages(); 
   QString executablesErrors = m_executables->checkExecutables(); 

   if(imagesErrors.isEmpty() && executablesErrors.isEmpty())
   {
      m_pipeline->writePipeline();

      initializePipelineLogging();
      initializeRegistrationsLogging();

      m_pipeline->setPlainTextEdit(m_log_plainTextEdit); 
      m_thread->setPipeline(m_pipeline);

      m_thread->start();

      setParametersWidgetEnabled(false);
      runPipeline_button->setEnabled(false);
      stopPipeline_button->setEnabled(true);
      displayResults_button->setEnabled(false);
      displayResults_action->setEnabled(false);
      tabs->setCurrentWidget(execution_tab);   

      runPipeline_progressBar->setTextVisible(false);
      runPipeline_progressBar->show();
      runPipeline_progressBar->setMinimum(0);
      runPipeline_progressBar->setMaximum(0); 
   }
   else
   {
      QMessageBox::critical(this, "Errors", imagesErrors + executablesErrors);
   }
}

void DerivedWindow::initializePipelineLogging()
{
   Logging logging; 

   // Radio Button
   pipeline_radioButton->setEnabled(true); 
   pipeline_radioButton->setChecked(true); 

   // Plain Text Edit
   m_log_plainTextEdit->show();
   m_log_plainTextEdit->clear();
   m_log_plainTextEdit->setMaximumBlockCount(500);

   // Log path
   QDir* output_dir = new QDir(m_parameters->getOutput());
   QString log_path = output_dir->filePath(m_parameters->getPrefix() + ".log");

   // Log File 
   QFile* log_file = new::QFile(log_path); 
   log_file->open(QIODevice::ReadWrite);    
   m_log_textStream = new::QTextStream(log_file); 

   // QFileSystemWatcher
   QFileSystemWatcher* log_watcher = new::QFileSystemWatcher(this); 
   log_watcher->addPath(log_path); 
   connect(log_watcher, SIGNAL(fileChanged(QString)), this, SLOT(printPipelineLog(QString)));
}

void DerivedWindow::initializeRegistrationsLogging()
{
   QStringList selectedAtlases = m_parameters->getSelectedAtlases(); 

   QDir output_dir(m_parameters->getOutput());
   QString atlasRegistration_path = output_dir.filePath("3.AtlasRegistration");
   QDir atlasRegistration_dir(atlasRegistration_path);

   Logging logging; 

   QStringList::iterator it; 
   for(it = selectedAtlases.begin(); it != selectedAtlases.end(); ++it)
   {
      // Existing QPlainTextEdit
      if(m_registrationLoggings.contains(*it))
      {
         delete (m_registrationLoggings[*it]).plainTextEdit;
      }

      // QPlainTextEdit   
      QPlainTextEdit* log_plainTextEdit = new::QPlainTextEdit(execution_widget); 

      QFont font("Courier 10 Pitch", 9);
      log_plainTextEdit->setFont(font);  

      QLayout* execution_layout = execution_widget->layout(); 
      execution_layout->addWidget(log_plainTextEdit);

      log_plainTextEdit->setMaximumBlockCount(500);

      log_plainTextEdit->hide(); 

      // Atlas Directory 
      QString atlas_path = atlasRegistration_dir.filePath(*it);
      QDir atlas_dir(atlas_path); 

      // Log File
      QString log_name = *it + "_to_neo.log";
      QString log_path = atlas_dir.filePath(log_name); 
      QFile* log_file = new::QFile(log_path); 
      log_file->open(QIODevice::ReadWrite);
      QTextStream* log_textStream = new::QTextStream(log_file); 
      
      // QFileSystemWatcher
      QFileSystemWatcher* log_watcher = new::QFileSystemWatcher(this); 
      log_watcher->addPath(log_path); 
      connect(log_watcher, SIGNAL(fileChanged(QString)), this, SLOT(printRegistrationLog(QString)));

      logging.textStream = log_textStream;
      logging.plainTextEdit = log_plainTextEdit; 

      m_registrationLoggings.insert(*it, logging); 
   }

   // Radio Button
   registrations_radioButton->setEnabled(true);

   // Combo Box 
   registrations_comboBox->clear();
   registrations_comboBox->setEnabled(true); 
   registrations_comboBox->insertItems(0, selectedAtlases); 
   connect(registrations_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeExecutionPlainTextEdit(int)));
}

void DerivedWindow::printPipelineLog(QString log_path)
{  
   QScrollBar *scrollBar = m_log_plainTextEdit->verticalScrollBar();

   if(scrollBar->value() == scrollBar->maximum())
   {
      m_log_plainTextEdit->insertPlainText((m_log_textStream->readAll()));
      scrollBar->setValue(scrollBar->maximum());
   }
   else
   {
      m_log_plainTextEdit->insertPlainText((m_log_textStream->readAll()));
   }
}

void DerivedWindow::printRegistrationLog(QString log_path)
{  

   QString atlas_name = (QFileInfo(log_path).absoluteDir()).dirName(); 
   Logging logging = m_registrationLoggings[atlas_name];
   QTextStream* log_textStream = logging.textStream;
   QPlainTextEdit* log_plainTextEdit = logging.plainTextEdit; 

   QScrollBar *scrollBar = log_plainTextEdit->verticalScrollBar();

   if(scrollBar->value() == scrollBar->maximum())
   {
      log_plainTextEdit->insertPlainText((log_textStream->readAll()));
      scrollBar->setValue(scrollBar->maximum());
   }
   else
   {
      log_plainTextEdit->insertPlainText((log_textStream->readAll()));
   }
}

void DerivedWindow::selectLog()
{
   QString registration = registrations_comboBox->currentText();   
   Logging logging = m_registrationLoggings[registration];

   if(pipeline_radioButton->isChecked())
   {
      m_log_plainTextEdit->show();
      logging.plainTextEdit->hide(); 
   }

   if(registrations_radioButton->isChecked())
   {
      m_log_plainTextEdit->hide();
      logging.plainTextEdit->show();     
   }
}

void DerivedWindow::changeExecutionPlainTextEdit(int index)
{
   QString atlas_name = registrations_comboBox->currentText(); 
   
   QMap <QString, Logging>::iterator it; 
   QString name; 
   Logging logging; 
   for(it = m_registrationLoggings.begin(); it != m_registrationLoggings.end(); ++it)
   {
      name = it.key(); 
      logging = it.value(); 

      if(name == atlas_name && registrations_radioButton->isChecked())
      {
         logging.plainTextEdit->show();
      }
      else
      {
         logging.plainTextEdit->hide();
      }   
   }   
}

void DerivedWindow::stopPipeline()
{
   m_thread->terminate(); 

   setParametersWidgetEnabled(true); 

   runPipeline_button->setEnabled(true);
   runPipeline_action->setEnabled(true);

   stopPipeline_button->setEnabled(false); 
   stopPipeline_action->setEnabled(false); 

   runPipeline_progressBar->hide();
}

void DerivedWindow::enableDisplayButton()
{
   if(QFile(m_parameters->getSegmentation()).exists())
   {   
      displayResults_button->setEnabled(true); 
      displayResults_action->setEnabled(true); 
   }

   setParametersWidgetEnabled(true); 

   runPipeline_button->setEnabled(true);
   runPipeline_action->setEnabled(true);

   stopPipeline_button->setEnabled(false); 
   stopPipeline_action->setEnabled(false); 

   runPipeline_progressBar->hide();
}

void DerivedWindow::displayResults()
{
   QProcess insightSNAP_process;

   QString command = m_executables->getExecutablePath("InsightSNAP") + " -g " + m_parameters->getT1() + " -s " + m_parameters->getSegmentation(); 
   insightSNAP_process.start(command);

   while (!insightSNAP_process.waitForFinished())
   {
   } 
 
}

void DerivedWindow::closeEvent(QCloseEvent *event)
{
   if(m_thread->isRunning())
   {
      QMessageBox* messageBox = new::QMessageBox(this);

      messageBox->setText("The pipeline is still running,");
      messageBox->setInformativeText("Do you want to terminate it?");
      messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      messageBox->setDefaultButton(QMessageBox::No);

      int answer = messageBox->exec();
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
            event->ignore();
            break;
      }
   }
}






