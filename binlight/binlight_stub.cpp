#include "binlight_stub.h"
#include <QDebug>

BinaryLightStub::BinaryLightStub(QObject* parent, MiUpnpDevice* upnp_device)
    :QObject(parent)
{
    upnp_device_ = upnp_device;
}

BinaryLightStub::~BinaryLightStub()
{
    //uninit();
    switch_power_service_ = NULL;
}

bool BinaryLightStub::isPowerOn() const
{
    qDebug() << "isPowerOn:" << is_power_on_;
    return is_power_on_;
}

void BinaryLightStub::onPowerStatusChanged(bool currentValue)
{
    qDebug() << "power status:" << (currentValue ? "ON" : "OFF");
    if (currentValue != is_power_on_) {
        is_power_on_ = currentValue;
        emit powerOnChanged(currentValue);
    }
}

bool BinaryLightStub::init()
{
    switch_power_service_ = upnp_device_->getService(ID_SwitchPower);
    if (!switch_power_service_) {
        qCritical() << "Service not found:" << ID_SwitchPower;
        return false;
    }


    qDebug() << "to subcribe event.";
    if (!switch_power_service_->subscribe()) {
        qDebug() << "Fail to subcribe event status:";
        return false;
    }
    QObject::connect(switch_power_service_, SIGNAL(serviceEvent(MiUpnpEvent*)),
                                                   this, SLOT(onSubscribeEvent(MiUpnpEvent*)));
    is_good_ = true;
    checkPowerStatus();
    return true;
}

void BinaryLightStub::uninit()
{
    qDebug() << "to unsubcribe event.";
    if (!switch_power_service_) {
        qCritical() << "Service not found:" << ID_SwitchPower;
        return;
    }
    switch_power_service_->unsubscribe();
}

void BinaryLightStub::checkPowerStatus()
{
    MiUpnpAction *action = switch_power_service_->getAction(ACTION_GetStatus);
    if (!action) {
        qWarning() << "invalid action " << ACTION_GetStatus;
        return;
    }
    bool ret = action->invoke();
    if (!ret) {
        qCritical() << "Invoke GetStatus fail";
        return;
    }
    bool retStatus = false;
    ret = action->getResult(ACTION_GetStatus_ARG_ResultStatus, retStatus);
    if (!ret) {
         qCritical() << "cannot get light status!";
        return;
    }

    if (retStatus != is_power_on_) {
        is_power_on_ = retStatus;
    }
    qDebug() << "Finish getStatus." << is_power_on_;
}

void BinaryLightStub::setPowerOn(const bool& powerStatus)
{
    int newStatus = (powerStatus) ? 1 : 0;
    qDebug() << "change light status to " << newStatus;
    if (!switch_power_service_) {
        return;
    }

    MiUpnpAction* actionSetTarget = switch_power_service_->getAction(ACTION_SetTarget);
    if (!actionSetTarget) {
        qCritical() << "invalid action";
        return;
    }

    int ret = actionSetTarget->setInputPara(ACTION_SetTarget_ARG_newTargetValue, powerStatus);

    if (!ret) {
        qCritical() << "cannot set input para";
        return;
    }
    if (!actionSetTarget->invoke()) {
        qCritical() << "fail to change light status";
        return;
    }

    onPowerStatusChanged(!is_power_on_);
    qDebug() << "Finish changeStatus.";
}

void BinaryLightStub::onSubscribeEvent(MiUpnpEvent* event)
{
    QString value = event->GetArgumentValue("Status");
    if (!value.isEmpty()) {
        qDebug() << "event-new light status:" << value;
        int intValue = value.toInt();
        onPowerStatusChanged(intValue != 0);
    }
}

QString BinaryLightStub::deviceId() const
{
    if (!upnp_device_) {
        return QString();
    }

    return upnp_device_->id();
}

QString BinaryLightStub::friendlyName() const
{
    if (!upnp_device_) {
        return QString();
    }

    return upnp_device_->friendlyName();
}
