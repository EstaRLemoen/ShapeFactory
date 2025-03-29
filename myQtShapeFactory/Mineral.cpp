#include"Mineral.h"
#include"Reader.h"
#include <QDebug>
#include<QTransform>
#include<QTimer>

extern AccountReader* cur_account;

int M_NUM_calcite = 1;
int M_NUM_graphite = 2;

Mineral::Mineral(QWidget* parent) :
	QLabel(parent),disp_angle(0),cutable(0),image_num(0),value(8)
{
	;
}
void Mineral::init_m(int da, int img_num, bool cut, int val) {
	disp_angle = da;
	image_num = img_num;
	cutable = cut;
	value = val;
}
Mineral::~Mineral() {
	;
}
int Mineral::get_value() {
	if (image_num < 0) {
		qDebug() << "Error! Incorrect image_num.";
	}
	int ret = value;
	if (image_num == 0) return ret;
	else if (image_num <= 4)return ret / 2;
	else if (image_num <= 8)return ret / 8;
	else return -1;
}
bool Mineral::is_the_same(const Mineral* other) {
	if (kind_num != other->kind_num)return false;
	return true;
}
void Calcite::renew_value() {
	value = cur_account->VALUE_calcite + cur_account->mineral_addition_value;
	return;
}
void Graphite::renew_value() {
	value = cur_account->VALUE_graphite + cur_account->mineral_addition_value;
	return;
}

//to_be_cut
// □□ □（左半） □(右半)   □□        以及只有四个角(左上 5，右上 6，左下 7，右下 8）的情况
// □□ □		   □               □□
//   0     1      2             3     4
Mineral* Mineral::opr_cut() {
	if (!cutable) {
		return NULL;
	}

	Mineral* new_m = NULL;
	int img_num = 0;
	img_num = image_num;
	assert(img_num >= 0 && img_num < 9);

	int pic1 = -1;
	int pic2 = -1;
	if (img_num > 0 && img_num < 5) {
		if (img_num == 1) {
			if (disp_angle == 0 || disp_angle == 180)return NULL;
			else if (disp_angle == 90) {
				pic1 = 7;
				pic2 = 5;
			}
			else if (disp_angle == 270) {
				pic1 = 5;
				pic2 = 7;
			}
		}
		else if (img_num == 2) {
			if (disp_angle == 0 || disp_angle == 180)return NULL;
			else if (disp_angle == 90) {
				pic1 = 8;
				pic2 = 6;
			}
			else if (disp_angle == 270) {
				pic1 = 6;
				pic2 = 8;
			}
		}
		else if (img_num == 3) {
			if (disp_angle == 270 || disp_angle == 90)return NULL;
			else if (disp_angle == 0) {
				pic1 = 5;
				pic2 = 6;
			}
			else if (disp_angle == 180) {
				pic1 = 6;
				pic2 = 5;
			}
		}

		else {
			if (disp_angle == 270 || disp_angle == 90)return NULL;
			else if (disp_angle == 0) {
				pic1 = 7;
				pic2 = 8;
			}
			else if (disp_angle == 180) {
				pic1 = 8;
				pic2 = 7;
			}
		}
	}
	else if (img_num == 0) {
		if (disp_angle == 0) {
			pic1 = 1;
			pic2 = 2;
		}
		else if (disp_angle == 180) {
			pic1 = 2;
			pic2 = 1;
		}
		else if (disp_angle == 90) {
			pic1 = 4;
			pic2 = 3;
		}
		else if (disp_angle == 270) {
			pic1 = 3;
			pic2 = 4;
		}
		else return NULL;
	}

	if (pic1 == -1) return NULL;
	else {
		image_num = pic1;
		new_m = this->get_a_copy();
		new_m->init_m(disp_angle, pic2, true,value);
		return new_m;
	}
}
Mineral* Mineral::to_be_cut() {
	if (cutable) {
		return opr_cut();
	}
	else {
		qDebug() << "Not a cutable Mineral.";
		return NULL;
	}
}

/*
void Mineral::move(int time_ms, int x1, int y1) {
	int times = time_ms / 100;
	int x0 = this->x();
	int y0 = this->y();
	int x_disp = (x1 - x0) / times;
	int y_disp = (y1 - y0) / times;
	this->QLabel::move((x1 - x0) % times + x0, (y1 - y0) % times + y0);
}
*/

void Mineral::spin_it(int ang_) {
	disp_angle = (ang_ + disp_angle) % 360;
	return;
}

Calcite::Calcite(QWidget* parent) : Mineral(parent) {
	kind_num = M_NUM_calcite;
	value = cur_account->VALUE_calcite + cur_account->mineral_addition_value;
	cutable = 0;
}
Calcite::~Calcite() {
	;
}
Graphite::Graphite(QWidget* parent) : Mineral(parent) {
	kind_num = M_NUM_graphite;
	value = cur_account->VALUE_graphite + cur_account->mineral_addition_value;
	cutable = 1;
}
Graphite::~Graphite() {
	;
}

Mineral* Calcite::get_a_copy() {
	Mineral* temp = new Calcite((QWidget*)this->parent());
	temp->init_m(disp_angle, image_num, cutable, value);
	temp->setPixmap(this->pixmap());
	return temp;
}
Mineral* Graphite::get_a_copy() {
	Mineral* temp = new Graphite((QWidget*)this->parent());
	temp->init_m(disp_angle, image_num, cutable, value);
	temp->setPixmap(this->pixmap());
	return temp;
}
void Calcite::reload_image(int angle) {
	if (image_num != 0) {
		qDebug() << "Error! Illegal image_num for Calcite::reload_image.";
	}
	//qDebug() << "reload image 0 " << disp_angle << " degree of a Calcite.";
	QTransform trans_;
	trans_.rotate((angle + disp_angle) % 360);
	QPixmap pic;
	pic.load(":/resouce/image/Mineral/Calcite/0.png");
	pic = pic.transformed(trans_);
	pic = pic.scaled(30, 30);
	this->setPixmap(pic);
	return;
}
void Graphite::reload_image(int angle) {
	if (image_num < 0 || image_num > 8) {
		qDebug() << "Error! Illegal image_num for reload_image.";
		return;
	}
	//qDebug() << "reload image "<< image_num <<" " << disp_angle << " degree of a Graphite.";

	QString temp;
	temp.assign(":/resouce/image/Mineral/Graphite/");
	temp.append((char)(image_num + 48));
	temp.push_back(".png");

	QTransform trans_;
	trans_.rotate((angle + disp_angle) % 360);
	QPixmap pic;
	pic.load(temp);
	pic = pic.transformed(trans_);
	pic = pic.scaled(30, 30);
	this->setPixmap(pic);
	return;
}
bool Calcite::is_the_same(const Mineral* other) {
	if (!Mineral::is_the_same(other))return false;
	return true;
}
bool Graphite::is_the_same(const Mineral* other) {
	if (!Mineral::is_the_same(other))return false;
	if (image_num == 0 && other->image_num == 0) {
		return true;
	}
	else if (image_num >= 1 && image_num <= 4 && other->image_num >= 1 && other->image_num <= 4) {
		return true;
	}
	else if (image_num >= 5 && image_num <= 8 && other->image_num >= 5 && other->image_num <= 8) {
		return true;
	}
	return false;
}