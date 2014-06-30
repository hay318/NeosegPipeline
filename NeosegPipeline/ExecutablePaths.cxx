#include "ExecutablePaths.h"

ExecutablePaths::ExecutablePaths()
{
   // Path
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   QString path = env.value("PATH", QString::null);
   m_splitPath = path.split(":");
   m_splitPath.prepend(".");

   // Executables with version
   m_executables_withVersionLongFlag << "SegPostProcessCLP" << "N4ITKBiasFieldCorrection" << "dtiestim" << "dtiprocess" << "ResampleVolume2" << "WeightedLabelsAverage" << "ReassignWhiteMatter" << "Neoseg";
   m_executables_withVersionShortFlag << "ImageMath";
   m_executables_withVersionArgument << "ITKTransformTools";
   m_executables_withoutVersionFlag << "bet2" << "ANTS" << "SNAP" << "unu";
}

void ExecutablePaths::setProgramPath(QString programPath)
{
   m_currentDirectory = ((QFileInfo(programPath)).dir()).absolutePath();
   m_splitPath.prepend(m_currentDirectory); 
}

QString ExecutablePaths::findExecutablePath(QString executableName)
{
   QStringList::iterator it;
   for(it = m_splitPath.begin(); it != m_splitPath.end(); ++it)
   {
      if(QFileInfo(QDir(*it).filePath(executableName)).isExecutable())
      {
         return QDir(*it).filePath(executableName);
      }
   }   
   return NULL;  
}

bool ExecutablePaths::checkExecutablePath(QString executable_name, QString executable_path)
{
   if(executable_path.isEmpty() || !QFileInfo(executable_path).isExecutable())
   {
      return false; 
   }
   
   QString command;

   if(m_executables_withVersionLongFlag.contains(executable_name))
   {
      command =  executable_path + " --version "; 
   }
   else if(m_executables_withVersionShortFlag.contains(executable_name))
   {
      command =  executable_path + " -version "; 
   }
   else if(m_executables_withVersionArgument.contains(executable_name))
   {
      command =  executable_path + " version "; 
   }
   else
   {
      command =  executable_path + " --help"; 
   }

   QProcess test_process;   
   test_process.start(command);
   while (!test_process.waitForFinished())
   {
   } 
   
   QString output(test_process.readAllStandardOutput());
   QString error(test_process.readAllStandardError());
   output = output + error; 

   QString subString; 
   if(executable_name == "InsightSNAP")
   {
      subString = "SNAP"; 
   }
   else
   {
      subString = executable_name; 
   }

   if(output.contains(subString, Qt::CaseInsensitive))
   {
      return true;
   }
   else
   {
      return false;
   }
 
}

void ExecutablePaths::setDefaultExecutablePath(QString name)
{
   m_executables[name]=findExecutablePath(name);
}
QString ExecutablePaths::getDefaultExecutablePath(QString name)
{
   return findExecutablePath(name);
} 

void ExecutablePaths::setExecutablePath(QString name, QString path)
{
   m_executables[name]=path;
}
QString ExecutablePaths::getExecutablePath(QString name)
{
   return m_executables[name];
}

QString ExecutablePaths::checkExecutables()
{
   QString errors;

   QMap<QString, QString>::iterator it; 
   for(it = m_executables.begin(); it != m_executables.end(); ++it)
   {
      QString name = it.key();
      QString path = it.value(); 

      if(path.isEmpty())
      {
         errors += name + " path is empty\n";
      }      
      else if(!checkExecutablePath(name, path))
      {
         errors += name + " path does not seem to be the good one\n";
      }
   }

   return errors;
}
