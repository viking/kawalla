#ifndef __PHOTO_H
#define __PHOTO_H

#include <kurl.h>
#include <qstring.h>
#include <QFile>

struct Photo {
  KUrl url;
  KUrl thumbUrl;
  KUrl pageUrl;
  QString title;
  QFile thumbFile;

  int width;
  int height;
  float ratio;
  int desktop;
};

#endif
