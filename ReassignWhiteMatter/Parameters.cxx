#include "Parameters.h"

bool Parameters::CheckValues()
{
   if (m_Input.empty())
   {
      return false;
   }

   if (m_Threshold < 0)
   {
      return false;
   }

   if ( m_White.empty() || m_Gray.empty() || m_Csf.empty() )
   {
      return false;
   }

   if (m_Output.empty())
   {
      return false;
   }

   return true;
}
