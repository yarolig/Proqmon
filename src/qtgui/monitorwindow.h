#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class MonitorWindow;
}

class MonitorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonitorWindow(QWidget *parent = nullptr);
    ~MonitorWindow();

private:
    QScopedPointer<Ui::MonitorWindow> ui;
};

#endif // MONITORWINDOW_H
