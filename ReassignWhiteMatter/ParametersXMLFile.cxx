#include "ParametersXMLFile.h"

ParametersXMLFileReader::ParametersXMLFileReader()
{
   m_PObject = 0;
   m_CurrentString = "";
}

int
ParametersXMLFileReader::CanReadFile(const char* name)
{
   if(!itksys::SystemTools::FileExists(name) || itksys::SystemTools::FileIsDirectory(name) || itksys::SystemTools::FileLength(name) == 0)
   {
      return 0;
   }
   return 1;
}

void ParametersXMLFileReader::StartElement(const char* name, const char** atts)
{
  if(itksys::SystemTools::Strucmp(name,"REASSIGN-WHITE-MATTER-PARAMETERS") == 0)
  {  
    m_PObject = Parameters::New();
  }
}

void ParametersXMLFileReader::EndElement(const char* name)
{
   if(itksys::SystemTools::Strucmp(name,"REASSIGN-WHITE-MATTER-PARAMETERS") == 0)
   {
      m_OutputObject = &(*m_PObject);
   }

   else if(itksys::SystemTools::Strucmp(name,"INPUT") == 0)
   {
      m_PObject->SetInput(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"THRESHOLD") == 0)
   {
      m_PObject->SetThreshold(atoi(m_CurrentString.c_str()));
   }

   else if(itksys::SystemTools::Strucmp(name,"WHITE") == 0)
   {
      m_PObject->SetWhite(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"GRAY") == 0)
   {
      m_PObject->SetGray(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"CSF") == 0)
   {
      m_PObject->SetCsf(m_CurrentString);
   }

   else if(itksys::SystemTools::Strucmp(name,"OUTPUT") == 0)
   {
      m_PObject->SetOutput(m_CurrentString);
   }
   else if(itksys::SystemTools::Strucmp(name,"PROBABILITY-MAP") == 0)
   {
      m_PObject->AddProbabilityMap(m_CurrentString);
   }
   else if(itksys::SystemTools::Strucmp(name,"VOXEL-BY-VOXEL") == 0){
       m_PObject->SetVoxelByVoxel(atof(m_CurrentString.c_str()));
   }
   else if(itksys::SystemTools::Strucmp(name,"LABEL-VALUE") == 0){
       m_PObject->SetLabelValue(atof(m_CurrentString.c_str()));
   }
}

void ParametersXMLFileReader::CharacterDataHandler(const char* inData, int inLength)
{
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

