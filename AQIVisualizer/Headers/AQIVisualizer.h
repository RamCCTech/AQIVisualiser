#pragma once

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
    QWidget* centralWidget;
    QWidget* gridLayoutWidget;
    QGridLayout* gridLayout;
    QVBoxLayout* verticalLayout;
    QHBoxLayout* horizontalLayout;
    QLabel* label;
    QDateEdit* dateEdit;
    QPushButton* pushButton;
    QOpenGLWidget* openGLWidget;
};
