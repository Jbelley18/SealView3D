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

class ViewerWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *parent = nullptr);
    void loadModel(const std::string& path);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void processNode(aiNode* node, const aiScene* scene);
    void drawMesh(aiMesh* mesh, const aiScene* scene);

    float zoom;
    float rotationX;
    float rotationY;
    bool isRightMousePressed;
    QPointF lastMousePosition;
    QPointF panOffset;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif // VIEWERWIDGET_H
