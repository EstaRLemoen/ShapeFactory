#include"Reader.h"
#include"Equipment.h"
#include"Map.h"
#include"Mineral.h"
#include<QDebug>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QByteArray>

extern AccountReader* cur_account;

AccountReader::AccountReader(QWidget* parent):
	QLabel(parent),mineral_addition_value(0)
{
	this->hide();
	account_addr = new QString("./account/account_file/test.txt");
	account_file = NULL;
	Saved_foundation = 0;
	EFFI_of_miner = 1;
	EFFI_of_belt = 2;
	EFFI_of_cutter = 1;
	EFFI_of_dustbin = 4;
	EFFI_of_spiner = 1;
	EFFI_of_reciver = 50;
	VALUE_calcite = 8;
	VALUE_graphite = 8;

}

AccountReader::~AccountReader() {
	;
}

void AccountReader::load_it() {
	account_file = new QFile(this);
	account_file->setFileName(*account_addr);
	qDebug() << "open the file" << *account_addr << account_file->open(QIODevice::ReadOnly);
	QTextStream temp(account_file);
	QString s(" ");
	while (!temp.atEnd()) {
		temp >> s;
		if (s != "[begin]") continue;
		
		if(!temp.atEnd()){
			temp >> s;
			if (s == "efficient") {
				while (!temp.atEnd() && s != "[end]") {
					temp >> s;
					if (s == "Miner")temp >> EFFI_of_miner;
					else if (s == "Belt")temp >> EFFI_of_belt;
					else if (s == "Cutter")temp >> EFFI_of_cutter;
					else if (s == "Dustbin")temp >> EFFI_of_dustbin;
					else if (s == "Reciver")temp >> EFFI_of_reciver;
					else {
						qDebug() << "Error! Illegal content in File for AccountReader";
					}
				}
			}
			else if (s == "value") {
				while (!temp.atEnd() && s != "[end]") {
					temp >> s;
					if (s == "Calcite")temp >> VALUE_calcite;
					else if (s == "Graphite")temp >> VALUE_graphite;
					else {
						qDebug() << "Error! Illegal content in File for AccountReader";
					}
				}
			}
			else if (s == "saved_foundation") {
				temp >> Saved_foundation;
			}
		}
	}
	account_file->close();
	return;
}
void AccountReader::save_it(QString* new_name) {
	if (new_name == NULL){
		emit warning_account_covered("NOTICE: Old account_file will be covered.");
	}
	else {
		ensure_addr(*new_name, " ");
	}

	qDebug() << "begin write account.";

	if (account_file != NULL) {
		account_file->close();
		delete account_file;
		account_file = new QFile(this);
	}
	account_file->setFileName(*account_addr);
	qDebug() << "open the file" << *account_addr << account_file->open(QIODevice::WriteOnly);
	account_file->write("[begin]\n");
	account_file->write("efficient\n");
	account_file->write("Miner ");
	account_file->write(QByteArray::number(EFFI_of_miner));
	account_file->write("\n");
	account_file->write("Belt ");
	account_file->write(QByteArray::number(EFFI_of_belt));
	account_file->write("\n");
	account_file->write("Cutter ");
	account_file->write(QByteArray::number(EFFI_of_cutter));
	account_file->write("\n");
	account_file->write("Dustbin ");
	account_file->write(QByteArray::number(EFFI_of_dustbin));
	account_file->write("\n");
	account_file->write("Reciver ");
	account_file->write(QByteArray::number(EFFI_of_reciver));
	account_file->write("\n");
	account_file->write("[end]\n");

	account_file->write("[begin]\n");
	account_file->write("value\n");
	account_file->write("Calcite ");
	account_file->write(QByteArray::number(VALUE_calcite));
	account_file->write("\n");
	account_file->write("Graphite ");
	account_file->write(QByteArray::number(VALUE_graphite));
	account_file->write("\n");
	account_file->write("[end]\n");

	account_file->write("[begin]\n");
	account_file->write("saved_foundation\n");
	account_file->write(QByteArray::number(Saved_foundation));
	account_file->write("\n");
	account_file->write("[end]\n");

	account_file->close();
	qDebug() << "finish write account.";

	return;
}
void AccountReader::ensure_addr(const QString& UserID, const QString& Password) {
	//todo: password check
	// 
	QString temp("./account/account_file/");
	temp.push_back(UserID);
	temp.push_back(".txt");
	//todo: illegal addr check
	*account_addr = temp;
	return;
}
void AccountReader::increase_mineral_value(int val_) {
	mineral_addition_value += val_;
	emit renew_model_value();
	return;
}

MapReader::MapReader(QWidget* parent) :
	QLabel(parent)
{
	this->hide();
	map_addr = new QString("./map/test.txt");
	map_file = NULL;
}
MapReader::~MapReader() {
	;
}
void MapReader::load_it(Reciver*& reciver_, Map* map) {
	//todo User_saved_ check
	map_file = new QFile(this);
	map_file->setFileName(*map_addr);
	qDebug() << "open the file" << *map_addr << map_file->open(QIODevice::ReadOnly);

	int open_cost;
	QTextStream temp(map_file);
	QString s(" ");
	int y, x, times;
	QPixmap block_;
	Mineral* min = NULL;
	while (!temp.atEnd()) {
		temp >> s;
		if (s != "[begin]") continue;

		if (!temp.atEnd()) {
			temp >> s;
			if (s == "reciver") {
				if (!temp.atEnd() && s != "[end]") {
					int center_w;
					int center_h;
					temp >> y >> x >> center_w >> center_h;
					reciver_ = new Reciver(map, center_w, center_h);
					reciver_->set_loc(x, y);
					map->link_eq(reciver_, y, x, center_h, center_w);
					for (int i = 0; i < center_h; i++) {
						for (int j = 0; j < center_w; j++) {
							map->occupyBlock(y + i, x + j);
						}
					}
					reciver_->move(x * Map::B_sidelen, y * Map::B_sidelen);
					reciver_->show();
				}
			}
			else if (s == "open_cost") {
				if (!temp.atEnd()) {
					//todo:check open_cost
					temp >> open_cost;
				}
			}
			else if (s == "mineral") {
				if (!temp.atEnd()) {
					temp >> s;
					if (s == "Calcite") {
						temp >> times;
						min = NULL;
						block_.load(":/resouce/image/block_Calcite.jpg");
						min = new Calcite(map);
					}
					else if (s == "Graphite") {
						temp >> times;
						min = NULL;
						block_.load(":/resouce/image/block_Graphite.jpg");
						min = new Graphite(map);
					}
					else {
						continue;
					}
					connect(cur_account, &AccountReader::renew_model_value, min, &Mineral::renew_value);
					for (int i = 0; i < times; i++) {
						if (!temp.atEnd()) {
							temp >> y >> x;
							map->setResBlock(y, x, &block_, min);
						}
					}
				}
			}
		}
	}
	map_file->close();
	return;
}
void MapReader::ensure_addr(const QString& MapName) {
	QString temp("./map/");
	temp.push_back(MapName);
	temp.push_back(".txt");
	//todo: illegal addr check
	*map_addr = temp;
	return;
}