#ifndef __PHOTO_H
#define __PHOTO_H

#include <kurl.h>
#include <qstring.h>

struct Photo {
  KURL url;
  KURL thumbUrl;
  KURL pageUrl;
  QString title;
  QString thumbfn;
  int width;
  int height;
  float ratio;
  int desktop;
};

#endif
