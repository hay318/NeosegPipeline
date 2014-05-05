#include "XmlReader.h" 

void XmlReader::setPipelineParameters(PipelineParameters* parameters) 
{
   m_parameters=parameters;

   m_antsParameters=m_parameters->getAntsParameters();

   m_neosegParameters=m_parameters->getNeosegParameters();

   m_executablePaths=m_parameters->getExecutablePaths(); 
}

bool XmlReader::isBoolean(int value)
{
   if(value==0 || value==1)
   {
      return true; 
   }
   return false; 
}


QString XmlReader::readParametersConfigurationFile(QString file_path)
{
   QFile* file = new::QFile(file_path);

   if(!file->exists())
   {
      return " - " + file_path + " does not exist\n";
   }

   else
   {
      file->open(QIODevice::ReadOnly);

      QXmlStreamReader* stream = new::QXmlStreamReader(file);

      QString errors; 
      bool ok; 

      while(!stream->atEnd())
      {
         stream->readNext();
               
         if (stream->isStartElement())
         {
            QXmlStreamAttributes attributes = stream->attributes();

            if(stream->name() == "New-atlas-computation")
            {
               bool newAtlas = ((attributes.value("bool")).toString()).toInt(&ok);
               if(ok && isBoolean(newAtlas))
               {
                  m_parameters->setNewAtlas(newAtlas);
               }
               else
               {
                  errors += " - 'New-atlas-computation' is not valid, it must be a boolean\n";
               }
            }

            else if (stream->name() == "Atlas-population-directory")
            {
               QString atlasPopulationPath = (attributes.value("path")).toString();
               if(m_parameters->checkAtlasPopulationDirectory(atlasPopulationPath))
               {
                  m_parameters->setAtlasPopulationDirectory(atlasPopulationPath);
               } 
               else
               {
                  errors += " - 'Atlas-population-directory' is not valid, it does not exist\n";
               }
            }

            else if (stream->name() == "Atlas")
            {
               QString atlas = (attributes.value("name")).toString();   

               if(m_parameters->checkAtlasFiles(atlas))
               {
                  if(m_parameters->checkAtlasRange(atlas))
                  {
                     std::cout<<atlas.toStdString()<<" is valid"<<std::endl; 
                     m_selectedAtlases += atlas;
                  }
                  else
                  {
                     errors += " - " + atlas + " is not a valid atlas, the probability maps have to range from 0 to 1\n";  
                  }
               }
               else
               {
                  errors += " - " + atlas + " is not a valid atlas, it does not have all the files required\n";
               }
            }

            else if (stream->name() == "Smoothing")
            {
               QString smoothing = (attributes.value("type")).toString();
               if(m_parameters->checkSmoothing(smoothing))
               {
                  m_parameters->setSmoothing(smoothing);
               }
               else if(!smoothing.isEmpty())
               {
                  errors += " - 'Smoothing-type' is not valid, it must be one of the following : " + (m_parameters->getSmoothingValues()).join(", ") + "\n";
               }

               QString smoothingSize_QString = (attributes.value("size")).toString();
               double smoothingSize = smoothingSize_QString.toDouble(&ok);
               if(ok || m_parameters->checkSmoothingSize(smoothingSize))
               {
                  m_parameters->setSmoothingSize(smoothingSize);
               }
               else if(!smoothingSize_QString.isEmpty())
               {
                  errors += " - 'Smoothing-size' is not valid, it must be a positive number\n";
               }
            }

            else if (stream->name() == "Computing-weights")
            {
               QString computingWeights_QString = (attributes.value("bool")).toString();
               bool computingWeights = computingWeights_QString.toInt(&ok);
               if(ok && isBoolean(computingWeights))
               {
                  m_parameters->setComputingWeights(computingWeights);
               }
               else if(!computingWeights_QString.isEmpty())
               {
                  errors += " - 'Computing-weights' is not valid, it must be a boolean\n";
               }

               QString weightsModality = (attributes.value("modality")).toString(); 
               if(m_parameters->checkWeightsModality(weightsModality))
               {
                  m_parameters->setWeightsModality(weightsModality);
               }
               else if(!weightsModality.isEmpty())
               {
                  errors += " - 'Weights-modality' is not valid, it must be one of the following : " + (m_parameters->getWeightsModalityValues()).join(", ") + "\n";
               }

               QString weightsRadius_QString = (attributes.value("radius")).toString();
               double weightsRadius = weightsRadius_QString.toDouble(&ok);
               if(ok && m_parameters->checkWeightsRadius(weightsRadius))
               {
                  m_parameters->setWeightsRadius(weightsRadius);
               }
               else if(!weightsRadius_QString.isEmpty())
               {
                  errors += " - 'Weights-radius' is not valid, it must be a positive number\n";
               }

               QString weightsRadiusUnit = (attributes.value("unit")).toString(); 
               if(m_parameters->checkWeightsRadiusUnit(weightsRadiusUnit))
               {
                  m_parameters->setWeightsRadiusUnit(weightsRadiusUnit);
               }
               else if(!weightsRadiusUnit.isEmpty())
               {
                  errors += " - 'Weights-radius-unit' is not valid, it must be one of the following : " + (m_parameters->getWeightsRadiusUnitValues()).join(", ") + "\n";
               }
            }

            else if (stream->name() == "Including-FA")
            {
               QString includingFA_QString = (attributes.value("bool")).toString();
               double includingFA = includingFA_QString.toInt(&ok);
               if(ok && isBoolean(includingFA))
               {
                  m_parameters->setIncludingFA(includingFA);
               }
               else if(!includingFA_QString.isEmpty())
               {
                  errors += " - 'Including-FA' is not valid, it must be a boolean\n";
               }

               QString weight_QString = (attributes.value("weight")).toString();
               double weight = weight_QString.toDouble(&ok);
               if(ok && m_parameters->checkFAWeight(weight))
               {
                  m_parameters->setFAWeight(weight);
               }
               else if(!weight_QString.isEmpty())
               {
                  errors += " - 'FA-weight' is not valid, it must be a positive number\n";
               }

               QString smoothingSize_QString = (attributes.value("smoothing-size")).toString();
               double smoothingSize = smoothingSize_QString.toDouble(&ok);
               if(ok && m_parameters->checkFASmoothingSize(smoothingSize))
               {
                  m_parameters->setFASmoothingSize(smoothingSize);
               }
               else if(!smoothingSize_QString.isEmpty())
               {
                  errors += " - 'FA-smoothing' is not valid, it must be a positive number\n";
               }
            }

            else if (stream->name() == "Neoseg-images")
            {
               QString usingFA_QString = (attributes.value("using-FA")).toString();
               double usingFA = usingFA_QString.toInt(&ok);
               if(ok && isBoolean(usingFA))
               {
                  m_parameters->setUsingFA(usingFA);
               }
               else if(!usingFA_QString.isEmpty())
               {
                  errors += " - 'Using-FA' is not valid, it must be a boolean\n";
               }

               QString usingAD_QString = (attributes.value("using-AD")).toString();
               double usingAD = usingAD_QString.toInt(&ok);
               if(ok && isBoolean(usingAD))
               {
                  m_parameters->setUsingAD(usingAD);
               }
               else if(!usingAD_QString.isEmpty())
               {
                  errors += " - 'Using-AD' is not valid, it must be a boolean\n";
               }
            }

            else if (stream->name() == "Computing-3-labels-seg")
            {
               bool computing3LabelsSeg = ((attributes.value("bool")).toString()).toInt(&ok);
               if(ok && isBoolean(computing3LabelsSeg))
               {
                  m_parameters->setComputing3LabelsSeg(computing3LabelsSeg);
               }
               else 
               {
                  errors += " - 'Computing-3-labels-seg' is not valid, it must be a boolean\n";
               }
            }

            else if (stream->name() == "Reassigning-white-matter")
            {
               QString reassigningWhiteMatter_QString = (attributes.value("bool")).toString();
               bool reassigningWhiteMatter = reassigningWhiteMatter_QString.toInt(&ok);
               if(ok && isBoolean(reassigningWhiteMatter))
               {
                  m_parameters->setReassigningWhiteMatter(reassigningWhiteMatter);
               }
               else if(!reassigningWhiteMatter_QString.isEmpty())
               {
                  errors += " - 'Reassigning-white-matter' is not valid, it must be a boolean\n";
               }

               QString sizeThreshold_QString = (attributes.value("size-threshold")).toString();
               int sizeThreshold = sizeThreshold_QString.toInt(&ok);
               if(ok && m_parameters->checkSizeThreshold(sizeThreshold))
               {
                  m_parameters->setSizeThreshold(sizeThreshold);
               }
               else if(!sizeThreshold_QString.isEmpty())
               {
                  errors += " - 'Size-threshold' is not valid, it must be a positive integer\n";
               }
            }

            else if (stream->name() == "Overwriting")
            {
               bool overwriting = ((attributes.value("bool")).toString()).toInt(&ok);
               if(ok && isBoolean(overwriting))
               {
                  m_parameters->setOverwriting(overwriting);
               }
               else 
               {
                  errors += " - 'Overwriting' is not valid, it must be a boolean\n";
               }
            }

            else if (stream->name() == "Cleaning-up")
            {
               bool cleaningUp = ((attributes.value("bool")).toString()).toInt(&ok);
               if(ok && isBoolean(cleaningUp))
               {
                  m_parameters->setCleaningUp(cleaningUp);
               } 
               else 
               {
                  errors += " - 'Cleaning-up' is not valid, it must be a boolean\n";
               }
            }

            else if (stream->name() == "Computing-system")
            {
               QString computingSystem = (attributes.value("name")).toString(); 
               if(m_parameters->checkComputingSystem(computingSystem))
               {
                  m_parameters->setComputingSystem(computingSystem);
               }
               else 
               {
                  errors += " - 'Computing-system' is not valid, it must be one of the following : " + (m_parameters->getComputingSystemValues()).join(", ") + "\n";
               }
            }

            else if (stream->name() == "Number-of-cores")
            {
               int numberOfCores = ((attributes.value("nb")).toString()).toInt(&ok);
               if(ok && m_parameters->checkNumberOfCores(numberOfCores))
               { 
                  m_parameters->setNumberOfCores(numberOfCores);
               }
               else 
               {
                  errors += " - 'Number-of-cores' is not valid, it must be a positive number\n";
               }
            }

            else if (stream->name() == "First-modality")
            {
               QString imageMetric = (attributes.value("metric")).toString(); 
               if(m_antsParameters->checkImageMetric(imageMetric))   
               {
                  m_antsParameters->setImageMetric1(imageMetric);
               }
               else if(!imageMetric.isEmpty())
               {
                  errors += " - 'First-modality-metric' is not valid, it must be one of the following : " + (m_antsParameters->getImageMetricValues()).join(", ") +"\n";
               }

               QString radius_QString = (attributes.value("radius")).toString(); 
               int radius = radius_QString.toInt(&ok);
               if(ok && m_antsParameters->checkRadius1(radius))
               {
                  m_antsParameters->setRadius1(radius);
               }
               else if(!radius_QString.isEmpty())
               {
                  errors += " - 'First-modality-radius' is not valid, it must be a positive integer\n";
               }
      
               QString weight_QString = (attributes.value("weight")).toString();
               double weight = weight_QString.toDouble(&ok); 
               if(ok && m_antsParameters->checkWeight1(weight))
               {
                  m_antsParameters->setWeight1(weight);
               }
               else if(!weight_QString.isEmpty())
               {
                  errors += " - 'First-modality-weight' is not valid, it must be a positive number\n";
               }
            }

            else if (stream->name() == "Second-modality")
            {
               QString imageMetric = (attributes.value("metric")).toString(); 
               if(m_antsParameters->checkImageMetric(imageMetric))   
               {
                  m_antsParameters->setImageMetric2(imageMetric);
               }
               else if(!imageMetric.isEmpty())
               {
                  errors += " - 'Second-modality-metric' is not valid, it must be one of the following : " + (m_antsParameters->getImageMetricValues()).join(", ") +"\n";
               }

               QString radius_QString = (attributes.value("radius")).toString();
               int radius = radius_QString.toInt(&ok);
               if(ok && m_antsParameters->checkRadius2(radius))
               {
                  m_antsParameters->setRadius2(radius);
               }
               else if(!radius_QString.isEmpty())
               {
                  errors += " - 'Second-modality-radius' is not valid, it must be a positive integer\n";
               }

               QString weight_QString = (attributes.value("weight")).toString();
               double weight = weight_QString.toDouble(&ok); 
               if(ok && m_antsParameters->checkWeight2(weight))
               {
                  m_antsParameters->setWeight2(weight);
               }
               else if(!weight_QString.isEmpty())
               {
                  errors += " - 'Second-modality-weight' is not valid, it must be a positive number\n";
               }
            }

            else if (stream->name() == "Iterations")
            {
               QString iterationsJ_QString = (attributes.value("J")).toString();
               int iterationsJ = iterationsJ_QString.toInt(&ok);
               if(ok && m_antsParameters->checkIterationsJ(iterationsJ))  
               {
                  m_antsParameters->setIterationsJ(iterationsJ); 
               }
               else if(!iterationsJ_QString.isEmpty())
               {
                  errors += " - 'Iterations-J' is not valid, it must be a positive integer\n";
               }

               QString iterationsK_QString = (attributes.value("K")).toString();
               int iterationsK = iterationsK_QString.toInt(&ok);
               if(ok && m_antsParameters->checkIterationsK(iterationsK))
               {
                  m_antsParameters->setIterationsK(iterationsK);
               }
               else if(!iterationsK_QString.isEmpty())
               {
                  errors += " - 'Iterations-K' is not valid, it must be a positive integer\n";
               }
            
               QString iterationsL_QString = (attributes.value("L")).toString();
               int iterationsL = iterationsL_QString.toInt(&ok);
               if(ok && m_antsParameters->checkIterationsL(iterationsL))
               {
                  m_antsParameters->setIterationsL(iterationsL);            
               }
               else if(!iterationsL_QString.isEmpty()) 
               {
                  errors += " - 'Iterations-L' is not valid, it must be a positive integer\n";
               }
            }

            else if (stream->name() == "Transformation")
            {
               QString type = (attributes.value("type")).toString(); 
               if(m_antsParameters->checkTransformationType(type))
               {             
                  m_antsParameters->setTransformationType(type);        
               }
               else if(!type.isEmpty())
               {
                  errors += " - 'Transformation-type' is not valid, it must be one of the following : " + (m_antsParameters->getTransformationTypeValues()).join(", ") + "\n";
               }

               QString gradientStepLength_QString = (attributes.value("gradient-step-length")).toString(); 
               double gradientStepLength = gradientStepLength_QString.toDouble(&ok);
               if(ok && m_antsParameters->checkGradientStepLength(gradientStepLength))
               {
                  m_antsParameters->setGradientStepLength(gradientStepLength);
               }
               else if(!gradientStepLength_QString.isEmpty())
               {
                  errors += " - 'Transformation-gradient-step-length' is not valid, it must be a positive number\n";
               }

               QString numberOfTimeSteps_QString = (attributes.value("number-of-time-steps")).toString();
               double numberOfTimeSteps = numberOfTimeSteps_QString.toDouble(&ok);
               if(ok && m_antsParameters->checkNumberOfTimeSteps(numberOfTimeSteps))
               {
                  m_antsParameters->setNumberOfTimeSteps(numberOfTimeSteps); 
               }
               else if(!numberOfTimeSteps_QString.isEmpty())
               {
                  errors += " - 'Transformation-number-of-time-steps' is not valid, it must be a positive number\n";
               }

               QString deltaTime_QString = (attributes.value("delta-time")).toString();
               double deltaTime = deltaTime_QString.toDouble(&ok);
               if(ok && m_antsParameters->checkDeltaTime(deltaTime))
               {
                  m_antsParameters->setDeltaTime(deltaTime);      
               }
               else if(!deltaTime_QString.isEmpty())
               {
                  errors += " - 'Transformation-delta-time' is not valid, it must be a positive number\n";
               }
            }

            
            else if (stream->name() == "Regularization")
            { 
               QString type = (attributes.value("type")).toString();  
               if(m_antsParameters->checkRegularizationType(type)) 
               {              
                  m_antsParameters->setRegularizationType(type);
               }    
               else if(!type.isEmpty())
               {
                  errors += " - 'Regularization-type' is not valid, it must be one of the following : " + (m_antsParameters->getRegularizationTypeValues()).join(", ") + "\n";
               }  

               QString gradientFieldSigma_QString = (attributes.value("gradient-field-sigma")).toString();
               double gradientFieldSigma = gradientFieldSigma_QString.toDouble(&ok);
               if(ok && m_antsParameters->checkGradientFieldSigma(gradientFieldSigma))
               {
                  m_antsParameters->setGradientFieldSigma(gradientFieldSigma);
               }
               else if(!gradientFieldSigma_QString.isEmpty())
               {
                  errors += " - 'Regularization-gradient-field-sigma' is not valid, it must be a positive number\n";
               }   

               QString deformationFieldSigma_QString = (attributes.value("deformation-field-sigma")).toString();
               double deformationFieldSigma = deformationFieldSigma_QString.toDouble(&ok);
               if(ok && m_antsParameters->checkDeformationFieldSigma(deformationFieldSigma))
               {
                  m_antsParameters->setDeformationFieldSigma(deformationFieldSigma);             
               }
               else if(!deformationFieldSigma_QString.isEmpty())
               {
                  errors += " - 'Regularization-deformation-field-sigma' is not valid, it must be a positive number\n";
               }     
            }

            else if (stream->name() == "Filtering")
            {
               QString type = (attributes.value("type")).toString(); 
               if(m_neosegParameters->checkFilterMethod(type))
               {  
                  m_neosegParameters->setFilterMethod(type);    
               }
               else if(!type.isEmpty())
               {
                  errors += " - 'Filtering-type' is not valid, it must be one of the following : " + (m_neosegParameters->getFilterMethodValues()).join(", ") + "\n";
               }

               QString iterations_QString = (attributes.value("iterations")).toString();
               int iterations = iterations_QString.toInt(&ok);
               if(ok && m_neosegParameters->checkNumberOfIterations(iterations))
               { 
                  m_neosegParameters->setNumberOfIterations(iterations);   
               }             
               else if(!iterations_QString.isEmpty())
               {
                  errors += " - 'Filtering-iterations' is not valid, it must be a positive integer\n";
               }   

               QString timeStep_QString = (attributes.value("time-step")).toString();
               double timeStep = timeStep_QString.toDouble(&ok);
               if(ok && m_neosegParameters->checkTimeStep(timeStep))
               {
                  m_neosegParameters->setTimeStep(timeStep);   
               } 
               else if(!timeStep_QString.isEmpty())
               {
                  errors += " - 'Filtering-time-step' is not valid, it must be a positive number\n";
               }           
            }

            else if (stream->name()=="Reference-image")
            { 
               QString referenceImage = (attributes.value("name")).toString(); 
               if(m_neosegParameters->checkReferenceImage(referenceImage))
               {
                  m_neosegParameters->setReferenceImage(referenceImage);    
               }
               else if(!referenceImage.isEmpty())
               {
                  errors += " - 'Reference-image' is not valid, it must be one of the following :" + (m_neosegParameters->getReferenceImageValues()).join(", ") + "\n";
               }    
            }

            else if (stream->name()=="Prior-threshold")
            { 
               QString priorThreshold_QString = (attributes.value("value")).toString();
               double priorThreshold = priorThreshold_QString.toDouble(&ok); 
               if(ok && m_neosegParameters->checkPriorThreshold(priorThreshold))
               {
                  m_neosegParameters->setPriorThreshold(priorThreshold);    
               }
               else if(!priorThreshold_QString.isEmpty())
               {
                  errors += " - 'Prior-threshold' is not valid, it must be a positive number between 0 and 1\n";
               }   
            }

            else if (stream->name()=="Max-bias-degree")
            { 
               QString maxBiasDegree_QString = (attributes.value("value")).toString();
               int maxBiasDegree = maxBiasDegree_QString.toInt(&ok); 
               if(ok && m_neosegParameters->checkMaxBiasDegree(maxBiasDegree))
               {
                  m_neosegParameters->setMaxBiasDegree(maxBiasDegree);    
               }
               else if(!maxBiasDegree_QString.isEmpty())
               {
                  errors += " - 'Max-bias-degree' is not valid, it must be a positive integer\n";
               }   
            }

            else if (stream->name()=="Priors")
            { 
               QString prior1_QString = (attributes.value("prior1")).toString();
               double prior1 = prior1_QString.toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior1(prior1))
               {
                  m_neosegParameters->setPrior1(prior1);    
               }
               else if(!prior1_QString.isEmpty())
               {
                  errors += " - 'Prior1' is not valid, it must be a positive number between 0 and 1\n";
               }   

               QString prior2_QString = (attributes.value("prior2")).toString();
               double prior2 = prior2_QString.toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior2(prior2))
               {
                  m_neosegParameters->setPrior2(prior2);    
               }
               else if(!prior2_QString.isEmpty())
               {
                  errors += " - 'Prior2' is not valid, it must be a positive number\n";
               }

               QString prior3_QString = (attributes.value("prior3")).toString();
               double prior3 = prior3_QString.toDouble(&ok);
               if( ok && m_neosegParameters->checkPrior3(prior3))
               {
                  m_neosegParameters->setPrior3(prior3);    
               }
               else if(!prior3_QString.isEmpty())
               {
                  errors += " - 'Prior3' is not valid, it must be a positive number\n";
               }

               QString prior4_QString = (attributes.value("prior4")).toString();
               double prior4 = prior4_QString.toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior4(prior4))
               {
                  m_neosegParameters->setPrior4(prior4);    
               }
               else if(!prior4_QString.isEmpty())
               {
                  errors += " - 'Prior4' is not valid, it must be a positive number\n";
               }

               QString prior5_QString = (attributes.value("prior5")).toString();
               double prior5 = prior5_QString.toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior5(prior5))
               {
                  m_neosegParameters->setPrior5(prior5);    
               }
               else if(!prior5_QString.isEmpty())
               {
                  errors += " - 'Prior5' is not valid, it must be a positive number\n";
               }
            }  

            else if (stream->name()=="Refinement")
            { 
               QString refinement_QString = (attributes.value("bool")).toString();
               bool refinement = refinement_QString.toInt(&ok); 
               if(ok && isBoolean(refinement))
               {
                  m_neosegParameters->setRefinement(refinement);    
               }
               else if(!refinement_QString.isEmpty())
               {
                  errors += " - 'Refinement' is not valid, it must be a boolean\n";
               }

               QString initialParzenKernelWidth_QString = (attributes.value("initial-parzen-kernel-width")).toString();
               double initialParzenKernelWidth = initialParzenKernelWidth_QString.toDouble(&ok); 
               if(ok && m_neosegParameters->checkInitialParzenKernelWidth(initialParzenKernelWidth))
               {
                  m_neosegParameters->setInitialParzenKernelWidth(initialParzenKernelWidth);    
               }
               else if(!initialParzenKernelWidth_QString.isEmpty())
               {
                  errors += " - 'Refinement-initial-parzen-kernel-width' is not valid, it must be a positive number\n";
               }
            }
         }
      }
      m_parameters->setSelectedAtlases(m_selectedAtlases); 

      return errors; 
   }
}


QString XmlReader::readExecutablesConfigurationFile(QString file_path)
{
   QFile* file = new::QFile(file_path);

   if(!file->exists())
   {
      return " - " + file_path + " does not exist\n"; 
   }
   else
   {
      file->open(QIODevice::ReadOnly);

      QXmlStreamReader* stream = new::QXmlStreamReader(file);

      QString errors;

      while(!stream->atEnd())
      {
         stream->readNext();

         if(stream->isStartElement())
         {
            QXmlStreamAttributes attributes = stream->attributes();
      
            if((stream->name()).toString()!="Executables")
            {
               QString name = (stream->name()).toString(); 
               QString path = (attributes.value("path")).toString(); 

               if(m_executablePaths->checkExecutablePath(name, path))
               {
                  m_executablePaths->setExecutablePath(name, path);    
               }
               else
               {
                  errors += " - " + name + " path is not valid\n"; 
               } 
            }
         }
      }

      return errors; 
   }
}

