#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tobii_interactive.h"
#include "action_integration.h"

#include <QMessageBox>
#include <QThread>
#include <list>
#include <iostream>


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    TobiiInteractive::init(this);
    reloadTobiiDeviceList();
    gazeThread = new QThread();
}

void MainWindow::reloadTobiiDeviceList()
{
    auto devices = TobiiInteractive::reload_devices();
    ui->tobiiDevicesList->clear();
    for (size_t i = 0; i < devices.size(); i++) {
        ui->tobiiDevicesList->addItem(QString::fromStdString(devices[i]));
    }
    if(ui->tobiiDevicesList->count() > 0)
    {
        ui->tobiiDevicesList->setCurrentItem(ui->tobiiDevicesList->item(0));
    }
}
MainWindow::~MainWindow()
{
    TobiiInteractive::stop_subscribe_gaze();
    delete ui;
}

void MainWindow::on_reloadListButton_clicked()
{
    reloadTobiiDeviceList();
}

void MainWindow::on_useSelectedDeviceButton_clicked()
{
    if(ui->tobiiDevicesList->selectedItems().count() > 0){
        QString currentSelected = ui->tobiiDevicesList->selectedItems().first()->text();
        ui->currentDeviceLabel->setText(currentSelected);
        TobiiInteractive::start_subscribe_gaze(currentSelected.toStdString().c_str());
    }
}

void MainWindow::on_actionQuit_triggered()
{
    TobiiInteractive::stop_subscribe_gaze();
    QApplication::quit();
}
