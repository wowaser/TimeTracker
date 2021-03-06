#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hour.h"
#include "hour_group.h"



const QString def_button_border = "#878391";

const char file_sep = ';';
const QString btnHoverPressed_txt = "QPushButton:hover{border:1px solid #00bbff}\n"
                                    "QPushButton:pressed{border-style:solid;border-color: #00bbff; "
                                    "border-width:3px}\n";

/*
 * TODO:
 *  TODAY/YESTERDAy STRUCT + funcs
 *  ADDING/DELETING COLOR BUTTONS
 *  DESIGN XD
 *  STORING ALL THE COLORS DYNAMICALLY>?
 *  RESIZE EVENT ON QGRAPHICS AND BTNS
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0,0,1200,750);

    // SEtting up today page

    today_scene_ = new QGraphicsScene(this);
    ui->graphicsView_today->setScene(today_scene_);
    today_scene_->setSceneRect(0,0,900,490);
    // setting up yesterday
    yesterday_scene_ = new QGraphicsScene(this);
    ui->graphicsView_yesterday->setScene(yesterday_scene_);
    yesterday_scene_->setSceneRect(0,0,900,490);

    // Setting up scroll page
    scroll_scene_ = new QGraphicsScene(this);
    ui->scroll_view->setScene(scroll_scene_);

    // This func creates a hover and click animation for buttons AND logs their colors for piechart
    loop_trough_btns();


    create_hours(today_scene_, mapped_today);
    create_hours(yesterday_scene_, mapped_yesterday);

    parse_file();

    ui->toolBar->hide();
    statusBar()->hide();

    ui->selected_button->setDisabled(true);
    //  Connecting selected color button to a variable "selected_color"
    connect(ui->selected_button, SIGNAL(selected_clr_changed(QColor&)), this, SLOT(selectColor(QColor&)));
    //  Setting the default color of the selected "button".

    selected_clr = allButtons[0]->palette().button().color();
    allButtons[0]->pressed(selected_clr);



    create_pieChart();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectColor(QColor &clr){
    selected_clr = clr;
}

void MainWindow::create_hours(QGraphicsScene* scene, QMap<int, hour*>& mapp){
    for(int i=0; i<24;++i){

        num = new QGraphicsTextItem();
        QString str_i = QString::number(i);
        num->setPlainText(str_i);
        scene->addItem(num);
        num->setPos((i+0.9)*33,120);

        test_hour_ = new hour(this);
        scene->addItem(test_hour_);
        test_hour_->setRect(test_hour_->boundingRect());
        test_hour_->setPos((i+1)*33,140);
        mapp[i] = test_hour_;
        // save on clicking an hour SEE FUNC save()
        connect(test_hour_, SIGNAL(clickSave()), this, SLOT(save()));
        // updating values of pieChart slices on clicking day object
        connect(test_hour_, SIGNAL(update_piechart(const QColor&, const QColor&)), this, SLOT(update_pie_chart(const QColor&, const QColor&)));
    }
}

void MainWindow::parse_file(){

    bool today_logged = false;
    bool yesterday_logged = false;

    QStringList today_line;
    QStringList yest_line;

    if(!file.exists()){
        qDebug()<<"no such file";
    }

    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);        
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList split_line = line.split(file_sep,Qt::SkipEmptyParts);
            // paint today, not adding it anywhere yet
            if(line.startsWith(str_today)){
                paint_day(mapped_today, split_line);
                today_logged = true;
                today_line = split_line;
            }

            // same with yesterday
            else if(line.startsWith(str_yesterday)){
                paint_day(mapped_yesterday, split_line);
                yesterday_logged = true;
                yest_line = split_line;
            }
            else{
                process_dayLine(split_line);
            }
        }
        file.close();

    }
    else{
        qDebug()<<"cannot open file!";
    }
    // processing today and yesterday differently due to possibility
    // of one or both of them missing
    // if one of the days wasn't logged, we create a blank day onto the scroll area, otherwise it is
    // processed just lijke any other day

    // mapped yest/today hours are all blank(white) if not logged
    if(!yesterday_logged){
       yest_line = create_fileline(yesterday, str_yesterday, mapped_yesterday).split(file_sep, Qt::SkipEmptyParts);
    }
    if(!today_logged){
        today_line = create_fileline(today, str_today, mapped_today).split(file_sep, Qt::SkipEmptyParts);
    }
    // SEE ORDER!!
    process_dayLine(yest_line, false, true);
    process_dayLine(today_line, true);

}

void MainWindow::process_dayLine(QStringList &line, bool isToday, bool isYesterday){
    // counting number of lines in a file
    day_entries+=1;
    // creating a day onto scroll area
    create_scroll_hours(line,isToday, isYesterday);
    // logging data for piechart
    log_for_pie(line);
}

void MainWindow::paint_day(QMap<int, hour *> &mapped_day, QStringList& split_line){// bugcheck needed
    int i = 0;
    for(QString clr:QStringList(split_line.begin()+2, split_line.end())){
        mapped_day[i]->change_color(clr);
        ++i;
    }
}


void MainWindow::create_scroll_hours(QStringList& split_data, bool isToday, bool isYesterday){
    update_scroll_scene();
    group = new hour_group(this);
    QGraphicsTextItem* txt = create_scroll_text(split_data[0], split_data[1]);
    group->addToGroup(txt);

    group->num = day_entries;
    for(int i=0; i<24;++i){        
        test_hour_ = new hour(this);

        group->addToGroup(test_hour_);

        test_hour_->setRect(test_hour_->boundingRect());
        test_hour_->setPos((i+1)*33,0); // y is const
        test_hour_->change_color(split_data[i+2]);
        if(isToday){
            hour* conn_hour = mapped_today[i];
            connect(conn_hour, SIGNAL(update_color(const QColor&)), test_hour_, SLOT(slot_change_clr(const QColor&)));
        }
        else if(isYesterday){
            hour* conn_hour = mapped_yesterday[i];
            connect(conn_hour, SIGNAL(update_color(const QColor&)), test_hour_, SLOT(slot_change_clr(const QColor&)));
        }
    }
    group->setPos(50,(day_entries-1)*210); // x is const

    scroll_scene_->addItem(group);
}

void MainWindow::update_scroll_scene(){
    int scroll_height = test_hour_->hour_height*day_entries;
    scroll_height+=10*day_entries;
    scroll_scene_->setSceneRect(0,0,900,scroll_height);

}

QGraphicsTextItem* MainWindow::create_scroll_text(QString date, QString wday){ // needs text allignment !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    QGraphicsTextItem* txt = new QGraphicsTextItem(group);

    QString day = "";
    if(date == str_today){
        day = "Today\n";
    }
    else if(date == str_yesterday){
        day = "Yesterday\n";
    }
    txt->setPlainText(day+date+"\n"+wday);
    txt->setPos(-45,100);
    txt->setFont(QFont("Bahnschrift",12));
    return txt;
}


QString MainWindow::create_fileline(QDate date, QString str_date, QMap<int, hour *>& mapp){
    // add date
    QString line = str_date+file_sep;
    // add weekday
    int wday = date.dayOfWeek();
    QLocale locale;
    QString wday_name = locale.dayName(wday);
    line+=wday_name+file_sep;
    // adding 24 colors
    for(auto hourr:mapp){
        line+=hourr->brush_.color().name()+file_sep;
    }

    return line;

}

void MainWindow::log_for_pie(QStringList &lst){
    for(QString clr:QStringList(lst.begin()+2, lst.end())){
        for(auto& log: pie_log){
            if(log.color == clr){
                log.num += 1;
            }
        }
    }
}

void MainWindow::create_pieChart(){
    series_ = new QPieSeries(this);
    QPieSlice *slice;
    int i = 0;
    for(auto log:pie_log){
        series_->append(log.activity, log.num);
        slice = series_->slices().at(i);
        if(log.num != 0){
            slice->setLabelVisible(true);
            slice->setLabelFont(QFont("Bahnschrift",12));
        }
        slice->setBrush(QColor(log.color));
        ++i;
    }
    series_->setHoleSize(0.30);
    QChart *chart = new QChart();
    chart->addSeries(series_);

    chart->setTitle("ANALYTICS OF YOR TIME XD");
    chart->setTitleFont(QFont("Bahnschrift",18));
    chart->legend()->hide();

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

}

void MainWindow::update_pie_chart(const QColor &clr, const QColor &prev_clr){
    for(auto slice: series_->slices()){
        // subtract 1 from value of previous color; if color wasn't chosen(white), nothing happens
        // because we don't have(or at least shouldn't have) white slices
        int cur_val = slice->value();
        if(slice->brush().color() == prev_clr){
            slice->setValue(cur_val - 1);
        }
        // adding 1 to same colored slice, if white nothing happens
        if(slice->brush().color() == clr){
            slice->setValue(cur_val + 1);
        }

        if(slice->value() == 0){
            slice->setLabelVisible(false);
        }
        else{
            slice->setLabelVisible(true);
            slice->setLabelFont(QFont("Bahnschrift",12));
        }
    }
}

void MainWindow::add_click_animation(QWidget *btn){
    QString btn_sSheet = btn->styleSheet();
    btn_sSheet+=btnHoverPressed_txt;
    btn->setStyleSheet(btn_sSheet);
}

//  I don't like that one bit
void MainWindow::loop_trough_btns(){
    for(auto child:ui->page_1->children()){
        QLabel* lbl = qobject_cast<QLabel*>(child);
        // if the object is label{...
        if(lbl != NULL){
            QString activity = lbl->text();
            QWidget* wid_bud_btn = lbl->buddy();
            ColorButton*  bud_btn = qobject_cast<ColorButton*>(wid_bud_btn);
            if(bud_btn != nullptr){ // preventing pointer crashes
                // addind border highlight to buttons
                add_click_animation(bud_btn);

                // fetching colors of buttons to store in struct dType for further creating of the piechart
                QColor btn_clr = bud_btn->palette().button().color();
                QString btn_clr_name = btn_clr.name();
                dType log = {btn_clr_name, activity, 0};
                pie_log.push_back(log);
                connect(bud_btn, SIGNAL(pressed(QColor&)), ui->selected_button, SLOT(set_color(QColor&)));
                allButtons.push_back(bud_btn);
            }
        }
    }
}

void MainWindow::save(){
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        QString new_file = "";
        // special cases for days that are modifiable, ie yesterday and today
        QString today_line = create_fileline(today, str_today, mapped_today);
        QString yesterday_line = create_fileline(yesterday, str_yesterday, mapped_yesterday);

        // searching for previous log of the same days and "deleting" them by not including them into file
        while(!stream.atEnd()){
            QString file_line = stream.readLine();
            if(!file_line.startsWith(str_today) and !file_line.startsWith(str_yesterday)){
                new_file.append(file_line+'\n');
            }
        }
        // deleting contents of the file and replacing all the lines !! SEE ORDER !!
        file.resize(0);
        stream<<new_file;
        stream<<yesterday_line<<'\n';
        stream<<today_line<<'\n';
        file.close();

    }
    else{
        qDebug()<<"didnt open for writing";
    }
}


void MainWindow::on_today_button_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_small->setCurrentIndex(0);
}

void MainWindow::on_yesterday_button_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_small->setCurrentIndex(1);
}

void MainWindow::on_scroll_button_clicked(){
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_tot_anal_button_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_testButton_clicked(){
    ui->stackedWidget->setCurrentIndex(3);
}
