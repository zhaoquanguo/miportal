#ifndef MIUPNPSERVICE_H
#define MIUPNPSERVICE_H
#include <QObject>
class MiUpnpDevice;
class MiUpnpAction;
class MiUpnpEvent;

struct _UpnpService;
struct _UpnpEvent;

class MiUpnpService: public QObject
{
    friend class MiUpnpAction;
    Q_OBJECT
public:
    MiUpnpService(MiUpnpDevice *device, _UpnpService* service);

    MiUpnpDevice* device() const;
    QString getProperty(const QString& property_name);

    bool subscribe();
    void unsubscribe();

    MiUpnpAction* getAction(const QString& name);

signals:
    void serviceEvent(MiUpnpEvent* event);
private:
    static void __event_listener(_UpnpEvent *event, void *ctx);
    void event_listener(_UpnpEvent *event);
protected:
    _UpnpService* inner_upnp_service_ = nullptr;
    MiUpnpDevice* device_ = nullptr;
};

#endif // MIUPNPSERVICE_H
