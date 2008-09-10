#include <kapplication.h>
#include <kaboutdata.h>
#include "window.h"

int main( int argc, char **argv )
{

  KAboutData aboutData( "kawalla", "kawalla",
      ki18n("Flickr Wallpaper Grabber"), "1.0",
      ki18n("Application to download images from Flickr to use as wallpaper"),
      KAboutData::License_GPL,
      ki18n("Copyright (c) 2008 Jeremy Stephens") );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication a;

  MainWindow *window = new MainWindow();
  window->resize( 400, 600 );

  a.setMainWidget( window );
  window->show();

  return a.exec();
}
