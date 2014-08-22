#ifndef __Origin_txx
#define __Origin_txx

#include "Origin.h"


template <class InputImagePixelType> 
Origin<InputImagePixelType>::Origin(std::string image_path)
   {
      ImageFileReaderPointerType imageFileReader = ImageFileReaderType::New();
      imageFileReader->SetFileName(image_path);
      imageFileReader->Update();

      m_image = InputImageType::New();
      m_image = imageFileReader->GetOutput();
   }

template <class InputImagePixelType> 
typename Origin<InputImagePixelType>::InputImagePointType Origin<InputImagePixelType>::GetOrigin()
   {
      return m_image->GetOrigin();
   }

#endif
