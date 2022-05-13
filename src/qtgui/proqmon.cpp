// based on procmon
// Licensed under the MIT License.

#include <iostream>
#include <version.h>

#include "configuration/procmon_configuration.h"
#include "display/screen.h"
#include "display/headless.h"
#include "logging/easylogging++.h"


#include "display/screen.h"
#include "display/event_formatter.h"
#include "display/kill_event_formatter.h"
#include "../logging/easylogging++.h"
#include "../common/telemetry.h"


INITIALIZE_EASYLOGGINGPP

#undef scroll
#undef border
#undef timeout


#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QDebug>

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QTimer>

#include <QTextStream>

#include <QToolBar>
#include <QToolButton>

#include <QTableWidget>
#include <QTimer>

class TimerView : public QWidget  {
    Q_OBJECT

public:
    ProcmonConfiguration * config;
    std::shared_ptr<IStorageEngine> storageEngine;

    QAction* openAct;
    QAction* saveAct;
    QAction* captureAct;
    QAction* autoscrollAct;
    QAction* clearAct;
    QAction* filterAct;
    QAction* highligntAct;
    QAction* includeAct;
    QAction* treeAct;
    QAction* propertiesAct;
    QAction* findAct;
    QAction* jumpAct;
    QAction* fileAct;
    QAction* networkAct;
    QAction* processAct;
    QAction* profilingAct;


    explicit TimerView(QWidget *parent, std::shared_ptr<ProcmonConfiguration> configPtr)
        : QWidget(parent)
    {

        config = configPtr.get();
        storageEngine = config->GetStorage();
        InitializeFormatters();

        QGridLayout* hl = new QGridLayout(this);

        int row=0;
        QToolBar* toolbar = new QToolBar(this);
        openAct = toolbar->addAction("open");
        saveAct = toolbar->addAction("save");
        toolbar->addSeparator();
        captureAct = toolbar->addAction("capture");
        captureAct->setChecked(true);

        autoscrollAct = toolbar->addAction("autoscroll");
        autoscrollAct->setCheckable(true);
        autoscrollAct->setChecked(true);

        clearAct = toolbar->addAction("clear");
        toolbar->addSeparator();
        filterAct = toolbar->addAction("filter");
        highligntAct = toolbar->addAction("highlignt");
        includeAct = toolbar->addAction("include");
        toolbar->addSeparator();
        treeAct = toolbar->addAction("process tree");
        toolbar->addSeparator();
        propertiesAct = toolbar->addAction("properties");
        findAct = toolbar->addAction("find");
        jumpAct = toolbar->addAction("jump");
        toolbar->addSeparator();
        fileAct = toolbar->addAction("file");
        fileAct->setCheckable(true);
        networkAct = toolbar->addAction("network");
        networkAct->setCheckable(true);
        processAct = toolbar->addAction("process");
        processAct->setCheckable(true);
        profilingAct = toolbar->addAction("profiling");
        profilingAct->setCheckable(true);


        connect(captureAct, &QAction::triggered, this, &TimerView::on_capture);
        connect(openAct, &QAction::triggered, this, &TimerView::on_timer);
        connect(clearAct, &QAction::triggered, this, &TimerView::on_clear);
        connect(autoscrollAct, &QAction::triggered, this, &TimerView::on_autoscroll);
        hl->addWidget(toolbar, row, 0);
        row++;


        tw = new QTableWidget(1, 7,this);
        tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Time of Day"));
        tw->setHorizontalHeaderItem(1, new QTableWidgetItem("Process Name"));
        tw->setHorizontalHeaderItem(2, new QTableWidgetItem("PID"));
        tw->setHorizontalHeaderItem(3, new QTableWidgetItem("Operation"));
        tw->setHorizontalHeaderItem(4, new QTableWidgetItem("Path"));
        tw->setHorizontalHeaderItem(5, new QTableWidgetItem("Result"));
        tw->setHorizontalHeaderItem(6, new QTableWidgetItem("Detail"));

        hl->addWidget(tw, row, 0);

        row++;

        QTimer* timer = new QTimer(this);
        timer->setInterval(1000);
        timer->setSingleShot(false);

        connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
        timer->start();

        setLayout(hl);
    }

    std::vector<EventFormatter*> formatters;

    void InitializeFormatters()
    {
        // reserve enough space assuming we will have one formatter per syscall
        formatters.reserve(335);

        // ALWAYS keep the default formatter in the first slot of vector.
        EventFormatter* default_formatter = new EventFormatter;
        default_formatter->Initialize("", config);
        formatters.push_back(default_formatter);

        // Formatter for the kill syscall
        KillEventFormatter* kill = new KillEventFormatter;
        kill->Initialize("kill", config);
        formatters.push_back(kill);
    }


    EventFormatter* GetFormatter(ITelemetry lineData)
    {
        EventFormatter* ret = NULL;

        // Check to see if a formatter exists for this event
        for (std::vector<EventFormatter*>::iterator it = formatters.begin() ; it != formatters.end(); ++it)
        {
            if((*it)->GetSyscall().compare(lineData.syscall)==0)
            {
                ret=(*it);
                break;
            }
        }

        return ret;
    }

public Q_SLOTS:

    void on_clear() {
        storageEngine->Clear();
    }

    void on_autoscroll() {
        qDebug() << "AS";
    }

    void on_capture() {
        config->GetTracer()->SetRunState(config->GetTracer()->GetRunState() == TRACER_RUNNING ? TRACER_SUSPENDED : TRACER_RUNNING);
        on_timer();
    }

    void on_timer() {

        if (autoscrollAct->isChecked()) {
            autoscrollAct->setText("AUTOSCROLL");
        } else {
            autoscrollAct->setText("autoscroll");
        }


        if (config->GetTracer()->GetRunState() == TRACER_SUSPENDED) {
            captureAct->setText("capture");
            captureAct->setChecked(false);
            return;
        } else {
            captureAct->setText("CAPTURE");
            captureAct->setChecked(true);
        }

        int pos = tw->rowCount();
        tw->insertRow(pos);
        tw->setItem(pos, 0, new QTableWidgetItem("123"));
        tw->setItem(pos, 1, new QTableWidgetItem("a.out"));
        tw->setItem(pos, 2, new QTableWidgetItem("456"));
        tw->setItem(pos, 3, new QTableWidgetItem("read"));
        tw->setItem(pos, 4, new QTableWidgetItem("/dev/stdout"));
        tw->setItem(pos, 5, new QTableWidgetItem("ok"));
        tw->setItem(pos, 6, new QTableWidgetItem("10 bytes"));
        //qDebug() << "qew";



        //std::vector<ITelemetry> newEventList = storageEngine->QueryByEventsinPage(config->pids, 1, 1000, ScreenConfiguration::sort::time, true, config->events);
        // std::vector<pid_t> pids, double start_time, double end_time, const std::vector<Event>& syscalls

        std::vector<pid_t> nopids;
        std::vector<ITelemetry> newEventList = storageEngine->QueryByPidsInTimespan(nopids, 0.0, 0.0, config->events);
        int i = 0;

        LOG(INFO) << "New Eventlist Size" << newEventList.size();

        //tw->clear();
        tw->setRowCount(0);

        qDebug() << "newEventList.size()" << newEventList.size();
        pos = 0;
        for(int i = std::max<int>(0, newEventList.size()-1000000); i < newEventList.size(); i++)
        {
            tw->insertRow(pos);
            auto& lineData = newEventList[pos];
            EventFormatter* format = GetFormatter(newEventList[pos]);
            if(format == NULL)
            {
                // If we dont find a formatter for that syscall, use the default formatter.
                // Our default formatter is always the first item in the vector with a syscall name of ""
                std::vector<EventFormatter*>::iterator it = formatters.begin();
                format = (*it);
            }

//            pidColumn->addLine(" " + format->GetPID(lineData));
//            processColumn->addLine(" " + format->GetProcess(lineData));
//            operationColumn->addLine(" " + format->GetOperation(lineData));
//            resultColumn->addLine(" " + format->GetResult(lineData));
//            durationColumn->addLine(" " + format->GetDuration(lineData));
//            detailColumn->addLine(" " + format->GetDetails(lineData));

            tw->setItem(pos, 0, new QTableWidgetItem(format->GetTimestamp(lineData).c_str()));
            tw->setItem(pos, 1, new QTableWidgetItem(newEventList[pos].processName.c_str()));
            tw->setItem(pos, 2, new QTableWidgetItem(QString("%1").arg(newEventList[pos].pid)));
            tw->setItem(pos, 3, new QTableWidgetItem(newEventList[pos].syscall.c_str()));
            tw->setItem(pos, 4, new QTableWidgetItem("/dev/stdout"));
            tw->setItem(pos, 5, new QTableWidgetItem(QString("%1").arg(newEventList[pos].result)));
            tw->setItem(pos, 6, new QTableWidgetItem(format->GetDetails(newEventList[pos]).c_str()));


            pos++;
            //newEventList[pos];
        }


        if (autoscrollAct->isChecked()) {
            tw->scrollToBottom();
        }

    }
private:
    QTableWidget* tw;
};

#include "proqmon.moc"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);


    auto configPtr = std::make_shared<ProcmonConfiguration>(argc, argv);

    qDebug() << "Tracing " << configPtr->events.size() << " system calls";
    el::Configurations defaultConf;
    defaultConf.setToDefault();



    qDebug()  << "Starting main UI thread";
    qDebug()  << "Tracing Events:" << configPtr->events.size();
    for (auto&i:configPtr->events) {
        qDebug() << i.Name().c_str();
    }

    TimerView* ttv = new TimerView(nullptr, configPtr);
    ttv->show();
    return a.exec();
}
