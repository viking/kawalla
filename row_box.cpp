#include "row_box.h"

PhotoRowBox::PhotoRowBox( QWidget *parent ) : QWidget( parent )
{
  layout = new QVBoxLayout;
  setLayout(layout);
}

void PhotoRowBox::addRow( PhotoRow *row )
{
  layout->addWidget(row);
}
