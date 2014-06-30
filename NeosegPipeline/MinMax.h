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

      MinMax(std::string image_path)
      {
         ImageFileReaderPointerType imageFileReader = ImageFileReaderType::New(); 
         imageFileReader->SetFileName(image_path);  
         imageFileReader->Update();  

         m_MinMaxImageCalculator = MinMaxImageCalculatorType::New();     
         m_MinMaxImageCalculator->SetImage(imageFileReader->GetOutput());
      }

      InputImagePixelType GetMin()
      {
         m_MinMaxImageCalculator->ComputeMinimum();
         return m_MinMaxImageCalculator->GetMinimum(); 
      }
 
      InputImagePixelType GetMax()
      {
         m_MinMaxImageCalculator->ComputeMaximum();
         return m_MinMaxImageCalculator->GetMaximum();
      }
     
      private :

      MinMaxImageCalculatorPointerType m_MinMaxImageCalculator;
      
   };

//}
#endif
