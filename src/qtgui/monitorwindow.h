#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class MonitorWindow;
}



namespace FilterWindowPrivate {
    class D;
    struct FilterWindowParams;
}


class MonitorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonitorWindow(QWidget *parent, FilterWindowPrivate::FilterWindowParams p);
    ~MonitorWindow();

private:
    QScopedPointer<Ui::MonitorWindow> ui;
    QScopedPointer<FilterWindowPrivate::D> d;


public Q_SLOTS:
    void on_timer();
};

#endif // MONITORWINDOW_H
