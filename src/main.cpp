#include <memory>

#include <QApplication>

#include "Gui/MainWindow.h"

#include "Logic/SlideCtrl.h"
#include "Logic/SlideshowCtrl.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto slideCtrl = std::make_shared<Logic::SlideCtrl>();
    auto slideshowCtrl = std::make_shared<Logic::SlideshowCtrl>(slideCtrl);

    MainWindow mainWindow(slideshowCtrl, slideCtrl);
    mainWindow.show();

    return a.exec();
}
