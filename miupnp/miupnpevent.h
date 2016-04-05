#ifndef MIUPNPEVENT_H
#define MIUPNPEVENT_H

#include <QObject>
struct _UpnpEvent;
class MiUpnpEvent : public QObject
{
    Q_OBJECT
public:
    explicit MiUpnpEvent(QObject *parent, _UpnpEvent* event);

    QString GetArgumentValue(const QString& argument);
signals:

public slots:

private:
    _UpnpEvent* event_ = nullptr;
};

#endif // MIUPNPEVENT_H
