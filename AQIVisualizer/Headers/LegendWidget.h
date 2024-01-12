#pragma once
#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColor>
#include <vector>  // Include the vector header

class LegendWidget : public QWidget {
    Q_OBJECT

public:
    LegendWidget(QWidget* parent = nullptr);

private:
    void setupUi();
    void addLegendItem(const QString& text, const QColor& color);

private:
    QLabel* mLegendLabel;
    std::vector<QLabel*> mLegendItems;  // Declare mLegendItems as a vector
};