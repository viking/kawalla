#include "window.h"

MainWindow::MainWindow ( const char * name ) : KMainWindow ( 0L, name )
{
  QPopupMenu  *filemenu;
  KMenuBar    *menu;

  setCaption("Flickr Wallpaper Grabber");

  filemenu = new QPopupMenu;
  filemenu->insertItem( i18n( "&Quit" ), kapp, SLOT( quit() ) );
  menu = menuBar();
  menu->insertItem( i18n( "&File" ), filemenu);

  central = new QWidget(this);
  grid    = new QGridLayout(central, 11, 3, 0, 5);
  count   = 0;

  goButton = new KPushButton( "Go!", central );
  grid->addMultiCellWidget( goButton, 10, 10, 0, 2 ); 

  setCentralWidget(central);
  grabPhotos();
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

void MainWindow::addFlickr( QString &thumbUrl, QString &photoUrl, QString &title, QString &id )
{
  QLabel  *label;
  QString tmpFile;
  KURL thumb( thumbUrl );
  KURL photo( photoUrl );

  grid->addWidget(new QCheckBox( central ), count, 0); 

  KIO::NetAccess::download( thumb, tmpFile, this );
  label = new QLabel( central );
  label->setPixmap( QPixmap( tmpFile ) );
  grid->addWidget(label, count, 1);
  KIO::NetAccess::removeTempFile(tmpFile);

  label = new QLabel( central );
  label->setTextFormat(Qt::RichText);
  label->setText( QString("<b>Title:</b> %1").arg(title) );
  grid->addWidget(label, count, 2);

  count++;
}
