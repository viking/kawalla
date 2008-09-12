#ifndef __ROW_H
#define __ROW_H

#include <q3hbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <QHBoxLayout>
#include <QFileInfo>
#include <kpushbutton.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <kio/job.h>
#include "photo.h"

class PhotoRow : public QWidget
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
  void dataRecv( KIO::Job *job, const QByteArray &data );

private:
  QHBoxLayout *layout;
};

#endif
