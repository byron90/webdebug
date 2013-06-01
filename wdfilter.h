#ifndef WDFILTER_H
#define WDFILTER_H

#include <QString>

class WDFilter
{
public:
    WDFilter();

    //different filtering func
    //*** BlockUrl
    bool blockUrl(QString &qstrUrl);
    //*** AutoResponse
    QString autoResponse(QString &qstrUrl);
    //*** hide with some resp codes
    bool hideCodes(QString &qstrUrl, QString &qstrCode);
    //*** block some respn type
    bool blockType(QString &qstrUrl, QString &qstrType);

};

#endif // WDFILTER_H
