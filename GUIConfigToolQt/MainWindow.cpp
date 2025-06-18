#include "MainWindow.h"
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    deviceList = new QListWidget(central);
    auto *refreshBtn = new QPushButton(tr("Refresh"), central);

    layout->addWidget(deviceList);
    layout->addWidget(refreshBtn);
    setCentralWidget(central);

    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshDevices);

    // menu
    auto *fileMenu = menuBar()->addMenu(tr("File"));
    auto *refreshAct = fileMenu->addAction(tr("Refresh"));
    fileMenu->addSeparator();
    auto *quitAct = fileMenu->addAction(tr("Quit"));
    connect(refreshAct, &QAction::triggered, this, &MainWindow::refreshDevices);
    connect(quitAct, &QAction::triggered, this, &QWidget::close);

    auto *helpMenu = menuBar()->addMenu(tr("Help"));
    auto *aboutAct = helpMenu->addAction(tr("About"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::showAbout);

    statusBar();
    refreshDevices();
}

void MainWindow::refreshDevices()
{
    deviceList->clear();
    std::list<VendorInterfaceDesc> devs;
    if (VendorInterface::EnumerateDevices(devs) >= 0) {
        for (const auto &d : devs)
            deviceList->addItem(QString::fromStdString(d.path));
        statusBar()->showMessage(tr("%1 device(s) found").arg(devs.size()));
    } else {
        statusBar()->showMessage(tr("Error enumerating devices"));
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About"), tr("Pinscape Pico Config Tool"));
}

