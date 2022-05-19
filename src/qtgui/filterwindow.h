#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class FilterWindow;
}


class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget *parent);
    ~FilterWindow();

private:
    QScopedPointer<Ui::FilterWindow> ui;
};

#endif // FILTERWINDOW_H
