#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include <QLabel>

class Equipment;
class Mineral;

class Block :public QLabel
{
    Q_OBJECT

signals:
    ;
private:
    Mineral* resou = NULL;
public slots:
    ;
public:
    bool special_occupy = false;
    Equipment* equip = NULL;
    int loc_x = 0;
    int loc_y = 0;
    explicit Block(QWidget* parent = nullptr);
    ~Block();
    void ensure_pos(int h_i, int w_j);
    void set_loc(int y, int x);
    void set_occupy(bool is_occupy);
    void strengthen_single_Equipment(int ef_addition);
    void build_Equipment(const QString* kind_, const QString* kind_2);
    void remove_Equipment();
    void ensure_Mineral(QPixmap* pic_, Mineral* resou_);
    Equipment* get_equip();
};


#endif