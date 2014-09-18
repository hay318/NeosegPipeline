#ifndef DEF_About
#define DEF_About

#include <QtGui>
#include <QDialog> 
#include <QString> 

#include "ui_About.h"

class About : public QDialog , public Ui_Dialog
{
   public : 
   
   About()
   {
      setupUi(this);
   }

   void setVersion(QString version)
   {
      QString text = aboutTitle_label->text(); 
      text.replace(QString("VERSION"), version);
      aboutTitle_label->setText(text); 
   }

}; 

#endif
