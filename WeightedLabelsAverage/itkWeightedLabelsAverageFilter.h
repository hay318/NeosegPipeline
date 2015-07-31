/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Applications/CLI/DiffusionApplications/ResampleDTI/itkDiffusionTensor3DResample.h $
  Language:  C++
  Date:      $Date: 2010-06-28 07:45:15 -0400 (Mon, 28 Jun 2010) $
  Version:   $Revision: 13964 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkWeightedLabelsAverageFilter_h
#define __itkWeightedLabelsAverageFilter_h

// General Librairies // 
#include <vector>
#include <string>
#include <map>

// ITK Librairies //
#include "itkImageToImageFilter.h"
#include "itkLocalNormalizedCorrelationMetricFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkSpreadWeights.h"  
#include "itkUnaryFunctorImageFilter.h"

namespace itk
{
   template <class InputImageType, class OutputImageType>
   class WeightedLabelsAverageFilter : public ImageToImageFilter
   < InputImageType, OutputImageType >
   {
      public:

      typedef ImageToImageFilter < InputImageType, OutputImageType >       Superclass;

      typedef WeightedLabelsAverageFilter                                  Self;
      typedef SmartPointer<Self>                                           Pointer;
      typedef SmartPointer<const Self>                                     ConstPointer;

      typedef typename InputImageType::Pointer                             InputImagePointerType;
      typedef typename OutputImageType::Pointer                            OutputImagePointerType;

      typedef typename InputImageType::RegionType                          InputImageRegionType;
      typedef typename OutputImageType::RegionType                         OutputImageRegionType;

      typedef typename InputImageType::SizeType                            InputImageSizeType;
      typedef typename OutputImageType::SizeType                           OutputImageSizeType;

      typedef typename InputImageType::IndexType                           InputImageIndexType;
      typedef typename OutputImageType::IndexType                          OutputImageIndexType;

      typedef typename InputImageType::PixelType                           InputImagePixelType;
      typedef typename OutputImageType::PixelType                          OutputImagePixelType;

      typedef ImageRegionIterator<OutputImageType>                         IteratorType;
      typedef ImageRegionIterator<InputImageType>                          InputImageIteratorType;

      typedef Image <double, 3>                                            WeightsImageType; 
      typedef typename WeightsImageType::Pointer                           WeightsImagePointerType; 
      typedef typename WeightsImageType::PixelType                         WeightsImagePixelType; 
      
      itkNewMacro( Self );

      using ImageToImageFilter<InputImageType, OutputImageType>::SetInput; 
      void SetInput( InputImagePointerType inputImage );

      void AddAtlas( InputImagePointerType image, InputImagePointerType labels );
      void AddAtlas( InputImagePointerType image, InputImagePointerType white, InputImagePointerType gray, InputImagePointerType csf );

      void SetComputingWeightsOn( );
      void SetComputingWeightsOff( );

      void SetRadiusValueInMillimetersOn( );
      void SetRadiusValueInMillimetersOff( );
 
      itkSetMacro( RadiusValue, double );
      itkGetMacro( RadiusValue, double );

      typedef std::map< InputImagePixelType, int > InputImageLabelIndexMapType;

      InputImageLabelIndexMapType GetInputImagePixelType(){
        return m_InputImageLabelIndexMap;
      }

   protected:

     WeightedLabelsAverageFilter();
     void BeforeThreadedGenerateData();
     void ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId );
     void GenerateOutputInformation();
     void GenerateInputRequestedRegion();
     void UpdateNumberOfOutputs(int n, InputImageRegionType inputRegion);

private:

      //Input 
      InputImagePointerType                m_inputImage;

      
      InputImageLabelIndexMapType m_InputImageLabelIndexMap;

      // Outputs
//      OutputImagePointerType               m_white;
//      OutputImagePointerType               m_gray;
//      OutputImagePointerType               m_csf;

      // Weights 
      bool                                 m_ComputingWeights;
      bool                                 m_radiusValueInMillimeters;
      double                               m_RadiusValue;        

      // Atlas
      struct Atlas
      {
         InputImagePointerType             image; 
         WeightsImagePointerType           weights; 
         bool                              probabilistic;
         InputImagePointerType             labels; 
         InputImagePointerType             white; 
         InputImagePointerType             gray; 
         InputImagePointerType             csf; 

      };

      // Atlas Population 
      std::vector<Atlas>                   m_AtlasPopulation;

};

} // end namespace itk
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWeightedLabelsAverageFilter.hxx"
#endif

#endif
