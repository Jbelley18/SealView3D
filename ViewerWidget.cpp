#include "ViewerWidget.h"
#include <iostream>
#include <cmath>  // For std::tan and other math functions
#include <GL/gl.h>  // For OpenGL functions

ViewerWidget::ViewerWidget(QWidget *parent)
    : QOpenGLWidget(parent), zoom(1.0f), rotationX(0.0f), rotationY(0.0f),
      isRightMousePressed(false), panOffset(0.0f, 0.0f) {}

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

// Custom function to draw the cylinder manually
void ViewerWidget::drawCylinder(float baseRadius, float topRadius, float height, int slices) {
    float angleStep = 2.0f * M_PI / slices;

    // Draw the sides of the cylinder
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= slices; ++i) {
        float angle = i * angleStep;
        float x = cos(angle);
        float y = sin(angle);

        // Bottom circle
        glVertex3f(baseRadius * x, baseRadius * y, 0.0f);

        // Top circle
        glVertex3f(topRadius * x, topRadius * y, height);
    }
    glEnd();

    // Draw the base of the cylinder
    if (baseRadius > 0.0f) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, 0.0f);  // Center of the base
        for (int i = 0; i <= slices; ++i) {
            float angle = i * angleStep;
            float x = cos(angle);
            float y = sin(angle);
            glVertex3f(baseRadius * x, baseRadius * y, 0.0f);
        }
        glEnd();
    }

    // Draw the top of the cylinder
    if (topRadius > 0.0f) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, height);  // Center of the top
        for (int i = 0; i <= slices; ++i) {
            float angle = i * angleStep;
            float x = cos(angle);
            float y = sin(angle);
            glVertex3f(topRadius * x, topRadius * y, height);
        }
        glEnd();
    }
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
    glTranslatef(panOffset.x(), panOffset.y(), -20.0f * zoom);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    if (isNeuronLoaded) {
        // Iterate over each neuron node and draw a cylinder between the node and its parent
        for (const NeuronNode& node : neuronNodes) {
            if (node.parent != -1) {
                NeuronNode parent = neuronNodes[node.parent - 1];

                // Calculate the height of the cylinder (distance between the parent and child node)
                float dx = node.x - parent.x;
                float dy = node.y - parent.y;
                float dz = node.z - parent.z;
                float height = sqrt(dx * dx + dy * dy + dz * dz);

                // Calculate the direction of the cylinder to be drawn
                float directionX = dx / height;
                float directionY = dy / height;
                float directionZ = dz / height;

                // Move to the parent node's position
                glPushMatrix();
                glTranslatef(parent.x, parent.y, parent.z);

                // Compute the rotation needed to align the cylinder with the child node
                // You'll need more complex rotation logic here for 3D alignment (see below)
                float angle = acos(directionZ) * 180.0f / M_PI;  // Rotate based on z-axis alignment
                glRotatef(angle, -directionY, directionX, 0.0f);

                // Draw the cylinder with varying radius from parent to child node
                drawCylinder(parent.radius, node.radius, height, 20);

                glPopMatrix();
            }
        }
    }
}

void ViewerWidget::loadNeuron(const std::vector<NeuronNode>& nodes) {
    neuronNodes = nodes;
    isNeuronLoaded = true;
    update();  // Trigger a repaint
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
