#include "mygraph.h"
#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  MyGraph w;
  w.show();

  return a.exec();
}
