#pragma once
#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include <QLabel>
#include <QMouseEvent>

class Mineral;
class QString;
class Block;
class Map;
const int MAX_INPUT = 5;
const int MAX_OUTPUT = 5;

class Equipment :public QLabel
{
	Q_OBJECT


friend Block;
signals:
	void send_cost(int cost_);
	void send_check_result(bool result);
	void spin_loose(Equipment* eq,int b_i, int b_j, int b_h, int b_w);
	void spin_link(Equipment* eq,int b_i, int b_j, int b_h, int b_w);
protected:
	QTimer* work_timer;
	int efficient = 1;					
	int loc_x = 0;
	int loc_y = 0;
	int hei = 1;
	int wid = 1;
	short int angle = 0;				//画面12点方向为基准的顺时针旋转角度
	Mineral* content[MAX_INPUT] = { 0 };
	Mineral* product[MAX_OUTPUT] = { 0 };
	int num_input = 1;
	int num_output = 1;
public slots:
	virtual void check();
	virtual void set_on();
	virtual void set_off();
	void efficient_add(int ef_addition);
	void efficient_change(int new_ef);
public:
	explicit Equipment(QWidget* parent = nullptr);
	virtual ~Equipment();
	void set_loc(int x, int y);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual bool input(Mineral*& p, int No_);
	virtual Mineral*& output(int No_);
	virtual Equipment* find_from_();
	virtual Equipment* find_from_(int x,int y,int a);
	//virtual void show_it();				//传送带的show_it是不一样的，到时候要覆写
	virtual void do_work() = 0;
	virtual int can_send_to(int x, int y); //single direction
	virtual int can_get_from(int x, int y);
	virtual void clean_itself();

};

class Miner :public Equipment
{
	Q_OBJECT

signals:
	;
private:
	Mineral* reso = NULL;
public slots:
	;
public:
	explicit Miner(QWidget* parent = nullptr,Mineral* reso_ = nullptr);
	int can_get_from(int x, int y);
	void do_work();
};

class Belt :public Equipment
{
	Q_OBJECT

signals:
	;
private:
	;
public slots:
	;
public:
	explicit Belt(QWidget* parent = nullptr);
	int can_get_from(int x, int y);
	void do_work();
};

class Cutter :public Equipment
{
	Q_OBJECT

signals:
	;
private:
	;
public slots:
	void check();
public:
	explicit Cutter(QWidget* parent = nullptr);
	int can_send_to(int x, int y);
	//void mouseDoubleClickEvent(QMouseEvent* event);
	void do_work();
	//void clean_itself();
};

class Dustbin :public Equipment
{
	Q_OBJECT

signals:
	;
private:
	;
public slots:
	void check();
public:
	explicit Dustbin(QWidget* parent = nullptr);
	int can_send_to(int x, int y);
	int can_get_from(int x, int y);
	void do_work();
};

class Spiner :public Equipment
{
	Q_OBJECT

signals:
	;
private:
	bool to_right = true;
public slots:
	;
public:
	explicit Spiner(bool to_r = true, QWidget* parent = nullptr);
	void do_work();
};

class Reciver :public Equipment
{
	Q_OBJECT

friend void load_map(Map* map);
signals:
	void push_p_tasks(Mineral* p);
	void show_Mineral_income(long long int coin_);
	void renew_foundation(long long int money);
private:
	QTimer* trade_timer;
	long long int coin = 0;
public slots:
	void check();
	void deliver_products();
	void set_on();
	void set_off();
public:
	explicit Reciver(QWidget* parent = nullptr,int w_ = 2,int h_ = 2);
	void mouseDoubleClickEvent(QMouseEvent* event);
	bool input(Mineral*& p, int No_);
	int can_send_to(int x, int y);
	int can_get_from(int x, int y);
	void do_work();
	//void clean_itself();
};

Equipment* find_equip(Map* map_, int loc_x, int loc_y);
void load_map(Map* map);

#endif