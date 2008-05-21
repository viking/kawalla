#include "window.h"

MainWindow::MainWindow ( const char * name ) : KMainWindow ( 0L, name )
{
  QPopupMenu *filemenu;
  KMenuBar   *menu;
  QGrid      *grid;
  QVBox      *vbox;

  setCaption("Flickr Wallpaper Grabber");

  filemenu = new QPopupMenu;
  filemenu->insertItem( i18n( "&Quit" ), kapp, SLOT( quit() ) );
  menu = menuBar();
  menu->insertItem( i18n( "&File" ), filemenu);

  vbox = new QVBox(this);
  grid = new QGrid(2, vbox);
  setCentralWidget(vbox);

  grabPhotos();
}

void MainWindow::grabPhotos()
{
  KURL url( "http://api.flickr.com/services/rest/?api_key=6efcde4b429a5569196d2a99a2669097&method=flickr.interestingness.getList&extras=o_dims,original_format,original_secret&per_page=250" );
  QString tmpFile;
  if( KIO::NetAccess::download( url, tmpFile, this ) )
  {
    QXmlSimpleReader reader;
    FlickrHandler handler( grid );
    QFile file( tmpFile );
    QXmlInputSource source( file );

    reader.setContentHandler( &handler );
    reader.parse( source );

    KIO::NetAccess::removeTempFile( tmpFile );
  } else {
    KMessageBox::error(this, KIO::NetAccess::lastErrorString() );
  }
}
