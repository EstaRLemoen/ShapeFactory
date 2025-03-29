#include"Map.h"
#include "Block.h"
#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>

Map::Map(QWidget* parent):QWidget(parent),is_all_eq_legal(true){
	if (map_h > 30) {
		qDebug() << "Error Map Height!";
	}
	if (map_w > 60) {
		qDebug() << "Error Map Width!";
	}
	
	QPixmap* block_pic = new QPixmap;
	block_pic->load(":/resouce/image/block.jpg");
	for (int i = 0; i < map_h; i++) {
		for (int j = 0; j < map_w; j++) {
			blocks[i][j] = new Block(this);
			//connect(this, &delete_it_all, blocks[i][j], &deleteLater);
			blocks[i][j]->ensure_pos(i*B_sidelen, j*B_sidelen);
			blocks[i][j]->setPixmap(*block_pic);
			blocks[i][j]->set_loc(i, j);
		}
	}
	delete block_pic;
}

Map::~Map() {
	// delete_it_all();
	;
}

void Map::mousePressEvent(QMouseEvent* event) {
	QWidget::mousePressEvent(event);
	if (is_working) return;

	emit ask_opr(event);

	return;
}
void Map::setResBlock(int b_i, int b_j, QPixmap* pic_, Mineral* resou_) {
	blocks[b_i][b_j]->ensure_Mineral(pic_,resou_);
	return;
}
void Map::occupyBlock(int y, int x, bool is_occupy) {
	blocks[y][x]->set_occupy(is_occupy);
	return;
}

void Map::cost_add(int amount) {
	emit send_cost(amount);
	return;
}
void Map::strengthen_single_eq(int w_x, int h_y,int ef_addition) {
	int b_i = 0, b_j = 0;
	b_i = h_y / B_sidelen;
	b_j = w_x / B_sidelen;
	blocks[b_i][b_j]->strengthen_single_Equipment(ef_addition);
	return;
}
void Map::build_eq(const QString* kind_, const QString* kind_2, int w_x,int h_y) {
	int b_i = 0, b_j = 0;
	b_i = h_y / B_sidelen;
	b_j = w_x / B_sidelen;

	//QPixmap* block_pic = new QPixmap;
	//block_pic->load(":/resouce/image/block_built.jpg");
	//blocks[b_i][b_j]->setPixmap(*block_pic);

	blocks[b_i][b_j]->build_Equipment(kind_,kind_2);
	return;
}
void Map::remove_eq(int w_x, int h_y) {
	int b_i = 0, b_j = 0;
	b_i = h_y / B_sidelen;
	b_j = w_x / B_sidelen;

	//QPixmap* block_pic = new QPixmap;
	//block_pic->load(":/resouce/image/block.jpg");
	//blocks[b_i][b_j]->setPixmap(*block_pic);

	blocks[b_i][b_j]->remove_Equipment();
	return;
}

void Map::loose_eq(Equipment* eq,int b_i, int b_j, int b_h, int b_w) {
	for (int i = 0; i < b_h; i++) {
		for (int j = 0; j < b_w; j++) {
			if (blocks[b_i + i][b_j + j]->equip == eq) blocks[b_i + i][b_j + j]->equip = NULL;
		}
	}
}
void Map::link_eq(Equipment* eq, int b_i, int b_j, int b_h, int b_w) {
	for (int i = 0; i < b_h; i++) {
		for (int j = 0; j < b_w; j++) {
			if (blocks[b_i + i][b_j + j]->equip == NULL) blocks[b_i + i][b_j + j]->equip = eq;
			else if (blocks[b_i + i][b_j + j]->equip == eq);
			else qDebug() << "Error! No enough space to spin.";
		}
	}
}

void Map::check_legal(bool is_legal) {
	if (!is_legal) {
		is_all_eq_legal = false;
	}
	return;
}
void Map::map_on() {
	is_all_eq_legal = true;
	emit ask_legal();
	if (is_all_eq_legal) {
		emit on_eq_all();
		work_status_chage(true);
	}
	else {
		qDebug() << "Illegal connection of Equipment.";
		emit send_illegal("Error Connection of Equipment.");
	}
	return;
}
void Map::map_off() {
	emit off_eq_all();
	work_status_chage(false);
	return;
}
void Map::work_status_chage(bool sta_) {
	is_working = sta_;
	return;
}

//find_from_ func
//base on location，to find "last" equipmant, which might give it resouce
//a static function not a function of a class
//0 angle:
//   ↑出
//   □
//   ↑进
Equipment* find_equip(Map* map_,int loc_x, int loc_y) {
	Equipment* from_ = NULL;
	from_ = map_->blocks[loc_y][loc_x]->get_equip();
	return from_;
}