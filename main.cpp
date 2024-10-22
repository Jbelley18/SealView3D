#include <QApplication>
#include "ViewerWidget.h"
#include "SWCParser.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ViewerWidget viewerWidget;
    viewerWidget.resize(1200, 800);

    // Load the SWC file for testing
    SWCParser parser("C:\\Users\\justi\\Downloads\\neuron_nmo\\sweedler\\CNG version\\101711-10_4p5-of-16_initial.CNG.swc");
    std::vector<NeuronNode> neuronData = parser.parse();
    viewerWidget.loadNeuron(neuronData);  // Load neuron data into the viewer

    viewerWidget.show();
    return app.exec();
}
