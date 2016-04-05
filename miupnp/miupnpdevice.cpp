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

QString __getProperty(PropertyList* properties, const QString& name)
{
    QString value;
    for (uint32_t i = 0; i < PropertyList_GetSize(properties); ++i) {
        Property *p = PropertyList_GetPropertyAt(properties, i);
        if (name.compare(p->definition.name) == 0) {
            switch (p->definition.type.clazzType) {
            case CLAZZ_BYTE:
                value = QString::asprintf("%d", p->value.object.value.byteValue);
                break;

            case CLAZZ_WORD:
                value = QString::asprintf("%d", p->value.object.value.wordValue);
                break;

            case CLAZZ_INTEGER:
                value = QString::asprintf("%d", p->value.object.value.integerValue);
                break;

            case CLAZZ_LONG:
                value = QString::asprintf("%lld", p->value.object.value.longValue);
                break;

            case CLAZZ_BOOLEAN:
                value = QString::asprintf("%d",p->value.object.value.boolValue);
                break;

            case CLAZZ_FLOAT:
                value = QString::asprintf("%f", p->value.object.value.floatValue);
                break;

            case CLAZZ_DOUBLE:
                value = QString::asprintf("%f", p->value.object.value.doubleValue);
                break;

            case CLAZZ_CHAR:
                value = QString::asprintf("%c", p->value.object.value.charValue);
                break;

            case CLAZZ_STRING:
                value = QString::asprintf("%s", p->value.object.value.stringValue);
                break;

            default:
                break;
            }
             break;
        }
    }

    qDebug() << name << ":" << value;
    return value;
}

QString MiUpnpDevice::id() const
{
    PropertyList *properties = UpnpDevice_GetPropertyList(inner_device);
    QString id = __getProperty(properties, UPNP_DEVICE_UDN);
    return id;
}

QString MiUpnpDevice::friendlyName() const
{
    PropertyList *properties = UpnpDevice_GetPropertyList(inner_device);
    QString friendlyName = __getProperty(properties, UPNP_DEVICE_FriendlyName);
    return friendlyName;
}

QString MiUpnpDevice::getProperty(const QString& properyName)
{
    PropertyList *properties = UpnpDevice_GetPropertyList(inner_device);
    return __getProperty(properties, properyName);
}

MiUpnpService* MiUpnpDevice::getService(QString serviceId)
{
    UpnpServiceList *svcList = UpnpDevice_GetServiceList(inner_device);
    _UpnpService* upnp_service = UpnpServiceList_GetService(svcList, serviceId.toLocal8Bit().constData());
    if (!upnp_service) {
        qWarning() << "Service not found:" << serviceId;
        return nullptr;
    }

    MiUpnpService* miupnp_service = new MiUpnpService(this, upnp_service);
    return miupnp_service;
}

