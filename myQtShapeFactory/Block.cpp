#include"Block.h"
#include"Equipment.h"
#include"Map.h"
#include <QPixmap>
#include <QString>


Block::Block(QWidget* parent) :QLabel(parent), loc_x(0), loc_y(0),special_occupy(false) {
	;
}

Block::~Block() {
	;
}

void Block::ensure_pos(int h_i, int w_j) {
	move(w_j, h_i);
	return;
}
void Block::set_loc(int y, int x) {
	loc_x = x;
	loc_y = y;
	return;
}
void Block::set_occupy(bool is_occupy) {
	special_occupy = is_occupy;
	return;
}

Equipment* Block::get_equip() {
	return equip;
}
void Block::strengthen_single_Equipment(int ef_addition) {
	if (this->special_occupy) {
		return;
	}

	if (equip != NULL) {
		Equipment* temp = NULL;
		if (equip->loc_x == loc_x && equip->loc_y == loc_y) {
			equip->efficient_add(ef_addition);
		}
		else {
			qDebug() << "Not register Block of it's Equip.";
		}
	}
	else {
		qDebug() << "Empty Block.";
	}
	return;
}
void Block::build_Equipment(const QString* kind_, const QString* kind_2) {
	if (*kind_ == "Miner" || *kind_ == "Belt" || *kind_ == "Dustbin" || *kind_ == "Cutter" || *kind_ == "Spiner") {
		if (equip != NULL) {
			this->remove_Equipment();
			if (equip != NULL) {
				qDebug() << "Error! the Block refuse a new Equipment .";
				return;
			}
		}

		Map* p = (Map*)parent();
		if (*kind_ == "Miner") {
			equip = new Miner(this->parentWidget(),resou);
			qDebug() << "Built a Miner.";
		}
		else if (*kind_ == "Belt") {
			equip = new Belt(this->parentWidget());
			qDebug() << "Built a Belt.";
		}
		else if (*kind_ == "Dustbin") {
			equip = new Dustbin(this->parentWidget());
			qDebug() << "Built a Dustbin.";
		}
		else if (*kind_ == "Cutter") {
			equip = new Cutter(this->parentWidget());
			qDebug() << "Built a Cutter.";
			p->blocks[loc_y][loc_x + 1]->equip = equip;
		}
		else if (*kind_ == "Spiner") {
			if (*kind_2 == "Left") {
				equip = new Spiner(false,this->parentWidget());
				qDebug() << "Built a Spiner toward Left.";
			}
			else {
				equip = new Spiner(true,this->parentWidget());
				qDebug() << "Built a Spiner toward Right.";
			}
		}
		equip->move(this->pos());
		equip->set_loc(loc_x, loc_y);
		equip->show();
		connect(equip, &Equipment::spin_link, p, &Map::link_eq);
		connect(equip, &Equipment::spin_loose, p, &Map::loose_eq);
		connect(this, &Block::strengthen_single_Equipment, equip, &Equipment::efficient_add);
	}
	else {
		qDebug() << "Error! Equipment kind fault.";
	}
	return;
}
void Block::remove_Equipment() {
	if (this->special_occupy) {
		return;
	}

	if (equip != NULL) {
		Equipment* temp = NULL;
		if (equip->loc_x == loc_x && equip->loc_y == loc_y) {
			temp = equip;
			equip = NULL;
			temp->clean_itself();
			delete temp;
			//this->show();
			qDebug() << "Removed old building.";
		}
		else {
			qDebug() << "Not register Block of it's Equip.";
		}
	}
	else {
		qDebug() << "Empty Block.";
	}
	return;
}

void Block::ensure_Mineral(QPixmap* pic_, Mineral* resou_) {
	this->setPixmap(*pic_);
	resou = resou_;
	return;
}

