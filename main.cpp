#include <QApplication>
#include <QScreen>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QShortcut>
#include "ViewerWidget.h"
#include "ControlPanel.h"
#include "SWCParser.h"

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

    // Create the ViewerWidget instance (3D viewer)
    ViewerWidget *viewerWidget = new ViewerWidget;

    // Create the ControlPanel instance (UI controls)
    ControlPanel *controlPanel = new ControlPanel;

    // Create a horizontal layout to place ControlPanel and ViewerWidget side by side
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(controlPanel);   // Add ControlPanel on the left
    mainLayout->addWidget(viewerWidget);   // Add ViewerWidget on the right

    // Create a container widget to hold the layout
    QWidget window;
    window.setLayout(mainLayout);
    window.resize(windowWidth, windowHeight);

    // Connect the ControlPanel's load button signal to the ViewerWidget's loadNeuron function
    QObject::connect(controlPanel, &ControlPanel::loadSWCFileRequested, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&window, "Open SWC File", "", "SWC Files (*.swc);;All Files (*)");
        if (!filePath.isEmpty()) {
            std::string filePathStd = filePath.toStdString();
            SWCParser parser(filePathStd);
            std::vector<NeuronNode> neuronData = parser.parse();

            // Load neuron data into the viewer
            viewerWidget->loadNeuron(neuronData);
        }
    });

    // Add a keyboard shortcut for toggling ControlPanel visibility with the T key
    QShortcut *toggleShortcut = new QShortcut(QKeySequence("T"), &window);
    QObject::connect(toggleShortcut, &QShortcut::activated, [&]() {
        controlPanel->setVisible(!controlPanel->isVisible());
    });

    // Show the main window with the layout
    window.show();

    return app.exec();
}
