#include "ViewerWidget.h"
#include <iostream>
#include <cmath> // For std::tan
#include <QPushButton>
#include <QMenuBar>
#include <QHBoxLayout>

ViewerWidget::ViewerWidget(QWidget *parent)
    : QOpenGLWidget(parent), zoom(1.0f), rotationX(0.0f), rotationY(0.0f),
      isRightMousePressed(false), panOffset(0.0f, 0.0f) {
    // Load the STL model from the given path
    loadModel("C:/Users/justi/Downloads/ArticulatedSealPup.stl");
}

void ViewerWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void ViewerWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    float aspectRatio = static_cast<float>(w) / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    float top = nearPlane * std::tan(fov * M_PI / 360.0);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;

    glFrustum(left, right, bottom, top, nearPlane, farPlane);
}

void ViewerWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = static_cast<float>(width()) / height();
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    float top = nearPlane * std::tan(fov * M_PI / 360.0);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;

    glFrustum(left, right, bottom, top, nearPlane, farPlane);

    // Set up model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(panOffset.x(), panOffset.y(), -20.0f * zoom); // Adjust zoom here
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // Draw the loaded model
    glBegin(GL_TRIANGLES);
    for (unsigned int i : indices) {
        glVertex3f(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    }
    glEnd();
}

void ViewerWidget::wheelEvent(QWheelEvent *event) {
    zoom *= (event->angleDelta().y() > 0) ? 1.1f : 0.9f;
    update();
}

void ViewerWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePosition = event->position();  // Capture current position
    if (event->button() == Qt::RightButton) {
        isRightMousePressed = true;
    }
}

void ViewerWidget::mouseMoveEvent(QMouseEvent *event) {
    QPointF currentMousePosition = event->position();
    float dx = currentMousePosition.x() - lastMousePosition.x();
    float dy = currentMousePosition.y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy;
        rotationY += dx;
    } else if (isRightMousePressed) {
        panOffset += QPointF(dx * 0.01f, -dy * 0.01f);  // Adjust for panning sensitivity
    }

    lastMousePosition = currentMousePosition;
    update();
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isRightMousePressed = false;
    }
}

// Load model using Assimp
void ViewerWidget::loadModel(const std::string& path) {
    vertices.clear();
    indices.clear();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void ViewerWidget::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        drawMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void ViewerWidget::drawMesh(aiMesh* mesh, const aiScene* scene) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D position = mesh->mVertices[i];
        vertices.push_back(position.x);
        vertices.push_back(position.y);
        vertices.push_back(position.z);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
}
