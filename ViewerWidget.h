#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QMouseEvent>
#include <vector>
#include <array>  // For std::array
#include "SWCParser.h"  // Include the SWCParser to handle neuron data

class ViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *parent = nullptr);

    // Method for loading neuron data from SWC files
    void loadNeuron(const std::vector<NeuronNode>& nodes);

    public slots:
        void toggleWireframeMode();  // Slot to toggle wireframe mode

protected:
    // OpenGL initialization and resizing
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;  // This method will also handle color assignment based on neuron type

    // Event handling for user interaction
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Custom function to manually draw a cylinder (for neuron branches)
    void drawCylinder(float baseRadius, float topRadius, float height, int slices);

    // Custom function to manually draw a sphere (for neuron soma)
    void drawSphere(float radius, int slices, int stacks);

    // Variables to control model-view transformations
    float zoom;
    float rotationX;
    float rotationY;
    bool isRightMousePressed;
    QPointF lastMousePosition;
    QPointF panOffset;

    // Neuron data
    std::vector<NeuronNode> neuronNodes;    // Stores neuron nodes parsed from SWC
    bool isNeuronLoaded = false;            // Flag to know whether neuron data is loaded

    // Wireframe mode flag
    bool isWireframeMode = false;
};

#endif // VIEWERWIDGET_H
