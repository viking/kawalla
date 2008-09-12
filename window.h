#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <kwindowsystem.h>
#include <qscrollarea.h>
#include <QVBoxLayout>
#include <kconfig.h>
#include <kio/netaccess.h>
#include <qlabel.h>
#include <qlayout.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <q3ptrlist.h>
#include <qdir.h>
#include <q3cstring.h>
#include <q3listbox.h>
#include <qcolor.h>
#include <qcombobox.h>
#include <q3gridlayout.h>
#include <qpixmap.h>
#include <qdesktopwidget.h>
#include <Magick++.h>
using namespace Magick;

#include "handler.h"
#include "photo.h"
#include "row.h"
#include "row_box.h"

class MainWindow : public QWidget
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();
  void addPhoto( QString &, QString &, QString &, QString &, QString &, int, int, float );

private slots:
  void go();
  void next();
  void back();

private:
  void grabPhotos();
  void switchPage();

  PhotoRowBox *row_box;
  QVBoxLayout *layout;
  QWidget     *bbox;
  QHBoxLayout *blayout;
  QScrollArea *scroll;
  KPushButton *goButton;
  KPushButton *nextButton;
  KPushButton *backButton;
  Q3PtrList<Photo> photos;
  Q3PtrList<PhotoRow> rows;
  int count;
  int desktops;
  int dwidth;
  int dheight;
  int page;
  float dratio;
};

#endif
