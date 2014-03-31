#ifndef DEF_ExecutablePaths
#define DEF_ExecutablePaths

#include <iostream>
#include <vector>
#include <map>

#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoListIterator>
#include <QProcessEnvironment>

//#include <itksys/SystemTools.hxx> // for findProgramm

class ExecutablePaths
{
   public:

   ExecutablePaths();

   void setProgramPath(QString programPath);

   bool checkExecutablePath(QString path);

   void setExecutablePath(QString name, QString path); 
   QString getExecutablePath(QString name); 

   void setDefaultExecutablePath(QString name);
   QString getDefaultExecutablePath(QString name);

   QString findExecutablePath(QStringList splitPath, QString executableName);
  
   private: 

   QString m_currentDirectory; 

   std::map<QString, QString> m_executables; 

   QStringList m_splitPath;
};

#endif
