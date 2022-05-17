#include "monitorwindow.h"
#include "ui_monitorwindow.h"

MonitorWindow::MonitorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonitorWindow)
{
    ui->setupUi(this);
    ui->actionOpen->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    ui->actionCaptureEvents->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->actionAutoScroll->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown));
    ui->actionClearDisplay->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));

    //ui->actionFilter->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    //ui->actionHighlight->setIcon(this->style()->standardIcon(QStyle::SP_));
}

MonitorWindow::~MonitorWindow()
{
    //delete ui;
}
