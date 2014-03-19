#include "NeosegPipelineTool.h"

NeosegPipelineTool::NeosegPipelineTool()
{
   m_pipeline.setPipelineParameters(&m_parameters); 

   m_thread = new MainScriptThread(); 
}

void NeosegPipelineTool::setOutput(QString output)
{
   if(m_parameters.checkOutput(output))
   {
      m_parameters.setOutput(output);
   }
}

void NeosegPipelineTool::setT1(QString T1)
{
   if(m_parameters.checkT1(T1))
   {
      m_parameters.setT1(T1);
   }
}

void NeosegPipelineTool::setT2(QString T2)
{
   if(m_parameters.checkT2(T2))
   {
      m_parameters.setT2(T2);
   }
}

void NeosegPipelineTool::setMask(QString mask)
{
   if(m_parameters.checkMask(mask))
   {
      m_parameters.setMask(mask);
   }
}

void NeosegPipelineTool::setDWI(QString DWI)
{
   if(m_parameters.checkDWI(DWI))
   {
      m_parameters.setDWI(DWI);
   }
}

void NeosegPipelineTool::setb0(QString b0)
{
   if(m_parameters.checkb0(b0))
   {
      m_parameters.setb0(b0);
   }
}

void NeosegPipelineTool::setSkullStripping(bool skullStripping)
{
   m_parameters.setSkullStripping(skullStripping);
}

void NeosegPipelineTool::setParametersFile(QString xmlFile) 
{
   if(!xmlFile.isEmpty())
   {
      XmlReader m_xmlReader;
      m_xmlReader.setPipelineParameters(&m_parameters);
      m_xmlReader.readParametersConfigurationFile(xmlFile); 
   }
}

void NeosegPipelineTool::setExecutablesFile(QString xmlFile) 
{
   if(!xmlFile.isEmpty())
   {
      XmlReader m_xmlReader;
      m_xmlReader.setPipelineParameters(&m_parameters);
      m_xmlReader.readExecutablesConfigurationFile(xmlFile); 
   }
}

void NeosegPipelineTool::launch(int argc, char *argv[], bool gui)
{
   if(gui)
   {
      QApplication app(argc, argv);

	   DerivedWindow window;
      window.setPipelineParameters(&m_parameters); 
      window.setPipeline(&m_pipeline);
      window.setMainScriptThread(m_thread);
	   window.show();

	   app.exec();
      m_thread->wait();
   }
   else
   {
      m_pipeline.runPipeline();
   }   
}
	 
