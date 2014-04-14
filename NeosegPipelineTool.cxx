#include "NeosegPipelineTool.h"

NeosegPipelineTool::NeosegPipelineTool()
{
   m_pipeline = new::Pipeline();
   m_pipeline->setPipelineParameters(&m_parameters); 

   m_thread = new MainScriptThread(); 
}

void NeosegPipelineTool::setProgramPath(QString programPath)
{
   m_parameters.setProgramPath(programPath);
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
void NeosegPipelineTool::setParametersFile(QString xmlFile) 
{
   if(!xmlFile.isEmpty())
   {
      XmlReader m_xmlReader;
      m_xmlReader.setPipelineParameters(&m_parameters);
      m_xmlReader.readParametersConfigurationFile(xmlFile); 
   }
}

void NeosegPipelineTool::setExecutablesFile(QString xml_CLI) 
{
   XmlReader m_xmlReader;
   m_xmlReader.setPipelineParameters(&m_parameters);

   if(!xml_CLI.isEmpty())
   {
      m_xmlReader.readExecutablesConfigurationFile(xml_CLI); 
   }
   else
   {
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      QString xml_ENV = env.value("NEOSEG_PIPELINE_EXECUTABLES", QString::null);

      if(!xml_ENV.isEmpty())
      {
         m_xmlReader.readExecutablesConfigurationFile(xml_ENV);
      }
   }
}

void NeosegPipelineTool::launch(int argc, char *argv[], bool gui)
{
   if(gui)
   {
      QApplication app(argc, argv);

	   DerivedWindow window;
      window.setPipelineParameters(&m_parameters); 
      window.setPipeline(m_pipeline);
      window.setMainScriptThread(m_thread);
	   window.show();

	   app.exec();
      m_thread->wait();
   }
   else
   {
      m_pipeline->writePipeline(); 
      m_pipeline->runPipeline();
   }   
}
	 
