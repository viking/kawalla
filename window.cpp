#include "window.h"

MainWindow::MainWindow ( const char * name ) : KMainWindow ( 0L, name )
{
//  QPopupMenu  *filemenu;
//  KMenuBar    *menu;

  setCaption("Flickr Wallpaper Grabber");
  setIcon( QPixmap( "flickr.png" ) );

//  filemenu = new QPopupMenu;
//  filemenu->insertItem( i18n( "&Quit" ), kapp, SLOT( quit() ) );
//  menu = menuBar();
//  menu->insertItem( i18n( "&File" ), filemenu);

  // global color scheme info
  KConfig *cfg = KGlobal::config();
  cfg->setGroup( "General" );
  alternateBackground = cfg->readColorEntry( "alternateBackground" );

  // desktop info
  dwidth   = KApplication::desktop()->width();
  dheight  = KApplication::desktop()->height();
  dratio   = (float)dwidth / (float)dheight;
  desktops = KWin::numberOfDesktops();

  // layout
  vbox    = new QVBox(this);
  sv      = new KScrollView(vbox);
  central = new QWidget(sv->viewport());
  grid    = new QGridLayout(central, 10, 1, 0, 5);
  vbox->setSpacing(5);
  sv->addChild(central);
  sv->setResizePolicy(QScrollView::AutoOneFit);
  count = 0;
  page  = 0;

  // setup photo rows
  PhotoRow *row;
  for (int i = 0; i < 10; i++) {
    row = new PhotoRow( central, desktops );
    row->setSpacing(5);
    if (i % 2 == 1)
      row->setPaletteBackgroundColor( alternateBackground );

    grid->addWidget(row, i, 0);
    rows.append(row);
  }

  // button setup
  QHBox *box = new QHBox( vbox );
  backButton = new KPushButton( "Back", box );
  goButton   = new KPushButton( "Go!",  box );
  nextButton = new KPushButton( "Next", box );
  connect(backButton, SIGNAL(clicked()), this, SLOT(back()));
  connect(goButton,   SIGNAL(clicked()), this, SLOT(go()));
  connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
  backButton->setEnabled(false);

  setCentralWidget(vbox);
  grabPhotos();
}

MainWindow::~MainWindow()
{
  Photo *photo;
  for (photo = photos.first(); photo; photo = photos.next()) {
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

void MainWindow::addPhoto( QString &thumbUrlStr, QString &photoUrlStr, QString &pageUrlStr, QString &title, QString &, int width, int height, float ratio )
{
  Photo    *photo;
  PhotoRow *row;

  // save photo information for later
  photo = new Photo;
  photo->url      = KURL( photoUrlStr );
  photo->thumbUrl = KURL( thumbUrlStr );
  photo->pageUrl  = KURL( pageUrlStr );
  photo->title    = QString( title );
  photo->width    = width;
  photo->height   = height;
  photo->ratio    = ratio;
  photo->desktop  = 0;
  photos.append( photo );

  if (count < 10) {
    row = rows.at(count);
    row->setPhoto(photo);
  }

  count++;
}

void MainWindow::go() {
  Photo  *photo;
  Image   img;
  int     num, diff, offset;
  QDir    dir = QDir::home();
  DCOPRef desktop;

  desktop.setRef( "kdesktop", "KBackgroundIface" );   // turns out DCOP is the easiest way
  dir.cd( "images" );
  if (!dir.exists("flickr"))
    dir.mkdir("flickr");
  dir.cd( "flickr" );

  for (photo = photos.first(); photo; photo = photos.next()) {
    num = photo->desktop; 
    if (num == 0)
      continue;

    qDebug("width: %d; height: %d; ratio: %f", photo->width, photo->height, photo->ratio);

    KURL destUrl( QString("file://%1/%2").arg(dir.absPath()).arg(photo->url.fileName()) );
    qDebug( destUrl.url() );
    qDebug( photo->url.url() );
    if (KIO::NetAccess::file_copy(photo->url, destUrl, -1, false)) {
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

void MainWindow::next() {
  page++;
  switchPage();
}

void MainWindow::back() {
  page--;
  switchPage();
}

void MainWindow::switchPage()
{
  PhotoRow *row;
  Photo    *photo;
  int count, index;

  count = photos.count();
  index = page * 10;

  for (int i = 0; i < 10; i++, index++) {
    row = rows.at(i);
    if (index < count) {
      photo = photos.at(index);
      row->setPhoto(photo);
      row->show();
    }
    else {
      row->hide();
    }
  }

  backButton->setEnabled(page > 0);
  nextButton->setEnabled(index < count);
}
