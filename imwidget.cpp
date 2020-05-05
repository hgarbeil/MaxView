#include "imwidget.h"
#include <QPainter>
#include <QToolTip>
#include <QPoint>
#include <QRect>
#include <QMenu>
#include <QDebug>

ImWidget::ImWidget(QWidget *parent) :
    QWidget(parent)
{
    qim = NULL ;
    lineLoc = 0 ;
    hLineLoc = 0 ;
    startTracking = false ;
    mouseClicking = false ;
    ns = 1354 ; // from Instrument_config.h
    nl = 2030 ;
    setImageSize (ns, nl);
    userFlag = false ;
    imtype = "IMAGE" ;
    uMax = 10. ;
    uMin = 0. ;
    alflag = false ;
    xntiflag = false ;
    xmaxflag = false ;
    setContextMenuPolicy(Qt::CustomContextMenu) ;
    connect (this, SIGNAL(customContextMenuRequested(const QPoint &)),
             this, SLOT(ShowContextMenu(const QPoint&))) ;
}


void ImWidget::setImageType (QString s){
    imtype = s ;
}

void ImWidget::paintEvent (QPaintEvent *){

    int i, width, height ;
    width = this->width() ;
    height = this->height() ;

    QPainter p (this) ;
    if (qim){
        //QPen qp (Qt::yellow, 2, Qt::DashDotLine) ;

        p.drawImage (QRect (0,0,ns,nl), *qim) ;
        //p.setPen (qp);

    }
    else p.drawRect (0,0,ns, nl) ;

    int ix, iy ;

    if (alflag){
    QPen qp (Qt::yellow,2) ;
    p.setPen(qp) ;
    if (alvec.count()>0){
        for (i=0; i<alvec.count(); i++){
            ix = alvec[i].x()-2 ;
            iy = alvec[i].y() -2 ;
            QRect qrec (ix,iy, 5,5) ;
            p.drawRect(qrec);
        }
    }
    }

    if (xntiflag){
    QPen qp1 (Qt::red,2) ;
    p.setPen(qp1) ;
    if (exnti.count()>0){
        for (i=0; i<exnti.count(); i++){
            ix = exnti[i].x()-2 ;
            iy = exnti[i].y() -2 ;
            QRect qrec1 (ix,iy, 5,5) ;
            p.drawRect(qrec1);
        }
    }
    }

    if (xmaxflag){
    QPen qp1 (Qt::green,2) ;
    p.setPen(qp1) ;
    if (exmax.count()>0){
        for (i=0; i<exmax.count(); i++){
            ix = exmax[i].x()-2 ;
            iy = exmax[i].y() -2 ;
            QRect qrec2 (ix,iy, 5,5) ;
            p.drawRect(qrec2);
        }
    }
    }

}


void ImWidget::loadQImage (unsigned short *dat, int ns, int nl) {
    int   i, npix ;
    float fval, scale, off ;
    float min, max, mnmx[2] ;
    QRgb *line ;
    if (qim) delete qim ;
    qim = new QImage (ns, nl, QImage::Format_ARGB32) ;

    lineLoc = ns / 2 ;
    hLineLoc = nl / 4 ;


    unsigned char cbyte ;
    min=1.E9 ;
    max=-1.E9 ;
    npix = nl * ns ;
    int *idata = new int [ns * nl] ;

    if (userFlag) {
        max = uMax ;
        min = uMin ;
    } else {
        for (i=0; i<npix; i++){
            fval = *(dat+i) ;
            if (fval <=0) continue ;
            if (fval > max) max = fval ;
            if (fval< min) min = fval ;
        }
        mnmx[0] = min ;
        mnmx[1] = max ;
        uMax = max ;
        uMin = min ;
        emit (setMnMx ((float*)&mnmx[0])) ;

    }
    scale = 255. / (max - min) ;
    for (i=0; i<nl; i++){
        line = (QRgb *) qim->scanLine(i) ;
        for (int j=0; j<ns; j++){
            fval = *(dat+i*ns+j) ;
            fval = ((fval - min) * scale) ;
            if (fval < 0.) fval = 0. ;
            if (fval >255.) fval = 255. ;
            cbyte = (unsigned char)fval ;
            *(line+j) = qRgb ( cbyte, cbyte, cbyte) ;
        }
    }



    startTracking = true ;
    mouseClicking = true ;
}


void ImWidget::loadQImage (float *dat, int ns, int nl) {
    int   i, npix ;
    float fval, scale, off ;
    float min, max, mnmx[2] ;
    QRgb *line ;
    if (qim) delete qim ;
    qim = new QImage (ns, nl, QImage::Format_ARGB32) ;

    lineLoc = ns / 2 ;
    hLineLoc = nl / 2 ;

    unsigned char cbyte ;
    min=1.E9 ;
    max=-1.E9 ;
    npix = nl * ns ;
    int *idata = new int [ns * nl] ;

    if (userFlag) {
        max = uMax ;
        min = uMin ;
    } else {
        for (i=0; i<npix; i++){
            fval = *(dat+i) ;
            if (fval <=0) continue ;
            if (fval > max) max = fval ;
            if (fval< min) min = fval ;
        }
        mnmx[0] = min ;
        mnmx[1] = max ;
        //emit (setMnMx ((float*)&mnmx[0])) ;
        uMin = min ;
        uMax = max ;

    }



    scale = 255. / (max - min) ;
    for (i=0; i<nl; i++){
        line = (QRgb *) qim->scanLine(i) ;
        for (int j=0; j<ns; j++){
            fval = *(dat+i*ns+j) ;
            fval = ((fval - min) * scale) ;
            if (fval < 0.) fval = 0. ;
            if (fval >255.) fval = 255. ;
            cbyte = (unsigned char)fval ;
            *(line+j) = qRgb ( cbyte, cbyte, cbyte) ;
        }
    }







    startTracking = true ;
    mouseClicking = true ;
}


void ImWidget::loadQImage (float *dat, int ns, int nl, QVector<QPoint>alpts) {
    int   i, npix ;
    float fval, scale, off ;
    float min, max, mnmx[2] ;
    QRgb *line ;
    if (qim) delete qim ;
    qim = new QImage (ns, nl, QImage::Format_ARGB32) ;

    lineLoc = ns / 2 ;
    hLineLoc = nl / 2 ;

    unsigned char cbyte ;
    min=1.E9 ;
    max=-1.E9 ;
    npix = nl * ns ;
    int *idata = new int [ns * nl] ;

    if (userFlag) {
        max = uMax ;
        min = uMin ;
    } else {
        for (i=0; i<npix; i++){
            fval = *(dat+i) ;
            if (fval <=0) continue ;
            if (fval > max) max = fval ;
            if (fval< min) min = fval ;
        }
        mnmx[0] = min ;
        mnmx[1] = max ;
        //emit (setMnMx ((float*)&mnmx[0])) ;
        uMin = min ;
        uMax = max ;

    }



    scale = 255. / (max - min) ;
    for (i=0; i<nl; i++){
        line = (QRgb *) qim->scanLine(i) ;
        for (int j=0; j<ns; j++){
            fval = *(dat+i*ns+j) ;
            fval = ((fval - min) * scale) ;
            if (fval < 0.) fval = 0. ;
            if (fval >255.) fval = 255. ;
            cbyte = (unsigned char)fval ;
            *(line+j) = qRgb ( cbyte, cbyte, cbyte) ;
        }
    }


    int xval,yval, xinc ;
    for (i=0; i<alpts.count(); i++){
        yval = alpts[i].y();
        xval = alpts[i].x();

        for (int is=-3; is<4; is++){
            if (yval + is < 0) continue ;
            if (yval + is > nl-1) continue ;
            xinc = 1 ;
            if (is ==0) xinc = 3 ;

            line = (QRgb *) qim->scanLine(yval+is) ;
            for (int js=-xinc; js<xinc+1; js++){
                if (js + xval <0) continue ;
                if (js + xval > ns-1) continue ;
                if (is==0 & js==0){
                   *(line+xval+js) = qRgb (255,255,0);
                }
                else {
                   *(line+xval+js) = qRgb (255,0,0);
                }
            }
        }

    }


    startTracking = true ;
    mouseClicking = true ;
}

void ImWidget::setImageSize (int x, int y) {
    ns = x ;
    nl = y ;
    resize (x, y) ;
}


void ImWidget::mousePressEvent (QMouseEvent *ev){

    QToolTip::showText (ev->globalPos(),
                        QString::number(ev->pos().x())+","+
                        QString::number(ev->pos().y())) ;


}

void ImWidget::mouseMoveEvent (QMouseEvent *ev){
    lineLoc = ev->x() ;
    hLineLoc = ev->y() ;

    QToolTip::showText (ev->globalPos(),
                        QString::number(ev->pos().x())+","+
                        QString::number(ev->pos().y())) ;

    repaint() ;
}

void ImWidget::mouseReleaseEvent (QMouseEvent *ev){


    if (mouseClicking) emit(radClicked(ev->pos())) ;
    qDebug () << "(X Y) : " << ev->x() << " " << ev->y() ;
    //startTracking = false ;

}


void ImWidget::setMinMax (float mn, float mx) {
    uMin = mn ;
    uMax = mx ;
    userFlag = true ;
}


void ImWidget::ShowContextMenu(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    QAction *openDlg = myMenu.addAction("Adjust Display");
    connect (openDlg, SIGNAL (triggered()), this, SLOT(startScaleDlg()));

    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
        int hg = 0 ;


    }
    else
    {
        // nothing was chosen
    }
}


void ImWidget::startScaleDlg() {
    int hg = 0 ;
    hg +=1 ;
    bool autoFlag = !userFlag ;
    /*
    DispAdjustDialog *dadlg = new DispAdjustDialog(this);
    connect (dadlg, SIGNAL(setMinMaxVals(bool, float, float)), this, SLOT(setScaleValues(bool, float, float)));

    dadlg->setValues(autoFlag,this->uMin, uMax);
    dadlg->setTitlestring (imtype) ;
    dadlg->show () ;
    */
}

void ImWidget::setScaleValues (bool aFlag, float min, float max) {
    uMin = min ;
    uMax = max ;
    userFlag = !aFlag ;
    this->reDisplay () ;
}


void ImWidget::setAlerts (QVector<QPoint> maxvec, QVector<QPoint> xnti, QVector<QPoint> xmax){
    alvec = maxvec ;
    exnti = xnti ;
    exmax = xmax ;
    alflag = true ;

    //this->repaint() ;



}

void ImWidget::setAlertFlags (bool aflag, bool xm, bool xn){
    alflag = aflag ;
    xmaxflag = xm ;
    xntiflag = xn ;
    this->repaint() ;
}
