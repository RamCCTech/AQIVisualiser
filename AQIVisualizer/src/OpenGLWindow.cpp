#include "stdafx.h"
#include "OpenGLWindow.h"
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QRandomGenerator>
#include <QDomDocument>
#include <GL/GL.h>
#include "Point3D.h"
#include "KMLReader.h"

OpenGLWindow::OpenGLWindow(const QColor& background, QWidget* parent) : mBackground(background)
{
    setParent(parent);
    setMinimumSize(300, 250);

    const QStringList pathList = { Constants::VertexShaderPath ,Constants::FragmentShaderPath };
    mShaderWatcher = new QFileSystemWatcher(pathList, this);
    connect(mShaderWatcher, &QFileSystemWatcher::fileChanged, this, &OpenGLWindow::shaderWatcher);
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
    QString vertexShaderSource = readShader(Constants::VertexShaderPath);
    QString fragmentShaderSource = readShader(Constants::FragmentShaderPath);

    initializeOpenGLFunctions();


    // Create and link shader program
    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mProgram->link();

    // Get attribute and uniform locations
    mPosAttr = mProgram->attributeLocation("posAttr");
    Q_ASSERT(mPosAttr != -1);
    mColAttr = mProgram->attributeLocation("colAttr");
    Q_ASSERT(mColAttr != -1);
    mMatrixUniform = mProgram->uniformLocation("matrix");
    Q_ASSERT(mMatrixUniform != -1);

}

void OpenGLWindow::setupMatrix()
{
    QMatrix4x4 matrix;
    QMatrix4x4 matrixProj;
    QMatrix4x4 matrixModel;

    // Get the size of the OpenGL window
    QSize screenSize = size();
    matrixProj.setToIdentity();
    matrixModel.setToIdentity();

    // Adjust the orthographic projection matrix based on screen size
    qreal aspectRatio = static_cast<qreal>(screenSize.width()) / screenSize.height();
    qreal orthoWidth = 50.0f;
    qreal orthoHeight = orthoWidth / aspectRatio;

    matrixProj.ortho(-orthoWidth / 2, orthoWidth / 2, -orthoHeight / 2, orthoHeight / 2, -100.0f, 100.0f);

    // Apply transformations to the model matrix
    matrixModel.translate(mPanTranslationFactor);
    matrixModel.rotate(mRotationAngle);
    matrixModel.scale(mScaleFactor);

    // Combine the projection and model matrices
    matrix = matrixProj * matrixModel;

    mProgram->setUniformValue(mMatrixUniform, matrix);
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
    // Set attribute pointers
    glVertexAttribPointer(mPosAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glVertexAttribPointer(mColAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());

    glEnableVertexAttribArray(mPosAttr);
    glEnableVertexAttribArray(mColAttr);

    // Draw vertices
    glDrawArrays(mode, 0, vertices.size() / 3);

    glDisableVertexAttribArray(mColAttr);
    glDisableVertexAttribArray(mPosAttr);
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - mLastPos.x();
    int dy = event->y() - mLastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.1f * dx);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.1f * dy);

        mRotationAngle = rotX * rotY * mRotationAngle;
    }
    else if (event->buttons() & Qt::RightButton)
    {
        // Use the pan translation factor only when the right mouse button is pressed
        mPanTranslationFactor += QVector3D(0.01f * dx, -0.01f * dy, 0.0f);
        
    }
    update();
    mLastPos = event->pos();
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
    mScaleFactor *= 1.1f;
    update();
}

void OpenGLWindow::zoomOut()
{
    mScaleFactor /= 1.1f;
    update();
}

QString OpenGLWindow::readShader(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Invalid file!";

    QTextStream stream(&file);
    QString line = stream.readLine();
    while (!stream.atEnd()) {
        line.append(stream.readLine());
    }
    file.close();
    return line;
}

void OpenGLWindow::shaderWatcher()
{
    QString vertexShaderSource = readShader(Constants::VertexShaderPath);
    QString fragmentShaderSource = readShader(Constants::FragmentShaderPath);

    mProgram->release();
    mProgram->removeAllShaders();
    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    mProgram->link();
}