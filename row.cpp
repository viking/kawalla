#include "row.h"

PhotoRow::PhotoRow( QWidget *parent, int desktops ) : QWidget( parent )
{
  desktops = desktops;
  layout = new QHBoxLayout;

  combo = new QComboBox;
  combo->insertItem("");
  for (int j = 1; j <= desktops; j++) {
    combo->insertItem(QString::number(j));
  }
  combo->setMaximumSize(50, 25);
  connect(combo, SIGNAL( activated(int) ), this, SLOT( setDesktop(int) ));

  thumb = new KPushButton;
  thumb->setMinimumSize(75, 75);
  thumb->setMaximumSize(75, 75);
  thumb->setFlat(true);
  connect(thumb, SIGNAL( clicked() ), this, SLOT( runBrowser() ));

  title = new QLabel;
  title->setAlignment( Qt::AlignAuto | Qt::AlignTop | Qt::WordBreak );
  title->setMaximumWidth(200);

  layout->addWidget(combo);
  layout->addWidget(thumb);
  layout->addWidget(title);
  setLayout(layout);
}

void PhotoRow::setPhoto( Photo *photo )
{
  this->photo = photo;

  if (!photo->thumbFile.exists()) {
    QString filename = KStandardDirs::locateLocal("tmp", QFileInfo(photo->thumbUrl.path()).fileName());
    qDebug( filename );
    photo->thumbFile.setFileName( filename );

    if (photo->thumbFile.open( QIODevice::WriteOnly )) {
      KIO::TransferJob *tjob = KIO::get( photo->thumbUrl );
      connect( tjob, SIGNAL( data(KIO::Job *, const QByteArray &) ),
               this, SLOT( dataRecv(KIO::Job *, const QByteArray &) ));
    }
  }
  else {
    thumb->setPixmap( QPixmap( photo->thumbFile.fileName() ) );
    thumb->repaint();
  }

  title->setText( photo->title );
  combo->setCurrentItem( photo->desktop );
}

void PhotoRow::setDesktop( int index )
{
  photo->desktop = index;
}

void PhotoRow::runBrowser()
{
  KRun::runUrl( photo->pageUrl, "text/html", 0 );
}

void PhotoRow::dataRecv( KIO::Job *job, const QByteArray &data )
{
  if (data.size() == 0) {
    photo->thumbFile.close();
    qDebug( photo->thumbFile.fileName() );
    thumb->setPixmap( QPixmap( photo->thumbFile.fileName() ) );
    thumb->repaint();
  }
  else {
    photo->thumbFile.write(data);
  }
}
