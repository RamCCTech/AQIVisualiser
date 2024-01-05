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
    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    gridLayoutWidget = new QWidget(centralWidget);
    gridLayoutWidget->setObjectName("gridLayoutWidget");
    gridLayoutWidget->setGeometry(QRect(0, 0, 811, 611));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName("gridLayout");
    gridLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setObjectName("verticalLayout");
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setObjectName("horizontalLayout");
    label = new QLabel(gridLayoutWidget);
    label->setObjectName("label");
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy);

    horizontalLayout->addWidget(label);

    dateEdit = new QDateEdit(gridLayoutWidget);
    dateEdit->setObjectName("dateEdit");

    horizontalLayout->addWidget(dateEdit);

    pushButton = new QPushButton(gridLayoutWidget);
    pushButton->setObjectName("pushButton");

    horizontalLayout->addWidget(pushButton);


    verticalLayout->addLayout(horizontalLayout);


    gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

    openGLWidget = new QOpenGLWidget(gridLayoutWidget);
    openGLWidget->setObjectName("openGLWidget");

    gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

    setCentralWidget(centralWidget);

}