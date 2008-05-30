#ifndef __DBOX_H
#define __DBOX_H

#include <qcombobox.h>

class DesktopBox : public QComboBox
{
  Q_OBJECT

public:
  DesktopBox( int desktops, QWidget *parent );

signals:
  void desktopChanged(DesktopBox*, const QString&, const QString&);

private slots:
  void setLast(const QString&);

private:
  QString last, current;
};

#endif
