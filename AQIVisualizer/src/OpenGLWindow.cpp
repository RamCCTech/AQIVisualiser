#include "stdafx.h"
#include "OpenGLWindow.h"
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QRandomGenerator>
#include <QDomDocument>
#include "Point3D.h"
#include "KMLReader.h"

OpenGLWindow::OpenGLWindow(const QColor& background, QWidget* parent) : mBackground(background)
{
    setParent(parent);
    setMinimumSize(300, 250);
}

OpenGLWindow::~OpenGLWindow()
{
    reset();
}

void OpenGLWindow::reset()
{
    makeCurrent();
    delete mProgram;
    mProgram = nullptr;
    delete mVshader;
    mVshader = nullptr;
    delete mFshader;
    mFshader = nullptr;
    mVbo.destroy();
    doneCurrent();

    QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::initializeGL()
{
    // Vertex shader source code
    static const char* vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute highp vec3 normalAttr;\n" // New normal attribute
    "varying lowp vec4 col;\n"
    "varying highp vec3 normal;\n"       // New varying normal
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   normal = normalAttr;\n"         // Pass normal to the fragment shader
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

    // Fragment shader source code
    static const char* fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";

    initializeOpenGLFunctions();

    // Create and link shader program
    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mProgram->link();

    // Get attribute and uniform locations
    m_posAttr = mProgram->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = mProgram->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = mProgram->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
}

void OpenGLWindow::setupMatrix()
{
    QMatrix4x4 matrix;
    matrix.ortho(-30.0f, 30.0f, -20.0f, 20.0f, -100.0f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(rotationAngle);
    matrix.scale(scaleFactor);
    mProgram->setUniformValue(m_matrixUniform, matrix);
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mProgram->bind();
    setupMatrix();

    // Draw each shape with its vertices and colors
    for (int i = 0; i < mShapeVertices.size(); i++) {
        drawVertices(mShapeVertices[i], mShapeColors[i], GL_TRIANGLE_FAN);
    }
    mProgram->release();
}

void OpenGLWindow::updateShape(QVector<GLfloat>& vertices, QVector<GLfloat>& colors)
{
    mVertices = vertices;
    mColors = colors;
    update();
}

void OpenGLWindow::updateShape(QVector<QVector<GLfloat>>& vertices, QVector<QVector<GLfloat>>& colors)
{
    mShapeVertices = vertices;
    mShapeColors = colors;
    update();
}

void OpenGLWindow::drawVertices(const QVector<GLfloat> vertices, const QVector<GLfloat> colors, GLenum mode)
{
    glEnable(GL_DEPTH_TEST);
    // Set attribute pointers
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    // Draw vertices
    glDrawArrays(mode, 0, vertices.size() / 2);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.1f * dx);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.1f * dy);

        rotationAngle = rotX * rotY * rotationAngle;
        update();
    }
    lastPos = event->pos();
}

void OpenGLWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        zoomIn();
    }
    else {
        zoomOut();
    }
}

void OpenGLWindow::zoomIn()
{
    scaleFactor *= 1.1f;
    update();
}

void OpenGLWindow::zoomOut()
{
    scaleFactor /= 1.1f;
    update();
}