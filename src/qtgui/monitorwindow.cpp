#include <iostream>
#include <version.h>

#include "configuration/procmon_configuration.h"
#include "display/screen.h"
#include "display/headless.h"


#include "display/screen.h"
#include "display/event_formatter.h"
#include "display/kill_event_formatter.h"
#include "../common/telemetry.h"



#undef scroll
#undef border
#undef timeout





#include <QAbstractTableModel>
#include <QDebug>
#include <QTimer>

#include "monitorwindow.h"
#include "ui_monitorwindow.h"


namespace FilterWindowPrivate {
    struct FilterWindowParams {
        ProcmonConfiguration * config;
        IStorageEngine* storageEngine;
    };
}


namespace FilterWindowPrivate {

class MyModel : public QAbstractTableModel {

    ProcmonConfiguration * config;
    IStorageEngine* storageEngine;


    // QAbstractItemModel interface
public:
    MyModel(FilterWindowPrivate::FilterWindowParams p)
        : config(p.config)
        , storageEngine(p.storageEngine)
    {

    }

  //  QModelIndex index(int row, int column, const QModelIndex &parent) const override {
  //      return createIndex(row, column);
  //  }
  //  QModelIndex parent(const QModelIndex &child) const override{
  //      return QModelIndex();
  //  }
    void update_size() {
        qDebug() << "size:" << storageEngine->Size();
        int new_size = storageEngine->Size();
        if (new_size != lastSize) {
            beginInsertRows(QModelIndex(), lastSize, new_size-1);
            endInsertRows();
            lastSize = new_size;
        }
    }
    int rowCount(const QModelIndex &parent) const override{
        return lastSize;
    }
    int columnCount(const QModelIndex &parent) const override{
        return 7;
    }

    int page_size = 1000;
    mutable int lastSize = -1;
    mutable int lastRow = -1;
    mutable int lastPage = -1;
    mutable ITelemetry lastLineData;
    mutable std::vector<ITelemetry> lastPageData;
    QVariant data(const QModelIndex &index, int role) const override{
        if (role == Qt::DisplayRole ||  role == Qt::EditRole){
            // pass
        } else {
            return QVariant();
        }
        int row = index.row();
        int page = row / page_size;

        if (lastPage == page && lastPageData.size() > row % page_size) {
            // Nothing
           // qDebug() << "data cached" << index.row() << index.column();
        } else {
            std::vector<ITelemetry> eventList = storageEngine->QueryByEventsinPage(
                        config->pids,
                        page,
                        page_size,
                        ScreenConfiguration::time,
                        true
                        );

            qDebug() << "data" << index.row() << index.column() << "last:" << lastPage << page;
            //if (eventList.size() != 1) {
            //    return QVariant::fromValue<QString>("???");
            //}
            lastPage = page;
            lastPageData = (eventList);
        }

        if (row % page_size >= lastPageData.size()) {
            return QVariant::fromValue<QString>("...");
        }

        ITelemetry& lineData = lastPageData[row % page_size]; // !!!
        // ITelemetry& lineData = lastLineData;

/*
        tw->setItem(pos, 0, new QTableWidgetItem(format->GetTimestamp(lineData).c_str()));
        tw->setItem(pos, 1, new QTableWidgetItem(newEventList[pos].processName.c_str()));
        tw->setItem(pos, 2, new QTableWidgetItem(QString("%1").arg(newEventList[pos].pid)));
        tw->setItem(pos, 3, new QTableWidgetItem(newEventList[pos].syscall.c_str()));
        tw->setItem(pos, 4, new QTableWidgetItem("/dev/stdout"));
        tw->setItem(pos, 5, new QTableWidgetItem(QString("%1").arg(newEventList[pos].result)));
*/

        switch(index.column()) {
        case 0: return QVariant::fromValue<QString>(QString("%1").arg(lineData.timestamp));
        case 1: return QVariant::fromValue<QString>(lineData.processName.c_str());
        case 2: return QVariant::fromValue<QString>(QString("%1").arg(lineData.pid));
        case 3: return QVariant::fromValue<QString>(lineData.syscall.c_str());
        case 4: return QVariant::fromValue<QString>("0");
        case 5: return QVariant::fromValue<QString>("0");
        case 6: return QVariant::fromValue<QString>("0");
        case 7: return QVariant::fromValue<QString>("0");
        case 8: return QVariant::fromValue<QString>("0");
        case 9: return QVariant::fromValue<QString>("0");
        default: return QVariant::fromValue<QString>("0");
        }
    }
};

class D{
public:
    D(FilterWindowPrivate::FilterWindowParams p)
        : model(p)
    {

    }
    MyModel model;
};

}
MonitorWindow::MonitorWindow(QWidget *parent, FilterWindowPrivate::FilterWindowParams p) :
    QMainWindow(parent),
    ui(new Ui::MonitorWindow),
    d(new FilterWindowPrivate::D(p))
{
    ui->setupUi(this);
    ui->actionOpen->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    ui->actionCaptureEvents->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->actionAutoScroll->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown));
    ui->actionClearDisplay->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));

    ui->tableView->setModel(&d->model);


    QTimer* timer = new QTimer(this);
    timer->setInterval(10);
    timer->setSingleShot(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
    timer->start();
    //ui->actionFilter->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    //ui->actionHighlight->setIcon(this->style()->standardIcon(QStyle::SP_));
}

MonitorWindow::~MonitorWindow()
{
    //delete ui;
}

void MonitorWindow::on_timer() {
    d->model.update_size();

    if (ui->actionAutoScroll->isChecked() && ui->actionCaptureEvents->isChecked()) {
        ui->tableView->scrollToBottom();
    }
}
