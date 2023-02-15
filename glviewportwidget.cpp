#include "glviewportwidget.h"
#include <QApplication>
#include <QTime>
#include <QTimer>
#include <QtMath>
#include <cmath>
#include <thread>

GLViewportWidget::GLViewportWidget()
{

}

GLViewportWidget::~GLViewportWidget()
{
    // resource cleanup

    // since we release resources related to an OpenGL context,
    // we make this context current before cleaning up our resources
    makeCurrent();


}

void GLViewportWidget::initializeGL()
{
    // this function is called once, when the window is first shown, i.e. when
    // the the window content is first rendered
    this->initializeOpenGLFunctions();
    if(context()->currentContext()->isValid()){
        qDebug() << "init";
    }

    m_mesh = new Mesh("assets/DamagedHelmet.gltf");

    // Set frame update interval
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(1); // update 1/1000 times in second

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void GLViewportWidget::paintGL()
{
    render();
}

void GLViewportWidget::repaint()
{
    //render();
}

void GLViewportWidget::resizeEvent(QResizeEvent *event)
{
    auto s = std::to_string( (float)(this->parentWidget()->size().width()) / (float)(this->parentWidget()->size().height()));
    qDebug() << QString::fromStdString(s);
    aspect = (float)(this->parentWidget()->size().width()) / (float)(this->parentWidget()->size().height());

    if(!isInitialized(this->QOpenGLFunctions::d_ptr)){
        return;
    }
    glViewport(0, 0, this->parentWidget()->size().width(), this->parentWidget()->size().height());
}

void GLViewportWidget::render()
{


    static auto lastTime = std::chrono::high_resolution_clock::now();
    static float timer = 0.0f;
    timer += mDeltaTime;

    auto newtime = std::chrono::high_resolution_clock::now();
    lastTime = std::chrono::high_resolution_clock::now();

    //std::cout << "dt: " << std::to_string(std::sin(timer * 100.0f)) << std::endl;

    // set the background color = clear color
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    QVector3D camPos = QVector3D(0.0f, 0.0f, 10.0f);
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camPos, camPos + QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f));

    QMatrix4x4 proj;
    proj.setToIdentity();
    proj.perspective(40, 1.0f * this->size().width() / this->size().height(), 0.1, 100.0f);


    m_mesh->m_program->setUniformValue("view", view);
    m_mesh->m_program->setUniformValue("proj", proj);

    m_mesh->render(mDeltaTime);



    newtime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = newtime - lastTime;
    mDeltaTime = delta.count();


    newtime = lastTime;
}
