#ifndef DEF_Neo
#define DEF_Neo

#include <QString>
#include <QFile>

struct Neo {
QString prefix; 
QString T1;
QString T2;  
QString mask;
QString smoothedMask;
QString DWI;
QString FA;
QString AD;
QString seg4Labels;
QString seg3Labels;
int origin[3];
};

#endif
