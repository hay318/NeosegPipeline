#include "LibraryPaths.h"

LibraryPaths::LibraryPaths()
{
}
 

void LibraryPaths::setLibraryPath(QString name, QString path)
{
   m_libraries[name]=path;
}
QString LibraryPaths::getLibraryPath(QString name)
{
   return m_libraries[name];
}
bool LibraryPaths::checkLibraryPath(QString library_name, QString library_path)
{
   if(library_path.isEmpty() || !QFileInfo(library_path).isExecutable())
   {
      return false; 
   }
}

QString LibraryPaths::checkLibraries()
{
   QString errors;

   QMap<QString, QString>::iterator it; 
   for(it = m_libraries.begin(); it != m_libraries.end(); ++it)
   {
      QString name = it.key();
      QString path = it.value(); 

      if(path.isEmpty())
      {
         errors += name + " path is empty\n";
      }      
      else if(!QFileInfo(path).isDir())
      {
         errors += name + " path is not a directory\n";
      }
   }

   return errors;
}
