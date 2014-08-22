#ifndef __MinMax_txx
#define __MinMax_txx

#include "MinMax.h"

template <class InputImagePixelType> 
MinMax<InputImagePixelType>::MinMax(std::string image_path)
{
         ImageFileReaderPointerType imageFileReader = ImageFileReaderType::New(); 
         imageFileReader->SetFileName(image_path);  
         imageFileReader->Update();  

         m_MinMaxImageCalculator = MinMaxImageCalculatorType::New();     
         m_MinMaxImageCalculator->SetImage(imageFileReader->GetOutput());
}

template <class InputImagePixelType> 
InputImagePixelType
MinMax<InputImagePixelType>::GetMin()
{
   m_MinMaxImageCalculator->ComputeMinimum();
   return m_MinMaxImageCalculator->GetMinimum(); 
}

template <class InputImagePixelType>  
InputImagePixelType
MinMax<InputImagePixelType>::GetMax()
{
   m_MinMaxImageCalculator->ComputeMaximum();
   return m_MinMaxImageCalculator->GetMaximum();
}
     

#endif
