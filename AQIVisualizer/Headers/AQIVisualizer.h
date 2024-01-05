#pragma once

#include "OpenGLWindow.h"

#include <QtCore/QVariant>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class AQIVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    AQIVisualizer(QWidget *parent = nullptr);
    ~AQIVisualizer();

private:
    void setupUi();

private:
    QWidget* mCentralWidget;
    QWidget* mGridLayoutWidget;
    QGridLayout* mGridLayout;
    QVBoxLayout* mVerticalLayout;
    QHBoxLayout* mHorizontalLayout;
    QLabel* mLabel;
    QDateEdit* mDateEdit;
    QPushButton* mPushButton;
    OpenGLWindow* mOpenGLWidget;
};
