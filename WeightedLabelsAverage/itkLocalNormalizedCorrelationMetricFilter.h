/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer3/trunk/Applications/CLI/DiffusionApplications/ResampleDTI/itkDiffusionTensor3DResample.h $
  Language:  C++
  Date:      $Date: 2010-06-28 07:45:15 -0400 (Mon, 28 Jun 2010) $
  Version:   $Revision: 13964 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkLocalNormalizedCorrelationMetricFilter_h
#define __itkLocalNormalizedCorrelationMetricFilter_h


#include "itkImageToImageFilter.h"

// Metric
#include "itkTranslationTransform.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"

#include <itkVector.h>


namespace itk
{

template <class TInput, class TOutput>
class LocalNormalizedCorrelationMetricFilter : public ImageToImageFilter
   < TInput, TOutput >
{

public:

  typedef ImageToImageFilter < TInput, TOutput >                   Superclass;
  typedef LocalNormalizedCorrelationMetricFilter                   Self;
  typedef SmartPointer<Self>                                       Pointer;
  typedef SmartPointer<const Self>                                 ConstPointer;

  //typedef TInput  InputImageType;
  //typedef TOutput OutputImageType;

  typedef typename TInput::Pointer                                 InputImagePointerType;
  typedef typename TOutput::Pointer                                OutputImagePointerType;

  typedef typename TInput::RegionType                              InputImageRegionType;
  typedef typename TOutput::RegionType                             OutputImageRegionType;

  typedef typename TInput::SizeType                                InputImageSizeType;
  typedef typename TOutput::SizeType                               OutputImageSizeType;

  typedef typename TInput::IndexType                               InputImageIndexType;
  typedef typename TOutput::IndexType                              OutputImageIndexType;

  typedef typename TInput::PixelType                               InputImagePixelType;
  typedef typename TOutput::PixelType                              OutputImagePixelType;

  typedef TranslationTransform < double , 3 >                      TransformType;
  typedef typename TransformType::Pointer                          TransformPointerType;
  typedef typename TransformType::ParametersType                   TransformParametersType;

  typedef NormalizedCorrelationImageToImageMetric <TInput,TInput>  MetricType;
  typedef typename MetricType::Pointer                             MetricPointerType;

  typedef Vector <double, 3>                                       VectorType;




  typedef ImageRegionIterator<TOutput>                             IteratorType;
  typedef ImageRegionIterator<TInput>                             InputIteratorType;

  itkNewMacro( Self );


/// Set the input image
   using ImageToImageFilter<TInput, TOutput>::SetInput; 
   void SetInput(int index, InputImagePointerType inputImage );

/// Set the radius value
  void SetRadiusValue ( double radiusValue );

  void SetRadiusValueInMillimetersOn ();
  void SetRadiusValueInMillimetersOff ();

  itkSetMacro( OutputOrigin, typename TOutput::PointType );
  itkSetMacro( OutputSpacing, typename TOutput::SpacingType );
  itkSetMacro( OutputSize, typename TOutput::SizeType );
  itkSetMacro( OutputDirection, typename TOutput::DirectionType );

  itkGetMacro( OutputOrigin, typename TOutput::PointType );
  itkGetMacro( OutputSpacing, typename TOutput::SpacingType );
  itkGetMacro( OutputSize, typename TOutput::SizeType );
  itkGetMacro( OutputDirection, typename TOutput::DirectionType );

///Set the output parameters (size, spacing, origin, orientation) from a reference image
  void SetOutputParametersFromImage( InputImagePointerType Image );

//protected:

  LocalNormalizedCorrelationMetricFilter();
  void BeforeThreadedGenerateData();
  void ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId );
  void GenerateOutputInformation();
  void GenerateInputRequestedRegion();
  double GetMetric (InputImageRegionType &region);

  itkSetMacro( InputImageMask, InputImagePointerType )
  itkGetMacro( InputImageMask, InputImagePointerType )

private:

  bool                              m_radiusValueInMillimeters;
  double                            m_radiusValue;     
  VectorType                        m_radiusVector; 
  InputImagePointerType             m_fixedImage;
  InputImagePointerType             m_movingImage;
  InputImagePointerType             m_InputImageMask;
  InputImageIndexType               m_start; 
  InputImageSizeType                m_patchSize;
  InputImageRegionType              m_region;
  MetricPointerType                 m_metric;
  TransformParametersType           m_params;

  typename TOutput::PointType       m_OutputOrigin;
  typename TOutput::SpacingType     m_OutputSpacing;
  typename TOutput::SizeType        m_OutputSize;
  typename TOutput::DirectionType   m_OutputDirection;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLocalNormalizedCorrelationMetricFilter.hxx"
#endif

#endif
