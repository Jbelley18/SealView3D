#include <QApplication>
#include <QScreen>
#include "ViewerWidget.h"
#include "SWCParser.h"
#include <iostream>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Get the screen geometry to dynamically adjust the window size
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Set the window size to be 80% of the screen size
    int windowWidth = static_cast<int>(screenWidth * 0.8);
    int windowHeight = static_cast<int>(screenHeight * 0.8);

    // Create the ViewerWidget instance
    ViewerWidget *viewerWidget = new ViewerWidget;

    // Create a "Load File" button
    QPushButton *loadFileButton = new QPushButton("Load SWC File");

    // Create a vertical layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(loadFileButton);   // Add the button at the top
    layout->addWidget(viewerWidget);     // Add the ViewerWidget below the button

    // Create a container widget to hold the layout
    QWidget window;
    window.setLayout(layout);
    window.resize(windowWidth, windowHeight);

    // Connect the button click event to the file loading logic
    QObject::connect(loadFileButton, &QPushButton::clicked, [&](){
        QString filePath = QFileDialog::getOpenFileName(&window, "Open SWC File", "", "SWC Files (*.swc);;All Files (*)");
        if (!filePath.isEmpty()) {
            std::string filePathStd = filePath.toStdString();
            SWCParser parser(filePathStd);
            std::vector<NeuronNode> neuronData = parser.parse();

            // Load neuron data into the viewer
            viewerWidget->loadNeuron(neuronData);
        }
    });

    // Show the window with the layout
    window.show();

    return app.exec();
}

