#ifndef IMWIDGET_H
#define IMWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QVector>

class ImWidget : public QWidget
{
    Q_OBJECT
public:
    int lineLoc, hLineLoc ;
    bool userFlag, startTracking, mouseClicking, alflag, xntiflag, xmaxflag ;
    float uMax, uMin ;
    explicit ImWidget(QWidget *parent = 0);
    void loadQImage (float *dat, int ns, int nl, QVector <QPoint>) ;
    void loadQImage (float *dat, int ns, int nl) ;
    void loadQImage (unsigned short *dat, int ns, int nl) ;
    void setImageSize (int x,int y) ;
    void setImageType (QString nm) ;
    void setMinMax (float min, float max) ;
    //void setScaleValues (bool aflag, float min, float max) ;
    void setAlerts (QVector<QPoint>, QVector<QPoint>, QVector<QPoint>);
    void setAlertFlags (bool aflag, bool xm, bool xn);
    QString imtype ;
    QImage *qim ;
    QVector<QPoint> alvec, exnti, exmax ;
protected:
    void paintEvent (QPaintEvent *) ;
    void mousePressEvent (QMouseEvent *) ;
    void mouseMoveEvent (QMouseEvent *) ;
    void mouseReleaseEvent (QMouseEvent *) ;


    int ns, nl ;
signals:
    void radClicked (QPoint) ;
    void setMnMx (float *) ;
    void reDisplay() ;
public slots:
    void ShowContextMenu(const QPoint& pos) ;
    void startScaleDlg() ;
    void setScaleValues (bool aflag, float min, float max) ;

};

#endif // IMWIDGET_H
