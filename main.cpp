#include <kapplication.h>
#include "window.h"

int main( int argc, char **argv )
{
  KApplication a( argc, argv, "kflickr" );

  MainWindow *window = new MainWindow( "Flickr Wallpaper Grabber" );
  window->resize( 300, 200 );

  a.setMainWidget( window );
  window->show();

  return a.exec();
} 
