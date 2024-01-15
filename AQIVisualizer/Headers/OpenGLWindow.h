#pragma once

#include "Constants.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QList>
#include <QVector3D>

class QOpenGLShader;
class QOpenGLShaderProgram;

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

signals:
    void shapesUpdated();

public:
    OpenGLWindow(const QColor& background, QWidget* parent);
    ~OpenGLWindow();
    void updateShape(QVector<GLfloat>& vertices, QVector<GLfloat>& colors);
    void updateShape(QVector<QVector<GLfloat>>& vertices, QVector<QVector<GLfloat>>& colors);
    void mouseMoveEvent(QMouseEvent* event);
  
protected:
    void paintGL() override;
    void initializeGL() override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void shaderWatcher();

private:
    void reset();
    void setupMatrix();
    void drawVertices(const QVector<GLfloat> vertices, const QVector<GLfloat> colors, GLenum mode);
    void zoomOut();
    void zoomIn();
    QString readShader(QString path);

private:
    bool mAnimating = false;
    QOpenGLContext* mContext = nullptr;
    QOpenGLShader* mVshader = nullptr;
    QOpenGLShader* mFshader = nullptr;
    QOpenGLShaderProgram* mProgram = nullptr;
    QOpenGLBuffer mVbo;
    GLint mVertexAttr = 0;
    GLint mNormalAttr = 0;
    GLint mPosAttr = 0;
    GLint mColAttr = 0;
    GLint mMatrixUniform = 0;
    QColor mBackground;
    QQuaternion mRotationAngle;
    QVector3D mPanTranslationFactor;
    QPoint mLastPos;
    QMetaObject::Connection mContextWatchConnection;
    QFileSystemWatcher* mShaderWatcher;

    QVector<GLfloat> mVertices;
    QVector<GLfloat> mColors;
    QVector<QVector<GLfloat>> mShapeColors;
    QVector<QVector<GLfloat>> mShapeVertices;
    GLint mScaleFactor=1;
};
