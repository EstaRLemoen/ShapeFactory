#pragma once
#ifndef TASK_H
#define TASK_H
#include <QLabel>

class QString;
class Mineral;

class Task :public QLabel
{
    Q_OBJECT

signals:
	;
protected:
	int award = 100;
    bool process_full = false;
public slots:
	virtual void show_task() = 0;
public:
    explicit Task(QWidget* parent = nullptr);
    ~Task();
    bool is_finished();
	
};

class ProductTask :public Task 
{
	Q_OBJECT

signals:
	void mineral_value_increase(int val_);
	void show_itself(QString mess_);
	void show_finish();
private:
	int val_addition;
	Mineral* model = NULL;
	QString t_name;
	int aim_amount = 0;
	int cur_amount = 0;
public slots:
	void show_task();
	void push_process(Mineral* p);
public:
	ProductTask(QWidget* parent = nullptr, QString* m_name = NULL, Mineral* mod_ = NULL,int aim_ = 0);
	~ProductTask();
};

#endif