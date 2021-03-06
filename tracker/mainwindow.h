#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QTest>
#include <algorithm>
#include <QLayout>
#include <QPushButton>
#include <QStyle>
#include <QStringList>
#include <QLocale>

#include <QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "colorbutton.h"

class hour;
class hour_group;
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QColor selected_clr;

public slots:
    void selectColor(QColor& clr);

private slots:
    // side menu buttons
    void on_today_button_clicked();
    void on_yesterday_button_clicked();
    void on_scroll_button_clicked();
    void on_tot_anal_button_clicked();

    // autosve on changing coloers xdD !!these are connected!!
    void save();
    void update_pie_chart(const QColor& clr, const QColor& prev_clr);


private:
    Ui::MainWindow* ui;

    QGraphicsScene* today_scene_;
    QGraphicsScene* yesterday_scene_;
    QGraphicsScene* scroll_scene_;

    hour* test_hour_;
    QGraphicsTextItem* num;
    hour_group* group;

    QMap<int, hour*> mapped_today;
    QMap<int, hour*> mapped_yesterday;
    void create_hours(QGraphicsScene* scene, QMap<int, hour*>& mapp);

    struct dType{
        QString color;
        QString activity;
        int num;
    };

    std::vector<dType> pie_log;
    void log_for_pie(QStringList& lst);
    void create_pieChart();
    QPieSeries *series_;

    QPushButton* prev_btn;
    QPushButton* cur_btn;

    void parse_file();
    void process_dayLine(QStringList& line, bool isToday = false, bool isYesterday = false);
    void paint_day(QMap<int, hour*>& mapped_day, QStringList& split_line);
    QString create_fileline(QDate date, QString str_date, QMap<int, hour*>& mapp);

    void create_scroll_hours(QStringList& data, bool isToday, bool isYesterday);
    void update_scroll_scene();
    QGraphicsTextItem* create_scroll_text(QString date, QString wday);

    void add_click_animation(QWidget* btn);
    void loop_trough_btns();
    QVector<ColorButton*> allButtons;

    QFile file{"C:/Users/fofa/qt_projects/time_tracker/data.txt"};

    int day_entries = 0;

    QDate today = QDate::currentDate();
    QDate yesterday = today.addDays(-1);

    QString str_today = today.toString("dd-MM-yyyy");
    QString str_yesterday = yesterday.toString("dd-MM-yyyy");


};
#endif // MAINWINDOW_H
