#pragma once
#ifndef READER_H
#define READER_H
#include <QLabel>
#include <vector>

class QString;
class QFile;
class Equipment;
class Mineral;
class Reciver;
class Map;

class AccountReader : public QLabel 
{
	Q_OBJECT

signals:
	void renew_model_value();
	void warning_account_covered(QString content_);
private:
	QString* account_addr;
	QFile* account_file;
public slots:
	void increase_mineral_value(int val_);
	void ensure_addr(const QString& UserID, const QString& Password);
public:
	int Saved_foundation;
	int EFFI_of_miner;
	int EFFI_of_belt;
	int EFFI_of_cutter;
	int EFFI_of_dustbin;
	int EFFI_of_spiner;
	int EFFI_of_reciver;
	int VALUE_calcite;
	int VALUE_graphite;

	int mineral_addition_value;

	explicit AccountReader(QWidget* parent = NULL);
	~AccountReader();
	void load_it();
	void save_it(QString* new_name = NULL);
};


class MapReader :public QLabel
{

	Q_OBJECT

signals:
	;
private:
	QString* map_addr;
	QFile* map_file;
public slots:
	void ensure_addr(const QString& MapName);
public:
	explicit MapReader(QWidget* parent = NULL);
	~MapReader();
	void load_it(Reciver*& reciver_, Map* map);
};

#endif 
