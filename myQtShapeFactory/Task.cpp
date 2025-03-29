#include"Task.h"
#include"Mineral.h"
#include"Equipment.h"
#include"Reader.h"

extern AccountReader* cur_account;

Task::Task(QWidget* parent) :QLabel(parent)
{
	;
}
Task::~Task() 
{
	;
}
bool Task::is_finished() {
	return process_full;
}

ProductTask::ProductTask(QWidget* parent, QString* name, Mineral* mod_, int aim_) 
	:Task(parent),model(mod_),aim_amount(aim_),cur_amount(0),val_addition(20)
{
	//todo:a view of the task
	t_name = "Product Task:";
	t_name.push_back(*name);
	t_name.push_back(" Process: ");
	this->hide();
}
ProductTask::~ProductTask() {
	;
}

void ProductTask::push_process(Mineral* p) {
	Mineral* pd = (Mineral*)p;
	if (model->is_the_same(p)) {
		cur_amount++;
		this->show_task();
		if (!this->process_full && cur_amount >= aim_amount) {
			process_full = true;
			cur_account->Saved_foundation += award;
			emit show_finish();
			emit mineral_value_increase(val_addition);
		}
		return;
	}
	return;
}
void ProductTask::show_task() {
	this->setText(t_name + QString::number(cur_amount) + " / " + QString::number(aim_amount));
	emit show_itself(this->text());
	return;
}