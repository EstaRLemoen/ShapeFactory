#include"WorkToolBar.h"
#include"Reader.h"
#include <QPushButton>
#include <QMenuBar>
#include <QDebug>
#include <QString>
#include <QMouseEvent>
#include <QPoint>
#include <string>
#include <QFont>

extern AccountReader* cur_account;

WorkToolBar::WorkToolBar(QWidget* parent) :QToolBar(parent),foundation(cur_account->Saved_foundation)
{
    qDebug() << "正在初始化工具区";
    QFont form_;
    form_.setPointSize(12);

    //add
    QMenu* add_menu = new QMenu;

    QAction* add_miner = new QAction;
    add_miner->setFont(form_);
    add_miner->setText("采矿机");

    QAction* add_belt = new QAction;
    add_belt->setFont(form_);
    add_belt->setText("传送带");

    QAction* add_cutter = new QAction;
    add_cutter->setFont(form_);
    add_cutter->setText("切割机");

    QMenu* add_spiner = new QMenu;
    add_spiner->setFont(form_);
    add_spiner->setTitle("旋转机");
    QAction* spiner_l = new QAction;
    spiner_l->setFont(form_);
    spiner_l->setText("向左");
    QAction* spiner_r = new QAction;
    spiner_r->setFont(form_);
    spiner_r->setText("向右");
    add_spiner->addAction(spiner_l);
    add_spiner->addAction(spiner_r);

    
    QAction* add_dustbin = new QAction;
    add_dustbin->setFont(form_);
    add_dustbin->setText("回收槽");

    add_menu->addAction(add_miner);
    add_menu->addAction(add_belt);
    add_menu->addAction(add_cutter);
    add_menu->addMenu(add_spiner);
    add_menu->addAction(add_dustbin);
    QPushButton* btn_add = new QPushButton;
    btn_add->setFont(form_);
    btn_add->setText("建造");
    btn_add->setFixedSize(80, 40);
    btn_add->setMenu(add_menu);
    connect(btn_add, &QPushButton::clicked, btn_add, &QPushButton::showMenu);

    //delete
    QPushButton* btn_delete = new QPushButton;
    btn_delete->setFont(form_);
    btn_delete->setText("移除");
    btn_delete->setFixedSize(80, 40);
    
    //on and off
    QPushButton* btn_poweron = new QPushButton;
    btn_poweron->setFont(form_);
    btn_poweron->setText("工作模式");
    btn_poweron->setFixedSize(100, 40);
    QPushButton* btn_poweroff = new QPushButton;
    btn_poweroff->setFont(form_);
    btn_poweroff->setText("修改模式");
    btn_poweroff->setFixedSize(100, 40);

    //foundation
    QPushButton* btn_foundation = new QPushButton;
    btn_foundation->setFont(form_);
    btn_foundation->setFixedSize(180, 40);

    //strengthen
    QPushButton* btn_strengthen = new QPushButton;
    btn_strengthen->setFont(form_);
    btn_strengthen->setText("强化");
    btn_strengthen->setFixedSize(80, 40);

    //close and save game
    QPushButton* btn_close = new QPushButton;
    btn_close->setFont(form_);
    btn_close->setText("保存并退出");
    btn_close->setFixedSize(100, 40);

    this->addWidget(btn_foundation);
    this->addWidget(btn_poweron);
    this->addWidget(btn_poweroff);
    this->addWidget(btn_add);
    this->addWidget(btn_delete);
    this->addWidget(btn_strengthen);
    this->addWidget(btn_close);

    this->clear_preopr();
    //connect buttons
    connect(add_miner, &QAction::triggered, this, &WorkToolBar::call_a_miner);
    connect(add_belt, &QAction::triggered, this, &WorkToolBar::call_a_belt);
    connect(add_cutter, &QAction::triggered, this, &WorkToolBar::call_a_cutter);
    connect(add_dustbin, &QAction::triggered, this, &WorkToolBar::call_a_dustbin);
    connect(spiner_l, &QAction::triggered, this, &WorkToolBar::call_a_spiner_l);
    connect(spiner_r, &QAction::triggered, this, &WorkToolBar::call_a_spiner_r);
    connect(btn_delete, &QPushButton::clicked, this, &WorkToolBar::call_d);
    connect(btn_poweron, &QPushButton::clicked, this, &WorkToolBar::call_eq_on);
    connect(btn_poweroff, &QPushButton::clicked, this, &WorkToolBar::call_eq_off);
    connect(this, &WorkToolBar::show_foundtion, btn_foundation, &QPushButton::setText);
    connect(btn_strengthen, &QPushButton::clicked, this, &WorkToolBar::call_eq_strengthen);
    connect(btn_close, &QPushButton::clicked, this, &WorkToolBar::save_and_exit);

    
}

WorkToolBar::~WorkToolBar() {
    ;
}

void WorkToolBar::do_change(QMouseEvent* event_) {
    QPoint pos_ = event_->pos();
    operate_map(pos_.x(), pos_.y());
    return;
}

void WorkToolBar::operate_map(int x,int y) {
    if (opr_status == NULL || *opr_status == "") {
        //qDebug() << "Empty Operation.";
        return;
    }
    else if (*opr_status == "Build") {
        qDebug() << "Operation Build";
        if (opr_kind == NULL || *opr_kind == "") {
            qDebug() << "Error! No Equipment Kind.";
        }
        else {
            qDebug() << "Order sending to map.";
            emit call_map_build(opr_kind,opr_kind_2, x, y);
        }
    }
    else if (*opr_status == "Remove") {
        qDebug() << "Operation Remove";
        qDebug() << "Order sending to map.";
        emit call_map_remove(x, y);
    }
    else if (*opr_status == "Strengthen") {
        qDebug() << "Operation Strengthen";
        qDebug() << "Order sending to map.";
        emit call_map_strengthen(x, y, ef_addition);
    }
    else {
        qDebug() << "Error! Operation fault.";
    }
    this->clear_preopr();
    return;
}

void WorkToolBar::prepare_Opera(QString status_,QString kind_,QString kind_2_l_or_r) {
    this->clear_preopr();
    qDebug() << "New operation preparing.";
    opr_status->assign(status_);
    opr_kind ->assign(kind_);
    opr_kind_2->assign(kind_2_l_or_r);
    return;
}

void WorkToolBar::clear_preopr() {
    qDebug() << "Old operation clearing.";
    if (opr_status == NULL) {
        qDebug() << "initing opr status.";
        opr_status = new QString;
    }
    if (opr_kind == NULL) {
        qDebug() << "initing opr kind.";
        opr_kind = new QString;
    }
    if (opr_kind_2 == NULL) {
        qDebug() << "initing opr kind_2.";
        opr_kind_2 = new QString;
    }
    opr_status->clear();
    opr_kind->clear();
    opr_kind_2->clear();
    return;
}


void WorkToolBar::add_foundation(long long int money) {
    foundation += money;
    QString temp = "可用资金：";
    temp.push_back(QString::number(foundation));
    emit show_foundtion(temp);
    return;
}
void WorkToolBar::sub_foundation(long long int money) {
    foundation -= money;
    QString temp = "可用资金：";
    temp.push_back(QString::number(foundation));
    emit show_foundtion(temp);
    return;
}
void WorkToolBar::call_a_miner() {
    prepare_Opera("Build", "Miner","");
    return;
}
void WorkToolBar::call_a_belt() {
    prepare_Opera("Build", "Belt","");
    return;
}
void WorkToolBar::call_a_cutter() {
    prepare_Opera("Build", "Cutter","");
    return;
}
void WorkToolBar::call_a_spiner_l() {
    prepare_Opera("Build", "Spiner","Left");
    return;
}
void WorkToolBar::call_a_spiner_r() {
    prepare_Opera("Build", "Spiner","Right");
    return;
}
void WorkToolBar::call_a_dustbin() {
    prepare_Opera("Build", "Dustbin","");
    return;
}
void WorkToolBar::call_d() {
    prepare_Opera("Remove","","");
    return;
}
void WorkToolBar::call_eq_strengthen(int ef_add){
    prepare_Opera("Strengthen", "","");
    if (ef_add <= 0) ef_addition = 1;
    else ef_addition = ef_add;
    return;
}
void WorkToolBar::call_eq_on() {
    this->clear_preopr();
    qDebug() << "Operation Power On";
    if (foundation >= 0) {
        qDebug() << "Order sending to map.";
        emit call_map_poweron();
        emit power_map(true);
    }
    else {
        qDebug() << "Failed! debt too much!";
    }
    return;
}
void WorkToolBar::call_eq_off() {
    qDebug() << "Operation Power Off";
    qDebug() << "Order sending to map.";
    emit call_map_poweroff();
    emit power_map(false);
    return;
}

void WorkToolBar::save_and_exit() {
    //test
    QString* test_file_name = new QString("text1");
    cur_account->save_it(test_file_name);
    ((QWidget*)this->parent())->close();
    return;
}