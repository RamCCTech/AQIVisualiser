#include "stdafx.h"
#include "AQIVisualizer.h"
#include "State.h"
#include "KMLReader.h"
#include "JSONReader.h"

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
    resize(830, 625);
    mCentralWidget = new QWidget(this);

    mGridLayoutWidget = new QWidget(mCentralWidget);
    mGridLayoutWidget->setGeometry(QRect(0, 0, 830, 625));
    mGridLayout = new QGridLayout(mGridLayoutWidget);

    mVerticalLayout = new QVBoxLayout();

    mHorizontalLayout = new QHBoxLayout();

    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    mPushButton = new QPushButton("Load Country", mGridLayoutWidget);
    mHorizontalLayout->addWidget(mPushButton);

    mLabel = new QLabel("Selected Date", mGridLayoutWidget);
    mHorizontalLayout->addWidget(mLabel);

    mDateEdit = new QDateEdit(mGridLayoutWidget);
    mDateEdit->setDisplayFormat("dd/MM/yyyy");
    mHorizontalLayout->addWidget(mDateEdit);

    QDate currentDate = QDate::currentDate();
    QDate minDate(2023, 1, 1);
    QDate maxDate(2023, 12, 31);
    mDateEdit->setMinimumDate(minDate);
    mDateEdit->setMaximumDate(maxDate);
    mDateEdit->setDate(currentDate);

    mPushButton1 = new QPushButton("Fetch AQI", mGridLayoutWidget);
    mHorizontalLayout->addWidget(mPushButton1);

    mHorizontalLayout1 = new QHBoxLayout();

    mOpenGLWidget = new OpenGLWindow(QColor(0, 0, 0), mCentralWidget);
    mHorizontalLayout1->addWidget(mOpenGLWidget);

    mTableView = new QTableView(mGridLayoutWidget);
    mTableView->setSizePolicy(sizePolicy);
    mListModel = new QStandardItemModel(this);
    mListModel->setHorizontalHeaderLabels(QStringList() << "State and UT" << "AQI");
    mTableView->setModel(mListModel);
    mHorizontalLayout1->addWidget(mTableView);
    mGridLayout->addLayout(mHorizontalLayout1, 0, 0, 1, 1);

    mVerticalLayout->addLayout(mHorizontalLayout);
    mGridLayout->addLayout(mVerticalLayout, 1, 0, 1, 1);

    setCentralWidget(mCentralWidget);
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