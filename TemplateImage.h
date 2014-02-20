#ifndef DEF_TemplateImage
#define DEF_TemplateImage

#include <QString>
#include <QDir>

struct TemplateImage {
QString     name;
QDir*       dir;
QString     finalImage;
};

#endif
