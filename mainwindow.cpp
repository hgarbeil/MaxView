#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QScrollArea>
#include <QDebug>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mf = new ModisFiles ("F:/modis/modisfiles.txt") ;
//    mf->setPreferredDirectories(QString("/hotspot3/data2/maxalerts/2014"),
//     "/local/worldbase/MOD021KM/2014","/hotspot3/data2/MOD03/2014") ;

    ui->imwidget->setImageSize (1354,2030);

    connect (ui->imwidget, SIGNAL(radClicked(QPoint)), this, SLOT(defLoc(QPoint))) ;
    connect (ui->alertListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(alertClicked(QListWidgetItem *))) ;


    ns = 1354 ;
    nl = 2030 ;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_browseButton_clicked()
{

   QString fname = QFileDialog::getOpenFileName(this, "Input Alert File", mf->alertDir) ;
   ui->maxAlertLE->setText(fname) ;
   qDebug() << "Alert File is : " <<fname ;

}


void MainWindow::on_loadButton_clicked()
{
    int iloc ;
    QString alName = ui->maxAlertLE->text() ;
    loadAlerts (alName) ;
    //qDebug() << "Input max alert web file : " << alName ;
    QFileInfo *qf = new QFileInfo (alName) ;
    qDebug() << qf->fileName () ;
    QString fname = qf->fileName() ;
    if (qf->exists()){
        mf->loadFiles (alName);
    }
    ui->imwidget->loadQImage (mf->b22, 1354, 2030) ;
    ui->imwidget->setImageSize (1354,2030) ;
    ui->imwidget->repaint() ;


}


void MainWindow::loadAlerts (QString al){
    int count = 0 ;
    int xval, yval ;
    QString tmpfile, curline ;
    QStringList strl ;

    alvec.clear() ;
    exmaxvec.clear() ;
    exntivec.clear() ;

    QFile *alfile = new QFile (al);
    alfile->open(QIODevice::ReadOnly | QIODevice::Text) ;
    QTextStream *qts = new QTextStream (alfile) ;
    while (!qts->atEnd()){
        curline = qts->readLine () ;
        qDebug()<< curline ;
        if (count > 0){
            ui->alertListWidget->insertItem (count-1, curline) ;
            strl = curline.split(QRegExp("\\s+"));

            xval = strl[12].toInt();
            yval = strl[11].toInt() ;
            alvec << QPoint (xval, yval);
        }

        count++ ;
    }
    alfile->close() ;
    qDebug() << "Number of alerts : " << alvec.count() ;
    count = 0 ;

    tmpfile = al ;
    tmpfile.replace ("maxalerts", "alerts_exclusive") ;
    tmpfile.replace ("_web.txt", "_ntix.txt") ;
    alfile = new QFile (tmpfile);
    alfile->open(QIODevice::ReadOnly | QIODevice::Text) ;
    qts = new QTextStream (alfile) ;
    while (!qts->atEnd()){
        curline = qts->readLine () ;
        qDebug()<< curline ;
        if (count > 0){
            ui->exNTIAlertListWidget->insertItem (count-1, curline) ;
            strl = curline.split(QRegExp("\\s+"));

            xval = strl[12].toInt();
            yval = strl[11].toInt() ;
            exntivec << QPoint (xval, yval);
        }

        count++ ;
    }
    alfile->close() ;
    qDebug() << "Number of alerts : " << exntivec.count() ;
    count = 0 ;

    tmpfile = al ;
    tmpfile.replace ("maxalerts", "alerts_exclusive") ;
    tmpfile.replace ("_web.txt", "_maxx.txt") ;
    alfile = new QFile (tmpfile);
    alfile->open(QIODevice::ReadOnly | QIODevice::Text) ;
    qts = new QTextStream (alfile) ;
    while (!qts->atEnd()){
        curline = qts->readLine () ;
        qDebug()<< curline ;
        if (count > 0){
            ui->exMaxAlertListWidget->insertItem (count-1, curline) ;
            strl = curline.split(QRegExp("\\s+"));

            xval = strl[12].toInt();
            yval = strl[11].toInt() ;
            exmaxvec << QPoint (xval, yval);
        }

        count++ ;
    }
    alfile->close() ;
    qDebug() << "Number of alerts : " << exmaxvec.count() ;
    ui->imwidget->setAlerts (alvec, exntivec, exmaxvec);
    ui->imwidget->setAlertFlags (ui->maxCB->isChecked(), ui->xmaxCB->isChecked(), ui->xntiCB->isChecked()) ;


}

void MainWindow::defLoc(QPoint qp) {
    int samploc, line, samp ;
    float val, val22, t22, t32 ;
    //qDebug() << "clicked "<<qp.x() << "  " << qp.y() ;
    line = qp.y() ;
    samp = qp.x() ;
    samploc = line * ns + samp ;
    val = mf->b22[samploc] ;
    if (val <0. || val > 2.0){
        val = mf->b21[samploc] ;
    }
    val22 = val ;
    ui->b22LE->setText (QString("%1").arg(val,6,'g',3)) ;
    val = mf->b32[samploc] ;
    ui->b32LE->setText (QString("%1").arg(val,6,'g',3)) ;
    val = mf->nti[samploc] ;
    ui->ntiLE->setText (QString("%1").arg(val,6,'g',3)) ;
    val = mf->b6[samploc] ;
    ui->b6LE->setText (QString("%1").arg(val,6,'g',3)) ;
    ui->lineLE->setText (QString("%1").arg(line,6)) ;
    ui->sampLE->setText (QString("%1").arg(samp,6)) ;
    val = mf->lat[samploc] ;
    ui->latLE->setText (QString("%1").arg(val,8,'g',4)) ;
    val = mf->lon[samploc] ;
    ui->lonLE->setText (QString("%1").arg(val,9,'g',4)) ;
    t22 = mf->bb2temp (val22, 4.) ;
    ui->tb22LE->setText (QString("%1").arg(t22,6,'g',3)) ;
    t32 = mf->bb2temp (mf->b32[samploc], 12) ;
    ui->tb32LE->setText (QString("%1").arg(t32,6,'g',3)) ;
    ui->tdiffLE->setText (QString("%1").arg(t22-t32,6,'g',3)) ;
    val = val22 / mf->max2232[samploc] ;
    ui->fracLE->setText (QString("%1").arg(val,6,'g',3)) ;
}


void MainWindow::alertClicked (QListWidgetItem *alt) {
    int xval, yval ;
    QString curline = alt->text() ;
    QStringList strl = curline.split(QRegExp("\\s+"));
    yval = strl[11].toInt() ;
    xval = strl[12].toInt() ;
    defLoc(QPoint(xval,yval));

}

void MainWindow::on_updateAlertButton_clicked()
{
    ui->imwidget->setAlertFlags (ui->maxCB->isChecked(), ui->xmaxCB->isChecked(), ui->xntiCB->isChecked()) ;
}
