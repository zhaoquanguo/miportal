#include "miupnpservice.h"
#include "miupnpdevice.h"
#include "miupnpruntime.h"
#include "miupnpaction.h"
#include "miupnpevent.h"
#include "Upnp.h"

#include <QDebug>

MiUpnpService::MiUpnpService(MiUpnpDevice *device, _UpnpService* service)
    : QObject(device)
{
    inner_upnp_service_ = service;
    device_ = device;
}

MiUpnpDevice* MiUpnpService::device() const
{
    return device_;
}

void MiUpnpService::__event_listener(_UpnpEvent *event, void *ctx)
{//-------------------------------------------------------
    // Property Name (2)
    //-------------------------------------------------------
    MiUpnpService *thiz = (MiUpnpService*)ctx;
    thiz->event_listener(event);
}

void MiUpnpService::event_listener(_UpnpEvent *event)
{
    MiUpnpEvent *mi_event = new MiUpnpEvent(this, event);
    emit serviceEvent(mi_event);
}

bool MiUpnpService::subscribe()
{
    qDebug() << "to subcribe event.";
    _UpnpRuntime* inner_runtime = device_->runtime()->innerRuntime();

    UpnpError upnp_error = { 0 };

    TinyRet ret = UpnpRuntime_Subscribe(
                inner_runtime,
                inner_upnp_service_,
                0,
                __event_listener,
                this,
                &upnp_error);
    if (RET_FAILED(ret)) {
        qCritical() << "Fail to subcribe event : " << ret << " msg:" << upnp_error.description;
        return false;
    }
    return true;
}

void MiUpnpService::unsubscribe()
{
    qDebug() << "to unsubcribe event :";
    _UpnpRuntime* inner_runtime = device_->runtime()->innerRuntime();

    UpnpError upnp_error = { 0 };

    TinyRet ret = UpnpRuntime_Unsubscribe(
                inner_runtime,
                inner_upnp_service_,
                &upnp_error);
    if (RET_FAILED(ret)) {
        qCritical() << "Fail to unsubcribe event : " << ret << " msg:" << upnp_error.description;
    }
}

MiUpnpAction* MiUpnpService::getAction(const QString& name)
{
    UpnpAction *action = UpnpService_GetAction(inner_upnp_service_, name.toLocal8Bit().constData());
    if (!action) {
        qDebug() << "invalid action: " << name;
        return nullptr;
    }

    MiUpnpAction* miaction = new MiUpnpAction(this, action);
    return miaction;
}
