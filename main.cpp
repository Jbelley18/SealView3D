#include <QApplication>
#include <QScreen>
#include "ViewerWidget.h"
#include "SWCParser.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Get the screen geometry to dynamically adjust the window size
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Set the window size to be, for example, 80% of the screen size
    int windowWidth = static_cast<int>(screenWidth * 0.8);
    int windowHeight = static_cast<int>(screenHeight * 0.8);

    ViewerWidget viewerWidget;
    viewerWidget.resize(windowWidth, windowHeight);

    // Load the SWC file for testing
    SWCParser parser("C:\\Users\\justi\\Downloads\\neuron_nmo\\sweedler\\CNG version\\101711-10_4p5-of-16_initial.CNG.swc");
    std::vector<NeuronNode> neuronData = parser.parse();

    // Debugging: Print the parsed SWC data
    std::cout << "Parsed Neuron Data:\n";
    for (const NeuronNode& node : neuronData) {
        std::cout << "Node ID: " << node.id
                  << ", Type: " << node.type
                  << ", X: " << node.x
                  << ", Y: " << node.y
                  << ", Z: " << node.z
                  << ", Radius: " << node.radius
                  << ", Parent: " << node.parent << std::endl;
    }

    // Load neuron data into the viewer
    viewerWidget.loadNeuron(neuronData);

    viewerWidget.show();
    return app.exec();
}
