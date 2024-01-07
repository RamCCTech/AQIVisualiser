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
#include "State.h"

class AQIVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    AQIVisualizer(QWidget *parent = nullptr);
    ~AQIVisualizer();

private:
    void setupUi();
    void addRowToTable(QString name,int aqi);
    void loadFile();
    void loadAQIData(const QString& filePath);
    void updateAQI();
    void updateAQIInListView(std::string name, int aqi);
    void updateStateColor();
    void displayMap();
private:
    QWidget* mCentralWidget;
    QWidget* mGridLayoutWidget;
    QGridLayout* mGridLayout;
    QVBoxLayout* mVerticalLayout;
    QHBoxLayout* mHorizontalLayout;
    QHBoxLayout* mHorizontalLayout1;
    QLabel* mLabel;
    QDateEdit* mDateEdit;
    OpenGLWindow* mOpenGLWidget;
    QTableView* mTableView;
    QPushButton* mPushButton;
    QPushButton* mPushButton1;
    QStandardItemModel* mListModel;
    std::map<std::string, std::map<std::string, int>> mAQIData;
    std::vector<State> mStates;
    QVector<QVector<GLfloat>> mStateVertices;
    QVector<QVector<GLfloat>> mStateColors;
    std::map<std::string, int> mSelectedDateAQI;
};
