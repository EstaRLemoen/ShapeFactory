#include"GameStatusBar.h"
#include"Task.h"
#include <QString>
#include <QFont>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QPushButton>

GameStatusBar::GameStatusBar(QWidget* parent) :QStatusBar(parent)
{
	qDebug() << "正在初始化状态栏";
	
	QFont form_;
	form_.setPointSize(15);
	this->setBaseSize(1200, 50);
	
	QPushButton* task_status = new QPushButton(this);
	task_status->resize(300, 50);
	task_status->setFont(form_);
	task_status->setText("No task to be finished.");

	p_task_menu = new QMenu;
	task_status->setMenu(p_task_menu);
	connect(task_status, &QPushButton::clicked, task_status, &QPushButton::showMenu);

	this->addWidget(task_status);
	connect(this, &GameStatusBar::showTaskStatus, task_status, &QPushButton::setText);

	QPushButton* mineral_income = new QPushButton(this);
	mineral_income->resize(500, 50);
	mineral_income->setFont(form_);
	mineral_income->setText("Stored Mineral Income: 0");
	this->addWidget(mineral_income);
	connect(this, &GameStatusBar::showMineralIncome, mineral_income, &QPushButton::setText);
}
GameStatusBar::~GameStatusBar() {
	;
}
void GameStatusBar::finish_one() {
	if (finished_task < task) {
		setTaskStatus(task, finished_task + 1);
	}
	return;
}
void GameStatusBar::setTaskStatus(int t,int ft) {
	if (ft < 0 || t < ft) {
		qDebug() << "Error number of task or Error number of finished task";
		return;
	}

	qDebug() << "Correct numbers.";
	task = t;
	finished_task = ft;
	if(t == 0){
		emit showTaskStatus("No task to be finished.");
	}
	else {
		QString s("0/0");
		s[0] = (char)(finished_task + 48);
		s[2] = (char)(task + 48);
		s = "Task status: " + s;
		emit showTaskStatus(s);
	}
	return;
}
void GameStatusBar::renew_mineral_income(long long int coin_) {
	QString temp = "Stored Mineral Income: ";
	temp.push_back(QString::number(coin_));
	emit showMineralIncome(temp);
	return;
}
void GameStatusBar::addProductTask(ProductTask* p_task_) {
	QAction* temp = new QAction(p_task_menu);
	p_task_menu->addAction(temp);
	connect(p_task_, &ProductTask::show_itself, temp, &QAction::setText);
	connect(p_task_, &ProductTask::show_finish, this, &GameStatusBar::finish_one);
	return;
}

	
