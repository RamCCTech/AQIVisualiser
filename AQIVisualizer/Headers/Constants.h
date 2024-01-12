#pragma once
#include <qstring.h> 
#include <qcolor.h> 

namespace Constants {
    const QString VertexShaderPath = "Resources/Shaders/vertexShader.glsl";
    const QString FragmentShaderPath = "Resources/Shaders/fragmentShader.glsl";
    const QString AQIDataPath = "Resources/aqi_data.json";
    const QString KMLFilePath = "Resources/k.txt";
    
    // Stylesheets
    const QString ButtonStyleSheet =
        "QPushButton {"
        "   font-size: 14pt;"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 8px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #349846;"
        "}";

    const QString LabelStyleSheet = "QLabel { color: #336699; }";

    const QString TableViewStyleSheet =
        "QTableView {"
        "   background-color: #f0f0f0;"
        "   alternate-background-color: #e0e0e0;"
        "   border: 2px solid #d3d3d3;"
        "   gridline-color: #a8a8a8;"
        "}";

    // Legend items
    const QString AQILevel0 = "0 - AQI not selected";
    const QString AQILevel1 = "1 - Good (0 to 50)";
    const QString AQILevel2 = "2 - Moderate (51 to 100)";
    const QString AQILevel3 = "3 - Unhealthy for Sensitive Groups (101 to 150)";
    const QString AQILevel4 = "4 - Unhealthy (151 to 200)";
    const QString AQILevel5 = "5 - Very Unhealthy (201 to 300)";
    const QString AQILevel6 = "6 - Hazardous (301 and higher)";

    const QColor AQIColor0(0.50196 * 255, 0.50196 * 255, 0.50196 * 255);
    const QColor AQIColor1(0, 255, 0);
    const QColor AQIColor2(255, 255, 0);
    const QColor AQIColor3(255, 102, 0);
    const QColor AQIColor4(255, 0, 0);
    const QColor AQIColor5(153, 0, 255);
    const QColor AQIColor6(153, 0, 0);
}
