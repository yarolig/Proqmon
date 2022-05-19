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





#include "filterwindow.h"
#include "ui_filterwindow.h"

#include <QAbstractTableModel>



FilterWindow::FilterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);
}

FilterWindow::~FilterWindow()
{
    //delete ui;
}
