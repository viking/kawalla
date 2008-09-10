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
#include <q3scrollview.h>
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
#include <Q3GridLayout>
#include <QPixmap>
#include <QDesktopWidget>
#include <Magick++.h>
#include "photo.h"
#include "row.h"
#include "handler.h"
using namespace Magick;

class PhotoRow;

class MainWindow : public KMainWindow
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

  Q3ScrollView *sv;
  QWidget     *central;
  Q3GridLayout *grid;
  Q3VBox       *vbox;
  KPushButton *goButton;
  KPushButton *nextButton;
  KPushButton *backButton;
  Q3PtrList<Photo> photos;
  Q3PtrList<PhotoRow> rows;
  QColor alternateBackground;
  int count;
  int desktops;
  int dwidth;
  int dheight;
  int page;
  float dratio;
};

#endif
