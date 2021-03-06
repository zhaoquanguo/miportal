#ifndef MIUPNPACTION_H
#define MIUPNPACTION_H

#include <QObject>

struct _UpnpAction;
class MiUpnpService;
class MiUpnpAction : public QObject
{
    Q_OBJECT
public:
    explicit MiUpnpAction(MiUpnpService *parent, _UpnpAction *inner_action);

    bool setInputPara(const QString& para_name, const bool& para_value);

    bool invoke();

    bool getResult(const QString& field_name, bool& value);

    QString GetArgumentRelatedStateVariable(const QString& argumentName);
signals:

public slots:

protected:
    MiUpnpService* service_ = nullptr;
    _UpnpAction* inner_action_ = nullptr;
};

#endif // MIUPNPACTION_H
