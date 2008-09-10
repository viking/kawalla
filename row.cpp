#include "row.h"
//Added by qt3to4:
#include <QPixmap>
#include <QLabel>

PhotoRow::PhotoRow( QWidget *parent, int desktops ) :
  Q3HBox( parent )
{
  parent = parent;
  desktops = desktops;

  combo = new QComboBox( this );
  combo->insertItem("");
  for (int j = 1; j <= desktops; j++) {
    combo->insertItem(QString::number(j));
  }
  combo->setMaximumSize(50, 25);
  connect(combo, SIGNAL( activated(int) ), this, SLOT( setDesktop(int) ));

  thumb = new KPushButton( this );
  thumb->setMaximumSize(75, 75);
  thumb->setFlat(true);
  connect(thumb, SIGNAL( clicked() ), this, SLOT( runBrowser() ));

  title = new QLabel( this );
  title->setAlignment( Qt::AlignAuto | Qt::AlignTop | Qt::WordBreak );
  title->setMaximumWidth(200);
}

void PhotoRow::setPhoto( Photo *photo )
{
  this->photo = photo;

  if (photo->thumbfn.isNull()) {
    KIO::NetAccess::download( photo->thumbUrl, photo->thumbfn, this );
  }
  thumb->setPixmap( QPixmap( photo->thumbfn ) );

  title->setText( photo->title );
  combo->setCurrentItem( photo->desktop );
}

void PhotoRow::setDesktop( int index )
{
  photo->desktop = index;
}

void PhotoRow::runBrowser()
{
  KRun::runUrl( photo->pageUrl, "text/html", parent->parentWidget()->parentWidget() );
}
