#include "MiUpnpRuntime.h"
#include <Upnp.h>
#include "miupnpdevice.h"
#include <QDebug>


MiUpnpRuntime::MiUpnpRuntime(QObject *parent) : QObject(parent)
{
    qDebug() << "to initliaze upnp.";
    UpnpInitializer_Initialize();
    upnp_runtime_ = UpnpRuntime_New();
    if (!upnp_runtime_) {
        qCritical()<< "fail to create upnp runtime";
        return;
    };
    qDebug() << "upnp initliazed";
}

MiUpnpRuntime::~MiUpnpRuntime()
{
    if (!upnp_runtime_) {
        return;
    }

    qDebug() << "to destroy upnp.";
    UpnpRuntime_Delete(upnp_runtime_);
    upnp_runtime_ = nullptr;
    UpnpInitializer_Destroy();
    qDebug() << "upnp destroyed";
}

void MiUpnpRuntime::addFilter(const QString& deviceType)
{
    device_filters_.push_back(deviceType);
}

void MiUpnpRuntime::removeFilter(const QString& deviceType)
{
    device_filters_.removeAll(deviceType);
}

const QList<QString>& MiUpnpRuntime::getFilters() const
{
    return device_filters_;
}

void MiUpnpRuntime::start()
{
    if (!upnp_runtime_) {
        qWarning("upnp runtime not inited!");
        return;
    }

    if (is_started_) {
        qWarning("upnp already started!");
        return;
    }

    TinyRet ret = UpnpRuntime_Start(upnp_runtime_);
    if (RET_SUCCEEDED(ret)) {
        qDebug("upnp runtime started success(code:%d)", ret);
        is_started_ = true;
    }
    else {
        qCritical("upnp runtime started fail(code:%d)", ret);
    }
}

void MiUpnpRuntime::stop()
{
    if (!upnp_runtime_) {
        qWarning("upnp runtime not inited!");
        return;
    }

    if (!is_started_) {
        qWarning("upnp already stopped!");
        return;
    }

    TinyRet ret = UpnpRuntime_Stop(upnp_runtime_);
    if (RET_SUCCEEDED(ret)) {
        qDebug("upnp runtime stop success(code:%d)", ret);
    }
    else {
        qCritical("upnp runtime stop fail(code:%d)", ret);
    }

    is_started_ = false;
}

bool __device_filter(_UpnpUri *uri, void *ctx)
{
    MiUpnpRuntime* thiz = (MiUpnpRuntime*)ctx;
    const QString inDevType = uri->string;
    bool found = thiz->getFilters().contains(inDevType);
    qDebug() << (found ? "new device: " : "ignore device:") << inDevType;
    return found;
}

void MiUpnpRuntime::__upnp_device_listener(_UpnpDeviceSummary *deviceSummary, bool alive, void *ctx)
{
    MiUpnpRuntime* thiz = (MiUpnpRuntime*)ctx;
    thiz->onDeviceEvent(deviceSummary, alive);
}

void MiUpnpRuntime::onDeviceEvent(_UpnpDeviceSummary* deviceSummary, bool alive)
{
    QString deviceId = deviceSummary->deviceId;
    if (alive) {
        UpnpDevice* innerDevice = UpnpDeviceFactory_Create(deviceSummary);
        if (!innerDevice) {
            qCritical()<<QString("fail to create device %1").arg(deviceId);
            return;
        }
        qDebug() << QString("new device %1").arg(deviceId);
        MiUpnpDevice* newUpnpDevice = new MiUpnpDevice(this, innerDevice);
        //notify
        emit deviceOnline(newUpnpDevice);
    }
    else {
        emit deviceOffline(deviceId);
    }
}

void MiUpnpRuntime::startDiscover()
{
    if (!upnp_runtime_) {
        qWarning("upnp runtime not inited!");
        return;
    }

    if (!is_started_) {
        qWarning("upnp already stopped!");
        return;
    }

    qDebug() << "start scan";
    TinyRet ret = UpnpRuntime_StartScan(upnp_runtime_, __upnp_device_listener, __device_filter, this);
    if (RET_SUCCEEDED(ret)) {
        qDebug("start scan success(code:%d)", ret);
    }
    else {
        qCritical("start scan  fail(code:%d)", ret);
    }
}

void MiUpnpRuntime::stopDiscover()
{
    if (!upnp_runtime_) {
        qWarning("upnp runtime not inited!");
        return;
    }

    if (!is_started_) {
        qWarning("upnp already stopped!");
        return;
    }

    qDebug() << "stop scan";
    TinyRet ret =  UpnpRuntime_StopScan(upnp_runtime_);
    if (RET_SUCCEEDED(ret)) {
        qDebug("stop scan success(code:%d)", ret);
    }
    else {
        qCritical("stop scan  fail(code:%d)", ret);
    }
}

bool MiUpnpRuntime::isStarted() const
{
   return is_started_;
}

bool MiUpnpRuntime::subscribe(_UpnpSubscription *subscription)
{
    UpnpError upnp_error;
    int ret = UpnpRuntime_Subscribe(upnp_runtime_, subscription, &upnp_error);
    if (RET_FAILED(ret)) {
        qDebug() << "Fail to subcribe event status:" << ret;
        return false;
    }
    return true;
}

bool MiUpnpRuntime::unsubscribe(_UpnpSubscription *subscription)
{
    UpnpError upnp_error;
    int ret = UpnpRuntime_Unsubscribe(upnp_runtime_, subscription, &upnp_error);
    if (RET_FAILED(ret)) {
        qDebug() << "Fail to unsubcribe event status:" << ret;
        return false;
    }
    return true;
}
