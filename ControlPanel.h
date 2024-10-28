// ControlPanel.h

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    QPushButton *wireframeButton;

    signals:
        void loadSWCFileRequested();

    private slots:
        void onLoadSWCFile();

private:
    QPushButton *loadSWCButton;
    QTableWidget *dataTable;
    QPushButton *toggleVisibilityButton;
    QPushButton *exportButton;


    void setupUI();
    void setupConnections();
    void exportData();
};

#endif // CONTROLPANEL_H
