#include <QApplication>
#include "ViewerWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create an instance of ViewerWidget
    ViewerWidget viewerWidget;
    viewerWidget.resize(1200, 800);  // Set initial window size
    viewerWidget.show();

    return app.exec();
}
