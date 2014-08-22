#ifndef DEF_MinMax
#define DEF_MinMax

// General Libraries
#include <string>

// ITK Libraries
#include <itkImageFileReader.h>
#include <itkMinimumMaximumImageCalculator.h>

//namespace itk
//{

   template <class InputImagePixelType> 
   class MinMax
   {
      public :
         
      //MinMax(){}

      // Image Type 
      typedef itk::Image<InputImagePixelType, 3>                   InputImageType;

      // Reader Type 
      typedef itk::ImageFileReader <InputImageType>                     ImageFileReaderType; 
      typedef typename ImageFileReaderType::Pointer                ImageFileReaderPointerType; 

      // Min Max Calculator Type
      typedef itk::MinimumMaximumImageCalculator <InputImageType>       MinMaxImageCalculatorType;
      typedef typename MinMaxImageCalculatorType::Pointer          MinMaxImageCalculatorPointerType;

      MinMax(std::string image_path);

      InputImagePixelType GetMin();
 
      InputImagePixelType GetMax();
     
      private :

      MinMaxImageCalculatorPointerType m_MinMaxImageCalculator;
      
   };

//}

#include "MinMax.txx"

#endif
