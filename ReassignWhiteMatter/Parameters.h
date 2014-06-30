#ifndef Parameters_h
#define Parameters_h

// ITK Librairies // 
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImage.h"

// General Librairies // 
#include <iostream>
#include <string>

class Parameters: public itk::Object
{
  public:

     typedef Parameters Self;
     typedef itk::SmartPointer<Self> Pointer;
     typedef itk::SmartPointer<const Self> ConstPointer;

     // RTTI
     itkNewMacro(Self);

     // Set & Get
     itkGetMacro(Input, std::string);
     itkSetMacro(Input, std::string);

     itkGetMacro(Threshold, int);
     itkSetMacro(Threshold, int);

     itkGetMacro(White, std::string);
     itkSetMacro(White, std::string);

     itkGetMacro(Gray, std::string);
     itkSetMacro(Gray, std::string);

     itkGetMacro(Csf, std::string);
     itkSetMacro(Csf, std::string);

     itkGetMacro(Output, std::string);
     itkSetMacro(Output, std::string);


     // Checking Parameters
     virtual bool CheckValues();

protected:

     std::string m_Input;

     int m_Threshold;

     std::string m_White;
     std::string m_Gray;
     std::string m_Csf;

     std::string m_Output;
};

#endif
