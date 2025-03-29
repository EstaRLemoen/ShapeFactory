#pragma once
#ifndef GAMESTATUSBAR_H
#define GAMESTATUSBAR_H
#include <QStatusBar>

class String;
class ProductTask;
class QLabel;
class QMenu;

class GameStatusBar :public QStatusBar
{
    Q_OBJECT

signals:
    void showTaskStatus(const QString& string);
    void showMineralIncome(const QString& string);
    //void show_p_task(QString* p_task_text);
private:
    QMenu* p_task_menu = NULL;
    int finished_task = 0;
    int task = 0;
public slots:
    void finish_one();
    void renew_mineral_income(long long int coin_);
public:
    explicit GameStatusBar(QWidget* parent = nullptr);
    ~GameStatusBar();
    void setTaskStatus(int t, int ft);
    void addProductTask(ProductTask* p_task_);
};


#endif