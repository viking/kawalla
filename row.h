#ifndef __ROW_H
#define __ROW_H

#include <qhbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <kpushbutton.h>
#include <krun.h>
#include <kio/netaccess.h>
#include "photo.h"
#include "window.h"

class PhotoRow : public QHBox
{
  Q_OBJECT

public:
  PhotoRow( QWidget *parent, int desktops );
  void setPhoto( Photo *photo );

  QComboBox   *combo;
  KPushButton *thumb;
  QLabel      *title;
  Photo       *photo;
  int          desktops;

private slots:
  void setDesktop( int index );
  void runBrowser();
};

#endif
