#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"

class QMouseEvent;

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

signals:
    //void click_postion(QMouseEvent event_);
public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();
    //void mousePressEvent(QMouseEvent* event);

private:
    Ui::QtWidgetsApplication1Class ui;
};

