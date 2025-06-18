#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include "../GUIConfigTool/PinscapePicoAPI.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QAction;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void refreshDevices();
    void showAbout();

private:
    QListWidget *deviceList;
};

#endif // MAINWINDOW_H
