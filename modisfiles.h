#ifndef MODISFILES_H
#define MODISFILES_H

#include <QObject>
#include "modis_hdf.h"

class ModisFiles {

public:
    ModisFiles(QString paramfile);
    ~ModisFiles() ;
    //void setPreferredDirectories (QString alertdir, QString mod21dir, QString mod03dir) ;
    void loadFiles (QString) ;
    void readRadianceFile (QString) ;
    void readGeomFile (QString ifile) ;
    float bb2temp (float rad, float wave);
    void calcNTI ();
    void getMaxRadiances ();
    QString alertDir, mod21Dir, mod03Dir ;
    int ns, nl ;
    float *b21, *b22, *b32, *b6, *nti, *max2232 ;
    // pointers to hdf arrays
    float *lat, *lon ;
    int16 *sunzen, *satzen ;
    modis_hdf *geom ;


};

#endif // MODISFILES_H
