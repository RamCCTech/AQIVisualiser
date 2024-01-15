    // AQIVisualizer.cpp

    #include "stdafx.h"
    #include "AQIVisualizer.h"
    #include "KMLReader.h"
    #include "JSONReader.h"
    #include "LegendWidget.h"
    #include <QtConcurrent/QtConcurrentRun>
    #include <QProgressDialog>
    #include <QThread>

    AQIVisualizer::AQIVisualizer(QWidget* parent)
        : QMainWindow(parent)
    {
        setupUi();
        connect(mPushButton, &QPushButton::clicked, this, &AQIVisualizer::loadFile);
        connect(mCalendarWidget, &QCalendarWidget::selectionChanged, this, &AQIVisualizer::updateAQI);
        loadAQIData(Constants::AQIDataPath);

        // Initialize AQI values for each state in the table
        for (const auto& pair : mAQIData) {
            const std::string& stateName = pair.first;
            addRowToTable(QString::fromStdString(stateName), 0);
        }
    }

    AQIVisualizer::~AQIVisualizer()
    {}

    void AQIVisualizer::setupUi()
    {
        // Create the central widget
        mCentralWidget = new QWidget(this);

        // Create a vertical layout for the central widget
        QHBoxLayout* centralLayout = new QHBoxLayout(mCentralWidget);

        // Create the OpenGLWidget
        mOpenGLWidget = new OpenGLWindow(QColor(0, 0, 0), mCentralWidget);
        centralLayout->addWidget(mOpenGLWidget, 1);

        // Set margins around the OpenGLWidget
        QMargins margins(20, 20, 20, 20);
        centralLayout->setContentsMargins(margins);

        QVBoxLayout* mVerticalLayout1 = new QVBoxLayout(mCentralWidget);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);

        // Create "Load Country" button
        mPushButton = new QPushButton("Load Country", mCentralWidget);
        mPushButton->setSizePolicy(sizePolicy);

        // Set styles for the button
        mPushButton->setStyleSheet(Constants::ButtonStyleSheet);

        mVerticalLayout1->addWidget(mPushButton, 0, Qt::AlignHCenter);  // Add the button and center it horizontally

        // Create "Selected Date" label
        mLabel = new QLabel("", mCentralWidget);
        mLabel->setSizePolicy(sizePolicy);
        mLabel->setAlignment(Qt::AlignCenter);
        QFont boldFont = mLabel->font();
        boldFont.setPointSize(16);  // Set the font size to 16 (you can adjust the size as needed)
        boldFont.setBold(true);
        mLabel->setFont(boldFont);
        mLabel->setStyleSheet(Constants::LabelStyleSheet);  // Set text color to a shade of blue
        mVerticalLayout1->addWidget(mLabel);

        // Create a small calendar widget
        mCalendarWidget = new QCalendarWidget(mCentralWidget);
        mCalendarWidget->setGridVisible(true);  // You can customize as needed
        mCalendarWidget->setSizePolicy(sizePolicy);

        // Set the maximum and minimum date of the calendar to 2023 start and end
        QDate minDate(2023, 1, 1);
        QDate maxDate(2023, 12, 31);
        mCalendarWidget->setMaximumDate(maxDate);
        mCalendarWidget->setMinimumDate(minDate);
        mVerticalLayout1->addWidget(mCalendarWidget);

        // Create the LegendWidget
        LegendWidget* legendWidget = new LegendWidget(mCentralWidget);
        legendWidget->setSizePolicy(sizePolicy);
        mVerticalLayout1->addWidget(legendWidget);

        centralLayout->addLayout(mVerticalLayout1);

        // Create the TableView
        mTableView = new QTableView(mCentralWidget);

        // Create the list model for the TableView
        mListModel = new QStandardItemModel(this);
        mListModel->setHorizontalHeaderLabels(QStringList() << "State and UT" << "AQI Level");
        mTableView->setModel(mListModel);

        // Set bold font for the horizontal header
        QFont headerFont = mTableView->horizontalHeader()->font();
        headerFont.setBold(true);
        mTableView->horizontalHeader()->setFont(headerFont);
        headerFont.setPointSize(headerFont.pointSize() + 2);  // Increase font size by 2 points
        mTableView->horizontalHeader()->setFont(headerFont);

        // Set horizontal header properties
        mTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);  // Allow columns to stretch
        mTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);  // Allow resizing for the "AQI Level" column

        // Set the table styling
        mTableView->setStyleSheet(Constants::TableViewStyleSheet);

        // Add the OpenGLWidget and TableView to the bottom layout
        centralLayout->addWidget(mTableView);

        // Set the central widget
        setCentralWidget(mCentralWidget);
        showMaximized();
    }

    void AQIVisualizer::loadFile()
    {
        // Create and configure a progress dialog
        QProgressDialog progressDialog("Loading File...", "Cancel", 0, 100, this);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setWindowTitle("Loading");

        // Connect signals and slots for progress updates
        connect(this, &AQIVisualizer::loadingStarted, &progressDialog, &QProgressDialog::show);
        connect(this, &AQIVisualizer::loadingProgress, &progressDialog, &QProgressDialog::setValue);
        connect(this, &AQIVisualizer::loadingFinished, &progressDialog, &QProgressDialog::reset);

        // Start the loading process in a separate thread
        QFuture<void> future = QtConcurrent::run([this, &progressDialog]() {
            emit loadingStarted();

            // Your existing file loading logic
            KMLReader reader;
            mStates = reader.parseKMLFromFile(Constants::KMLFilePath);

            // Set initial AQI values to 0 for each state
            for (State& s : mStates) {
                s.setAQIData(mAQIData[s.name()]);
                mSelectedDateAQI[s.name()] = 0;
            }

            // Display the map after loading file
            displayMap();

            emit loadingFinished();
            });

        // Connect the progress dialog to the loading process
        progressDialog.setMinimum(0);
        progressDialog.setMaximum(0);

        while (!future.isFinished()) {
            // Calculate progress (modify this based on your actual progress)
            int progressValue = static_cast<int>(future.progressValue() * 100.0 / future.progressMaximum());
            emit loadingProgress(progressValue);

            qApp->processEvents();
            if (progressDialog.wasCanceled()) {
                // User clicked Cancel, stop the loading process
                future.cancel();
                break;
            }
        }

        // Close the progress dialog when loading is complete
        progressDialog.close();
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
                vertices << p.x() << p.y() << 0;

                // Set colors based on AQI level
                if (mSelectedDateAQI[s.name()] == 0) colors << 0.50196 << 0.50196 << 0.50196;
                else if (mSelectedDateAQI[s.name()] == 1) colors << 0.0 << 1.0 << 0.33333;
                else if (mSelectedDateAQI[s.name()] == 2) colors << 1 << 1 << 0;
                else if (mSelectedDateAQI[s.name()] == 3) colors << 1 << 0.4 << 0;
                else if (mSelectedDateAQI[s.name()] == 4) colors << 1 << 0 << 0;
                else if (mSelectedDateAQI[s.name()] == 5) colors << 0.6 << 0 << 1;
                else if (mSelectedDateAQI[s.name()] == 6) colors << 0.6 << 0 << 0;
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
        // Create and configure a progress dialog for the update process
        QProgressDialog progressDialog("Updating AQI...", "Cancel", 0, 100, this);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setWindowTitle("Updating");

        // Connect signals and slots for progress updates
        connect(this, &AQIVisualizer::loadingStarted, &progressDialog, &QProgressDialog::show);
        connect(this, &AQIVisualizer::loadingProgress, &progressDialog, &QProgressDialog::setValue);
        connect(this, &AQIVisualizer::loadingFinished, &progressDialog, &QProgressDialog::reset);

        // Start the update process in a separate thread
        QFuture<void> future = QtConcurrent::run([this, &progressDialog]() {
            emit loadingStarted();

            // Your existing update logic
            // Get the selected date from mDateEdit
            QDate selectedDate = mCalendarWidget->selectedDate();

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
            mLabel->setText(qs + QString::fromStdString(st));
            // Display the updated map
            displayMap();

            emit loadingFinished();
            });

        // Connect the progress dialog to the update process
        progressDialog.setMinimum(0);
        progressDialog.setMaximum(0);
        progressDialog.setCancelButton(nullptr);

        while (!future.isFinished()) {
            // Calculate progress (modify this based on your actual progress)
            int progressValue = static_cast<int>(future.progressValue() * 100.0 / future.progressMaximum());
            emit loadingProgress(progressValue);

            qApp->processEvents();
            if (progressDialog.wasCanceled()) {
                // User clicked Cancel, stop the update process
                future.cancel();
                break;
            }
        }

        // Close the progress dialog when updating is complete
        progressDialog.close();
    }

    void AQIVisualizer::updateAQIInListView(const std::string name, int aqi)
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