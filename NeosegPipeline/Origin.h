#ifndef DEF_Origin
#define DEF_Origin

// General Libraries
#include <string>

// ITK Libraries
#include <itkImageFileReader.h>
//#include <itkImageBase.h>

template <class InputImagePixelType> 
class Origin
{
   public :

   // Image Type 
   typedef itk::Image<InputImagePixelType, 3>            InputImageType;
   typedef typename InputImageType::Pointer              InputImagePointerType; 
   typedef typename InputImageType::PointType            InputImagePointType; 

   // Reader Type    
   typedef itk::ImageFileReader <InputImageType>         ImageFileReaderType; 
   typedef typename ImageFileReaderType::Pointer         ImageFileReaderPointerType; 

   Origin(std::string image_path) ;

   InputImagePointType GetOrigin();
     
   private :

   InputImagePointerType m_image; 
   
   };

#include "Origin.txx"

#endif
