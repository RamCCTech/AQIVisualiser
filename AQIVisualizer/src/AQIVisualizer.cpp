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

    mPushButton = new QPushButton("Set Date", mGridLayoutWidget);
    mHorizontalLayout->addWidget(mPushButton);

    mOpenGLWidget = new OpenGLWindow(QColor(0, 0, 0), mCentralWidget);
    mGridLayout->addWidget(mOpenGLWidget, 0, 0, 1, 1);

    mVerticalLayout->addLayout(mHorizontalLayout);
    mGridLayout->addLayout(mVerticalLayout, 1, 0, 1, 1);

    setCentralWidget(mCentralWidget);

}