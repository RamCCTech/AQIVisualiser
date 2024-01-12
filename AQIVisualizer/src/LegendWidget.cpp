#include "stdafx.h"
#include "Constants.h"
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
    addLegendItem(Constants::AQILevel0, Constants::AQIColor0);
    addLegendItem(Constants::AQILevel1, Constants::AQIColor1);
    addLegendItem(Constants::AQILevel2, Constants::AQIColor2);
    addLegendItem(Constants::AQILevel3, Constants::AQIColor3);
    addLegendItem(Constants::AQILevel4, Constants::AQIColor4);
    addLegendItem(Constants::AQILevel5, Constants::AQIColor5);
    addLegendItem(Constants::AQILevel6, Constants::AQIColor6);

    // Add the legend items to the vertical layout
    itemsLayout->addStretch();  // Add stretch to push items to the top

    // Align items to the top
    itemsLayout->setAlignment(Qt::AlignTop);

    // Create and configure QLabel for "AQI Levels" heading
    QLabel* aqiHeadingLabel = new QLabel("AQI Levels:", this);
    aqiHeadingLabel->setAlignment(Qt::AlignCenter);  // Center the text
    aqiHeadingLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #336699;"); // Increase text size, make it bold, and set a shade of blue color

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

    // Calculate the luminance of the background color to determine the text color
    double luminance = (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) / 255.0;

    // Choose white or black text based on the background luminance
    QString textColor = (luminance > 0.5) ? "color: black;" : "color: white;";

    // Add padding to the legend item
    int padding = 5;

    // Set the style sheet with background color, text color, and padding
    legendItem->setStyleSheet("background-color: rgb(" +
        QString::number(color.red()) + "," +
        QString::number(color.green()) + "," +
        QString::number(color.blue()) + ");" +
        textColor +
        "padding: " + QString::number(padding) + "px;");

    mLegendItems.push_back(legendItem);
}
