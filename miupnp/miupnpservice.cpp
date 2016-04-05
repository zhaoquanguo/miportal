#include "miupnpservice.h"
#include "miupnpdevice.h"
#include "miupnpruntime.h"
#include "miupnpaction.h"
#include "miupnpevent.h"

#include <Upnp.h>
#include <QDebug>

MiUpnpService::MiUpnpService(MiUpnpDevice *device, _UpnpService* service)
    : QObject(device)
{
    inner_upnp_service_ = service;
}

MiUpnpDevice* MiUpnpService::device() const
{
    MiUpnpDevice *device = dynamic_cast<MiUpnpDevice*>(parent());
    if (!device) {
        qWarning("cannot dynamic cast to MiUpnpDevice from parent.");
        return nullptr;
    }
    return device;
}

QString MiUpnpService::getProperty(const QString& property_name)
{
   QString property_value =  UpnpService_GetPropertyValue(
               inner_upnp_service_, property_name.toLocal8Bit().constData());
   return property_value;
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
    _UpnpSubscription sub = { 0 };
    sub.service = inner_upnp_service_;
    sub.listener = __event_listener;
    sub.ctx = this;
    sub.timeout = 0;
    MiUpnpDevice *device = this->device();
    bool ok = device->runtime()->subscribe(&sub);
    if (!ok) {
        qCritical() << "Fail to subcribe event";
        return false;
    }
    subscribeId_ = sub.subscribeId;
    qDebug() <<"subcribed with id:" << subscribeId_;
    return true;
}

void MiUpnpService::unsubscribe()
{
    qDebug() << "to unsubcribe event :" << subscribeId_;
    _UpnpSubscription sub = { 0 };
    sub.service = inner_upnp_service_;
    strncpy(sub.subscribeId, subscribeId_.toLocal8Bit().constData(), UPNP_UUID_LEN);
    sub.ctx = this;
    MiUpnpDevice *device = this->device();
    bool ok = device->runtime()->unsubscribe(&sub);
    if (!ok) {
        qCritical() << "Fail to unsubcribe event";
        return;
    }
    subscribeId_.clear();
    qDebug() << "success unsubcribe.";
    return;
}

MiUpnpAction* MiUpnpService::getAction(const QString& name)
{
    UpnpActionList *actionlist = UpnpService_GetActionList(inner_upnp_service_);
    if (!actionlist) {
        qCritical() << "invalid actionlist";
        return nullptr;
    }

    UpnpAction *action = UpnpActionList_GetAction(actionlist, name.toLocal8Bit().constData());
    if (!action) {
        qDebug() << "invalid action: " << name;
        return nullptr;
    }

    MiUpnpAction* miaction = new MiUpnpAction(this, action);
    return miaction;
}
