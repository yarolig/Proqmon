#ifndef FORM_H
#define FORM_H

#include "ui_form.h"

class Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);

private:
    Ui::Form ui;
};

#endif // FORM_H
