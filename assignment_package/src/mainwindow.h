#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTreeWidgetItem>
#include <QMainWindow>
#include <QListWidgetItem>

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

private Q_SLOTS:
    void on_actionImport_Skeleton_triggered();
    void on_actionImport_Obj_triggered();

    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    void slot_addRootToTreeWidget(QTreeWidgetItem*);
    void slot_addListItem(QListWidgetItem *i);

    void slot_setSelectedJoint(QTreeWidgetItem *i);
    void slot_setSelectedVert(QListWidgetItem *i);
    void slot_setSelectedFace(QListWidgetItem *i);
    void slot_setSelectedHedge(QListWidgetItem *i);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
