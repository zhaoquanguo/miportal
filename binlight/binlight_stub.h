#ifndef BINLIGHT_H
#define BINLIGHT_H

#include <QObject>
#include "miupnp/miupnp.h"

#define BINARYLIGHT_DEVICE_TYPE          "urn:schemas-upnp-org:device:BinaryLight:0.9"
#define ID_SwitchPower                   "urn:upnp-org:serviceId:SwitchPower"
#define SwitchPower_SERVICE_TYPE         "urn:schemas-upnp-org:service:SwitchPower:1"

#define ACTION_SetTarget "SetTarget"
#define ACTION_SetTarget_ARG_newTargetValue "newTargetValue"

static const char * ACTION_GetStatus = "GetStatus";
static const char * ACTION_GetStatus_ARG_ResultStatus = "ResultStatus";

class BinaryLightStub : public QObject
{
    Q_OBJECT
public:

    BinaryLightStub(QObject* parent, MiUpnpDevice* upnp_device);
    ~BinaryLightStub();

    bool init();
    void uninit();

    QString deviceId() const;
    QString friendlyName() const;
    bool isPowerOn() const;
    void setPowerOn(const bool& powerOn);

public slots:
    void onSubscribeEvent(MiUpnpEvent* event);
signals:
    void powerOnChanged(const bool& powerOn);
private:
    void checkPowerStatus();
    void onPowerStatusChanged(bool currentValue);
private:
    MiUpnpDevice* upnp_device_ = nullptr;
    bool is_power_on_ = false;
    MiUpnpService* switch_power_service_ = nullptr;
    bool is_good_ = false;
};
#endif // BINLIGHT_H
