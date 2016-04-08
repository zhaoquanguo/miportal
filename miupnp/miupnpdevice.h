#ifndef MIUPNPDEVICE_H
#define MIUPNPDEVICE_H
#include <QObject>

class MiUpnpRuntime;
class MiUpnpService;
struct _UpnpDevice;
struct _PropertyList;
class MiUpnpDevice : public QObject
{
    Q_OBJECT
public:
    MiUpnpDevice(MiUpnpRuntime *runtime, _UpnpDevice *device);
    ~MiUpnpDevice();

    _UpnpDevice* innerDevice();

    QString id() const;
    QString friendlyName() const;

    MiUpnpService* getService(QString serviceId);

    MiUpnpRuntime* runtime() const;
protected:
    MiUpnpRuntime *miupnp_runtime = nullptr;
    _UpnpDevice *inner_device = nullptr;
};

#endif // MIUPNPDEVICE_H
