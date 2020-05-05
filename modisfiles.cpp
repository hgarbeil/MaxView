#include "modisfiles.h"
#include "sinu_1km.h"
#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QString>

ModisFiles::ModisFiles(QString paramfl)
{
    QFile qf (paramfl);
    qf.open (QIODevice::ReadOnly | QIODevice::Text);
    QTextStream qts (&qf);
    this->mod21Dir = qts.readLine() ;
    this->mod03Dir = qts.readLine() ;
    this->alertDir = qts.readLine() ;
    qf.close() ;
    b21 = NULL ;
    b22 = NULL ;
    b32 = NULL ;
    b6 = NULL ;
    nti = NULL ;
    max2232 = NULL ;
    ns = 1354 ;
    nl = 2030 ;
    geom = NULL ;
}

ModisFiles::~ModisFiles(){
    if (b21) delete [] b21 ;
    if (b22) delete [] b22 ;
    if (b32) delete [] b32 ;
    if (b6) delete [] b6 ;
    if (nti)delete [] nti ;
    if (geom) delete geom ;
    if (max2232) delete max2232 ;

}

void ModisFiles::loadFiles (QString ifile){
    int iloc, iloc1,iloc2 ;
    QString instring ;
    QStringList lstr ;
    // ifile is the string from the web.txt file,
    // easiest way to go about finding the files of interest is
    // to open the day_xxx.txt file and getting the rad and geo files
    iloc = ifile.indexOf ("A2014");
    qDebug () << "start of string is " <<  ifile.indexOf ("A2014");
    QString daystr = ifile.mid(iloc+5,3) ;
    QString searchstring = ifile.mid (iloc, 13) ;
    QString dayfilename = QString("/home/harold/workdir/modis_alert_dev/globfiles/day_%1.txt").arg(daystr) ;
    qDebug() << "day file list is : " << dayfilename ;
    qDebug() << "search string is " << searchstring ;
    QString raddir = QString ("%1/%2").arg(this->mod21Dir).arg(daystr);
    QDirIterator *rdir = new QDirIterator(raddir) ;
    qDebug() << "Radiance dir is " << raddir ;



    while (rdir->hasNext()) {
        instring = rdir->next() ;

        iloc = instring.indexOf(searchstring) ;
        iloc1 = instring.indexOf(".rad");
        iloc2 = instring.indexOf(".hdr");
        if (iloc > 0 && iloc1 > 0 && iloc2<=0){
            qDebug () << "radiance file is : " << instring ;
            readRadianceFile (instring) ;
            break ;
        }
    }

    // get the geom file
    searchstring = searchstring.replace ("021KM","03") ;
    QString geodir = QString ("%1/%2").arg(this->mod03Dir).arg(daystr);
    qDebug() << "geodir is : " << geodir ;
    rdir = new QDirIterator (geodir) ;
    while (rdir->hasNext()) {
        instring = rdir->next() ;
        iloc = instring.indexOf(searchstring) ;
        iloc1 = instring.indexOf(".hdf");
        if (iloc > 0 && iloc1 > 0){
            qDebug () << "geo file is : " << instring ;
            //readRadianceFile (instring) ;
            readGeomFile(instring) ;
            break ;
        }
    }
    //for now, just use frac written in alert file
    getMaxRadiances() ;
}



void ModisFiles::readRadianceFile (QString idir){
    if (b21) delete [] b21 ;
    if (b22) delete [] b22 ;
    if (b32) delete [] b32 ;
    if (b6) delete [] b6 ;
    b21 = new float [ns * nl] ;
    b22 = new float [ns * nl] ;
    b6 = new float [ns * nl] ;
    b32 = new float [ns * nl] ;

    QFile qf (idir);
    qf.open (QIODevice::ReadOnly) ;
    qf.read((char *)b21, 4 * nl * ns) ;
    qf.read((char *)b22, 4 * nl * ns) ;
    qf.read((char *)b32, 4 * ns *nl) ;
    qf.read ((char *)b6, 4 * ns * nl) ;
    qf.close() ;

    this->calcNTI() ;

}


void ModisFiles::readGeomFile (QString ifile){
    if (geom) delete geom ;
    geom = new modis_hdf(ifile.toLatin1().data()) ;
    geom->init_MOD03() ;
    qDebug() << "Cent lat is : " << geom->latarr[1015*1354+670] ;
    qDebug() << "Cent lon is : " << geom->lonarr[1015*1354+670] ;
    sunzen = geom->solzen ;
    satzen = geom->senszen ;
    lat = geom->latarr ;
    lon = geom->lonarr ;

}


void ModisFiles::calcNTI(){
    int i ;
    float b221val, b32val ;
    if (nti) delete[] nti ;
    nti = new float [ns * nl] ;
    for (i=0; i<ns*nl; i++){
        nti[i] = -10. ;
        b221val = b22[i] ;
        b32val = b32[i] ;
        if (b32val <0) continue ;
        if (b221val <0 || b221val > 2.f){
            if (b21[i]<0) continue ;
            b221val = b21[i] ;
        }
        nti[i] = (b221val-b32val)/(b221val+b32val);
    }

}

void ModisFiles::getMaxRadiances(){

    if (max2232) delete [] max2232 ;
    max2232 = new float [ns * nl * 2] ;
    sinu_1km *sinu = new sinu_1km() ;
    sinu->get_max_array (lat, lon, ns*nl, 3, max2232);


    delete sinu ;

}

float ModisFiles::bb2temp (float rad, float wave){
    double k, k2, k1, emiss=1., val ;
    double h, c ;
    k = 1.38066e-23 ;
    c = 2.99793E8 ;
    h = 6.626068E-34 ;

    k1 = 119.104E7 ;
    //
    // convert microns to meters
    wave = wave / 1000000. ;
    // convert rad to m-3 rather than m-2 microns
    rad = rad * 1E6 ;
    k2 = h * c / (k * wave) ;
    k1 = 2. * h * c * c / pow(wave,5.) ;

    val = log ((emiss * k1)/(rad)+1.) ;
    val = k2 / val  ;
    return (float(val)) ;

}

