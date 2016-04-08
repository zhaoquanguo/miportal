#include "miupnpevent.h"
#include <UPnP.h>
#include <QDebug>

MiUpnpEvent::MiUpnpEvent(QObject *parent, _UpnpEvent* event) : QObject(parent)
{
    event_ = event;
}

QString MiUpnpEvent::GetArgumentValue(const QString& argument)
{
    const char * value = UpnpEvent_GetArgumentValue(event_, argument.toLocal8Bit().constData());
    if (value != NULL) {
        qDebug() << "event " << argument << ":" << value;
        return value;
    }
    else {
        qWarning() << "cannot find value for arg:" << argument;
        return QString();
    }
}
