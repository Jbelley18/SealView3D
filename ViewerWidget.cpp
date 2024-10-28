#include "ViewerWidget.h"
#include <iostream>
#include <cmath>
#include <GL/gl.h>

ViewerWidget::ViewerWidget(QWidget *parent)
    : QOpenGLWidget(parent), zoom(1.0f), rotationX(0.0f), rotationY(0.0f),
      isRightMousePressed(false), panOffset(0.0f, 0.0f), isWireframeMode(false) {}

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

// Slot to toggle wireframe mode
void ViewerWidget::toggleWireframeMode() {
    isWireframeMode = !isWireframeMode;
    update();  // Trigger a repaint
}

void ViewerWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Toggle between wireframe and solid rendering
    if (isWireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Solid mode
    }

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
        bool somaDrawn = false;

        for (const NeuronNode& node : neuronNodes) {
            switch (node.type) {
                case SOMA:
                    glColor3f(1.0f, 0.0f, 0.0f);
                    break;
                case AXON:
                    glColor3f(0.0f, 0.0f, 1.0f);
                    break;
                case DENDRITE:
                    glColor3f(0.0f, 1.0f, 0.0f);
                    break;
                case APICAL_DENDRITE:
                    glColor3f(1.0f, 1.0f, 0.0f);
                    break;
                case CUSTOM_TYPE_7:
                    glColor3f(1.0f, 0.5f, 0.0f);
                    break;
                default:
                    glColor3f(1.0f, 1.0f, 1.0f);
                    break;
            }

            if (node.type == SOMA && !somaDrawn) {
                glPushMatrix();
                glTranslatef(node.x, node.y, node.z);
                drawSphere(node.radius, 20, 20);
                glPopMatrix();
                somaDrawn = true;
            }
        }

        for (const NeuronNode& node : neuronNodes) {
            if (node.parent != -1 && node.type != SOMA) {
                NeuronNode parent = neuronNodes[node.parent - 1];

                switch (node.type) {
                    case AXON:
                        glColor3f(0.0f, 0.0f, 1.0f);
                        break;
                    case DENDRITE:
                        glColor3f(0.0f, 1.0f, 0.0f);
                        break;
                    case APICAL_DENDRITE:
                        glColor3f(1.0f, 1.0f, 0.0f);
                        break;
                    case CUSTOM_TYPE_7:
                        glColor3f(1.0f, 0.5f, 0.0f);
                        break;
                    default:
                        glColor3f(1.0f, 1.0f, 1.0f);
                        break;
                }

                float dx = node.x - parent.x;
                float dy = node.y - parent.y;
                float dz = node.z - parent.z;
                float height = sqrt(dx * dx + dy * dy + dz * dz);

                glPushMatrix();
                glTranslatef(parent.x, parent.y, parent.z);

                float directionX = dx / height;
                float directionY = dy / height;
                float directionZ = dz / height;
                float angle = acos(directionZ) * 180.0f / M_PI;
                glRotatef(angle, -directionY, directionX, 0.0f);

                drawCylinder(parent.radius, node.radius, height, 20);
                glPopMatrix();
            }
        }
    }

    // Reset to fill mode for other OpenGL content
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ViewerWidget::loadNeuron(const std::vector<NeuronNode>& nodes) {
    neuronNodes = nodes;
    isNeuronLoaded = true;
    update();
}


// Mouse Handling or Movement.

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

// Custom function to draw the cylinder
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

// Custom function to draw a sphere for the soma
void ViewerWidget::drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i <= stacks; ++i) {
        float lat0 = M_PI * (-0.5 + (float)(i) / stacks); // Latitude
        float z0  = radius * sin(lat0); // Z coordinate
        float r0  = radius * cos(lat0); // Radius at latitude

        float lat1 = M_PI * (-0.5 + (float)(i + 1) / stacks);
        float z1 = radius * sin(lat1);
        float r1 = radius * cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2 * M_PI * (float)(j) / slices; // Longitude
            float x = cos(lng); // X coordinate
            float y = sin(lng);

            glNormal3f(x * r0, y * r0, z0); // Normal
            glVertex3f(x * r0, y * r0, z0); // Vertex at latitude 0
            glNormal3f(x * r1, y * r1, z1); // Normal
            glVertex3f(x * r1, y * r1, z1); // Vertex at latitude 1
        }
        glEnd();
    }
}