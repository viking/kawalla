#include "handler.h"

FlickrHandler::FlickrHandler( QGrid *g ) :
  QXmlDefaultHandler()
{
  grid    = g;
  dwidth  = KApplication::desktop()->width();
  dheight = KApplication::desktop()->height();
  dratio  = (float)dwidth / (float)dheight;
  dfloor  = floorf(dratio);
  dceil   = ceilf(dratio);
}

bool FlickrHandler::startElement( const QString &, const QString &, const QString &name, const QXmlAttributes &attrs )
{
  QString farm, server, id, owner, osecret, ext, title;
  int width, height;
  float ratio;
  width = height = ratio = 0;

  if (name == "photo") {
    for (int i = 0; i < attrs.count(); i++) {
      if (attrs.localName(i) == "farm")
        farm = attrs.value(i);
      
      else if (attrs.localName(i) == "server")
        server = attrs.value(i);

      else if (attrs.localName(i) == "id")
        id = attrs.value(i);

      else if (attrs.localName(i) == "owner")
        owner = attrs.value(i);

      else if (attrs.localName(i) == "originalsecret")
        osecret = attrs.value(i);

      else if (attrs.localName(i) == "originalformat")
        ext = attrs.value(i);

      else if (attrs.localName(i) == "title")
        title = attrs.value(i);

      else if (attrs.localName(i) == "o_width")
        width = attrs.value(i).toInt();

      else if (attrs.localName(i) == "o_height")
        height = attrs.value(i).toInt();
    }

    if (width >= dwidth && height >= dheight) {
      ratio = (float)dwidth / (float)dheight;
      if (ratio >= dfloor && ratio <= dceil) {
        // this is a good candidate
      }
    }
  }

  return true;
}
