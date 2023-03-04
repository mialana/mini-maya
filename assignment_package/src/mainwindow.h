#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "mesh.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString filename;

private slots:
    void on_actionImport_Obj_triggered();

    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    void slot_addListItem(QListWidgetItem *i);

    void slot_setSelectedVert(Vertex*);
    void slot_setVertGeneric(QListWidgetItem *i);
    void slot_setSelectedFace(Face*);
    void slot_setFaceGeneric(QListWidgetItem *i);
    void slot_setSelectedHedge(HalfEdge*);
    void slot_setHedgeGeneric(QListWidgetItem *i);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
