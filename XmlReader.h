#ifndef DEF_XmlReader
#define DEF_XmlReader

#include <QXmlStreamReader>

#include "PipelineParameters.h"
#include "AntsParameters.h"
#include "NeosegParameters.h"

class XmlReader
{

   public:
   XmlReader();

   void setPipelineParameters(PipelineParameters* parameters);

   bool isBoolean(int value);

   void readParametersConfigurationFile(QString file_path); 
   void readExecutablesConfigurationFile(QString file_path);

   private:

   QFile* m_file;

   QString m_errors;

   PipelineParameters* m_parameters; 
   AntsParameters* m_antsParameters;  
   NeosegParameters* m_neosegParameters; 
   ExecutablePaths* m_executablePaths;

   QXmlStreamReader* m_stream; 
   
   QStringList m_selectedAtlases; 

}; 



#endif 

