

#include <QApplication>

#include "NeosegPipelineTool.h"
#include "NeosegPipelineCLP.h"

int main(int argc, char *argv[])
{	
   PARSE_ARGS;	

   NeosegPipelineTool tool;

   tool.setProgramPath(QString::fromStdString(argv[0])); 

   tool.setOutput(QString::fromStdString(output));
   tool.setT1(QString::fromStdString(T1));
   tool.setT2(QString::fromStdString(T2));
   tool.setMask(QString::fromStdString(mask));
   tool.setDWI(QString::fromStdString(DWI));
   tool.setb0(QString::fromStdString(b0));
   
   tool.setParametersFile(QString::fromStdString(parameters));
   tool.setExecutablesFile(QString::fromStdString(executables));

   tool.setDebug(debug);

   tool.launch(argc, argv, !noGUI);

   return 0;
}
