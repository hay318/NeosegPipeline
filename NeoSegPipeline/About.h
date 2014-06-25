#ifndef DEF_About
#define DEF_About

#include <QDialog>
#include <QtGui> 

#include "ui_About.h"

class About : public QDialog , public Ui_Dialog
{
   public : 

   About()
   {
   	setupUi(this);
   }
}; 

#endif
