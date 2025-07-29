#include "mainwindow.h"
#include "cameracontrolshelp.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    filename(QDir().cleanPath(
        QString(PROJECT_SOURCE_DIR) + "/../obj_files/cube.obj")),
      ui(new Ui::MainWindow) {

  ui->setupUi(this);
  ui->mygl->setFocus();

  connect(ui->mygl, SIGNAL(sig_sendListItem(QListWidgetItem *)), this,
          SLOT(slot_addListItem(QListWidgetItem *)));

    connect(ui->mygl,
            SIGNAL(sig_sendRootNode(QTreeWidgetItem*)),
            this,
            SLOT(slot_addRootToTreeWidget(QTreeWidgetItem*)));


    connect(ui->mygl,
            SIGNAL(sig_setSelectedVert(QListWidgetItem*)),
            this, SLOT(slot_setSelectedVert(QListWidgetItem*)));

  connect(ui->mygl, SIGNAL(sig_setSelectedFace(QListWidgetItem *)), this,
          SLOT(slot_setSelectedFace(QListWidgetItem *)));

  connect(ui->mygl, SIGNAL(sig_setSelectedHedge(QListWidgetItem *)), this,
          SLOT(slot_setSelectedHedge(QListWidgetItem *)));


    connect(ui->jointsTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(slot_setSelectedJoint(QTreeWidgetItem*)));

    connect(ui->vertsListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slot_setSelectedVert(QListWidgetItem*)));

  connect(ui->facesListWidget,
          SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
          this, SLOT(slot_setSelectedFace(QListWidgetItem *)));

  connect(ui->halfEdgesListWidget,
          SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
          this, SLOT(slot_setSelectedHedge(QListWidgetItem *)));

  connect(ui->splitEdge, SIGNAL(clicked()), ui->mygl, SLOT(slot_splitHedge()));

  connect(ui->triangulateFace, SIGNAL(clicked()), ui->mygl,
          SLOT(slot_triangulateFace()));

  connect(ui->subdivideMesh, SIGNAL(clicked()), ui->mygl,
          SLOT(slot_subdivideMesh()));

    connect(ui->bindSkeleton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_bindSkeleton()));


    connect(ui->pushButton_2, SIGNAL(clicked()), ui->mygl,
          SLOT(slot_exportToUSD()));

  connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_translateX(double)));

  connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), ui->mygl,
          SLOT(slot_translateY(double)));

  connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), ui->mygl,
          SLOT(slot_translateZ(double)));

  connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), ui->mygl,
          SLOT(slot_changeRed(double)));

  connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), ui->mygl,
          SLOT(slot_changeGreen(double)));

    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_changeBlue(double)));


    connect(ui->jointTransXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_jointTranslateX(double)));

    connect(ui->jointTransYSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_jointTranslateY(double)));

    connect(ui->jointTransZSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_jointTranslateZ(double)));

    connect(ui->jointRotatePlusXPushButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_jointRotatePlusX()));

    connect(ui->jointRotatePlusYPushButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_jointRotatePlusY()));

    connect(ui->jointRotatePlusZPushButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_jointRotatePlusZ()));

    connect(ui->jointRotateMinusXPushButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_jointRotateMinusX()));

    connect(ui->jointRotateMinusYPushButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_jointRotateMinusY()));

    connect(ui->jointRotateMinusZPushButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_jointRotateMinusZ()));
}

void MainWindow::slot_setSelectedJoint(QTreeWidgetItem *i) {
    if (Joint* j = dynamic_cast<Joint*>(i); j != nullptr) {
        ui->mygl->m_skeletonCurrent.mp_selectedJoint = j;

        ui->mygl->m_skeletonCurrent.destroy();
        ui->mygl->m_skeletonCurrent.create();

        ui->jointsTreeWidget->setCurrentItem(i);

        ui->jointTransXSpinBox->setValue(j->translation.x);
        ui->jointTransYSpinBox->setValue(j->translation.y);
        ui->jointTransZSpinBox->setValue(j->translation.z);

        ui->mygl->update();
    }
}

void MainWindow::slot_setSelectedVert(QListWidgetItem *i) {
  if (Vertex *v = dynamic_cast<Vertex *>(i); v != nullptr) {
    ui->mygl->mp_selectedVert = v;
    ui->mygl->m_vertDisplay.updateVert(ui->mygl->mp_selectedVert);
    ui->mygl->m_vertDisplay.destroy();
    ui->mygl->m_vertDisplay.create();

    ui->vertsListWidget->setCurrentItem(i);

        ui->vertPosXSpinBox->setValue(ui->mygl->mp_selectedVert->m_pos.x);
        ui->vertPosYSpinBox->setValue(ui->mygl->mp_selectedVert->m_pos.y);
        ui->vertPosZSpinBox->setValue(ui->mygl->mp_selectedVert->m_pos.z);

    ui->mygl->update();
  }
}

void MainWindow::slot_setSelectedFace(QListWidgetItem *i) {
  if (Face *f = dynamic_cast<Face *>(i); f != nullptr) {
    ui->mygl->mp_selectedFace = f;
    ui->mygl->m_faceDisplay.updateFace(ui->mygl->mp_selectedFace);
    ui->mygl->m_faceDisplay.destroy();
    ui->mygl->m_faceDisplay.create();

    ui->facesListWidget->setCurrentItem(i);

        ui->faceRedSpinBox->setValue(ui->mygl->mp_selectedFace->m_color.r);
        ui->faceGreenSpinBox->setValue(ui->mygl->mp_selectedFace->m_color.g);
        ui->faceBlueSpinBox->setValue(ui->mygl->mp_selectedFace->m_color.b);

        ui->mygl->update();
    }
}

void MainWindow::slot_setSelectedHedge(QListWidgetItem *i) {
  if (HalfEdge *he = dynamic_cast<HalfEdge *>(i); he != nullptr) {
    ui->mygl->mp_selectedHedge = he;
    ui->mygl->m_hedgeDisplay.updateHedge(ui->mygl->mp_selectedHedge);
    ui->mygl->m_hedgeDisplay.destroy();
    ui->mygl->m_hedgeDisplay.create();

    ui->halfEdgesListWidget->setCurrentItem(i);

    ui->mygl->update();
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionImport_Obj_triggered() {
    QString validate_filename = QFileDialog::getOpenFileName(
        nullptr,
        QString("Select OBJ File"),
        QDir().cleanPath(
            QFileInfo(PROJECT_SOURCE_DIR).absoluteFilePath() + "/../obj_files"),
        QString("OBJ Files (*.obj)"));    

  QFile file(validate_filename);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Could not open the file.");
    return;
  }

  ui->mygl->mp_selectedVert = nullptr;
  ui->mygl->mp_selectedFace = nullptr;
  ui->mygl->mp_selectedHedge = nullptr;

  Vertex::population = 0;
  Face::population = 0;
  HalfEdge::population = 0;

  filename = validate_filename;
  ui->mygl->m_meshCurrent.loadObj(file);
}

void MainWindow::on_actionImport_Skeleton_triggered() {
  QString validate_file = QFileDialog::getOpenFileName(
      0, QString("Select JSON File"),
      QDir().cleanPath(QString(PROJECT_SOURCE_DIR) + "/../jsons"),
      QString("*.json"));

  QFile file(validate_file);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Could not open the file.");
    return;
  }

    QString content = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject rootJsonObj = doc.object()["root"].toObject();

    Joint::population = 0;

    ui->mygl->m_skeletonCurrent.loadJson(rootJsonObj);

    Q_EMIT ui->mygl->sig_sendRootNode(ui->mygl->m_skeletonCurrent.root.get());
}

void MainWindow::on_actionQuit_triggered() { QApplication::exit(); }

void MainWindow::on_actionCamera_Controls_triggered() {
  CameraControlsHelp *c = new CameraControlsHelp();
  c->show();
}

void MainWindow::slot_addListItem(QListWidgetItem *i) {
  Vertex *v = dynamic_cast<Vertex *>(i);
  if (v != nullptr) {
    ui->vertsListWidget->addItem(i);
    return;
  }
  Face *f = dynamic_cast<Face *>(i);
  if (f != nullptr) {
    ui->facesListWidget->addItem(i);
    return;
  }

  HalfEdge *he = dynamic_cast<HalfEdge *>(i);
  if (he != nullptr) {
    ui->halfEdgesListWidget->addItem(i);
    return;
  }
}

void MainWindow::slot_addRootToTreeWidget(QTreeWidgetItem *i) {
    ui->jointsTreeWidget->addTopLevelItem(i);
}
