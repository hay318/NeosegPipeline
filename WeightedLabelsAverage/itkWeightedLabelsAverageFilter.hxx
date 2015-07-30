/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Applications/CLI/DiffusionApplications/ResampleDTI/itkDiffusionTensor3DResample.txx $
  Language:  C++
  Date:      $Date: 2010-04-29 11:58:49 -0400 (Thu, 29 Apr 2010) $
  Version:   $Revision: 13073 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkWeightedLabelsAverageFilter_hxx
#define __itkWeightedLabelsAverageFilter_hxx

#include "itkWeightedLabelsAverageFilter.h"


namespace itk
{

   template <class InputImageType, class OutputImageType>
   WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::WeightedLabelsAverageFilter()
   {
     // Input 
     this->SetNumberOfRequiredInputs( 1 );

     m_ComputingWeights = true;
     m_RadiusValue = 1;
   }

   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::UpdateNumberOfOutputs(int n, InputImageRegionType inputRegion){

       this->SetNumberOfRequiredOutputs(n);

       // Outputs
       for(int i = 0; i < n; i++){
           this->SetNthOutput( i, this->MakeOutput(i) );

           OutputImagePointerType out = this->GetOutput(i);
           out->SetRegions(inputRegion);
           out->Allocate();
           out->FillBuffer(0);
       }
   }


   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::SetInput(InputImagePointerType inputImage)
   {
     this -> Superclass::SetInput(0, inputImage);
   }

   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType> 
   ::SetComputingWeightsOff()
   {
      m_ComputingWeights = false;
   }

   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::SetComputingWeightsOn()
   {
      m_ComputingWeights = true;
   }

   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::SetRadiusValueInMillimetersOn ()
   {
     m_radiusValueInMillimeters = true;
   }

   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::SetRadiusValueInMillimetersOff ()
   {
     m_radiusValueInMillimeters = false;
   }


   template <class InputImageType, class OutputImageType>
   void 
   WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::AddAtlas(InputImagePointerType image, InputImagePointerType labels)
   {
      Atlas atlas; 
      
      atlas.image = image;
      atlas.probabilistic = 0;
      atlas.labels = labels; 
      atlas.weights = NULL;

      m_AtlasPopulation.push_back(atlas);     
   }

   template <class InputImageType, class OutputImageType>
   void 
   WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::AddAtlas(InputImagePointerType image, InputImagePointerType white, InputImagePointerType gray, InputImagePointerType csf)
   {
      Atlas atlas; 
      
      atlas.image=image;
      atlas.probabilistic=1;
      atlas.white=white; 
      atlas.gray=gray; 
      atlas.csf=csf; 
      atlas.weights=NULL;

      m_AtlasPopulation.push_back(atlas);     
   }

   template <class InputImageType, class OutputImageType>
   void
   WeightedLabelsAverageFilter<InputImageType, OutputImageType>
   ::BeforeThreadedGenerateData()
   {
      // Input 
      m_inputImage = const_cast <InputImageType*> (this->GetInput(0));


      //This map contains a mapping between the label value and an index position
      //i.e., m_InputImageLabelIndexMap[ grayMatter ] = index0, where grayMatter label value could be X and the index0, i. The index will be
      //used to generate the outputs.

      m_InputImageLabelIndexMap.clear();
      int indexOfLabel = 0;

      for(unsigned i = 0; i < m_AtlasPopulation.size(); i++){ //We'll go through all the atlas population

          int currentIndexOfLabel = 0; //This index will be used to check if the current atlas has the same number of labels as the first atlas
          InputImageLabelIndexMapType currentInputImageLabelIndexMap;
          Atlas at = m_AtlasPopulation[i];
          InputImageIteratorType it(at.labels, at.labels->GetLargestPossibleRegion());
          it.GoToBegin();
          while(!it.IsAtEnd()){
              InputImagePixelType p = it.Get();
              if(p != 0){//Assumming 0 is background!!!
                  if(i == 0 && m_InputImageLabelIndexMap.find(p) == m_InputImageLabelIndexMap.end()){//We will check only for the first atlas, the rest of the atlas must have the same index (number) of labels
                      m_InputImageLabelIndexMap[p] = indexOfLabel;
                      indexOfLabel++;
                  }else if(currentInputImageLabelIndexMap.find(p) == currentInputImageLabelIndexMap.end()){
                      currentInputImageLabelIndexMap[p] = currentIndexOfLabel;
                      currentIndexOfLabel++;
                  }
              }
              ++it;
          }
          if( i != 0 && currentIndexOfLabel != indexOfLabel){//We check if this is valid, comparing the number of labels from the first atlas to the rest
              std::string s = "Different number of labels in ATLAS, files! " + std::to_string(i);
              throw s;
          }
      }

      //  Generate the number of outputs
      //  Allocate the outputs using the region of the input image. 
      //  Each output is index in the map by label
      UpdateNumberOfOutputs(indexOfLabel, m_inputImage->GetLargestPossibleRegion());//indexOfLabel is the number of labels found in the atlas. At this point, all the atlas have the same type of labels.

      // Outputs

//      m_white = this->GetOutput(0);
//      m_gray = this->GetOutput(1);
//      m_csf = this->GetOutput(2);

          
      if (m_ComputingWeights)
      {
         typedef LocalNormalizedCorrelationMetricFilter < InputImageType, WeightsImageType >       LocalMetricFilterType;
         typedef typename LocalMetricFilterType::Pointer                                   LocalMetricFilterPointerType;

         LocalMetricFilterPointerType localMetricFilter = LocalMetricFilterType::New();

         localMetricFilter->SetNumberOfThreads(1);
         localMetricFilter->SetInput(0, m_inputImage);
         localMetricFilter->SetRadiusValueInMillimetersOn();
         localMetricFilter->SetRadiusValue(m_RadiusValue);
         localMetricFilter->SetOutputParametersFromImage(m_inputImage);

         typedef ThresholdImageFilter < WeightsImageType >                                 ThresholdFilterType;
         typedef typename ThresholdFilterType::Pointer                                     ThresholdFilterPointerType;

         ThresholdFilterPointerType thresholdFilter = ThresholdFilterType::New();

         thresholdFilter->ThresholdOutside(0, 1);
         thresholdFilter->SetOutsideValue(0);

         typedef ImageFileWriter < WeightsImageType >                                       FileWriterType; 
         typedef typename FileWriterType::Pointer                                           FileWriterPointerType;

         FileWriterPointerType writer = FileWriterType::New(); 
         std::string name;

         typedef SpreadWeights < WeightsImagePixelType, WeightsImagePixelType >                             SpreadWeightsFunctorType; 
         typedef UnaryFunctorImageFilter < WeightsImageType, WeightsImageType, SpreadWeightsFunctorType >   SpreadWeightsFilterType;
         typedef typename SpreadWeightsFilterType::Pointer                                                  SpreadWeightsFilterPointerType;

         SpreadWeightsFilterPointerType spreadWeightsFilter = SpreadWeightsFilterType::New();

         for(unsigned int i=0; i<m_AtlasPopulation.size(); i++)
         {
            localMetricFilter->SetInput(1, m_AtlasPopulation[i].image);
            localMetricFilter->Update();

            thresholdFilter->SetInput(localMetricFilter->GetOutput());
            thresholdFilter->Update();

            spreadWeightsFilter->SetInput( thresholdFilter->GetOutput() );
            spreadWeightsFilter->Update(); 

            WeightsImagePointerType output = spreadWeightsFilter->GetOutput();
            output->DisconnectPipeline();

            (m_AtlasPopulation[i]).weights = output;
         } 
      }

      else
      {
         WeightsImagePointerType weights = WeightsImageType::New(); 
         typename OutputImageType::RegionType region;
         region = m_inputImage->GetLargestPossibleRegion();     
         weights -> SetRegions( region );
         weights -> Allocate();
         weights -> FillBuffer(1);  

         for(unsigned int i=0; i<m_AtlasPopulation.size(); i++)
         {
            m_AtlasPopulation[i].weights = weights;    
         }
      }
   }


   template <class InputImageType, class OutputImageType>
   void WeightedLabelsAverageFilter <InputImageType, OutputImageType>
   ::ThreadedGenerateData (const OutputImageRegionType &outputRegionForThread, ThreadIdType itkNotUsed(threadId))
   {
      IteratorType it( m_inputImage, outputRegionForThread );

      OutputImageIndexType       index;

//      std::vector<double> labels(4,0.0);
      typedef std::map< InputImagePixelType, double > MapLabelsType;
      MapLabelsType labels;

      Atlas atlas;
      
      int label;

      double white;
      double gray; 
      double csf;

      double weight;
      double sumWeights = 0;

      long progress = 0;

      it.GoToBegin();  
      while( !it.IsAtEnd() )
      { 
         // Index
         index = it.GetIndex();

         // Reinitialize labels
         typename InputImageLabelIndexMapType::iterator labelindexmapit;
         for(labelindexmapit = m_InputImageLabelIndexMap.begin(); labelindexmapit != m_InputImageLabelIndexMap.end(); ++labelindexmapit){
          labels[labelindexmapit->first] = 0;//Initialize all the existing labels to 0 for this iteration
         }

         sumWeights = 0;

         if( ( m_inputImage->GetPixel( index ) ) != 0 )  // Inside the brain
         {   
            for(unsigned int i=0; i<m_AtlasPopulation.size(); i++)
            { 
               atlas = m_AtlasPopulation[i];
               weight = (atlas.weights)->GetPixel( index ); //

               if( atlas.probabilistic )
               {
                  white = (atlas.white)->GetPixel( index );
                  labels[1] = white*weight;
 
                  gray = (atlas.gray)->GetPixel( index ); 
                  labels[2] = gray*weight;

                  csf = (atlas.csf)->GetPixel( index ); 
                  labels[3] = csf*weight;
               }
               
               else
               {
                  label = (atlas.labels)->GetPixel( index );
                  labels[label] += weight;
               }
            }

            typename MapLabelsType::iterator itlabels;
            for(itlabels = labels.begin(); itlabels != labels.end(); ++itlabels){
                sumWeights += itlabels->second;
            }

            if (sumWeights!=0){
              for(labelindexmapit = m_InputImageLabelIndexMap.begin(); labelindexmapit != m_InputImageLabelIndexMap.end(); ++labelindexmapit){
                double r = labels[labelindexmapit->first]/sumWeights;
                int outputIndex = labelindexmapit->second;//Using the label value, retrieve the output index
                this->GetOutput(outputIndex)->SetPixel(index, r);//Change the output pixel to the normalized weighted value of r
              }
            }
         }

         // if ( progress % 500000 == 0 )
         // {
         //     std::cout<<"Avg At index:"<<index<<" - "<<std::endl;
         // } 

         ++it;
         ++progress;
     }
   }

   template <class InputImageType, class OutputImageType>
   void
   WeightedLabelsAverageFilter<InputImageType, OutputImageType>
   ::GenerateOutputInformation()
   {
     Superclass::GenerateOutputInformation();
     OutputImagePointerType outputPointer = this->GetOutput( 0 );

     if( !outputPointer )
     {
       return;
     }

     //OutputImageRegionType outputRegion;
     //outputRegion.SetSize( m_OutputSize );
     //OutputImageIndexType index;
     //index.Fill( 0 );

     //outputRegion.SetIndex( index );
     //outputPointer->SetRegions( outputRegion );
     //outputPointer->SetSpacing( m_OutputSpacing );
     //outputPointer->SetOrigin( m_OutputOrigin );
     //outputPointer->SetDirection( m_OutputDirection );

     return;
   }


   template <class InputImageType, class OutputImageType>
   void
   WeightedLabelsAverageFilter<InputImageType, OutputImageType>
   ::GenerateInputRequestedRegion()
   {
     Superclass::GenerateInputRequestedRegion();

     if( !(this->GetInput()) )
     {
       return;
     }

     // get pointers to the input and output
     InputImagePointerType inputPointer = const_cast<InputImageType *>( this->GetInput() );

     // Request the entire input image
     typename InputImageType::RegionType inputRegion;
     inputRegion = inputPointer->GetLargestPossibleRegion();
     inputPointer->SetRequestedRegion( inputRegion );

     return;
   }

} // end namespace itk
#endif

