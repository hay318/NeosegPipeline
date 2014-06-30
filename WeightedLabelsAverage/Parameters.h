#ifndef Parameters_h
#define Parameters_h

// ITK Librairies // 
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImage.h"

// General Librairies // 
#include <iostream>
#include <string>
#include <vector>

// My Specifics Librairies //
#include "Atlas.h"


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

     itkGetMacro(ComputingWeights, bool);
     itkSetMacro(ComputingWeights, bool);

     itkGetMacro(NeightborhoodRadius, double);
     itkSetMacro(NeightborhoodRadius, double);

     itkGetMacro(NeightborhoodRadiusUnit, std::string);
     itkSetMacro(NeightborhoodRadiusUnit, std::string);

     itkGetMacro(White, std::string);
     itkSetMacro(White, std::string);

     itkGetMacro(Gray, std::string);
     itkSetMacro(Gray, std::string);

     itkGetMacro(Csf, std::string);
     itkSetMacro(Csf, std::string);

     std::vector<Atlas> GetAtlasPopulation();
     void SetAtlasPopulation(std::vector<Atlas> atlasPopulation);

     // Checking Parameters
     virtual bool CheckValues();

protected:

     std::string m_Input;

     std::vector<Atlas> m_AtlasPopulation;

     bool m_ComputingWeights;
     double m_NeightborhoodRadius;
     std::string m_NeightborhoodRadiusUnit;

     std::string m_White;
     std::string m_Gray;
     std::string m_Csf;
};

#endif
