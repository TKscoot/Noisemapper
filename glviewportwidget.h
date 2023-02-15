#ifndef GLVIEWPORTWIDGET_H
#define GLVIEWPORTWIDGET_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>

#include "mesh.h"

class GLViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLViewportWidget();

    virtual ~GLViewportWidget() Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;


    void resizeEvent(QResizeEvent* event) override;

    void render();

public slots:
    void paintGL() Q_DECL_OVERRIDE;
    void repaint();

private:

    Mesh* m_mesh = nullptr;

    float aspect = 16.0f/9.0f;

    double mDeltaTime = 0.0;
};

#endif // GLVIEWPORTWIDGET_H
