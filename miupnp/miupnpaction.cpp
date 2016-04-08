#include "miupnpaction.h"
#include "Upnp.h"
#include <QDebug>
#include "miupnpdevice.h"
#include "miupnpservice.h"
#include "miupnpruntime.h"

MiUpnpAction::MiUpnpAction(MiUpnpService *parent, _UpnpAction *inner_action) : QObject(parent)
{
    service_ = parent;
    inner_action_ = inner_action;
}

QString MiUpnpAction::GetArgumentRelatedStateVariable(const QString& argumentName)
{
    const char* var = UpnpAction_GetArgumentRelatedStateVariable(
                inner_action_, argumentName.toLocal8Bit().constData());
    return QString(var);
}

bool MiUpnpAction::setInputPara(const QString &para_name, const bool &para_value)
{
    UpnpStateVariable * _newTargetValue = UpnpService_GetStateVariable(
                service_->inner_upnp_service_,
                UpnpAction_GetArgumentRelatedStateVariable(inner_action_, para_name.toLocal8Bit().constData()));
    if (!_newTargetValue) {
        qCritical() << "action para not found:" << para_name;
        return false;
    }

    _newTargetValue->value.internalValue.boolValue = para_value;

    return true;
}

bool MiUpnpAction::invoke()
{
    MiUpnpDevice* device = service_->device();
    if (!device) {
        return false;
    }
    MiUpnpRuntime* runtime = device->runtime();
    if (!device) {
        return false;
    }

    UpnpError error = {0};
    TinyRet ret = UpnpRuntime_Invoke(runtime->upnp_runtime_, inner_action_, &error);
    if (ret != TINY_RET_OK) {
        qDebug() << "Invoke action fail:" << ret;
        return false;
    }

    if (error.code != UPNP_SUCCESS) {
        qDebug() << "Invoke action fail: error" << error.code;
        return false;
    }

    return true;
}

bool MiUpnpAction::getResult(const QString& field_name, bool& value)
{
    /**
    * Argument OUT
    */
    UpnpStateVariable * _ResultStatus = UpnpService_GetStateVariable(
                service_->inner_upnp_service_,
                UpnpAction_GetArgumentRelatedStateVariable(inner_action_, field_name.toLocal8Bit().constData()));
    if (!_ResultStatus) {
        qCritical() << "action para not found:" << field_name;
        return false;
    }

    value = _ResultStatus->value.internalValue.boolValue;

    return true;
}
