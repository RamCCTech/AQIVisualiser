#include "stdafx.h"
#include "AQIVisualizer.h"

AQIVisualizer::AQIVisualizer(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
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

    mLabel = new QLabel("Date", mGridLayoutWidget);
    mHorizontalLayout->addWidget(mLabel);

    mDateEdit = new QDateEdit(mGridLayoutWidget);
    mHorizontalLayout->addWidget(mDateEdit);

    mHorizontalLayout1 = new QHBoxLayout();

    mOpenGLWidget = new OpenGLWindow(QColor(0, 0, 0), mCentralWidget);
    mHorizontalLayout1->addWidget(mOpenGLWidget);

    mTableView = new QTableView(mGridLayoutWidget);
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    mTableView->setSizePolicy(sizePolicy);
    mHorizontalLayout1->addWidget(mTableView);
    mGridLayout->addLayout(mHorizontalLayout1, 0, 0, 1, 1);

    mVerticalLayout->addLayout(mHorizontalLayout);
    mGridLayout->addLayout(mVerticalLayout, 1, 0, 1, 1);

    setCentralWidget(mCentralWidget);
    setupTableModel();
}

void AQIVisualizer::setupTableModel()
{
    // Create a standard item model
    QStandardItemModel* model = new QStandardItemModel(this);

    // Set the column headers
    model->setHorizontalHeaderLabels(QStringList() << "State" << "AUI");

    // Add dummy data (replace this with your actual data)
    QList<QStandardItem*> row;
    for (int i = 0; i < 10; i++) {
        row << new QStandardItem("State1") << new QStandardItem("AUI1");
        model->appendRow(row);

        row.clear();
        row << new QStandardItem("State2") << new QStandardItem("AUI2");
        model->appendRow(row);
        row.clear();
    }
    
    // Set the model for the table view
    mTableView->setModel(model);
}