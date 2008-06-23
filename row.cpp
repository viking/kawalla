#include "row.h"

PhotoRow::PhotoRow( QWidget *parent, int desktops ) :
  QHBox( parent )
{
  desktops = desktops;

  combo = new QComboBox( this );
  combo->insertItem("");
  for (int j = 1; j <= desktops; j++) {
    combo->insertItem(QString::number(j));
  }
  combo->setMaximumSize(50, 25);
  connect(combo, SIGNAL( activated(int) ), this, SLOT( setDesktop(int) ));

  thumb = new QLabel( this );
  thumb->setMaximumSize(75, 75);

  title = new QLabel( this );
  title->setAlignment( Qt::AlignAuto | Qt::AlignTop | Qt::WordBreak );
  title->setMaximumWidth(200);
}

void PhotoRow::setPhoto( Photo *photo )
{
  QString tmpFile;
  this->photo = photo;

  KIO::NetAccess::download( photo->thumbUrl, tmpFile, this );
  thumb->setPixmap( QPixmap( tmpFile ) );
  KIO::NetAccess::removeTempFile(tmpFile);

  title->setText( photo->title );
  combo->setCurrentItem( photo->desktop );
}

void PhotoRow::setDesktop( int index )
{
  photo->desktop = index;
}