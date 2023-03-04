#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    filename(QDir().cleanPath(QString(QFileInfo(".").absolutePath() + "/../../../../obj_files/cube.obj"))),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->mygl,
            SIGNAL(sig_sendListItem(QListWidgetItem*)),
            this,
            SLOT(slot_addListItem(QListWidgetItem*)));

    connect(ui->mygl,
            SIGNAL(sig_setSelectedVert(QListWidgetItem*)),
            this, SLOT(slot_setSelectedVert(QListWidgetItem*)));

    connect(ui->mygl,
            SIGNAL(sig_setSelectedFace(QListWidgetItem*)),
            this, SLOT(slot_setSelectedFace(QListWidgetItem*)));

    connect(ui->mygl,
            SIGNAL(sig_setSelectedHedge(QListWidgetItem*)),
            this, SLOT(slot_setSelectedHedge(QListWidgetItem*)));


    connect(ui->vertsListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slot_setSelectedVert(QListWidgetItem*)));

    connect(ui->facesListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slot_setSelectedFace(QListWidgetItem*)));

    connect(ui->halfEdgesListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slot_setSelectedHedge(QListWidgetItem*)));


    connect(ui->splitEdge, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_splitHedge()));

    connect(ui->triangulateFace, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_triangulateFace()));

    connect(ui->subdivideMesh, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_subdivideMesh()));

    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_translateX(double)));

    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_translateY(double)));

    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_translateZ(double)));

    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_changeRed(double)));

    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_changeGreen(double)));

    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_changeBlue(double)));

}

void MainWindow::slot_setSelectedVert(QListWidgetItem *i) {
    ui->mygl->mp_selectedVert = static_cast<Vertex*>(i);
    ui->mygl->m_vertDisplay.updateVert(ui->mygl->mp_selectedVert);
    ui->mygl->m_vertDisplay.destroy();
    ui->mygl->m_vertDisplay.create();

    ui->vertsListWidget->setCurrentItem(i);

    if (ui->mygl->mp_selectedVert != nullptr) {
        ui->vertPosXSpinBox->setValue(ui->mygl->mp_selectedVert->m_pos.x);
        ui->vertPosYSpinBox->setValue(ui->mygl->mp_selectedVert->m_pos.y);
        ui->vertPosZSpinBox->setValue(ui->mygl->mp_selectedVert->m_pos.z);
    }

    ui->mygl->update();
}

void MainWindow::slot_setSelectedFace(QListWidgetItem *i) {
    ui->mygl->mp_selectedFace = static_cast<Face*>(i);
    ui->mygl->m_faceDisplay.updateFace(ui->mygl->mp_selectedFace);
    ui->mygl->m_faceDisplay.destroy();
    ui->mygl->m_faceDisplay.create();

    ui->facesListWidget->setCurrentItem(i);

    if (ui->mygl->mp_selectedFace != nullptr) {
        ui->faceRedSpinBox->setValue(ui->mygl->mp_selectedFace->m_color.r);
        ui->faceGreenSpinBox->setValue(ui->mygl->mp_selectedFace->m_color.g);
        ui->faceBlueSpinBox->setValue(ui->mygl->mp_selectedFace->m_color.b);
    }

    ui->mygl->update();
}

void MainWindow::slot_setSelectedHedge(QListWidgetItem *i) {
    ui->mygl->mp_selectedHedge = static_cast<HalfEdge*>(i);

    ui->mygl->m_hedgeDisplay.updateHedge(ui->mygl->mp_selectedHedge);
    ui->mygl->m_hedgeDisplay.destroy();
    ui->mygl->m_hedgeDisplay.create();

    ui->halfEdgesListWidget->setCurrentItem(i);

    ui->mygl->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_Obj_triggered() {
    QString validate_filename = QFileDialog::getOpenFileName(0, QString("Select OBJ File"), QDir::currentPath().append(QString("../..")), QString("*.obj"));

    QFile file(validate_filename);

    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Could not open the file.");
        return;
    }

    Vertex::population = 0;
    Face::population = 0;
    HalfEdge::population = 0;

    filename = validate_filename;
    ui->mygl->m_meshCurrent.loadObj(file);

    ui->mygl->m_meshCurrent.destroy();
    ui->mygl->m_meshCurrent.create();

    slot_setSelectedVert(nullptr);
    slot_setSelectedFace(nullptr);
    slot_setSelectedHedge(nullptr);
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
