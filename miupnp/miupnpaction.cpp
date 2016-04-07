#include "miupnpaction.h"
#include <Upnp.h>
#include <QDebug>
#include <QMetaType>
#include "miupnpdevice.h"
#include "miupnpservice.h"
#include "miupnpruntime.h"

MiUpnpAction::MiUpnpAction(MiUpnpService *parent, _UpnpAction *inner_action) : QObject(parent)
{
    service_ = parent;
    inner_action_ = inner_action;
}

bool MiUpnpAction::setInputPara(const QString &para_name, const bool &para_value)
{
    if (!inner_action_) {
        qWarning() << "inner action is null";
        return false;
    }

    /**
    * Argument IN (1)
    */
    PropertyList *_in = UpnpAction_GetArgumentList(inner_action_);
    Property *_newTargetValue = PropertyList_GetProperty(_in, para_name.toLocal8Bit().constData());
    if (!_newTargetValue) {
        qWarning("Result invalid: %s NOT FOUND!", para_name.constData());
        return false;
    }

    _newTargetValue->value.object.value.boolValue = para_value;

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
    PropertyList *_out = UpnpAction_GetResultList(inner_action_);
    Property *_ResultStatus = PropertyList_GetProperty(_out, field_name.toLocal8Bit().constData());
    if (_ResultStatus == NULL) {
        qDebug() << "Result invalid: NOT FOUND!";
        return false;
    }

    value = _ResultStatus->value.object.value.boolValue;

    return true;
}
