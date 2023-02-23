#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->mygl,
            SIGNAL(sig_sendListItem(QListWidgetItem*)),
            this,
            SLOT(slot_addListItem(QListWidgetItem*)));

    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_setSelectedFace(QListWidgetItem*)));

    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_setSelectedFace(QListWidgetItem*)));

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

void MainWindow::slot_addListItem(QListWidgetItem *i) {
    Vertex *v = dynamic_cast<Vertex*>(i);
    if (v != nullptr) {
        ui->vertsListWidget->addItem(i);
        return;
    }
    Face *f = dynamic_cast<Face*>(i);
    if (f != nullptr) {
        ui->facesListWidget->addItem(i);
        return;
    }

    HalfEdge *he = dynamic_cast<HalfEdge*>(i);
    if (he != nullptr) {
        ui->halfEdgesListWidget->addItem(i);
        return;
    }
}
