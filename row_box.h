#ifndef __ROW_BOX_H
#define __ROW_BOX_H

#include <QVBoxLayout>
#include "row.h"

class PhotoRowBox : public QWidget
{
  Q_OBJECT

public:
  PhotoRowBox( QWidget *parent );
  void addRow( PhotoRow *row );

private:
  QVBoxLayout *layout;
};

#endif
