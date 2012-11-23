#ifndef XTEA_H
#define XTEA_H

#include <QtCore>
#include "plugininterface.h"
#include "jobtypes.h"

#define XOR     ^
#define MUL32   *

class XTea : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:

    XTea();

    QString name();
    QString author();
    int keyLength();
    int blockLength();
    int speedLevel();
    int safetyLevel();
    int types();

    void encode (char *in, char* out);
    void decode (char *in, char* out);

    bool init(JobTypes::JobType type, QString key,QString args);

    PluginInterface* clone();

private:

    JobTypes::JobType type;

    quint32 T;
    quint32 C;
    quint8  R;
    quint32 K[4];

};

#endif // TEA_H
