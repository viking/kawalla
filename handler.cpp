#include "handler.h"

FlickrHandler::FlickrHandler( MainWindow *w ) :
  QXmlDefaultHandler()
{
  window  = w;
  dwidth  = KApplication::desktop()->width();
  dheight = KApplication::desktop()->height();
  dratio  = (float)dwidth / (float)dheight;
  dfloor  = floorf(dratio);
  dceil   = ceilf(dratio);
  count   = 0;
}

bool FlickrHandler::startElement( const QString &, const QString &, const QString &name, const QXmlAttributes &attrs )
{
  QString farm, server, id, owner, secret, osecret, ext, title( "(none)" );
  int width, height;
  float ratio;
  width = height = ratio = 0;

  if (name == "photo" && count < 10) {
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

      else if (attrs.localName(i) == "title" && !attrs.value(i).isEmpty())
        title = attrs.value(i);

      else if (attrs.localName(i) == "o_width")
        width = attrs.value(i).toInt();

      else if (attrs.localName(i) == "o_height")
        height = attrs.value(i).toInt();

      else if (attrs.localName(i) == "secret")
        secret = attrs.value(i);
    }

    if (width >= dwidth && height >= dheight) {
      ratio = (float)dwidth / (float)dheight;
      if (ratio >= dfloor && ratio <= dceil) {
        // this is a good candidate
        QString thumbUrl( QString("http://farm%1.static.flickr.com/%2/%3_%4_s.jpg")
          .arg(farm)
          .arg(server)
          .arg(id)
          .arg(secret) );
        QString photoUrl( QString("http://farm%1.static.flickr.com/%2/%3_%4_o.%5")
          .arg(farm)
          .arg(server)
          .arg(id)
          .arg(osecret)
          .arg(ext) );
        window->addFlickr( thumbUrl, photoUrl, title, id );
        count++;
      }
    }
  }

  return true;
}
