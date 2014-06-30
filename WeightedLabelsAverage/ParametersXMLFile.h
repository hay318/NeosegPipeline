#ifndef ParametersXMLFile_h
#define ParametersXMLFile_h

// General Librairies //
#include <string>

// ITK Librairies //
#include "itkImageFileReader.h"
#include "itkXMLFile.h"
#include "itksys/SystemTools.hxx"

// My Specifics Librairies //
#include "Parameters.h"
#include "Atlas.h"

class ParametersXMLFileReader: public itk::XMLReader<Parameters>
{

public:

   // Standard typedefs
   typedef ParametersXMLFileReader         Self;
   typedef itk::XMLReader<Parameters>      Superclass;
   typedef itk::SmartPointer<Self>         Pointer;

   // RTTI
   itkTypeMacro(Self, Superclass)
   itkNewMacro(Self);

   // Constructor 
   ParametersXMLFileReader();

   // Reading XML File // 
   virtual int CanReadFile(const char* name);
   virtual void StartElement(const char * name, const char **atts);
   virtual void EndElement(const char *name);
   virtual void CharacterDataHandler(const char *inData, int inLength);

protected:

   Parameters::Pointer m_PObject;
   std::string m_CurrentString;

   std::vector<Atlas> m_AtlasPopulation; 
   std::string m_Image;
   std::string m_Seg; 
   std::string m_White; 
   std::string m_Gray; 
   std::string m_Csf; 
  
};

#endif
