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

  // desktop info
  dwidth   = KApplication::desktop()->width();
  dheight  = KApplication::desktop()->height();
  dratio   = (float)dwidth / (float)dheight;
  desktops = KWin::numberOfDesktops();
  count    = 0;
  selected = new bool[desktops+1];
  for (int i = 0; i <= desktops; i++)
    selected[i] = false;

  sv      = new KScrollView(this);
  central = new QWidget(sv->viewport());
  grid    = new QGridLayout(central, 11, 3, 0, 0);
  grid->setColSpacing(0, 10);
  grid->setColSpacing(1, 75);
  grid->setColSpacing(2, 215);
  sv->addChild(central);

  goButton = new KPushButton( "Go!", central );
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  grid->addMultiCellWidget( goButton, 10, 10, 0, 2 ); 

  setCentralWidget(sv);
  grabPhotos();
}

MainWindow::~MainWindow()
{
  Photo *photo;
  for (photo = photos.first(); photo; photo = photos.next()) {
    delete photo->url;
    delete photo;
  }
  delete [] selected;
}

void MainWindow::grabPhotos()
{
  KURL url( "http://api.flickr.com/services/rest/?api_key=6efcde4b429a5569196d2a99a2669097&method=flickr.interestingness.getList&extras=o_dims,original_format,original_secret&per_page=100" );
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
  QLabel     *label;
  DesktopBox *dbox;
  Photo      *photo;
  QString     tmpFile;
  KURL        thumbUrl( thumbUrlStr );

  dbox = new DesktopBox( desktops, central );
  connect(dbox, SIGNAL(desktopChanged(DesktopBox*, const QString&, const QString&)), 
      this, SLOT(updateBoxes(DesktopBox*, const QString&, const QString&)));
  grid->addWidget(dbox, count, 0); 

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
  photo->box    = dbox;
  photo->width  = width;
  photo->height = height;
  photo->ratio  = ratio;
  photos.append( photo );

  count++;
}

void MainWindow::go() {
  Photo  *photo;
  Image   img;
  QString ctext;
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
    ctext = photo->box->currentText();
    if (ctext.isEmpty())
      continue;

    num = ctext.toInt(&ok);
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

void MainWindow::updateBoxes(DesktopBox *sender, const QString &last, const QString &current) {
  Photo        *photo;
  DesktopBox   *box;
  QListBox     *lb;
  QListBoxItem *item;
  int           current_d, last_d, tmp_d, i_pos, r_pos;

  current_d = current.isEmpty() ? 0 : current.toInt();
  last_d    = last.isEmpty()    ? 0 : last.toInt();
  for (photo = photos.first(); photo; photo = photos.next()) {
    box = photo->box;
    if (box == sender)
      continue;

    // figure out where to add/remove items
    lb    = box->listBox();
    i_pos = r_pos = -1;
    for (int i = (int)lb->count()-1; i >= 0; i--) {
      item  = lb->item(i);
      tmp_d = item->text().isEmpty() ? 0 : item->text().toInt();

      if (last_d && i_pos < 0 && tmp_d < last_d)
        i_pos = i+1;
      if (current_d && r_pos < 0 && tmp_d == current_d)
        r_pos = i;
    }

    // delete item that is now selected and add back the one that was de-selected
    if (r_pos > 0) {
      box->removeItem(r_pos);
      if (i_pos > r_pos)
        i_pos--;
    }

    if (i_pos > 0) {
      box->insertItem(last, i_pos);
      if (i_pos == box->currentItem())
        box->setCurrentItem(i_pos+1);
    }
  }

  if (!last.isEmpty())
    selected[last.toInt()] = false;
  if (!current.isEmpty())
    selected[current.toInt()] = true;
}
