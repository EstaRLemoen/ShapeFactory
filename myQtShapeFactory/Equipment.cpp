#include"Equipment.h"
#include"Mineral.h"
#include"Map.h"
#include"Reader.h"
#include <QDebug>
#include <QPixmap>
#include <QString>
#include <QTransform> 
#include <QTimer>
#include <QTime>

/*
int EFFI_of_miner = 1;
int EFFI_of_belt = 1;
int EFFI_of_cutter = 1;
int EFFI_of_dustbin = 1;
int EFFI_of_Reciver = 100;
*/
extern AccountReader* cur_account;

Reciver* recive_center = NULL;


Equipment::Equipment(QWidget* parent): QLabel(parent) {
	work_timer = new QTimer(this);
	;
	connect((Map*)parent, &Map::on_eq_all, this,&Equipment::set_on);
	connect((Map*)parent, &Map::off_eq_all, this, &Equipment::set_off);
	connect((Map*)parent, &Map::ask_legal, this, &Equipment::check);
	connect((Map*)parent, &Map::strengthen_eq_all, this, &Equipment::efficient_add);
	connect(this, &Equipment::send_check_result, (Map*)parent, &Map::check_legal);
	connect(this, &Equipment::send_cost, (Map*)parent, &Map::cost_add);

}
Equipment::~Equipment() {
	if (content != NULL)delete content[0];
	if (product != NULL)delete product[0];
	content[0] = NULL;
	product[0] = NULL;
}

void Equipment::set_loc(int x, int y) {
	this->loc_x = x;
	this->loc_y = y;
	return;
}
void Equipment::mouseDoubleClickEvent(QMouseEvent* event) {
	if (((Map*)this->parent())->is_working){
		return;
	}
	//qDebug() << "Equipment spinning.";
	QPixmap* temp = new QPixmap(this->pixmap());
	QTransform trans;
	trans.rotate(90);
	*temp = temp->transformed(trans);
	this->resize(height(), width());
	this->setPixmap(*temp);
	this->angle = (this->angle + 90) % 360;
	delete temp;
	
	if (hei != 1 || wid != 1) {
		int h_ = hei;
		int w_ = wid;
		if (angle == 90) {
			emit spin_loose(this,loc_y, loc_x, h_, w_);
			emit spin_link(this, loc_y, loc_x - (hei - 1), w_, h_);
			this->move(Map::B_sidelen * (loc_x - (hei - 1)), Map::B_sidelen * loc_y);
		}
		else if(angle == 180){
			emit spin_loose(this, loc_y, loc_x - (hei - 1), w_, h_);
			emit spin_link(this, loc_y - (hei - 1), loc_x - (wid - 1), h_, w_);
			this->move(Map::B_sidelen * (loc_x - (wid - 1)), Map::B_sidelen * (loc_y - (hei - 1)));
		}
		else if (angle == 270) {
			emit spin_loose(this, loc_y - (hei - 1), loc_x - (wid - 1), h_, w_);
			emit spin_link(this, loc_y - (wid - 1), loc_x, w_, h_);
			this->move(Map::B_sidelen * loc_x, Map::B_sidelen * (loc_y - (wid - 1)));
		}
		else if (angle == 0) {
			emit spin_loose(this, loc_y - (wid - 1), loc_x, w_, h_);
			emit spin_link(this, loc_y, loc_x, h_, w_);
			this->move(Map::B_sidelen * loc_x, Map::B_sidelen * loc_y);
		}
	}
	return;
}
void Reciver::mouseDoubleClickEvent(QMouseEvent* event) {
	return;
}

void Equipment::set_on() {
	//qDebug() << "Equipment set on.";
	work_timer->start(1000 / efficient);
	return;
}
void Equipment::set_off() {
	work_timer->stop();
	return;
}
void Reciver::set_on() {
	Equipment::set_on();
	trade_timer->start();
	return;
}
void Reciver::set_off() {
	Equipment::set_off();
	trade_timer->stop();
	return;
}
void Reciver::deliver_products() {
	long long int temp = coin;
	coin = 0;
	emit renew_foundation(temp);
	return;
}

void Equipment::efficient_add(int ef_addition) {
	efficient += ef_addition;
	if (efficient > 100) efficient = 100;
	else if (efficient < 1) efficient = 1;
	emit send_cost(100 * ef_addition);
	return;
}
void Equipment::efficient_change(int new_ef) {
	efficient = new_ef;
	return;
}

Miner::Miner(QWidget* parent,Mineral* reso_) :Equipment(parent) ,reso(reso_){
	efficient = cur_account->EFFI_of_miner;
	QPixmap* temp = new QPixmap;
	qDebug() << temp->load(":/resouce/image/Equipment/Miner.jpg");
	this->setPixmap(*temp);
	delete temp;
	//this->set_on();
	connect(this->work_timer, &QTimer::timeout, this, &Miner::do_work);
}
Belt::Belt(QWidget* parent) :Equipment(parent) {
	efficient = cur_account->EFFI_of_belt;
	QPixmap* temp = new QPixmap;
	temp->load(":/resouce/image/Equipment/Belt.jpg");
	this->setPixmap(*temp);
	delete temp;
	//this->set_on();
	connect(this->work_timer, &QTimer::timeout, this, &Belt::do_work);
}
Cutter::Cutter(QWidget* parent) :Equipment(parent) {
	wid = 2;
	num_output = 2;
	efficient = cur_account->EFFI_of_cutter;
	QPixmap* temp = new QPixmap;
	temp->load(":/resouce/image/Equipment/Cutter.jpg");
	this->setPixmap(*temp);
	delete temp;
	//this->set_on();
	connect(this->work_timer, &QTimer::timeout, this, &Cutter::do_work);
}
Dustbin::Dustbin(QWidget* parent) :Equipment(parent) {
	efficient = cur_account->EFFI_of_dustbin;
	QPixmap* temp = new QPixmap;
	temp->load(":/resouce/image/Equipment/Dustbin.jpg");
	this->setPixmap(*temp);
	delete temp;
	//this->set_on();
	connect(this->work_timer, &QTimer::timeout, this, &Dustbin::do_work);
}
Spiner::Spiner(bool to_r, QWidget* parent) :Equipment(parent) ,to_right(to_r)
{
	efficient = cur_account->EFFI_of_spiner;
	QPixmap* temp = new QPixmap;
	if (to_r)temp->load(":/resouce/image/Equipment/Spiner_r.jpg");
	else temp->load(":/resouce/image/Equipment/Spiner_l.jpg");
	this->setPixmap(*temp);
	delete temp;
	//this->set_on();
	connect(this->work_timer, &QTimer::timeout, this, &Spiner::do_work);
}
Reciver::Reciver(QWidget* parent,int w_,int h_) :Equipment(parent) {
	efficient = cur_account->EFFI_of_reciver;
	wid = w_;
	hei = h_;
	QPixmap* temp = new QPixmap;
	temp->load(":/resouce/image/Equipment/Reciver.jpg");
	*temp = temp->scaled(Map::B_sidelen * wid, Map::B_sidelen * hei,Qt::IgnoreAspectRatio);
	this->setPixmap(*temp);
	//this->setText("Settlement Center");
	delete temp;
	//this->set_on();
	connect(this->work_timer, &QTimer::timeout, this, &Reciver::do_work);

	//trade
	trade_timer = new QTimer(this);
	trade_timer->setInterval(10000);
	connect(this->trade_timer, &QTimer::timeout, this, &Reciver::deliver_products);
}

bool Equipment::input(Mineral*& p, int No_) {
	if (No_ <= num_input) {
		if (content[No_ - 1] != NULL) {
			delete content[No_ - 1];
			content[No_ - 1] = NULL;
		}
		content[No_ - 1] = p;
		p = NULL;
		if (content[No_ - 1] != NULL) {
			content[No_ - 1]->reload_image(angle);
			//content[No_ - 1]->show();
		}
		return true;
	}
	else return false;
}
bool Reciver::input(Mineral*& p, int No_) {
	if (p != NULL) {
		int val = 0;
		val = p->get_value();
		coin += val;
		emit push_p_tasks(p);
		//二阶段
		//assert(task_num <= 10);
		//for (int i = 0; i < task_num; i++) {
		//	if (product_list[i]->push_process(p)) break;
		//}
		delete p;
		p = NULL;
		return true;
	}
	else return false;
}
Mineral*& Equipment::output(int No_) {
	if (No_ <= num_output) {
		//qDebug() << "Output the No_ " << No_ << " product.";
	}
	return product[No_ - 1];
}

//view from the LOCATION (LOCATION!!!);
//can_send_to funcs
//can the equip send its the mineral to that position ?
//return the signal of product
int Equipment::can_send_to(int x, int y) {
	//it is basic one-to-one
	//x→，y↓
	// ↑
	// □
	if (x == loc_x) {
		if (y == loc_y - 1) {
			if (angle == 0)return 1;
		}
		else if (y == loc_y + 1) {
			if (angle == 180)return 1;
		}
		else return 0;
	}
	//  □->
	else if (y == loc_y) {
		if (x == loc_x + 1) {
			if (angle == 90)return 1;
		}
		else if (x == loc_x - 1) {
			if (angle == 270)return 1;
		}
		else return 0;
	}
	return 0;
}
int Cutter::can_send_to(int x, int y) {
	//x→，y↓
	// ↑ ↑
	// □    
	if (angle == 0 && y == loc_y - 1) {
		if (x == loc_x)return 1;
		else if (x == loc_x + 1)return 2;
		else return 0;
	}
	//  □→
	//    →
	else if (angle == 90 && x == loc_x + 1) {
		if (y == loc_y)return 1;
		else if (y == loc_y + 1)return 2;
		else return 0;
	}
	//    □
	// ↓ ↓ 
	else if (angle == 180 && y == loc_y + 1) {
		if (x == loc_x)return 1;
		else if (x == loc_x - 1)return 2;
		else return 0;
	}
	//  ←
	//  ← □
	else if (angle == 90 && x == loc_x + 1) {
		if (y == loc_y)return 1;
		else if (y == loc_y - 1)return 2;
		else return 0;
	}
	return 0;
}
int Dustbin::can_send_to(int x, int y) {
	return 0;
}
int Reciver::can_send_to(int x, int y) {
	return 0;
}
//get from 
//return the signal of accept point
int Equipment::can_get_from(int x, int y) {
	if (angle == 0 && loc_x == x && loc_y == y - 1) return 1;
	else if (angle == 90 && loc_x == x + 1 && loc_y == y) return 1;
	else if (angle == 180 && loc_x == x && loc_y == y + 1) return 1;
	else if (angle == 270 && loc_x == x - 1 && loc_y == y) return 1;
	else return 0;
}
int Miner::can_get_from(int x, int y) {
	return 0;
}
int Dustbin::can_get_from(int x, int y) {
	return 1;

	if (loc_x == x) {
		if (loc_y == y - 1 || loc_y == y + 1)return 1;
		else return 1;
	}
	else if (loc_y == y) {
		if (loc_x == x - 1 || loc_x == x + 1)return 1;
		else return 0;
	}
	else return 0;
}
int Belt::can_get_from(int x, int y) {
	//诡异的bug
	return 1;

	if (this->loc_x == x) {
		if (this->loc_y == y - 1 || this->loc_y == y + 1)return 1;
		else return 0;
	}
	else if (this->loc_y == y) {
		if (this->loc_x == x - 1 || this->loc_x == x + 1)return 1;
		else return 0;
	}
	else return 0;
}
int Reciver::can_get_from(int x, int y) {
	if (x == loc_x - 1 || x == loc_x + wid) {
		if (y >= loc_y || y < loc_y + hei) return 1;
	}
	if (y == loc_y - 1 || y == loc_y + hei) {
		if (x >= loc_x || x < loc_x + wid) return 1;
	}
	return 0;
}
Equipment* Equipment::find_from_() {
	//input
	Equipment* from_ = NULL;
	if (angle == 0){
		from_ = find_equip((Map*)this->parent(), loc_x, loc_y + 1);
	}
	else if (angle == 180){
		from_ = find_equip((Map*)this->parent(), loc_x, loc_y - 1);
	}
	else if (angle == 90){
		from_ = find_equip((Map*)this->parent(), loc_x - 1, loc_y);
	}
	else if (angle == 270){
		from_ = find_equip((Map*)this->parent(), loc_x + 1, loc_y);
	}
	else {
		qDebug() << "Error! Illegal angle for find_from_.";
	}
	return from_;
}
Equipment* Equipment::find_from_(int x, int y, int a) {
	//input
	Equipment* from_ = NULL;
	if (a == 0) {
		from_ = find_equip((Map*)this->parent(), x, y + 1);
	}
	else if (a == 180) {
		from_ = find_equip((Map*)this->parent(), x, y - 1);
	}
	else if (a == 90) {
		from_ = find_equip((Map*)this->parent(), x - 1, y);
	}
	else if (a == 270) {
		from_ = find_equip((Map*)this->parent(), x + 1, y);
	}
	else {
		qDebug() << "Error! Illegal angle for find_from_.";
	}
	return from_;
}

//do_work funcs
void Miner::do_work() {
	//work
	if (reso == NULL) {
		qDebug() << "Error!get resouce failed";
	}
	else {
		Mineral* new_ = NULL;
		new_ = reso->get_a_copy();
		this->input(new_, 1);
	}

	if (content[0] != NULL) {
		if (product[0] != NULL)delete product[0];
		product[0] = content[0];
		content[0] = NULL;
	}
	return;
}
void Belt::do_work() {
	//input
	Equipment* from_[4] = { NULL };
	for (int i = 0; i < 4; i++) {
		from_[i] = find_from_(loc_x, loc_y, (90 * i));
		if (from_[i] != NULL) {
			int j = from_[i]->can_send_to(loc_x, loc_y);
			if (j != 0) {
				this->input(from_[i]->output(j), 1);
				break;
			}
		}
	}

	//work
	if (content[0] != NULL) {
		content[0]->move(this->pos());
		content[0]->show();
		if (product[0] != NULL)delete product[0];
		product[0] = content[0];
		content[0] = NULL;
	}
	return;
}
void Cutter::do_work() {
	//input
	Equipment* from_ = NULL;
	from_ = find_from_();
	if (from_ != NULL) {
		int i = from_->can_send_to(loc_x, loc_y);
		if (i != 0) {
			this->input(from_->output(i), 1);
		}
	}

	//work
	if (content[0] != NULL) {
		content[0]->move(this->pos());
		//content[0]->hide();
		if (content[1] != NULL) {
			delete content[1];
			content[1] = NULL;
		}
		content[1] = content[0]->to_be_cut();
		if (content[1] != NULL)content[1]->move(this->pos());
		//content[1]->hide();

		if (product[0] != NULL) {
			delete product[0];
			product[0] = NULL;
		}
		if (product[1] != NULL) {
			delete product[1];
			product[1] = NULL;
		}
		product[0] = content[0];
		content[0] = NULL;
		product[1] = content[1];
		content[1] = NULL;
	}
	return;
}
void Dustbin::do_work() {
	//input and work
	Equipment* from_[4] = { NULL };
	for (int i = 0; i < 4; i++) {
		from_[i] = find_from_(loc_x, loc_y, (90 * i));
		if (from_[i] != NULL) {
			int j = from_[i]->can_send_to(loc_x, loc_y);
			if (j != 0) {
				this->input(from_[i]->output(j), 1);
				if (content[0] != NULL) {
					content[0]->move(this->pos());
					//content[0]->hide();
					delete content[0];
					content[0] = NULL;
				}
			}
		}
	}
	return;
}
void Spiner::do_work() {
	//input
	Equipment* from_ = NULL;
	from_ = find_from_();
	if (from_ != NULL) {
		int i = from_->can_send_to(loc_x, loc_y);
		if (i != 0) {
			this->input(from_->output(i), 1);
		}
	}

	//work
	if (content[0] != NULL) {
		content[0]->move(this->pos());

		if (to_right)content[0]->spin_it(90);
		else content[0]->spin_it(270);

		if (product[0] != NULL)delete product[0];
		product[0] = content[0];
		content[0] = NULL;
	}
	return;
}
void Reciver::do_work() {
	///*
	//input and work
	emit show_Mineral_income(coin);
	Equipment* from_ = NULL;
	int product_num = 0;
	for (int i = 0; i < wid; i++) {
		from_ = find_from_(loc_x + i, loc_y, 180);
		if (from_ != NULL) {
			product_num = from_->can_send_to(loc_x + i, loc_y);
			if (product_num != 0) {
				this->input(from_->output(product_num), 1);
			}
		}
		from_ = find_from_(loc_x + i, loc_y + hei - 1, 0);
		if (from_ != NULL) {
			product_num = from_->can_send_to(loc_x + i, loc_y + hei - 1);
			if (product_num != 0) {
				this->input(from_->output(product_num), 1);
			}
		}
	}
	for (int i = 0; i < hei; i++) {
		from_ = find_from_(loc_x, loc_y + i, 90);
		if (from_ != NULL) {
			product_num = from_->can_send_to(loc_x, loc_y + i);
			if (product_num != 0) {
				this->input(from_->output(product_num), 1);
			}
		}
		from_ = find_from_(loc_x + wid - 1, loc_y + i, 270);
		if (from_ != NULL) {
			product_num = from_->can_send_to(loc_x + wid - 1, loc_y + i);
			if (product_num != 0) {
				this->input(from_->output(product_num), 1);
			}
		}
	}
	//*/
	return;
}

//check funcs
void Equipment::check() {
	Equipment* to_ = NULL;
	if (angle == 0) {
		to_ = find_equip((Map*)this->parent(), loc_x, loc_y - 1);
	}
	else if (angle == 180) {
		to_ = find_equip((Map*)this->parent(), loc_x, loc_y + 1);
	}
	else if (angle == 90) {
		to_ = find_equip((Map*)this->parent(), loc_x + 1, loc_y);
	}
	else if (angle == 270) {
		to_ = find_equip((Map*)this->parent(), loc_x - 1, loc_y);
	}
	else {
		qDebug() << "Error! Illegal angle for check.";
	}

	if (to_ == NULL) {
		emit send_check_result(false);
		return;
	}

	if (to_->can_get_from(loc_x, loc_y) <= 0) {
		emit send_check_result(false);
	}
	return;
}
void Cutter::check() {
	Equipment::check();
	//另一个口
	Equipment* to_ = NULL;
	int o2_x = 0;
	int o2_y = 0;
	if (angle == 0) {
		o2_x = loc_x + 1;
		o2_y = loc_y;
		to_ = find_equip((Map*)this->parent(), loc_x + 1, loc_y - 1);
	}
	else if (angle == 180) {
		o2_x = loc_x - 1;
		o2_y = loc_y;
		to_ = find_equip((Map*)this->parent(), loc_x - 1, loc_y + 1);
	}
	else if (angle == 90) {
		o2_x = loc_x;
		o2_y = loc_y + 1;
		to_ = find_equip((Map*)this->parent(), loc_x + 1, loc_y + 1);
	}
	else if (angle == 270) {
		o2_x = loc_x;
		o2_y = loc_y - 1;
		to_ = find_equip((Map*)this->parent(), loc_x - 1, loc_y - 1);
	}
	else {
		qDebug() << "Error! Illegal angle for check.";
	}

	if (to_ == NULL) {
		qDebug() << "Error! check Cutter.";
		emit send_check_result(false);
		return;
	}

	if (to_->can_get_from(o2_x, o2_y) <= 0) {
		qDebug() << "Error! check Cutter.";
		emit send_check_result(false);
	}

	return;
}
void Dustbin::check() {
	return;
}
void Reciver::check() {
	return;
}

void Equipment::clean_itself() {
	if (angle == 90) {
		emit spin_loose(this, loc_y, loc_x - (hei - 1), wid, hei);
	}
	else if (angle == 180) {
		emit spin_loose(this, loc_y - (hei - 1), loc_x - (wid - 1), hei, wid);
	}
	else if (angle == 270) {
		emit spin_loose(this, loc_y - (wid - 1), loc_x, wid, hei);
	}
	else if (angle == 0) {
		emit spin_loose(this, loc_y, loc_x, hei, wid);
	}
	return;
}