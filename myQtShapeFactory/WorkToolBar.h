#pragma once
#ifndef WORKTOOLBAR_H
#define WORKTOOLBAR_H
#include<QTooLBar>

class QString;
class QMouseEvent;

class WorkToolBar :public QToolBar 
{
    Q_OBJECT

signals:
    void show_foundtion(QString text_);
    void call_map_strengthen(int w_x,int w_y,int ef_add);
    void call_map_build(const QString* kind, const QString* kind_2,int w_x,int h_y);
    void call_map_remove(int w_x, int h_y);
    void call_map_poweron();
    void call_map_poweroff();
    void power_map(bool give_power);
private:
    int ef_addition;
    long long int foundation;
    QString* opr_status = NULL;
    QString* opr_kind = NULL;
    QString* opr_kind_2 = NULL;
public slots:
    void add_foundation(long long int money);
    void sub_foundation(long long int money);
    void call_a_miner();
    void call_a_belt();
    void call_a_cutter();
    void call_a_spiner_l();
    void call_a_spiner_r();
    void call_a_dustbin();
    void call_d();
    void call_eq_strengthen(int ef_add);
    void call_eq_on();
    void call_eq_off();
    void do_change(QMouseEvent* event_);
    void save_and_exit();
public:
    explicit WorkToolBar(QWidget* parent = nullptr);
    ~WorkToolBar();
    void clear_preopr();
    void prepare_Opera(QString status_, QString kind_, QString kind_2_l_or_r);
    void operate_map(int x, int y);
};


#endif