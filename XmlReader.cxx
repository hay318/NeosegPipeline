#include "XmlReader.h" 

XmlReader::XmlReader()
{
   m_errors = ""; 
}

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


void XmlReader::readParametersConfigurationFile(QString file_path)
{
   QFile* file = new::QFile(file_path);

   if(!file->exists())
   {
      std::cout<<file_path.toStdString()<<" does not exist"<<std::endl;  
   }

   else
   {
      file->open(QIODevice::ReadOnly);

      QXmlStreamReader* stream = new::QXmlStreamReader(file);

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
                  m_errors += "'New-atlas-computation' is not correct";
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
                  m_errors += "'Atlas-population-directory' is not correct";
               }
            }

            else if (stream->name() == "Atlas")
            {
                QString atlas = (attributes.value("name")).toString(); 
                m_selectedAtlases += atlas;
            }

            else if (stream->name() == "Smoothing")
            {
               QString smoothing = (attributes.value("type")).toString();
               if(m_parameters->checkSmoothing(smoothing))
               {
                  m_parameters->setSmoothing(smoothing);
               }
               else 
               {
                  m_errors += "'Smoothing-type' is not correct";
               }

               double smoothingSize = ((attributes.value("size")).toString()).toDouble(&ok);
               if(ok || m_parameters->checkSmoothingSize(smoothingSize))
               {
                  m_parameters->setSmoothingSize(smoothingSize);
               }
               else 
               {
                  m_errors += "'Smoothing-size' is not correct";
               }
            }

            else if (stream->name() == "Weights-computation")
            {
               bool computingWeights = ((attributes.value("bool")).toString()).toInt(&ok);
               if(ok && isBoolean(computingWeights))
               {
                  m_parameters->setComputingWeights(computingWeights);
               }
               else 
               {
                  m_errors += "'Weights-computation' is not correct";
               }
            }

            else if (stream->name() == "Weights-parameters")
            {
               double weightsRadius = ((attributes.value("radius")).toString()).toDouble(&ok);
               if(ok && m_parameters->checkWeightsRadius(weightsRadius))
               {
                  m_parameters->setWeightsRadius(weightsRadius);
               }
               else 
               {
                  m_errors += "'Weights-radius' is not correct";
               }

               QString weightsModality = (attributes.value("modality")).toString(); 
               if(m_parameters->checkWeightsModality(weightsModality))
               {
                  m_parameters->setWeightsModality(weightsModality);
               }
               else 
               {
                  m_errors += "'Weights-modality' is not correct";
               }
            }

            else if (stream->name() == "Including-FA")
            {
               double includingFA = ((attributes.value("bool")).toString()).toInt(&ok);
               if(ok && isBoolean(includingFA))
               {
                  m_parameters->setIncludingFA(includingFA);
               }
               else 
               {
                  m_errors += "'Including-FA' is not correct";
               }
            }

            else if (stream->name() == "Neoseg-images")
            {
               std::cout<<"Neoseg-images"<<std::endl;

               double usingFA = ((attributes.value("using-FA")).toString()).toInt(&ok);
               if(ok && isBoolean(usingFA))
               {
                  std::cout<<"Using-FA :"<<usingFA<<std::endl;
                  m_parameters->setUsingFA(usingFA);
               }
               else 
               {
                  m_errors += "'using-FA' is not correct";
               }

               double usingAD = ((attributes.value("using-AD")).toString()).toInt(&ok);
               if(ok && isBoolean(usingAD))
               {
                  std::cout<<"Using-AD :"<<usingAD<<std::endl;
                  m_parameters->setUsingAD(usingAD);
               }
               else 
               {
                  m_errors += "'using-AD' is not correct";
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
                  m_errors += "'Overwriting' is not correct";
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
                  m_errors += "'Cleaning-up' is not correct";
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
                  m_errors += "'Computing-system' is not correct";
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
                  m_errors += "'Number-of-cores' is not correct";
               }
            }

            else if (stream->name() == "First-modality")
            {
               QString imageMetric = (attributes.value("metric")).toString(); 
               if(m_antsParameters->checkImageMetric1(imageMetric))   
               {
                  m_antsParameters->setImageMetric1(imageMetric);
               }
               else 
               {
                  m_errors += "'First-modality-metric' is not correct";
               }

               int radius = ((attributes.value("radius")).toString()).toInt(&ok);
               if(ok && m_antsParameters->checkRadius1(radius))
               {
                  m_antsParameters->setRadius1(radius);
               }
               else 
               {
                  m_errors += "'First-modality-radius' is not correct";
               }
      
               double weight = ((attributes.value("weight")).toString()).toDouble(&ok); 
               if(ok && m_antsParameters->checkWeight1(weight))
               {
                  m_antsParameters->setWeight1(weight);
               }
               else 
               {
                  m_errors += "'First-modality-weight' is not correct";
               }
            }

            else if (stream->name() == "Second-modality")
            {
               QString imageMetric = (attributes.value("metric")).toString(); 
               if(m_antsParameters->checkImageMetric2(imageMetric))   
               {
                  m_antsParameters->setImageMetric2(imageMetric);
               }
               else 
               {
                  m_errors += "'Second-modality-metric' is not correct";
               }

               int radius = ((attributes.value("radius")).toString()).toInt(&ok);
               if(ok && m_antsParameters->checkRadius2(radius))
               {
                  m_antsParameters->setRadius2(radius);
               }
               else 
               {
                  m_errors += "'Second-modality-radius' is not correct";
               }
      
               double weight = ((attributes.value("weights")).toString()).toDouble(&ok); 
               if(ok && m_antsParameters->checkWeight2(weight))
               {
                  m_antsParameters->setWeight2(weight);
               }
               else 
               {
                  m_errors += "'Second-modality-weight' is not correct";
               }
            }

            else if (stream->name() == "Iterations")
            {
               int iterationsJ = ((attributes.value("J")).toString()).toInt(&ok);
               if(ok && m_antsParameters->checkIterationsJ(iterationsJ))  
               {
                  m_antsParameters->setIterationsJ(iterationsJ); 
               }
               else 
               {
                  m_errors += "'Iterations-J' is not correct";
               }

               int iterationsK = ((attributes.value("K")).toString()).toInt(&ok);
               if(ok && m_antsParameters->checkIterationsK(iterationsK))
               {
                  m_antsParameters->setIterationsK(iterationsK);
               }
               else 
               {
                  m_errors += "'Iterations-K' is not correct";
               }
            
               int iterationsL = ((attributes.value("L")).toString()).toInt(&ok);
               if(ok && m_antsParameters->checkIterationsL(iterationsL))
               {
                  m_antsParameters->setIterationsL(iterationsL);            
               }
               else 
               {
                  m_errors += "'Iterations-L' is not correct";
               }
            }

            else if (stream->name() == "Transformation-type")
            {
               QString type = (attributes.value("name")).toString(); 
               if(m_antsParameters->checkTransformationType(type))
               {             
                  m_antsParameters->setTransformationType(type);        
               }
               else 
               {
                  m_errors += "'Transformation-type' is not correct";
               }
            }

            else if (stream->name() == "Transformation-parameters")
            {
               double gradientStepLength = ((attributes.value("gradient-step-length")).toString()).toDouble(&ok);
               if(ok && m_antsParameters->checkGradientStepLength(gradientStepLength))
               {
                  m_antsParameters->setGradientStepLength(gradientStepLength);
               }
               else 
               {
                  m_errors += "'Transformation-gradient-step-length' is not correct";
               }

               double numberOfTimeSteps = ((attributes.value("number-of-time-steps")).toString()).toDouble(&ok);
               if(ok && m_antsParameters->checkNumberOfTimeSteps(numberOfTimeSteps))
               {
                  m_antsParameters->setNumberOfTimeSteps(numberOfTimeSteps); 
               }
               else 
               {
                  m_errors += "'Transformation-number-of-time-steps' is not correct";
               }

               double deltaTime = ((attributes.value("delta-time")).toString()).toDouble(&ok);
               if(ok && m_antsParameters->checkDeltaTime(deltaTime))
               {
                  m_antsParameters->setDeltaTime(deltaTime);      
               }
               else 
               {
                  m_errors += "'Transformation-delta-time' is not correct";
               }
            }

            else if (stream->name() == "Regularization-type")
            {
               QString type = (attributes.value("name")).toString();  
               if(m_antsParameters->checkRegularizationType(type)) 
               {              
                  m_antsParameters->setRegularizationType(type);
               }    
               else 
               {
                  m_errors += "'Regularization-type' is not correct";
               }    
            }   
            
            else if (stream->name() == "Regularization-parameters")
            { 
               double gradientFieldSigma = ((attributes.value("gradient-field-sigma")).toString()).toDouble(&ok);
               if(ok && m_antsParameters->checkGradientFieldSigma(gradientFieldSigma))
               {
                  m_antsParameters->setGradientFieldSigma(gradientFieldSigma);
               }
               else 
               {
                  m_errors += "'Regularization-gradient-field-sigma' is not correct";
               }   

               double deformationFieldSigma = ((attributes.value("deformation-field-sigma")).toString()).toDouble(&ok);
               if(ok && m_antsParameters->checkDeformationFieldSigma(deformationFieldSigma))
               {
                  m_antsParameters->setDeformationFieldSigma(deformationFieldSigma);             
               }
               else 
               {
                  m_errors += "'Regularization-deformation-field-sigma' is not correct";
               }     
            }

            else if (stream->name()=="Smoothing-type")
            { 
               QString type=(attributes.value("name")).toString(); 
               if(m_neosegParameters->checkFilterMethod(type))
               {  
                  m_neosegParameters->setFilterMethod(type);    
               }
               else 
               {
                  m_errors += "'Smoothing-type' is not correct";
               }   
            }

            else if (stream->name()=="Smoothing-parameters")
            { 
               int iterations=((attributes.value("iterations")).toString()).toInt(&ok);
               if(ok && m_neosegParameters->checkNumberOfIterations(iterations))
               { 
                  m_neosegParameters->setNumberOfIterations(iterations);   
               }             
               else 
               {
                  m_errors += "'Smoothing-iterations' is not correct";
               }   

               double timeStep=((attributes.value("time-step")).toString()).toDouble(&ok);
               if(ok && m_neosegParameters->checkTimeStep(timeStep))
               {
                  m_neosegParameters->setTimeStep(timeStep);   
               } 
               else 
               {
                  m_errors += "'Smoothing-time-step' is not correct";
               }           
            }

            else if (stream->name()=="Reference-image")
            { 
               QString referenceImage = (attributes.value("name")).toString(); 
               if(m_neosegParameters->checkReferenceImage(referenceImage))
               {
                  m_neosegParameters->setReferenceImage(referenceImage);    
               }
               else 
               {
                  m_errors += "'Reference-image' is not correct";
               }    
            }

            else if (stream->name()=="Prior-threshold")
            { 
               double priorThreshold=(attributes.value("value")).toString().toDouble(&ok); 
               if(ok && m_neosegParameters->checkPriorThreshold(priorThreshold))
               {
                  m_neosegParameters->setPriorThreshold(priorThreshold);    
               }
               else 
               {
                  m_errors += "'Prior-threshold' is not correct";
               }   
            }

            else if (stream->name()=="Max-bias-degree")
            { 
               int maxBiasDegree=(attributes.value("value")).toString().toInt(&ok); 
               if(ok && m_neosegParameters->checkMaxBiasDegree(maxBiasDegree))
               {
                  m_neosegParameters->setMaxBiasDegree(maxBiasDegree);    
               }
               else 
               {
                  m_errors += "'Max-bias-degree' is not correct";
               }   
            }

            else if (stream->name()=="Priors")
            { 
               double prior1=(attributes.value("prior1")).toString().toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior1(prior1))
               {
                  m_neosegParameters->setPrior1(prior1);    
               }
               else 
               {
                  m_errors += "'Prior1' is not correct";
               }   

               double prior2=(attributes.value("value")).toString().toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior2(prior2))
               {
                  m_neosegParameters->setPrior2(prior2);    
               }
               else 
               {
                  m_errors += "'Prior2' is not correct";
               }

               double prior3=(attributes.value("value")).toString().toDouble(&ok);
               if( ok && m_neosegParameters->checkPrior3(prior3))
               {
                  m_neosegParameters->setPrior3(prior3);    
               }
               else 
               {
                  m_errors += "'Prior3' is not correct";
               }

               double prior4=(attributes.value("value")).toString().toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior4(prior4))
               {
                  m_neosegParameters->setPrior4(prior4);    
               }
               else 
               {
                  m_errors += "'Prior4' is not correct";
               }

               double prior5=(attributes.value("value")).toString().toDouble(&ok);
               if(ok && m_neosegParameters->checkPrior5(prior5))
               {
                  m_neosegParameters->setPrior5(prior5);    
               }
               else 
               {
                  m_errors += "'Prior5' is not correct";
               }
            }  

            else if (stream->name()=="Refinement")
            { 
               bool refinement=(attributes.value("bool")).toString().toInt(&ok); 
               if(ok && isBoolean(refinement))
               {
                  m_neosegParameters->setRefinement(refinement);    
               }
               else 
               {
                  m_errors += "'Refinement' is not correct";
               }
            }

            else if (stream->name()=="Refinement-parameters")
            { 
               double initialParzenKernelWidth=(attributes.value("initial-parzen-kernel-width")).toString().toDouble(&ok); 
               if(ok && m_neosegParameters->checkInitialParzenKernelWidth(initialParzenKernelWidth))
               {
                  m_neosegParameters->setInitialParzenKernelWidth(initialParzenKernelWidth);    
               }
               else 
               {
                  m_errors += "'Refinement-initial-parzen-kernel-width' is not correct";
               }
            }
         }
      }
   }

   QStringList::const_iterator it;
   int i=0;
   
   for (it = m_selectedAtlases.constBegin(); it != m_selectedAtlases.constEnd(); ++it) 
   {
      if(!m_parameters->checkAtlas(*it))
      {
         m_selectedAtlases.removeAt(i);
      }
      i++;
   }

   m_parameters->setSelectedAtlases(m_selectedAtlases); 
}


void XmlReader::readExecutablesConfigurationFile(QString file_path)
{
   QFile* file = new::QFile(file_path);

   if(file->exists())
   {
      std::cout<<file_path.toStdString()<<" does not exist"<<std::endl; 
   }
   else
   {
      file->open(QIODevice::ReadOnly);

      QXmlStreamReader* stream = new::QXmlStreamReader(file);

      while(!stream->atEnd())
      {
         stream->readNext();

         if (stream->isStartElement())
         {
            QXmlStreamAttributes attributes = stream->attributes();
      
            QString name = (stream->name()).toString(); 
            QString path = (attributes.value("path")).toString(); 

            if(m_executablePaths->checkExecutablePath(path))
            {
               m_executablePaths->setExecutablePath(name, path);    
            }
         }
      }
   }
}

