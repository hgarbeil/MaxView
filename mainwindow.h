#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListWidgetItem>
#include "modisfiles.h"
#include "imwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadAlerts (QString) ;
    ModisFiles *mf ;
    ImWidget *im ;
    QVector <QPoint> alvec, exntivec, exmaxvec ;
    int ns, nl ;

private slots:
    void on_browseButton_clicked();
    void alertClicked (QListWidgetItem*);
    void defLoc (QPoint);
    void on_loadButton_clicked();

    void on_updateAlertButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
