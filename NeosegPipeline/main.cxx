#include <QApplication>

#include "NeosegPipelineCLP.h"
#include "NeosegPipelineTool.h"


int main(int argc, char *argv[])
{	
   PARSE_ARGS;

   NeosegPipelineTool tool(QString::fromStdString(argv[0]));

   tool.setOutput(QString::fromStdString(output));
   tool.setT1(QString::fromStdString(T1));
   tool.setT2(QString::fromStdString(T2));
   tool.setMask(QString::fromStdString(mask));
   tool.setDWI(QString::fromStdString(DWI));
   tool.setPrefix(QString::fromStdString(prefix));
   tool.setSuffix(QString::fromStdString(suffix));
   
   tool.setParametersFile(QString::fromStdString(parameters));
   tool.setExecutablesFile(QString::fromStdString(executables));

   tool.launch(argc, argv, !noGUI);

   return 0;
}
