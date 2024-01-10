#include "stdafx.h"
#include "AQIVisualizer.h"
#include "State.h"
#include "KMLReader.h"
#include "JSONReader.h"
#include "LegendWidget.h"

AQIVisualizer::AQIVisualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    connect(mPushButton, &QPushButton::clicked, this, &AQIVisualizer::loadFile);
    connect(mPushButton1, &QPushButton::clicked, this, &AQIVisualizer::updateAQI);
    loadAQIData("Resources/aqi_data.json");

    // Initialize AQI values for each state in the table
    for (const auto& pair : mAQIData) {
        const std::string& stateName = pair.first;
        addRowToTable(QString::fromStdString(stateName), 0);  // Initialize AQI with "0"
    }
}

AQIVisualizer::~AQIVisualizer()
{}

void AQIVisualizer::setupUi()
{
    // Create the central widget
    mCentralWidget = new QWidget(this);

    // Create a vertical layout for the central widget
    QVBoxLayout* centralLayout = new QVBoxLayout(mCentralWidget);

    // Create a horizontal layout for the top part (buttons, date edit)
    QHBoxLayout* topLayout = new QHBoxLayout();

    // Create "Load Country" button
    mPushButton = new QPushButton("Load Country", mCentralWidget);
    topLayout->addWidget(mPushButton);

    // Create "Selected Date" label
    mLabel = new QLabel("Selected Date", mCentralWidget);
    topLayout->addWidget(mLabel);

    // Create QDateEdit for date selection
    mDateEdit = new QDateEdit(mCentralWidget);
    mDateEdit->setDisplayFormat("dd/MM/yyyy");
    topLayout->addWidget(mDateEdit);

    QDate currentDate = QDate::currentDate();
    QDate minDate(2023, 1, 1);
    QDate maxDate(2023, 12, 31);
    mDateEdit->setMinimumDate(minDate);
    mDateEdit->setMaximumDate(maxDate);
    mDateEdit->setDate(currentDate);

    // Create "Fetch AQI" button
    mPushButton1 = new QPushButton("Fetch AQI", mCentralWidget);
    topLayout->addWidget(mPushButton1);

    // Add the top layout to the central layout
    centralLayout->addLayout(topLayout);

    // Create a horizontal layout for the OpenGLWidget, LegendWidget, and TableView
    QHBoxLayout* bottomLayout = new QHBoxLayout();

    // Create the OpenGLWidget
    mOpenGLWidget = new OpenGLWindow(QColor(0, 0, 0), mCentralWidget);
    bottomLayout->addWidget(mOpenGLWidget, 1);  // Set stretch factor to 1

    // Set margins around the OpenGLWidget
    QMargins margins(10, 10, 10, 10);
    bottomLayout->setContentsMargins(margins);

    // Create the LegendWidget
    LegendWidget* legendWidget = new LegendWidget(mCentralWidget);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    legendWidget->setSizePolicy(sizePolicy);
    bottomLayout->addWidget(legendWidget);

    // Create the TableView
    mTableView = new QTableView(mCentralWidget);

    // Create the list model for the TableView
    mListModel = new QStandardItemModel(this);
    mListModel->setHorizontalHeaderLabels(QStringList() << "State and UT" << "AQI Level");
    mTableView->setModel(mListModel);

    // Add the OpenGLWidget and TableView to the bottom layout
    bottomLayout->addWidget(mTableView);

    // Add the bottom layout to the central layout
    centralLayout->addLayout(bottomLayout);

    // Set the central widget
    setCentralWidget(mCentralWidget);
    showMaximized();
}






void AQIVisualizer::loadFile()
{
    KMLReader reader;
    mStates = reader.parseKMLFromFile("Resources/k.txt");

    // Set initial AQI values to 0 for each state
    for (State& s : mStates) {
        s.setAQIData(mAQIData[s.name()]);
        mSelectedDateAQI[s.name()] = 0;
    }

    // Display the map after loading file
    displayMap();
}

void AQIVisualizer::displayMap()
{
    QVector<QVector<GLfloat>> stateVertices;
    QVector<QVector<GLfloat>> stateColors;

    // Iterate through states to get their vertices and colors
    for (State& s : mStates) {
        QVector<GLfloat> vertices;
        QVector<GLfloat> colors;

        // Iterate through state coordinates
        for (Point3D p : s.coordinate()) {
            vertices << p.x() << p.y();

            // Set colors based on AQI level
            if (mSelectedDateAQI[s.name()] == 0) colors << 0.50196 << 0.50196 << 0.50196;
            else if (mSelectedDateAQI[s.name()] == 1) colors << 0.0 << 1.0 << 0.33333;
            else if (mSelectedDateAQI[s.name()] == 2) colors << 1 << 1 << 0;
            else if (mSelectedDateAQI[s.name()] == 3) colors << 1 << 0.4 << 0;
            else if (mSelectedDateAQI[s.name()] == 4) colors << 1 << 0 << 0;
            else if (mSelectedDateAQI[s.name()] == 5) colors << 0.6 << 0 << 1;
            else if (mSelectedDateAQI[s.name()] == 6) colors << 0.6 << 0 << 0;
            else colors << 0.50196 << 0.50196 << 0.50196;
        }

        stateVertices << vertices;
        stateColors << colors;
    }

    // Update the OpenGL widget with the new shapes
    mOpenGLWidget->updateShape(stateVertices, stateColors);
}

void AQIVisualizer::loadAQIData(const QString& filePath)
{
    JSONReader jr;
    mAQIData = jr.readJsonFile(filePath);
}

void AQIVisualizer::updateAQI()
{
    // Get the selected date from mDateEdit
    QDate selectedDate = mDateEdit->date();

    // Create a copy of mStates to avoid iterator invalidation
    std::vector<State> statesCopy = mStates;

    // Iterate through the states copy and update the AQI in the list view
    std::string st = selectedDate.toString("dd/MM/yyyy").toStdString();
    for (State& s : statesCopy) {
        if (s.getAQIData().count(st) > 0) {
            int aqi = s.getAQIData().at(st);
            mSelectedDateAQI[s.name()] = aqi;
            updateAQIInListView(s.name(), aqi);
        }
        else {
            updateAQIInListView(s.name(), 0);
        }
    }
    QString qs = "Selected Date: ";
    mLabel->setText(qs+QString::fromStdString(st));
    // Display the updated map
    displayMap();
}

void AQIVisualizer::updateAQIInListView(std::string name, int aqi)
{
    QString qName = QString::fromStdString(name);

    // Iterate through the items in the list view and update the corresponding AQI value
    for (int row = 0; row < mListModel->rowCount(); ++row) {
        if (mListModel->index(row, 0).data(Qt::DisplayRole).toString() == qName) {
            mListModel->setData(mListModel->index(row, 1), QString::number(aqi));
            break;
        }
    }
}

void AQIVisualizer::addRowToTable(QString name, int aqi)
{
    QList<QStandardItem*> row;

    // Set the name item as non-editable
    QStandardItem* nameItem = new QStandardItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    row << nameItem;

    // Set the AQI item as non-editable
    QStandardItem* aqiItem = new QStandardItem(QString::number(aqi));
    aqiItem->setFlags(aqiItem->flags() & ~Qt::ItemIsEditable);
    row << aqiItem;

    mListModel->appendRow(row);
}