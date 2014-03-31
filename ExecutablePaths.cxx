#include "ExecutablePaths.h"

ExecutablePaths::ExecutablePaths()
{
   // Path
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   QString path = env.value("PATH", QString::null);
   m_splitPath = path.split(":");
   m_splitPath.prepend(".");
}

void ExecutablePaths::setProgramPath(QString programPath)
{
   m_currentDirectory = ((QFileInfo(programPath)).dir()).absolutePath();
   m_splitPath.prepend(m_currentDirectory);
}

QString ExecutablePaths::findExecutablePath(QStringList splitPath, QString executableName)
{
   QStringList::iterator it;
   for(it=m_splitPath.begin(); it!=m_splitPath.end(); ++it)
   {
      if(QDir(*it).exists(executableName))
      {
         return QDir(*it).filePath(executableName);
      }
   }   
   return NULL;  
}

bool ExecutablePaths::checkExecutablePath(QString path)
{
   return QFileInfo(path).isExecutable();
}

void ExecutablePaths::setDefaultExecutablePath(QString name)
{
   m_executables[name]=findExecutablePath(m_splitPath, name);
}
QString ExecutablePaths::getDefaultExecutablePath(QString name)
{
   return findExecutablePath(m_splitPath, name);
} 

void ExecutablePaths::setExecutablePath(QString name, QString path)
{
   m_executables[name]=path;
}
QString ExecutablePaths::getExecutablePath(QString name)
{
   return m_executables[name];
}

