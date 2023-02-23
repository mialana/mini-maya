#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog>
#include <iostream>
#include "mesh.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_Obj_triggered() {
    QString filename = QFileDialog::getOpenFileName(0, QString("Select OBJ File"), QDir::currentPath().append(QString("../..")), QString("*.obj"));

    ui->mygl->m_meshCurrent.loadObj(filename);

    ui->mygl->m_meshCurrent.destroy();
    ui->mygl->m_meshCurrent.create();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}
