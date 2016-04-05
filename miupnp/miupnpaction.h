#ifndef MIUPNPACTION_H
#define MIUPNPACTION_H

#include <QObject>

struct _UpnpAction;
class MiUpnpAction : public QObject
{
    Q_OBJECT
public:
    explicit MiUpnpAction(QObject *parent, _UpnpAction *inner_action);

    bool setInputPara(const QString& para_name, const bool& para_value);

    bool invoke();

    bool getResult(const QString& field_name, bool& value);

signals:

public slots:

protected:
    _UpnpAction* inner_action_ = nullptr;
};

#endif // MIUPNPACTION_H
