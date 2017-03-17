/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Applications/CLI/DiffusionApplications/ResampleDTI/itkDiffusionTensor3DResample.txx $
  Language:  C++
  Date:      $Date: 2010-04-29 11:58:49 -0400 (Thu, 29 Apr 2010) $
  Version:   $Revision: 13073 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkLocalNormalizedCorrelationMetricFilter_hxx
#define __itkLocalNormalizedCorrelationMetricFilter_hxx

#include "itkLocalNormalizedCorrelationMetricFilter.h"

#include "itkNearestNeighborInterpolateImageFunction.h"

namespace itk
{

   template <class TInput, class TOutput>
   LocalNormalizedCorrelationMetricFilter <TInput, TOutput>
   ::LocalNormalizedCorrelationMetricFilter()
   {
     this->SetNumberOfRequiredInputs( 2 );

     m_OutputSpacing.Fill( 1.0 );
     m_OutputOrigin.Fill( 0.0 );
     m_OutputDirection.SetIdentity();
     m_OutputSize.Fill( 0 );

     m_radiusValueInMillimeters = true;
     m_InputImageMask = 0;
   }


   template <class TInput, class TOutput>
   void LocalNormalizedCorrelationMetricFilter <TInput, TOutput>
   ::SetInput(int index, InputImagePointerType inputImage)
   {
     this -> Superclass::SetInput(index, inputImage);
   }

   template <class TInput, class TOutput>
   void LocalNormalizedCorrelationMetricFilter <TInput, TOutput>
   ::SetRadiusValueInMillimetersOn ()
   {
     m_radiusValueInMillimeters = true;
   }

   template <class TInput, class TOutput>
   void LocalNormalizedCorrelationMetricFilter <TInput, TOutput>
   ::SetRadiusValueInMillimetersOff ()
   {
     m_radiusValueInMillimeters = false;
   }


   template <class TInput, class TOutput>
   void LocalNormalizedCorrelationMetricFilter <TInput, TOutput>
   ::SetRadiusValue ( double radiusValue )
   {
     m_radiusValue = radiusValue;
   }



   template <class TInput, class TOutput>
   void
   LocalNormalizedCorrelationMetricFilter<TInput, TOutput>
   ::SetOutputParametersFromImage( InputImagePointerType Image )
   {
     typename TOutput::RegionType region;
     region = Image->GetLargestPossibleRegion();
     m_OutputSize = region.GetSize();
     m_OutputSpacing = Image->GetSpacing();
     m_OutputDirection = Image->GetDirection();
     m_OutputOrigin = Image->GetOrigin();
   }

   template <class TInput, class TOutput>
   void
   LocalNormalizedCorrelationMetricFilter<TInput, TOutput>
   ::BeforeThreadedGenerateData()
   {
      
      // Inputs 
      m_fixedImage = const_cast <TInput*> (this->GetInput(0));
      m_movingImage = const_cast <TInput*> (this->GetInput(1));

      // Radius Vector
      if( m_radiusValueInMillimeters )
      {
         Vector <double, 3> spacing = m_fixedImage->GetSpacing();
         m_radiusVector = round(m_radiusValue/spacing[0]);
         m_radiusVector = round(m_radiusValue/spacing[1]);
         m_radiusVector = round(m_radiusValue/spacing[2]);
      }

      else 
      {
         m_radiusVector.Fill(m_radiusValue);
      }

      // Neighborhood 
      m_patchSize[0] = 2*m_radiusVector[0] + 1;
      m_patchSize[1] = 2*m_radiusVector[1] + 1;
      m_patchSize[2] = 2*m_radiusVector[2] + 1;

      m_region.SetSize(m_patchSize);
 
      // Transform
      typedef TranslationTransform < double , 3 >                              TransformType;
      typedef typename TransformType::Pointer                                  TransformPointerType;
      TransformPointerType transform = TransformType::New();

      // Interpolator 
      typedef NearestNeighborInterpolateImageFunction <TInput, double>         InterpolatorType;
      typedef typename InterpolatorType::Pointer                               InterpolatorPointerType;
      InterpolatorPointerType interpolator = InterpolatorType::New();
      interpolator -> SetInputImage (m_fixedImage);

      // Metric
      m_metric = MetricType::New();

      m_metric -> SetFixedImage( m_fixedImage );
      m_metric -> SetMovingImage( m_movingImage );
      m_metric -> SetTransform( transform );
      m_metric -> SetInterpolator( interpolator );
      m_metric -> ComputeGradientOff( );
      m_metric -> UseAllPixelsOn( );

      // Parameters
      m_params.SetSize(3);
      m_params.Fill(0);

   }

   template <class TInput, class TOutput>
   double 
   LocalNormalizedCorrelationMetricFilter <TInput, TOutput> 
   ::GetMetric (InputImageRegionType &region)
   {
      m_metric -> SetFixedImageRegion( region );
      m_metric -> Initialize();

      return m_metric->GetValue(m_params) * (-1);
   }

   template <class TInput, class TOutput>
   double 
   LocalNormalizedCorrelationMetricFilter <TInput, TOutput> 
   ::GetMetricNoITK (InputImageRegionType &region)
   {
     // for speed do not use ITK NCC as we do not need transform and interpolation
     // for speed do not compute region at image edge, i.e. NCC region has to be fully in image

     //check lower bound, give equal weight
     if ((m_start[0] < 0) || (m_start[1] < 0) || (m_start[2] < 0)) {
       return 1.0;
     }
     //check upper bound, give equal weight
     m_end[0] = m_start[0]+m_patchSize[0];
     m_end[1] = m_start[1]+m_patchSize[1];
     m_end[2] = m_start[2]+m_patchSize[2];
     if ((m_end[0] >= (unsigned int) m_OutputSize[0]) || 
	 (m_end[1] >= (unsigned int) m_OutputSize[1]) || 
	 (m_end[2] >= (unsigned int) m_OutputSize[2])) {
       return 1.0;
     }
      InputIteratorType ti( m_fixedImage, region );
      InputIteratorType mi( m_movingImage, region );
      double sff = 0;
      double smm = 0;
      double sfm = 0;
      double measure = 0;

      while( !ti.IsAtEnd() ) {

	double fixedValue  = ti.Get();
	double movingValue = mi.Get();
	sff += fixedValue  * fixedValue;
	smm += movingValue * movingValue;
	sfm += fixedValue  * movingValue;

	++ti;
	++mi;
      }
      double denom = -1.0 * std::sqrt(sff * smm);
      if(  denom != 0.0 ) {
	measure = sfm / denom;
      }
      return measure * (-1);
   }
   

   template <class TInput, class TOutput>
   void LocalNormalizedCorrelationMetricFilter <TInput, TOutput>
   ::ThreadedGenerateData (const OutputImageRegionType &outputRegionForThread, ThreadIdType itkNotUsed(threadId))
   {
      
      // Inputs

      InputIteratorType           fixedit( m_fixedImage, outputRegionForThread );
      InputIteratorType           movingit( m_movingImage, outputRegionForThread );

      InputIteratorType           maskit;

      if(m_InputImageMask){
          maskit = InputIteratorType( m_InputImageMask, outputRegionForThread );
          maskit.GoToBegin();
      }
      


      // Output
      OutputImagePointerType outputImagePtr = this->GetOutput( 0 );
      IteratorType           outit( outputImagePtr, outputRegionForThread );
      OutputImageIndexType index;

      double metricValue=0;

      // Progress
      //long progress = 0;

      outit.GoToBegin();
      fixedit.GoToBegin();
      movingit.GoToBegin();
      //std::cout <<"Computing NCC" << std::endl;

      while( !outit.IsAtEnd() )
      {
         index = outit.GetIndex();
         bool computemetric = !(m_InputImageMask && maskit.Get() == 0);
         metricValue=0;

         if(computemetric){
             // Regions
             m_start[0]=index[0] - m_radiusVector[0];
             m_start[1]=index[1] - m_radiusVector[1];
             m_start[2]=index[2] - m_radiusVector[2];

             m_region.SetIndex(m_start);

	     
             metricValue = GetMetricNoITK(m_region);

             //double metricValue2 = GetMetric(m_region);
	     //if (metricValue != 0)  std::cout<<metricValue<<"," << metricValue2<<std::endl;
             //if (progress % 500000 == 0)
             //{
             //  std::cout<<"Correl At index "<<index<<": fixed region="<<m_fixedImage->GetPixel(index)<<", moving region="<<m_movingImage->GetPixel(index)<<", metric="<<metricValue<<std::endl;
             //}

         }

         outit.Set(metricValue);


         //++progress ;
         ++outit;
         ++fixedit;
         ++movingit;

         if(m_InputImageMask){
             ++maskit;
         }
      }

   }


   /**
    * Inform pipeline of required output region
    */
   template <class TInput, class TOutput>
   void
   LocalNormalizedCorrelationMetricFilter<TInput, TOutput>
   ::GenerateOutputInformation()
   {
     // call the superclass' implementation of this method
     Superclass::GenerateOutputInformation();
     // get pointers to the input and output
     OutputImagePointerType outputPtr = this->GetOutput( 0 );

     if( !outputPtr )
       {
       return;
       }

     outputPtr->SetSpacing( m_OutputSpacing );
     outputPtr->SetOrigin( m_OutputOrigin );
     outputPtr->SetDirection( m_OutputDirection );

     // Set the size of the output region
     typename TOutput::RegionType outputLargestPossibleRegion;
     outputLargestPossibleRegion.SetSize( m_OutputSize );

     typename TOutput::IndexType index;
     for( int i = 0; i < 3; i++ )
     {
        index[i] = 0;
     }
     outputLargestPossibleRegion.SetIndex (index);

     outputPtr -> SetRegions(outputLargestPossibleRegion);
     return;
   }


   template <class TInput, class TOutput>
   void
   LocalNormalizedCorrelationMetricFilter<TInput, TOutput>
   ::GenerateInputRequestedRegion()
   {
     // call the superclass's implementation of this method
     Superclass::GenerateInputRequestedRegion();

     if( !(this->GetInput(0) && this->GetInput(1)) )
       {
       return;
       }

     // get pointers to the input and output
     InputImagePointerType input1Pointer = const_cast<TInput *>( this->GetInput(0) );

     // Request the entire input image
     typename TInput::RegionType input1Region;
     input1Region = input1Pointer->GetLargestPossibleRegion();
     input1Pointer->SetRequestedRegion( input1Region );


     // get pointers to the input and output
     InputImagePointerType input2Pointer = const_cast<TInput *>( this->GetInput(1) );

     // Request the entire input image
     typename TInput::RegionType input2Region;
     input2Region = input2Pointer->GetLargestPossibleRegion();
     input2Pointer->SetRequestedRegion( input2Region );
     return;
   }


} // end namespace itk
#endif


