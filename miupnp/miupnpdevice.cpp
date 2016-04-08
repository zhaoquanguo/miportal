#include "miupnpruntime.h"
#include "miupnpdevice.h"
#include "miupnpservice.h"
#include <Upnp.h>
#include <QDebug>

UpnpDevice* MiUpnpDevice::innerDevice()
{
    return inner_device;
}

MiUpnpDevice::MiUpnpDevice(MiUpnpRuntime *runtime, _UpnpDevice *device)
    : QObject(runtime)
{
    miupnp_runtime = runtime;
    inner_device = device;
}

MiUpnpDevice::~MiUpnpDevice()
{
    if (inner_device) {
        UpnpDevice_Delete(inner_device);
        inner_device = nullptr;
    }
}

MiUpnpRuntime* MiUpnpDevice::runtime() const
{
    return miupnp_runtime;
}

QString MiUpnpDevice::id() const
{
    QString id = UpnpDevice_GetDeviceId(inner_device);
    return id;
}

QString MiUpnpDevice::friendlyName() const
{
    QString friendlyName = UpnpDevice_GetFriendlyName(inner_device);
    return friendlyName;
}

MiUpnpService* MiUpnpDevice::getService(QString serviceId)
{
    _UpnpService* upnp_service = UpnpDevice_GetService(inner_device, serviceId.toLocal8Bit().constData());
    if (!upnp_service) {
        qWarning() << "Service not found:" << serviceId;
        return nullptr;
    }

    MiUpnpService* miupnp_service = new MiUpnpService(this, upnp_service);
    return miupnp_service;
}

