#include "ExecutablePaths.h"

ExecutablePaths::ExecutablePaths()
{
   // Path
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   QString path = env.value("PATH", QString::null);
   m_splitPath = path.split(":");
   m_splitPath << ".";
}

QString ExecutablePaths::findExecutablePath(QStringList splitPath, QString executableName)
{
   QStringList::iterator it;
   for(it=splitPath.begin(); it!=splitPath.end(); ++it)
   {
      if(QDir(*it).exists(executableName))
      {
         return QDir(*it).filePath(executableName);
      }
   }   
   return NULL;  
}


/*std::string FindProgram( const char* name , std::vector< std::string > m_FindProgramDTIABExecDirVec )
{
   std::string path ;
   path = itksys::SystemTools::FindProgram( name , m_FindProgramDTIABExecDirVec , true ) ;

   if( path.empty() )
   {
      path = itksys::SystemTools::FindProgram( name ) ;
   }
   return path ;
}*/


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

