#include "dbox.h"

DesktopBox::DesktopBox( int desktops, QWidget *parent ) :
  QComboBox( parent )
{
  insertItem("");
  for (int i = 0; i < desktops; i++) {
    insertItem(QString::number(i+1));
  }
  current = currentText();
  
  connect(this, SIGNAL(activated(const QString&)), this, SLOT(setLast(const QString&)));
}

void DesktopBox::setLast(const QString &index) {
  last = current;
  current = index;
  emit desktopChanged(this, last, current);
}
