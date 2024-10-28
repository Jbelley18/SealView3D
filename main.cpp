#include <QApplication>
#include <QScreen>
#include <QSplitter>
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
    controlPanel->setMinimumWidth(200);     // Optional: Minimum width
    controlPanel->setMaximumWidth(600);     // Optional: Maximum width (set to a wider limit)

    // Create a QSplitter to allow resizing between ControlPanel and ViewerWidget
    QSplitter *splitter = new QSplitter;
    splitter->addWidget(controlPanel);
    splitter->addWidget(viewerWidget);
    splitter->setSizes({250, windowWidth - 250});  // Initial proportions

    // Create a container widget to hold the splitter in a layout
    QWidget window;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(splitter);
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

    // Connect the wireframe toggle button to the ViewerWidget's toggleWireframeMode slot
    QObject::connect(controlPanel->wireframeButton, &QPushButton::clicked, viewerWidget, &ViewerWidget::toggleWireframeMode);

    // Add a keyboard shortcut for toggling ControlPanel visibility with the T key
    int lastWidth = controlPanel->width();
    QShortcut *toggleShortcut = new QShortcut(QKeySequence("T"), &window);
    QObject::connect(toggleShortcut, &QShortcut::activated, [&]() {
        if (controlPanel->isVisible()) {
            lastWidth = controlPanel->width();
            controlPanel->hide();
        } else {
            controlPanel->show();
            controlPanel->setFixedWidth(lastWidth);  // Restore the last known width
        }
    });

    // Show the main window with the layout
    window.show();

    return app.exec();
}
