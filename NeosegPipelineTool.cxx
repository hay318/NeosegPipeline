#include "NeosegPipelineTool.h"

NeosegPipelineTool::NeosegPipelineTool()
{
   m_parameters=m_pipeline.getParameters();
}

void NeosegPipelineTool::setOutput(QString output)
{
   m_parameters->setOutput(output);
}

void NeosegPipelineTool::setT1(QString T1) {m_parameters->setT1(T1);}

void NeosegPipelineTool::setT2(QString T2) {m_parameters->setT2(T2);}

void NeosegPipelineTool::setMask(QString mask) {m_parameters->setMask(mask);}

void NeosegPipelineTool::setDWI(QString DWI) {m_parameters->setDWI(DWI);}

void NeosegPipelineTool::setb0(QString b0) {m_parameters->setb0(b0);}

void NeosegPipelineTool::setSkullStripping(bool skullStripping) {m_parameters->setSkullStripping(skullStripping);}

void NeosegPipelineTool::setParametersFile(QString xmlFile) 
{
   m_xmlReader.setPipelineParameters(m_parameters);
   m_xmlReader.readParametersConfigurationFile(xmlFile); 
}

void NeosegPipelineTool::setExecutablesFile(QString xmlFile) 
{
   m_xmlReader.setPipelineParameters(m_parameters);
   m_xmlReader.readExecutablesConfigurationFile(xmlFile); 
}

void NeosegPipelineTool::launch(int argc, char *argv[], bool gui)
{
   if(gui)
   {
      QApplication app(argc, argv);

	   DerivedWindow window;
      window.setPipelineParameters(m_parameters); 
      window.setPipeline(&m_pipeline);
	   window.show();

	   app.exec();
   }
   else
   {
      m_pipeline.runPipeline();
   }   
}
	 
