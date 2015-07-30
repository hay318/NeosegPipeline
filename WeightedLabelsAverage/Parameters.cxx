#include "Parameters.h"

void
Parameters
::SetAtlasPopulation(std::vector<Atlas> atlasPopulation) 
{
   m_AtlasPopulation = atlasPopulation;
}

std::vector<Atlas> 
Parameters
::GetAtlasPopulation() 
{
   return m_AtlasPopulation;
}

bool 
Parameters
::CheckValues()
{
   if (m_Input.empty())
   {
      return false;
   }

   Atlas atlas; 

   std::vector<Atlas>::iterator it; 

   for(it = m_AtlasPopulation.begin(); it != m_AtlasPopulation.end(); ++it)
   {
      atlas = *it; 
      
      if ( (!atlas.probabilistic && ((atlas.image).empty() || (atlas.seg).empty())) && (atlas.probabilistic && ((atlas.white).empty() || (atlas.gray).empty() || (atlas.csf).empty()) ) )
      {
         return false;
      }
   }

  if ( (m_White.empty() || m_Gray.empty() || m_Csf.empty()) && m_OutputDirectory.empty() )
  {
    return false;
  }

  return true;
}

