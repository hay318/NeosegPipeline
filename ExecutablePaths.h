#ifndef DEF_ExecutablePaths
#define DEF_ExecutablePaths

#include <iostream>
#include <vector>

#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoListIterator>
#include <QProcessEnvironment>
#include <QMap>


class ExecutablePaths
{
   public:

   ExecutablePaths();

   void setProgramPath(QString programPath);

   bool checkExecutablePath(QString name, QString path);

   void setExecutablePath(QString name, QString path); 
   QString getExecutablePath(QString name); 

   void setDefaultExecutablePath(QString name);
   QString getDefaultExecutablePath(QString name);

   QString findExecutablePath(QString executableName);
  
   QString checkExecutables();

   private: 

   QString m_currentDirectory; 

   QMap<QString, QString> m_executables; 

   QStringList m_splitPath;

   QStringList m_executables_withVersionLongFlag; 
   QStringList m_executables_withVersionShortFlag; 
   QStringList m_executables_withVersionArgument; 
   QStringList m_executables_withoutVersionFlag; 

};

#endif
