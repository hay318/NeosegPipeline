#ifndef DEF_NeosegPipelineTool
#define DEF_NeosegPipelineTool

#include <QString>
#include <QCoreApplication>
#include <QThread>

#include "Pipeline.h"
#include "PipelineParameters.h"
#include "XmlReader.h"
#include "DerivedWindow.h"
#include "MainScriptThread.h"

class NeosegPipelineTool
{

   public:

      NeosegPipelineTool(); 

      void setOutput(QString output); 
      void setT1(QString T1);
      void setT2(QString T2);
      void setMask(QString mask);    
      void setDWI(QString DWI);
      void setb0(QString b0);

      void setParametersFile(QString xmlFile);
      void setExecutablesFile(QString xmlFile);

      void launch(int argc, char *argv[], bool gui);

   private:

      Pipeline             m_pipeline; 
      PipelineParameters   m_parameters;
      MainScriptThread*     m_thread;
}; 

#endif
