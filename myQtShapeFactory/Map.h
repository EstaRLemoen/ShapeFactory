#pragma once
#ifndef MAP_H
#define MAP_H
#include <QWidget>

class Mineral;
class Block;
class QPixmap;
class QMouseEvent;
class QString;
class Equipment;

class Map :public QWidget
{
    Q_OBJECT

        
friend Block;
friend Equipment* find_equip(Map* map_, int loc_x, int loc_y);
signals:
    void ask_legal();
    void send_illegal(QString waring_);
    void off_eq_all();
    void on_eq_all();
    void strengthen_eq_all(int ef_addition);
    void send_cost(int amount);
    void ask_opr(QMouseEvent* event_);
private:
    bool is_all_eq_legal = true;
    //int last_press_x;
    //int last_press_y;
    int map_w = 30;
    int map_h = 15;
    Block* blocks[30][60] = { NULL };
public slots:
    void check_legal(bool is_legal);
    void map_on();
    void map_off();
    void work_status_chage(bool sta_);
    void build_eq(const QString* kind_, const QString* kind_2, int w_x, int h_y);
    void remove_eq(int w_x, int h_y);
    void strengthen_single_eq(int w_x, int h_y,int ef_addition);
    void cost_add(int amount);
    void loose_eq(Equipment* eq,int b_i, int b_j, int b_h, int b_w);
    void link_eq(Equipment* eq, int b_i, int b_j, int b_h, int b_w);
public:
    static const int B_sidelen = 40;
    bool is_working = false;
    //int cost_of_all = 0;
    explicit Map(QWidget* parent = nullptr);
    ~Map();
    void mousePressEvent(QMouseEvent* event);
    //void mouseReleaseEvent(QMouseEvent* event);
    void setResBlock(int b_i, int b_j, QPixmap* pic_, Mineral* resou_);
    void occupyBlock(int y, int x, bool is_occupy = true);
};

Equipment* find_equip(Map* map_, int loc_x, int loc_y);

#endif