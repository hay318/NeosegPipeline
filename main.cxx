

#include <QApplication>

#include "NeosegPipelineTool.h"
#include "NeosegPipelineCLP.h"

int main(int argc, char *argv[])
{	
   PARSE_ARGS;	

   NeosegPipelineTool tool;

   if(!output.empty())
   {
      tool.setOutput(QString::fromStdString(output));
   }

   if(!T1.empty())
   {
      tool.setT1(QString::fromStdString(T1));
   }

   if(!T2.empty())
   {
      tool.setT2(QString::fromStdString(T2));
   }

   if(!mask.empty())
   {
      tool.setMask(QString::fromStdString(mask));
   }

   if(!DWI.empty())
   {
      tool.setDWI(QString::fromStdString(DWI));
   }

   if(!b0.empty())
   {
      tool.setb0(QString::fromStdString(b0));
   }

   tool.setSkullStripping(!skullStripping);
   
   if(!parameters.empty())
   {
      tool.setParametersFile(QString::fromStdString(parameters));
   }

   if(!executables.empty())
   {
      tool.setExecutablesFile(QString::fromStdString(executables));
   }

   tool.launch(argc, argv, !noGUI);

   return 0;
}
