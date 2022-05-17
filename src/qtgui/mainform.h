#ifndef MAINFORM_H
#define MAINFORM_H

#include "ui_mainform.h"

class MainForm : public QMainWindow, private Ui::MainForm
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
};

#endif // MAINFORM_H
