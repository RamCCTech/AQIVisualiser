#pragma once

#include "OpenGLWindow.h"
#include "State.h"
#include "Constants.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QCalendarWidget>

class AQIVisualizer : public QMainWindow
{
    Q_OBJECT
signals:
    void loadingStarted();
    void loadingProgress(int value);
    void loadingFinished();

public:
    AQIVisualizer(QWidget* parent = nullptr);
    ~AQIVisualizer();

private:
    void setupUi();
    void addRowToTable(QString name, int aqi);
    void loadFile();
    void loadAQIData(const QString& filePath);
    void updateAQI();
    void updateAQIInListView(const std::string name, int aqi);
    void displayMap();

private:
    QWidget* mCentralWidget;
    OpenGLWindow* mOpenGLWidget;
    QTableView* mTableView;
    QStandardItemModel* mListModel;
    QCalendarWidget* mCalendarWidget;

    std::map<std::string, std::map<std::string, int>> mAQIData;
    std::vector<State> mStates;
    std::map<std::string, int> mSelectedDateAQI;

    QLabel* mLabel;
    QPushButton* mPushButton;
};