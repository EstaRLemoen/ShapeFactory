#include "QtWidgetsApplication1.h"
#include "WorkToolBar.h"
#include "GameStatusBar.h"
#include "Map.h"
#include "Mineral.h"
#include "Equipment.h"
#include "Reader.h"
#include "Task.h"
#include <vector>
//#include <fstream>
#include <QLabel>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>

extern int M_NUM_calcite;
extern int M_NUM_graphite;

AccountReader* cur_account = NULL;
extern Reciver* recive_center;

void load_init(QWidget* par_) {
    cur_account->load_it();
    M_NUM_calcite = 1;
    M_NUM_graphite = 2;
    return;
}
void load_map(Map* map, QWidget* par_) {
    MapReader* temp = new MapReader(par_);
    temp->load_it(recive_center, map);
    return;
    /*
    int x = 0;
    int y = 0;
    int center_w = 0;
    int center_h = 0;

    QPixmap block_;
    Mineral* min = NULL;
    block_.load(":/resouce/image/block_Calcite.jpg");
    min = new Calcite(map);
    y = 2;
    x = 2;
    map->setResBlock(y,x, &block_, min);

    
    min = NULL;
    block_.load(":/resouce/image/block_Graphite.jpg");
    min = new Graphite(map);
    y = 7;
    x = 9;
    map->setResBlock(y, x, &block_, min);

    y = 4;
    x = 4;
    center_w = 3;
    center_h = 2;
    recive_center = new Reciver(map,center_w,center_h);
    recive_center->set_loc(x, y);
    map->link_eq(recive_center,y,x,center_h,center_w);
    for (int i = 0; i < center_h; i++) {
        for (int j = 0; j < center_w; j++) {
            map->occupyBlock(y + i, x + j);
        }
    }
    recive_center->move(x * Map::B_sidelen, y * Map::B_sidelen);
    recive_center->show();
    return;
    */
}

//void load_saved_status();

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setFixedSize(1200, 700);

    //初始化全局变量
    load_init(this);
    
    //底图
    QImage* background = new QImage;
    background->load(":/resouce/image/try.jpg");
    *background = background->scaled(1200, 700);
    QLabel* botton = new QLabel(this);
    botton->setPixmap(QPixmap::fromImage(*background));
    botton->resize(1200, 700);
    botton->move(0, 0);
    botton->show();
    delete background;

    //地图
    Map* map = new Map(this);
    setCentralWidget(map);
    //初始化地图
    load_map(map, this);

    //工具栏
    WorkToolBar* tools = new WorkToolBar(this);
    addToolBar(tools);
    //tools->setAllowedAreas(Qt::RightToolBarArea);
    tools->setFloatable(false);
    tools->add_foundation(0);
    //connect(this, &QWidget::mousePressEvent, tools, &WorkToolBar::do_change);

    connect(recive_center, &Reciver::renew_foundation, tools, &WorkToolBar::add_foundation);
    connect(map, &Map::ask_opr, tools, &WorkToolBar::do_change);
    connect(tools, &WorkToolBar::call_map_strengthen,map, &Map::strengthen_single_eq);
    connect(tools, &WorkToolBar::call_map_build, map, &Map::build_eq);
    connect(tools, &WorkToolBar::call_map_remove, map, &Map::remove_eq);
    connect(tools, &WorkToolBar::call_map_poweron, map, &Map::map_on);
    connect(tools, &WorkToolBar::call_map_poweroff, map, &Map::map_off);
    connect(map, &Map::send_cost, tools, &WorkToolBar::sub_foundation);
    //connect(tools, &WorkToolBar::power_map, map, &Map::work_status_chage);
     
    //状态栏
    GameStatusBar* status = new GameStatusBar(this);
    setStatusBar(status);
    //status->move(0, 40);
    status->setTaskStatus(5, 0);
    connect(recive_center, &Reciver::show_Mineral_income, status, &GameStatusBar::renew_mineral_income);
    
    //初始化任务
    QString* test_task_name = new QString("Graphite_0");
    Mineral* test_gra = new Graphite(this);
    test_gra->hide();
    ProductTask* test_p_task = new ProductTask(this, test_task_name, test_gra, 10);
    status->addProductTask(test_p_task);
    test_p_task->show_task();
    qDebug() << "Product connect reciver" << connect(recive_center, &Reciver::push_p_tasks, test_p_task, &ProductTask::push_process);
    connect(test_p_task, &ProductTask::mineral_value_increase, cur_account, &AccountReader::increase_mineral_value);
    //2
    QString* test_task_name2 = new QString("Graphite_5");
    Mineral* test_gra2 = new Graphite(this);
    test_gra2->init_m(0, 5, true, cur_account->VALUE_graphite);
    test_gra2->hide();
    ProductTask* test_p_task2 = new ProductTask(this, test_task_name2, test_gra2, 10);
    status->addProductTask(test_p_task2);
    test_p_task2->show_task();
    qDebug() << "Product connect reciver" << connect(recive_center, &Reciver::push_p_tasks, test_p_task2, &ProductTask::push_process);
    connect(test_p_task2, &ProductTask::mineral_value_increase, cur_account, &AccountReader::increase_mineral_value);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
