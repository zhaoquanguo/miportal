#ifndef MIUPNPRUNTIME_H
#define MIUPNPRUNTIME_H

#include <QObject>
#include <QList>

struct _UpnpRuntime;
struct _UpnpDeviceSummary;
struct _UpnpUri;

class MiUpnpDevice;
class MiUpnpService;
class MiUpnpAction;

class MiUpnpRuntime : public QObject
{
    friend class MiUpnpDevice;
    friend class MiUpnpService;
    friend class MiUpnpAction;
    Q_OBJECT
public:
    explicit MiUpnpRuntime(QObject* parent = 0);

    ~MiUpnpRuntime();

    void addFilter(const QString& deviceType);
    void removeFilter(const QString& deviceType);
    const QList<QString>& getFilters() const;

    void start();
    void stop();
    bool isStarted() const;

    void startDiscover();
    void stopDiscover();

    bool invoke(MiUpnpAction* action);

signals:
    void deviceOnline(MiUpnpDevice* device);
    void deviceOffline(QString deviceId);

    void safeDeviceEvent(_UpnpDeviceSummary *deviceSummary, bool alive);
private slots:
    void onSafeDeviceEvent(_UpnpDeviceSummary *deviceSummary, bool alive);

private:
    static void __upnp_device_listener(_UpnpDeviceSummary *deviceSummary, bool alive, void *ctx);
    void onDeviceEvent(_UpnpDeviceSummary *deviceSummary, bool alive);

    _UpnpRuntime* innerRuntime() const;
private:
    bool is_started_ = false;
    _UpnpRuntime *upnp_runtime_ = nullptr;
    QList<QString> device_filters_;
};

#endif // MIUPNPRUNTIME_H
