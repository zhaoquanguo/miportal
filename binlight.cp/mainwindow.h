#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include "miupnp/miupnp.h"
#include "binlight/binlight_stub.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnUpnpSwitch_clicked();
    void on_binLight_clicked();
    void on_btnDeviceRefresh_clicked();
    void on_btnNext_clicked();
    void on_btnPre_clicked();

    void OnDeviceOnline(MiUpnpDevice* device);
    void OnDeviceOffline(QString deviceId);


private:
    void showLight(int index);
private:
    Ui::MainWindow *ui;
    MiUpnpRuntime* miupnptime_ = nullptr;
    QHash<QString, BinaryLightStub*> binaryLights_;
    QList<QString> current_ids_;
    int curent_id_index_ = -1;
    BinaryLightStub* current_light_ = nullptr;
};

#endif // MAINWINDOW_H
