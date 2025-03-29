#pragma once
#ifndef MINERAL_H
#define MINERAL_H
#include <QLabel>

class QTimer;

class Mineral :public QLabel
{
    Q_OBJECT

signals:
    ;
protected:
	//QTimer* move_timer = NULL;
	int kind_num = 0;
    int value = 8;
    short int disp_angle = 0;			//以初始开采物为0方向基准的顺时针旋转角度
    bool cutable = 0;
public slots:
    virtual void renew_value() = 0;
public:
	// □□ □（左半） □(右半)   □□        以及只有四个角(左上 5，右上 6，左下 7，右下 8）的情况
	// □□ □		   □               □□
	//   0     1      2             3     4
	int image_num = 0;					//记录矿物切割状态
    explicit Mineral(QWidget* parent = nullptr);
	void init_m(int da, int img_num, bool cut, int val);
	virtual ~Mineral();
	virtual int get_value();
	virtual Mineral* get_a_copy() = 0;
	virtual void reload_image(int angle) = 0;
	virtual bool is_the_same(const Mineral* other);
	Mineral* opr_cut();
    Mineral* to_be_cut();
	void spin_it(int ang_);
};



class Calcite :public Mineral {

	Q_OBJECT

signals:
	;
private:
	;
public slots:
	void renew_value();
public:
	explicit Calcite(QWidget* parent = nullptr);
	~Calcite();
	Mineral* get_a_copy();
	void reload_image(int angle);
	bool is_the_same(const Mineral* other);
};

class Graphite : public Mineral {

	Q_OBJECT

signals:
	;
private:
	;
public slots:
	void renew_value();
public:
	explicit Graphite(QWidget* parent = nullptr);
	~Graphite();
	Mineral* get_a_copy();
	void reload_image(int angle);
	bool is_the_same(const Mineral* other);
	//Mineral* to_be_cut();
};



#endif