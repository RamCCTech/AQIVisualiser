#include "stdafx.h"
#include "LegendWidget.h"

#include "LegendWidget.h"

LegendWidget::LegendWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void LegendWidget::setupUi()
{
    // Create a horizontal layout for the legend widget
    QHBoxLayout* legendLayout = new QHBoxLayout(this);

    // Create a vertical layout for the legend items
    QVBoxLayout* itemsLayout = new QVBoxLayout();

    // Add color legend items (customize as needed)
    addLegendItem("0 - AQI not selected", QColor(0.50196 * 255, 0.50196 * 255, 0.50196 * 255));
    addLegendItem("1 - Good (0 to 50)", QColor(0, 255, 0));
    addLegendItem("2 - Moderate (51 to 100)", QColor(255, 255, 0));
    addLegendItem("3 - Unhealthy for Sensitive Groups (101 to 150)", QColor(255, 102, 0));
    addLegendItem("4 - Unhealthy (151 to 200)", QColor(255, 0, 0));
    addLegendItem("5 - Very Unhealthy (201 to 300)", QColor(153, 0, 255));
    addLegendItem("6 - Hazardous (301 and higher)", QColor(153, 0, 0));

    // Add the legend items to the vertical layout
    itemsLayout->addStretch();  // Add stretch to push items to the top

    // Align items to the top
    itemsLayout->setAlignment(Qt::AlignTop);

    // Create and configure QLabel for "AQI Levels" heading
    QLabel* aqiHeadingLabel = new QLabel("AQI Levels:", this);
    aqiHeadingLabel->setStyleSheet("font-size: 14pt; font-weight: bold;"); // Increase text size and make it bold

    // Add the "AQI Levels" heading to the layout
    itemsLayout->addWidget(aqiHeadingLabel);

    // Add some spacing
    itemsLayout->addWidget(new QLabel(""));

    // Add color legend items to the vertical layout
    for (auto item : mLegendItems) {
        itemsLayout->addWidget(item);
    }

    // Add the legend items layout to the main layout
    legendLayout->addLayout(itemsLayout);

    // Set margins around the legend
    QMargins margins(10, 10, 10, 10);
    legendLayout->setContentsMargins(margins);
}


void LegendWidget::addLegendItem(const QString& text, const QColor& color)
{
    QLabel* legendItem = new QLabel(this);

   
    legendItem->setText(text);

    // Add padding to the legend item
    int padding = 5;
    legendItem->setStyleSheet("background-color: rgb(" +
        QString::number(color.red()) + "," +
        QString::number(color.green()) + "," +
        QString::number(color.blue()) + ");" +
        "padding: " + QString::number(padding) + "px;");

    //legendItem->setFixedHeight(30);  // Adjust the height as needed

    mLegendItems.push_back(legendItem);
}
