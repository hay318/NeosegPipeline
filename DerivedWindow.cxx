#include "DerivedWindow.h"

DerivedWindow::DerivedWindow() : Ui_Window()
{
	setupUi(this);

   m_atlasPopulation_path="/work/mcherel/project/atlasPopulation/atlases";

   m_parametersSet = false; 
   m_executablesSet = false; 

   initializeImagesMap();

   //Connections

   // OutputDir //
   connect(output_button, SIGNAL(clicked()), this, SLOT(selectOuput()));
   connect(output_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterOutput()));

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

   // Run Pipeline //
   connect(runPipeline_button, SIGNAL(clicked()), this, SLOT(runPipeline()));

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
}

void DerivedWindow::initializeImagesMap()
{
  m_images.insert(std::pair<QString, QLineEdit*>("T1", T1_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("T2", T2_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("mask", mask_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("DWI", DWI_lineEdit));
  m_images.insert(std::pair<QString, QLineEdit*>("b0", b0_lineEdit));
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
         output_lineEdit->clear();
         QMessageBox::critical(this, "Output Directory", output + "\ndoes not exist, enter a new directory path");
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

//***** New Atlas/Existing Atlas *****//
void DerivedWindow::selectNewOrExistingAtlas()
{
   if(newAtlas_radioButton->isChecked())
   {
      newAtlas_widget->show();
      existingAtlas_widget->hide();

      atlasPopulationDirectory_lineEdit->setText(m_atlasPopulation_path); 
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
   QString atlasPopulationDirectory = QFileDialog::getExistingDirectory (0, "Open Directory", m_atlasPopulation_path, QFileDialog::ShowDirsOnly);
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

//***** Checking Atlases *****//
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

void DerivedWindow::initializeParameters()
{
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


   //smoothing_comboBox

   //smoothingSize_spinBox->setMinimum(m_parameters->getSmoothingSizeMin());
   smoothingSize_spinBox->setValue(m_parameters->getSmoothingSize());

   computingWeights_checkBox->setChecked(m_parameters->getComputingWeights()); 


   //weightsModality_comboBox
   
   //weightsRadius_spinBox->setMinimum(m_parameters->getWeightsRadiusMin());
   weightsRadius_spinBox->setValue(m_parameters->getWeightsRadius());   

   overwriting_checkBox->setChecked(m_parameters->getOverwriting()); 

   cleaningUp_checkBox->setChecked(m_parameters->getCleaningUp()); 


   //computingSystem_comboBox

   //numberOfCores_spinBox->setMinimum(m_parameters->getNumberOfCoresMin());
   numberOfCores_spinBox->setValue(m_parameters->getNumberOfCores());   


   //imageMetric1_comboBox

   //weight1_spinBox->setMinimum(m_antsParameters->getWeightMin());
   weight1_spinBox->setValue(m_antsParameters->getWeight1());
   
   //radius1_spinBox->setMinimum(m_antsParameters->getRadiusMin());   
   radius1_spinBox->setValue(m_antsParameters->getRadius1());


   //imageMetric1_comboBox

   //weight1_spinBox->setMinimum(m_antsParameters->getWeightMin());
   weight1_spinBox->setValue(m_antsParameters->getWeight1());
   
   //radius1_spinBox->setMinimum(m_antsParameters->getRadiusMin());   
   radius1_spinBox->setValue(m_antsParameters->getRadius1());

   //iterationsJ->setMinimum(m_antsParameters->getIterationsMin());   
   iterationsJ_spinBox->setValue(m_antsParameters->getIterationsJ());

   //iterationsK->setMinimum(m_antsParameters->getIterationsMin());   
   iterationsK_spinBox->setValue(m_antsParameters->getIterationsK());

   //iterationsL->setMinimum(m_antsParameters->getIterationsMin());   
   iterationsL_spinBox->setValue(m_antsParameters->getIterationsL());


   //transformatiomType_comboBox

   //gradientStepLength_spinBox->setMinimum(m_antsParameters->getGradientStepLengthMin());
   gradientStepLength_spinBox->setValue(m_antsParameters->getGradientStepLength());

   //numberOfTimeSteps_spinBox->setMinimum(m_antsParameters->getNumberOfTimeStepsMin());
   numberOfTimeSteps_spinBox->setValue(m_antsParameters->getNumberOfTimeSteps());

   //deltaTime_spinBox->setMinimum(m_antsParameters->getDeltaTimeMin());
   deltaTime_spinBox->setValue(m_antsParameters->getDeltaTime());

   //regularizationType_comboBox

   //gradientFieldSigma_spinBox->setMinimum(m_antsParameters->getGradientFieldSigmaMin());
   gradientFieldSigma_spinBox->setValue(m_antsParameters->getGradientFieldSigma());

   //deformationFieldSigma_spinBox->setMinimum(m_antsParameters->getDeformationFieldSigmaMin());
   deformationFieldSigma_spinBox->setValue(m_antsParameters->getDeformationFieldSigma());

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
   // Data
   if(!(prefix_lineEdit->text()).isEmpty())
   {
      m_parameters->setPrefix(prefix_lineEdit->text()); 
   }

   if(!(output_lineEdit->text()).isEmpty())
   {
      m_parameters->setOutput(output_lineEdit->text()); 
   }

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

   //m_parameters->setSkullStripping(skullStripping_checkbox->isChecked());

   //Atlas Population 
   if (newAtlas_radioButton->isChecked()) 
   {
      m_parameters->setNewAtlas(true);
      m_parameters->setAtlasPopulationDirectory(m_atlasPopulation_path);
      m_parameters->setSelectedAtlases(m_selectedAtlases);
      m_parameters->setSmoothing(smoothing_comboBox->currentText());
      m_parameters->setSmoothingSize(smoothingSize_spinBox->value());
      m_parameters->setComputingWeights(computingWeights_checkBox->isChecked());
      m_parameters->setWeightsRadius(weightsRadius_spinBox->value());
   }

   else 
   {
      m_parameters->setNewAtlas(false);  
      m_parameters->setAtlas(existingAtlas_lineEdit->text());    
   }

   // Computation
   m_parameters->setOverwriting(overwriting_checkBox->isChecked());
   m_parameters->setCleaningUp(cleaningUp_checkBox->isChecked());
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


   // Neoseg parameters 
   m_neosegParameters->setFilterMethod(filterMethod_comboBox->currentText()); 
   m_neosegParameters->setNumberOfIterations(numberOfIterations_spinBox->value());
   m_neosegParameters->setTimeStep(timeStep_spinBox->value()); 

   m_neosegParameters->setReferenceImage(referenceModality_comboBox->currentText()); 
   m_neosegParameters->setPriorThreshold(priorThreshold_spinBox->value()); 
   //m_neosegParameters->setMaxDegree(maxDegree_spinBox->value()); 
   m_neosegParameters->setPrior1(prior1_spinBox->value()); 
   m_neosegParameters->setPrior2(prior3_spinBox->value()); 
   m_neosegParameters->setPrior3(prior3_spinBox->value()); 
   m_neosegParameters->setPrior4(prior4_spinBox->value()); 
   m_neosegParameters->setPrior5(prior5_spinBox->value()); 

   m_neosegParameters->setRefinement(refinement_checkBox->isChecked());
   m_neosegParameters->setInitialParzenKernelWidth(initialParzenKernelWidth_spinBox->value());

   m_parameters->setUsingFA(usingFA_checkBox->isChecked());
   m_parameters->setUsingAD(usingAD_checkBox->isChecked());
   m_parameters->setComputing3LabelsSeg(computing3LabelsSeg_checkBox->isChecked());

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

   runPipeline_progressBar->show();

   m_thread->setPipeline(m_pipeline);
   m_thread->start();
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




