// ControlPanel.cpp

#include "ControlPanel.h"
#include <QFileDialog>
#include <QHeaderView>

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void ControlPanel::setupUI() {
    // Initialize buttons and data table
    loadSWCButton = new QPushButton("Load SWC File", this);
    toggleVisibilityButton = new QPushButton("Toggle Viewer", this);
    exportButton = new QPushButton("Export Data", this);

    dataTable = new QTableWidget(this);
    dataTable->setColumnCount(3); // Example: Branch Length, Radius, Parent Node
    dataTable->setHorizontalHeaderLabels({"Branch Length", "Radius", "Parent Node"});
    dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(loadSWCButton);
    layout->addWidget(dataTable);
    layout->addWidget(toggleVisibilityButton);
    layout->addWidget(exportButton);
    setLayout(layout);
}

void ControlPanel::setupConnections() {
    connect(loadSWCButton, &QPushButton::clicked, this, &ControlPanel::onLoadSWCFile);
    connect(toggleVisibilityButton, &QPushButton::clicked, [this]() {
        this->setVisible(!this->isVisible());  // Toggle visibility
    });
    connect(exportButton, &QPushButton::clicked, this, &ControlPanel::exportData);
}

void ControlPanel::onLoadSWCFile() {
    emit loadSWCFileRequested(); // Signal that ViewerWidget can connect to
}

void ControlPanel::exportData() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save Data", "", "CSV Files (*.csv);;All Files (*)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            // Write headers
            for (int col = 0; col < dataTable->columnCount(); ++col) {
                out << dataTable->horizontalHeaderItem(col)->text() << (col == dataTable->columnCount() - 1 ? "\n" : ",");
            }
            // Write data rows
            for (int row = 0; row < dataTable->rowCount(); ++row) {
                for (int col = 0; col < dataTable->columnCount(); ++col) {
                    out << dataTable->item(row, col)->text() << (col == dataTable->columnCount() - 1 ? "\n" : ",");
                }
            }
            file.close();
        }
    }
}

    // Implementation for exporting the data to a file (e.g., CSV)

