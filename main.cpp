#include <kapplication.h>
#include "window.h"

int main( int argc, char **argv )
{
  KApplication a( argc, argv, "kawalla" );

  MainWindow *window = new MainWindow( "Flickr Wallpaper Grabber" );
  window->resize( 400, 600 );

  a.setMainWidget( window );
  window->show();

  return a.exec();
} 
