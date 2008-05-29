#include "window.h"

MainWindow::MainWindow ( const char * name ) : KMainWindow ( 0L, name )
{
//  QPopupMenu  *filemenu;
//  KMenuBar    *menu;

  setCaption("Flickr Wallpaper Grabber");

//  filemenu = new QPopupMenu;
//  filemenu->insertItem( i18n( "&Quit" ), kapp, SLOT( quit() ) );
//  menu = menuBar();
//  menu->insertItem( i18n( "&File" ), filemenu);

  // desktop info
  dwidth   = KApplication::desktop()->width();
  dheight  = KApplication::desktop()->height();
  dratio   = (float)dwidth / (float)dheight;
  desktops = KWin::numberOfDesktops();
  count    = 0;

  central = new QWidget(this);
  grid    = new QGridLayout(central, 11, 3, 0, 3);
  grid->setColSpacing(0, 10);
  grid->setColSpacing(1, 75);
  grid->setColSpacing(2, 200);

  goButton = new KPushButton( "Go!", central );
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  grid->addMultiCellWidget( goButton, 10, 10, 0, 2 ); 

  setCentralWidget(central);
  grabPhotos();
}

MainWindow::~MainWindow()
{
  Photo *photo;
  for (photo = photos.first(); photo; photo = photos.next()) {
    delete photo->url;
    delete photo;
  }
}

void MainWindow::grabPhotos()
{
  KURL url( "http://api.flickr.com/services/rest/?api_key=6efcde4b429a5569196d2a99a2669097&method=flickr.interestingness.getList&extras=o_dims,original_format,original_secret&per_page=250" );
  QString tmpFile;
  if( KIO::NetAccess::download( url, tmpFile, this ) )
  {
    QXmlSimpleReader reader;
    FlickrHandler handler( this );
    QFile file( tmpFile );
    QXmlInputSource source( file );

    reader.setContentHandler( &handler );
    reader.parse( source );

    KIO::NetAccess::removeTempFile( tmpFile );
  } else {
    KMessageBox::error(this, KIO::NetAccess::lastErrorString() );
  }
}

void MainWindow::addFlickr( QString &thumbUrlStr, QString &photoUrlStr, QString &title, QString &id, int width, int height, float ratio )
{
  QLabel    *label;
  QLineEdit *ledit;
  Photo     *photo;
  QString    tmpFile;
  KURL       thumbUrl( thumbUrlStr );

  ledit = new QLineEdit( central );
//  if (count < desktops)
//    ledit->setText(QString::number(count+1));
  grid->addWidget(ledit, count, 0); 

  KIO::NetAccess::download( thumbUrl, tmpFile, this );
  label = new QLabel( central );
  label->setPixmap( QPixmap( tmpFile ) );
  grid->addWidget(label, count, 1);
  KIO::NetAccess::removeTempFile(tmpFile);

  label = new QLabel( central );
  label->setTextFormat(Qt::RichText);
  label->setText( QString("<b>Title:</b> %1").arg(title) );
  grid->addWidget(label, count, 2);

  // save photo information for later
  photo = new Photo;
  photo->url    = new KURL( photoUrlStr );
  photo->box    = ledit;
  photo->width  = width;
  photo->height = height;
  photo->ratio  = ratio;
  photos.append( photo );

  count++;
}

void MainWindow::go() {
  Photo  *photo;
  Image   img;
  QString ltext;
  bool    ok;
  int     num, diff, offset;
  QDir    dir = QDir::home();
  DCOPRef desktop;

  desktop.setRef( "kdesktop", "KBackgroundIface" );   // turns out DCOP is the easiest way
  dir.cd( "images" );
  if (!dir.exists("flickr"))
    dir.mkdir("flickr");
  dir.cd( "flickr" );

  for (photo = photos.first(); photo; photo = photos.next()) {
    ltext = photo->box->text();
    if (ltext.isEmpty())
      continue;

    num = ltext.toInt(&ok);
    if (!ok || num < 1 || num > desktops)
      continue;

    qDebug("width: %d; height: %d; ratio: %f", photo->width, photo->height, photo->ratio);

    KURL destUrl( QString("file://%1/%2").arg(dir.absPath()).arg(photo->url->fileName()) );
    qDebug( destUrl.url() );
    qDebug( photo->url->url() );
    if (KIO::NetAccess::file_copy(*(photo->url), destUrl, -1, false)) {
      img.read(destUrl.path());    
      
      if (photo->ratio < dratio) {
        // height needs to be changed
        diff   = (int)roundf(photo->height - (photo->width / dratio));
        offset = diff / 2;
        img.chop(Geometry(0, offset));
        img.crop(Geometry(photo->width, photo->height - diff));
      }
      else if (photo->ratio > dratio) {
        // width needs to be changed
        diff   = (int)roundf(photo->width - (photo->height * dratio));
        offset = diff / 2;
        img.chop(Geometry(offset, 0));
        img.crop(Geometry(photo->width - diff, photo->height));
      }

      img.scale(Geometry(dwidth, dheight));
      img.write(destUrl.path());
    }
    desktop.call( "setWallpaper", num, destUrl.path(), 1 );
  }
}
