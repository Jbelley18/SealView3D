#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QMouseEvent>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "SWCParser.h"  // Include the SWCParser to handle neuron data

class ViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *parent = nullptr);

    // Model and Neuron loading methods
    void loadModel(const std::string& path);                  // For loading STL models
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
    // Private helper methods for STL processing
    void processNode(aiNode* node, const aiScene* scene);
    void drawMesh(aiMesh* mesh, const aiScene* scene);

    // Variables to control model-view transformations
    float zoom;
    float rotationX;
    float rotationY;
    bool isRightMousePressed;
    QPointF lastMousePosition;
    QPointF panOffset;

    // STL model data
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Neuron data
    std::vector<NeuronNode> neuronNodes;    // Stores neuron nodes parsed from SWC
    bool isNeuronLoaded = false;            // Flag to know whether neuron data is loaded
};

#endif // VIEWERWIDGET_H
