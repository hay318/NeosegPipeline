#ifndef DEF_XmlReader
#define DEF_XmlReader

#include <QXmlStreamReader>

#include "PipelineParameters.h"
#include "AntsParameters.h"
#include "NeosegParameters.h"

class XmlReader
{

   public:

   void setPipelineParameters(PipelineParameters* parameters);

   bool isBoolean(int value);

   QString readParametersConfigurationFile(QString file_path); 
   QString readExecutablesConfigurationFile(QString file_path);

   private:

   PipelineParameters* m_parameters; 
   AntsParameters* m_antsParameters;  
   NeosegParameters* m_neosegParameters; 
   ExecutablePaths* m_executablePaths;
   
   QStringList m_selectedAtlases; 

}; 



#endif 

