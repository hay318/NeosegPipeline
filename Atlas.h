#ifndef DEF_Atlas
#define DEF_Atlas

#include <QString>
#include <QFile>

struct Atlas {
QString name;
QString T1;
QString T2;
bool probabilistic;
QString seg; 
QString white;
QString gray;
QString csf; 
};

#endif
