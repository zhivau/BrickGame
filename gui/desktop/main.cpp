#include <QApplication>

#include "menu_desktop_view.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MenuDesktopView w;
  w.show();
  return a.exec();
}
