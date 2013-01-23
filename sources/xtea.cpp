#include "xtea.h"
#include <QString>
#include <QDebug>

XTea::XTea()
{
    T = 0;
    C = 0x9E3779B9;
    R = 64;
    K[0]=K[1]=K[2]=K[3]=0;
}

QString  XTea::name() { return QString("XTea"); }

QString XTea::author() { return QString("D.J.Wheeler");}

int XTea::keyLength() { return 128; }

int XTea::blockLength() { return 64; }

int XTea::speedLevel() { return 8; }

int XTea::safetyLevel() { return 5; }

int XTea::types() {return static_cast<int>(JobTypes::ECB_ENC|JobTypes::ECB_DEC);}

bool XTea::init(JobTypes::JobType t, QString k,QString s)
{
    qDebug()<<"[XTea say:] Start plugin init";

    qDebug()<<"[XTea say:] Set type of plugin";
    // set type
    this->type = t;

    // if key is empty return
    if (k.isEmpty()) return false;

    qDebug()<<"[XTea say:] Set key of plugin";
    // fill key
    // reuse k if it is small
    for(quint8 i=0,j=0; i<4; i++)
    {
        K[i]=k.at(j++).unicode()<<16;

        if (j<k.count()) K[i] |= k.at(j++).unicode();
        else
        {
            j=0;
            K[i] |= k.at(j++).unicode();
        }
    }

    qDebug()<<"[XTea say:] Set special args of plugin";
    // remove all spaces
    s.remove(QChar(' '));

    qint16 ind=0;

    bool ok[3]={true,true,true};
    // parse
    while ((ind=s.indexOf(QChar(' ')))!=-1)
    {
        QString temp = s.left(ind+1);

        if (temp.left(2)=="C=") this->C=temp.remove(0,2).toInt(&ok[0]);
        if (temp.left(2)=="T=") this->T=temp.remove(0,2).toInt(&ok[1]);
        if (temp.left(2)=="R=") this->R=temp.remove(0,2).toInt(&ok[2]);
    }

    qDebug()<<"[XTea say:] End plugin init";

    return (ok[0] && ok[1] && ok[2]);
}


void XTea::encode (char *in, char* out)
{
    quint32 A=0,B=0;

    for(quint8 i=0; i<4; i++) A = A<<8 | static_cast<quint8>(in[i]);
    for(quint8 i=0; i<4; i++) B = B<<8 | static_cast<quint8>(in[i+4]);

    T=0; // DANGER!!!

    for(quint8 i=0; i<R/2; i++)
    {
        T+=C;
        A+=( ((B<<4) XOR (B>>5)) + B ) XOR ( T + K[ (T>>11) & 0x03 ]);
        B+=( ((A<<4) XOR (A>>5)) + A ) XOR ( T + K[ T & 0x03 ]);
    }

    out[0] = static_cast<char> ((A>>24) & 0xFF);
    out[1] = static_cast<char> ((A>>16) & 0xFF);
    out[2] = static_cast<char> ((A>> 8) & 0xFF);
    out[3] = static_cast<char> ((A    ) & 0xFF);

    out[4] = static_cast<char> ((B>>24) & 0xFF);
    out[5] = static_cast<char> ((B>>16) & 0xFF);
    out[6] = static_cast<char> ((B>> 8) & 0xFF);
    out[7] = static_cast<char> ((B    ) & 0xFF);

}

void XTea::decode (char *in, char* out)
{
    quint32 A=0,B=0;

    for(quint8 i=0; i<4; i++) A = A<<8 | static_cast<quint8>(in[i]);
    for(quint8 i=0; i<4; i++) B = B<<8 | static_cast<quint8>(in[i+4]);

    T+= C MUL32 (R/2);

    for(quint8 i=0; i<R/2; i++)
    {
        B-=( (((A>>5) XOR (A<<4)) + A) XOR (T+K[ T & 0x03 ]));
        A-=( (((B>>5) XOR (B<<4)) + B) XOR (T+K[ (T>>11) & 0x03 ]));
        T-=C;
    }

    out[0] = static_cast<char> ((A>>24) & 0xFF);
    out[1] = static_cast<char> ((A>>16) & 0xFF);
    out[2] = static_cast<char> ((A>> 8) & 0xFF);
    out[3] = static_cast<char> ((A    ) & 0xFF);

    out[4] = static_cast<char> ((B>>24) & 0xFF);
    out[5] = static_cast<char> ((B>>16) & 0xFF);
    out[6] = static_cast<char> ((B>> 8) & 0xFF);
    out[7] = static_cast<char> ((B    ) & 0xFF);
}


PluginInterface* XTea::clone()
{
    return new XTea();
}
