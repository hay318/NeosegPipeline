#include "ParametersXMLFile.h"

ParametersXMLFileReader
::ParametersXMLFileReader()
{
   m_PObject = 0;
}

int
ParametersXMLFileReader
::CanReadFile(const char* name)
{
   if(!itksys::SystemTools::FileExists(name) || itksys::SystemTools::FileIsDirectory(name) || itksys::SystemTools::FileLength(name) == 0)
   {
      return 0;
   }
   return 1;
}

void
ParametersXMLFileReader
::StartElement(const char* name, const char**)
{
  //std::cout << "StartElement" << name << std::endl;
  if(itksys::SystemTools::Strucmp(name,"WEIGHTED-AVERAGED-LABELS-PARAMETERS") == 0)
  {  
    m_PObject = Parameters::New();
  }
}

void 
ParametersXMLFileReader
::EndElement(const char* name)
{

   //std::cout << "ENDElement " << name << std::endl;

   if(itksys::SystemTools::Strucmp(name,"WEIGHTED-AVERAGED-LABELS-PARAMETERS") == 0)
   {
      m_OutputObject = &(*m_PObject);
   }

   else if(itksys::SystemTools::Strucmp(name,"INPUT") == 0)
   {
      m_PObject->SetInput(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"IMAGE") == 0)
   {
      m_Image=m_CurrentString;
   }
   else if(itksys::SystemTools::Strucmp(name,"MASK") == 0)
   {
      m_PObject->SetInputMask(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"SEG") == 0)
   {
      m_Seg=m_CurrentString;
   }

   else if(itksys::SystemTools::Strucmp(name,"WHITE") == 0)
   {
      m_White=m_CurrentString;
   }

   else if(itksys::SystemTools::Strucmp(name,"GRAY") == 0)
   {
      m_Gray=m_CurrentString;
   }

   else if(itksys::SystemTools::Strucmp(name,"CSF") == 0)
   {
      m_Csf=m_CurrentString;
   }

   else if(itksys::SystemTools::Strucmp(name,"ATLAS") == 0)
   {
      Atlas atlas; 
      
      atlas.image=m_Image;
      atlas.probabilistic=0;
      atlas.seg=m_Seg;

      m_AtlasPopulation.push_back(atlas);
   }

   else if(itksys::SystemTools::Strucmp(name,"PROBABILISTIC-ATLAS") == 0)
   {
      Atlas atlas; 

      atlas.image=m_Image;
      atlas.probabilistic=1;
      atlas.white=m_White;
      atlas.gray=m_Gray;
      atlas.csf=m_Csf;
      
      m_AtlasPopulation.push_back(atlas);
   }

   else if(itksys::SystemTools::Strucmp(name,"ATLAS-POPULATION") == 0)
   {
      m_PObject->SetAtlasPopulation(m_AtlasPopulation);
   }

   else if(itksys::SystemTools::Strucmp(name,"COMPUTING-WEIGHTS") == 0)
   {
      m_PObject->SetComputingWeights(atoi(m_CurrentString.c_str()));
   }   

   else if(itksys::SystemTools::Strucmp(name,"NEIGHTBORHOOD-RADIUS") == 0)
   {
      m_PObject->SetNeightborhoodRadius(atof(m_CurrentString.c_str()));
   }   

   else if(itksys::SystemTools::Strucmp(name,"NEIGHTBORHOOD-RADIUS-UNIT") == 0)
   {
      m_PObject->SetNeightborhoodRadiusUnit(m_CurrentString);
   } 

   else if(itksys::SystemTools::Strucmp(name,"WHITE-AVERAGE") == 0)
   {
      m_PObject->SetWhite(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"GRAY-AVERAGE") == 0)
   {
      m_PObject->SetGray(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"CSF-AVERAGE") == 0)
   {
      m_PObject->SetCsf(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"OUTPUT") == 0)
   {
      m_PObject->SetOutputDirectory(m_CurrentString);
   }
}

void
ParametersXMLFileReader
::CharacterDataHandler(const char* inData, int inLength)
{
  // std::cout << "Data: " << inLength << ": " << inData << std::endl;

  //if (inLength <= 1) return;
  
  char *data = new char[inLength+1];
  char *item = new char[inLength+1];
  strncpy(data, inData, inLength);
  data[inLength] = '\n';
  sscanf(data, " %s ", item);
  if (itksys::SystemTools::Strucmp(item,"") != 0) 
    {
      m_CurrentString = std::string(item);
      //std::cout << inLength << ": " << m_CurrentString << std::endl;
    }
}

