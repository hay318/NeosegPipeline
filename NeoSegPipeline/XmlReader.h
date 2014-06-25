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
   void readGeneralParameters(QXmlStreamReader* stream, QString errors);
   void readAntsParameters(QXmlStreamReader* stream, QString errors, AntsParameters* antsParameters);
   void readNeosegParameters(QXmlStreamReader* stream, QString errors);

   QString readExecutablesConfigurationFile(QString file_path);

   private:

   PipelineParameters* m_parameters; 
   AntsParameters* m_antsParameters_DTI;  
   AntsParameters* m_antsParameters_atlas;  
   NeosegParameters* m_neosegParameters; 
   ExecutablePaths* m_executablePaths;
   
   QStringList m_selectedAtlases; 

}; 



#endif 

