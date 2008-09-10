#include "window.h"

MainWindow::MainWindow () : KMainWindow ()
{
  setCaption("Flickr Wallpaper Grabber");
  setIcon(
    QPixmap( QString( "%1/flickr.png" ).arg( PWD ) )
  );

  // desktop info
  QRect geometry = KApplication::desktop()->screenGeometry();
  dwidth   = geometry.width();
  dheight  = geometry.height();
  dratio   = (float)dwidth / (float)dheight;
  desktops = KWindowSystem::numberOfDesktops();

  // layout
  vbox    = new Q3VBox(this);
  sv      = new Q3ScrollView(vbox);
  central = new QWidget(sv->viewport());
  grid    = new Q3GridLayout(central, 10, 1, 0, 5);
  vbox->setSpacing(5);
  sv->addChild(central);
  sv->setResizePolicy(Q3ScrollView::AutoOneFit);
  count = 0;
  page  = 0;

  // setup photo rows
  PhotoRow *row;
  for (int i = 0; i < 10; i++) {
    row = new PhotoRow( central, desktops );
    row->hide();
    row->setSpacing(5);

    grid->addWidget(row, i, 0);
    rows.append(row);
  }

  // button setup
  Q3HBox *box = new Q3HBox( vbox );
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
    KIO::NetAccess::removeTempFile(photo->thumbfn);
    delete photo;
  }
}

void MainWindow::grabPhotos()
{
  KUrl url( "http://api.flickr.com/services/rest/?api_key=6efcde4b429a5569196d2a99a2669097&method=flickr.interestingness.getList&extras=o_dims,original_format,original_secret&per_page=200" );
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
  photo->url      = KUrl( photoUrlStr );
  photo->thumbUrl = KUrl( thumbUrlStr );
  photo->pageUrl  = KUrl( pageUrlStr );
  photo->title    = QString( title );
  photo->thumbfn  = QString::null;
  photo->width    = width;
  photo->height   = height;
  photo->ratio    = ratio;
  photo->desktop  = 0;
  photos.append( photo );

  if (count < 10) {
    row = rows.at(count);
    row->setPhoto(photo);
    row->show();
  }

  count++;
}

void MainWindow::go() {
  Photo  *photo;
  Image   img;
  int     num, diff, offset;
  QDir    dir = QDir::home();

  dir.cd( "Pictures" );
  if (!dir.exists("kawalla"))
    dir.mkdir("kawalla");
  dir.cd( "kawalla" );

  for (photo = photos.first(); photo; photo = photos.next()) {
    num = photo->desktop;
    if (num == 0)
      continue;

    qDebug("width: %d; height: %d; ratio: %f", photo->width, photo->height, photo->ratio);

    KUrl destUrl( QString("file://%1/%2").arg(dir.absPath()).arg(photo->url.fileName()) );
    qDebug( destUrl.url() );
    qDebug( photo->url.url() );
    if (KIO::NetAccess::file_copy(photo->url, destUrl, this)) {
      QByteArray tmpArray = destUrl.path().toAscii();
      char * destPath = tmpArray.data();
      img.read(destPath);

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
      qDebug( QString( "width: %1, height: %2" ).arg(img.size().width()).arg(img.size().height()) );
      img.write(destPath);
    }
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
