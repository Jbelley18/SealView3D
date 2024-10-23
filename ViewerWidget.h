#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QMouseEvent>
#include <vector>
#include "SWCParser.h"  // Include the SWCParser to handle neuron data

class ViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *parent = nullptr);

    // Neuron loading method
    void loadNeuron(const std::vector<NeuronNode>& nodes);    // For loading neuron data from SWC files

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Event handling for user interaction
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Custom function to manually draw a cylinder
    void drawCylinder(float baseRadius, float topRadius, float height, int slices);

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
};

#endif // VIEWERWIDGET_H
