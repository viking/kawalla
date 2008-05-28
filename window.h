#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <kapplication.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <kio/netaccess.h>

class MainWindow : public KMainWindow
{
  Q_OBJECT

public:
  MainWindow( const char *name );
  void addFlickr( QString &thumbUrl, QString &photoUrl, QString &title, QString &id ); 

private:
  void grabPhotos();
  QWidget     *central;
  QGridLayout *grid;
  KPushButton *goButton;
  int count;
};

#endif

#include "handler.h"
