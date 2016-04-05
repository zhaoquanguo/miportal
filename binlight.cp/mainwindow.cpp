#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showLight(-1);
    ui->btnUpnpSwitch->setText("Start");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnUpnpSwitch_clicked()
{
    if (!miupnptime_) {
        miupnptime_ = new MiUpnpRuntime(this);
    }
    if (miupnptime_->isStarted()) {
        QObject::disconnect(miupnptime_, SIGNAL(deviceOnline(MiUpnpDevice*)),
                         this, SLOT(deviceOnline(MiUpnpDevice*)));

        QObject::disconnect(miupnptime_, SIGNAL(deviceOffline(QString)),
                                 this, SLOT(OnDeviceOffline(QString)));
        miupnptime_->removeFilter(BINARYLIGHT_DEVICE_TYPE);
        miupnptime_->stop();
        ui->btnUpnpSwitch->setText("Start");
    }
    else{
        QObject::connect(miupnptime_, SIGNAL(deviceOnline(MiUpnpDevice*)),
                         this, SLOT(deviceOnline(MiUpnpDevice*)));

        QObject::connect(miupnptime_, SIGNAL(deviceOffline(QString)),
                                 this, SLOT(OnDeviceOffline(QString)));
        miupnptime_->addFilter(BINARYLIGHT_DEVICE_TYPE);
        miupnptime_->start();
        ui->btnUpnpSwitch->setText("Stop");
    }
}

void MainWindow::on_binLight_clicked()
{
    if (ui->binLight->isChecked()) {
        ui->binLight->setStyleSheet("QRadioButton{border-image: url(:/background/images/light_on.jpg);}");
        if (current_light_) {
            current_light_->setPowerOn(true);
        }
    }
    else {
        ui->binLight->setStyleSheet("QRadioButton{border-image: url(:/background/images/light_off.jpg);}");
        if (current_light_) {
            current_light_->setPowerOn(false);
        }
    }
}

void MainWindow::OnDeviceOnline(MiUpnpDevice* device)
{
    BinaryLightStub* binaryLightStub = new BinaryLightStub(this, device);
    QString id = binaryLightStub->deviceId();
    qDebug() << "a binary light online :" <<id;
    binaryLights_.insert(id, binaryLightStub);
    showLight(-1);
}

void MainWindow::OnDeviceOffline(QString deviceId)
{
    auto iter = binaryLights_.find(deviceId);
    if (binaryLights_.end() != iter) {
         qDebug() << "the binary light offline :" << deviceId;
         (*iter)->deleteLater();
         binaryLights_.erase(iter);
    }

    current_ids_.removeAll(deviceId);
    showLight(-1);
}

void MainWindow::on_btnDeviceRefresh_clicked()
{
    if (!miupnptime_) {
        return;
    }
    miupnptime_->startDiscover();
}

void MainWindow::on_btnNext_clicked()
{
    showLight(curent_id_index_ + 1);
}

void MainWindow::showLight(int index)
{
    int max_index = binaryLights_.size();
    if (max_index == 0) {
        curent_id_index_ = -1;
        ui->btnNext->setVisible(false);
        ui->btnPre->setVisible(false);
        ui->labelLightName->setText("[None, Just Demo Effects.]");
        current_light_ = nullptr;
        return;
    }

    if (index <= -1 || index >= current_ids_.size() ) {
        index = current_ids_.size() - 1;
    }

    curent_id_index_ = index;

    bool is_first_one = (curent_id_index_ == 0);
    bool is_last_one = (curent_id_index_ == (current_ids_.size() - 1));
    ui->btnNext->setVisible(!is_last_one);
    ui->btnPre->setVisible(!is_first_one);

    QString devId = current_ids_.at(curent_id_index_);
    BinaryLightStub* bl = binaryLights_[devId];
    if (current_light_ != bl) {
        QObject::disconnect(current_light_, SIGNAL(powerOnChanged(bool)), ui->btnUpnpSwitch,
                         SLOT(setChecked(bool)));
        current_light_ = bl;
        QObject::connect(current_light_, SIGNAL(powerOnChanged(bool)), ui->btnUpnpSwitch,
                         SLOT(setChecked(bool)));
    }

    QString dname(bl->friendlyName());
    if (current_ids_.size() > 1) {
        dname += QString::asprintf("[%d]", curent_id_index_ + 1);
    }
    ui->labelLightName->setText(dname);
    ui->btnUpnpSwitch->setChecked(current_light_->isPowerOn());
}

void MainWindow::on_btnPre_clicked()
{
    showLight(curent_id_index_ - 1);
}
