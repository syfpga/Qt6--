#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QListWidget>
#include <QAudioOutput>
#include <QString>
#include <QAction>
#include <QCursor>
#include <QMenu>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QPixmap>
#include <QColor>
#include <QColorDialog>
#include <iostream>
#include <QMovie>
#include <QLabel>
#include <QPainter>

QString songLovename;
mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("ikun音乐播放器");
    setting_init();
}
mainWidget::~mainWidget()
{
    delete ui;
}
//设置初始化
void mainWidget::setting_init()
{  
    //new一个output对象
    audiOutput = new QAudioOutput(this);
    // new一个媒体播放对象
    mediaPalyer = new QMediaPlayer(this);
    mediaPalyer->setAudioOutput(audiOutput);

    //进度条和歌曲时间显示变化
    connect(mediaPalyer,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
    connect(mediaPalyer,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));

    //进度条改变时间改变
    connect(ui->PositionSlider,&QSlider::sliderMoved,mediaPalyer,&QMediaPlayer::setPosition);

    //音量设置
    connect(ui->VolumeSlider,SIGNAL(valueChanged(int)),audiOutput,SLOT(on_VolumeSlider_valueChanged(int value)));

    //主界面音量滑动条默认显示为隐藏
    ui->VolumeSlider->hide();

    //播放模式
    flag_mode = 2;
    //歌词显示
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_3->setAlignment(Qt::AlignCenter);
    ui->label_4->setAlignment(Qt::AlignCenter);
    ui->label_5->setAlignment(Qt::AlignCenter);
    //动图播放
    QMovie *movie = new QMovie(":/image/image/kun2.gif");
    ui->label_AutoInfo->setMovie(movie);
    movie->start();
    //换肤标志
    flag_color = 0;
    //进度条颜色
    ui->label_Position->setStyleSheet("color:rgb(255,150,21)");//时间进度标签颜色
    ui->PositionSlider->setStyleSheet("  \
         QSlider::add-page:Horizontal\
         {     \
            background-color: rgb(87, 97, 106);\
            height:4px;\
         }\
         QSlider::sub-page:Horizontal \
        {\
            background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));\
            height:4px;\
         }\
        QSlider::groove:Horizontal \
        {\
            background:transparent;\
            height:6px;\
        }\
        QSlider::handle:Horizontal \
        {\
            height: 30px;\
            width:30px;\
            border-image: url(:/image/image/kun.png);\
            border-image->resize(20,20);\
            margin: -8 0px; \
        }\
        ");
}
void mainWidget::lyrics_Display()
{
    //获取选中行号
    curPlayIndex = ui->listWidget->currentRow();

    ui->label_Motto->setText(playList[curPlayIndex].fileName());

    if(-1 != curPlayIndex)
    {
        //歌曲名字
        QString songname = ui->listWidget_lyrics->item(curPlayIndex)->text().left(ui->listWidget_lyrics->item(curPlayIndex)->text().lastIndexOf('.'));
        songLovename = songname;
        mediaPalyer->setSource(playList[curPlayIndex]);
        ui->listWidget_HistorytMusic->addItem(new QListWidgetItem(songname));
        qDebug() << songname;

        qDebug() <<playPath;
        //            读取歌词文件
        if(!lyrics.readLyricsFile(playPath+"/"+songname+".lrc")){
            qDebug() <<playPath;
            ui->label->setText("   未检测到歌词文件");
            ui->label_2->setText("请检查歌词文件是否存在");
        }
        qDebug() << lyrics.readLyricsFile(playPath+"/"+songname+".lrc");
        mediaPalyer->play();
        lyricsID = 0;
    }
}

//播放与暂停
void mainWidget::on_btn_play_clicked()
{

    switch (mediaPalyer->playbackState()) {
    case QMediaPlayer::PlaybackState::StoppedState:     //停止状态
    {
        if(playList.empty())
            break;
        if(playList1.empty())
            break;
        ui->btn_play->setIcon(QIcon(":/image/image-hover/play-hover.png"));
        lyrics_Display();
        break;
    }
    case QMediaPlayer::PlaybackState::PlayingState:     //执行状态
    {
        mediaPalyer->pause();
        ui->btn_play->setIcon(QIcon(":/image/image-hover/pase-hover.png"));
        break;
    }
    case QMediaPlayer::PlaybackState::PausedState:      //暂停状态
    {
        mediaPalyer->play();
        ui->btn_play->setIcon(QIcon(":/image/image-hover/play-hover.png"));
        break;
    }
    }
}
//添加文件
void mainWidget::on_btn_AddMusic_clicked()
{
    auto mplayPath = QFileDialog::getExistingDirectory(this,"请选择音乐歌词所在的目录", "F:/Ubuntu_System_Flie/Music_player/Music_Player/lyrics");
    playPath = mplayPath;
    QDir dir1(mplayPath);//访问文件目录
    auto musicList1 = dir1.entryList(QStringList()<<"*.lrc");
    qInfo()<<musicList1;
    auto path = QFileDialog::getExistingDirectory(this,"请选择音乐所在的目录", "F:/Ubuntu_System_Flie/Music_player/Music_Player/song");
    QDir dir2(path);
    auto musicList2 = dir2.entryList(QStringList()<<"*.mp3"<<"*.wav");
    qInfo()<<musicList2;
    //把歌词文件放到listWidget_lyrics
    ui->listWidget_lyrics->addItems(musicList1);
    //把音乐名字放到listwidget展示
    ui->listWidget->addItems(musicList2);
    ui->listWidget_localMusic->addItems(musicList2);
    //默认选中第一个音乐
    ui->listWidget_lyrics->setCurrentRow(0);
    ui->listWidget->setCurrentRow(0);
    ui->listWidget_localMusic->setCurrentRow(0);
    //歌词路径
    for(auto lyr : musicList1)
        playList1.append(QUrl::fromLocalFile(mplayPath+"/"+lyr));
    //把音乐完整路径保存起来
    for(auto file : musicList2)
        playList.append(QUrl::fromLocalFile(path+"/"+file));
    //qInfo() << playList1;
}
//上一首
void mainWidget::on_btn_Pre_clicked()
{
    if(playList.empty())
        return;
    if(playList1.empty())
        return;
    curPlayIndex--;
    if(curPlayIndex < 0)
        curPlayIndex = 0;
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPalyer->setSource(playList[curPlayIndex]);
    ui->label_Motto->setText(playList[curPlayIndex].fileName());
    lyrics.listLyricsTime.clear();
    mediaPalyer->play();
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    lyrics.listLyricsText.clear();
    lyrics_Display();
}
//下一首
void mainWidget::on_btn_Next_clicked()
{
    if(playList.empty())
        return;
    if(playList1.empty())
        return;
    curPlayIndex = (curPlayIndex + 1) % playList.size();
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPalyer->setSource(playList[curPlayIndex]);
    ui->label_Motto->setText(playList[curPlayIndex].fileName());
    lyrics.listLyricsTime.clear();
    mediaPalyer->play();
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    lyrics.listLyricsText.clear();
    lyrics_Display();
}
//点击播放
void mainWidget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPalyer->setSource(playList[curPlayIndex]);
    ui->label_Motto->setText(playList[curPlayIndex].fileName());
    ui->btn_play->setIcon(QIcon(":/image/image-hover/play-hover.png"));
    lyrics.listLyricsTime.clear();
    mediaPalyer->play();
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    lyrics.listLyricsText.clear();
    lyrics_Display();
}
void mainWidget::on_listWidget_HistorytMusic_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPalyer->setSource(playList[curPlayIndex]);
    ui->label_Motto->setText(playList[curPlayIndex].fileName());
    ui->btn_play->setIcon(QIcon(":/image/image-hover/play-hover.png"));
    lyrics.listLyricsTime.clear();
    mediaPalyer->play();
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    lyrics.listLyricsText.clear();
    lyrics_Display();
}
void mainWidget::on_listWidget_localMusic_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPalyer->setSource(playList[curPlayIndex]);
    ui->label_Motto->setText(playList[curPlayIndex].fileName());
    ui->btn_play->setIcon(QIcon(":/image/image-hover/play-hover.png"));
    lyrics.listLyricsTime.clear();
    mediaPalyer->play();
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    lyrics.listLyricsText.clear();
    lyrics_Display();
}
void mainWidget::on_listWidget_LikeMusic_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPalyer->setSource(playList[curPlayIndex]);
    ui->label_Motto->setText(playList[curPlayIndex].fileName());
    ui->btn_play->setIcon(QIcon(":/image/image-hover/play-hover.png"));
    lyrics.listLyricsTime.clear();
    mediaPalyer->play();
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    lyrics.listLyricsText.clear();
    lyrics_Display();
}
//播放总时间
void mainWidget::onDurationChanged(qint64 duration)
{
    ui->PositionSlider->setMaximum(duration); //设置进度条最大值 也就是歌曲时长 ms
    int secs = duration/1000; //全部秒数
    int mins = secs/60;//分
    secs = secs % 60;//秒
    durationTime = QString::asprintf("%d:%d", mins, secs);
    ui->label_Position->setText(positionTime + "/" + durationTime);
}
//播放当前时间
void mainWidget::onPositionChanged(qint64 position)
{
    setPlayTime();
    if(ui->PositionSlider->isSliderDown())
        return;//如果手动调整进度条，则不处理
    ui->PositionSlider->setSliderPosition(position);
    int secs = position/1000;
    int mins = secs/60;
    secs = secs % 60;
    positionTime = QString::asprintf("%d:%d", mins, secs);
    ui->label_Position->setText(positionTime + "/" + durationTime);
    if(positionTime == durationTime){
        positionTime.clear();
        int Index = rand()%(playList.size()-1);
        switch(flag_mode){
            case 0: ui->listWidget->setCurrentRow(Index);
                    mediaPalyer->setSource(playList[Index]);
                    ui->label_Motto->setText(playList[Index].fileName());
                    lyrics.listLyricsTime.clear();
                    mediaPalyer->play();
                    ui->label->clear();
                    ui->label_2->clear();
                    ui->label_3->clear();
                    ui->label_4->clear();
                    ui->label_5->clear();
                    lyrics.listLyricsText.clear();
                    break;
            case 1:mediaPalyer->setLoops(-1);break;
            default:on_btn_Next_clicked();break;
        }
    }
}
//静音
void mainWidget::on_btn_Voice_clicked()
{
    if(ui->VolumeSlider->isHidden())
    {
        ui->VolumeSlider->show();
    }else
    {
        ui->VolumeSlider->hide();
    }
    if(audiOutput->volume() != 0){
        ui->btn_Voice->setIcon(QIcon(":/image/image/mute.png"));
        audiOutput->setVolume(0);
        ui->VolumeSlider->setValue(0);
    }else{
        ui->btn_Voice->setIcon(QIcon(":/image/image/music-voice.png"));
        audiOutput->setVolume(0.5);
        ui->VolumeSlider->setValue(0.5);
    }
}
//调节音量
void mainWidget::on_VolumeSlider_valueChanged(int value)
{
    audiOutput->setVolume((float)value/100.0);
    if(audiOutput->volume() != 0){
        ui->btn_Voice->setIcon(QIcon(":/image/image/music-voice.png"));
    }
}
//播放方式
void mainWidget::on_btn_PlayMode_clicked()
{
    QAction *random=new QAction(QIcon(":/image/image/random2.png"),"随机播放");
    QAction *itemloop=new QAction(QIcon(":/image/image/loop-one2.png"),"单曲循环");
    QAction *loop=new QAction(QIcon(":/image/image/loop2.png"),"循环播放");
    //随机播放
    connect(random,&QAction::triggered,this,[=](){
        ui->btn_PlayMode->setIcon(QIcon(":/image/image/play-random.png"));
        ui->btn_PlayMode->setToolTip(u8"随机播放");
        flag_mode = 0;
    });
    //单曲循环
    connect(itemloop,&QAction::triggered,this,[=](){
        qDebug()<<"ok1";
        ui->btn_PlayMode->setIcon(QIcon(":/image/image/loop-one.png"));
        ui->btn_PlayMode->setToolTip(u8"单曲循环");
        flag_mode = 1;
    });
    //循环播放
    connect(loop,&QAction::triggered,this,[=](){
        ui->btn_PlayMode->setIcon(QIcon(":/image/image/loop.png"));
        ui->btn_PlayMode->setToolTip(u8"循环播放");
        flag_mode = 2;
    });
    QMenu *mode=new QMenu(this);
    mode->addAction(random);
    mode->addAction(itemloop);
    mode->addAction(loop);
    mode->exec(QCursor::pos());
}
//历史记录
void mainWidget::on_btn_CurrentMusic_clicked()
{
    ui->page_0->show();
    ui->page_1->hide();
    ui->page_2->hide();
    ui->page_lyrics->hide();
}
//本地歌曲
void mainWidget::on_btn_LocalMusic_clicked()
{
    ui->page_0->hide();
    ui->page_1->show();
    ui->page_2->hide();
    ui->page_lyrics->hide();
}
//我喜欢
void mainWidget::on_btn_LikeMusic_clicked()
{
    ui->page_0->hide();
    ui->page_1->hide();
    ui->page_2->show();
    ui->page_lyrics->hide();
}
//收藏(喜欢)
void mainWidget::on_love_Button_clicked()
{
    ui->listWidget_LikeMusic->addItem(new QListWidgetItem(songLovename));
}

//关于
void mainWidget::on_btn_About_clicked()
{
    QMessageBox::about(this,"关于",
                       "iku播放器 | 一款精致小巧又清新的音乐播放器\n"
                       "         著作人：ikun同学\n"
                       "           版本号：2.0\n"
                       "希望ikun播放器能给您带来享受音乐的快乐\n"
                       );
}
//设置主题
void mainWidget::on_btn_Skin_clicked()
{
    if(flag_color == 0){
        this->setStyleSheet("QWidget {background-color:rgb(85, 85, 255)}");
        flag_color = 1;
    }
    else if(flag_color == 1){
        this->setStyleSheet("QWidget {background-color:rgb(23, 120, 90)}");
        flag_color = 2;
    }
    else if(flag_color == 2){
        this->setStyleSheet("QWidget {background-color:rgb(255, 80, 100)}");
        flag_color = 3;
    }
    else if(flag_color == 3){
        this->setStyleSheet("QWidget {background-color:rgb(89, 190, 90)}");
        flag_color = 4;
    }
    else if(flag_color == 4){
        this->setStyleSheet("QWidget {background-color:rgb(123, 10, 90)}");
        flag_color = 0;
    }
}
//最小化
void mainWidget::on_btn_Min_clicked()
{
    showMinimized();
}
//最大化
void mainWidget::on_btn_Max_clicked()
{
    showMaximized();
}
//关闭
void mainWidget::on_btn_Close_clicked()
{
    this->close();
}
//歌词解析
void mainWidget::on_btn_lyrics_clicked()
{
    ui->page_0->hide();
    ui->page_1->hide();
    ui->page_2->hide();
    ui->page_lyrics->show();
}
void mainWidget::setPlayTime()
{
    if(!lyrics.getListLyricsTime().empty()&&mediaPalyer->position()>=lyrics.getListLyricsTime().at(lyricsID)&&lyricsID<lyrics.getListLyricsTime().size()-1)
    {
        if(lyricsID<=2)
        {
            ui->label_3->setText(lyrics.getListLyricsText().at(lyricsID));
            ui->label_4->setText(lyrics.getListLyricsText().at(lyricsID+1));
            ui->label_5->setText(lyrics.getListLyricsText().at(lyricsID+2));
            if(lyricsID==1)
            {
                ui->label_2->setText(lyrics.getListLyricsText().at(lyricsID-1));
            }
        }
        else if(lyricsID>2)
        {
            ui->label->setText(lyrics.getListLyricsText().at(lyricsID-2));
            ui->label_2->setText(lyrics.getListLyricsText().at(lyricsID-1));
            ui->label_3->setText(lyrics.getListLyricsText().at(lyricsID));
            ui->label_4->setText(lyrics.getListLyricsText().at(lyricsID+1));
            ui->label_5->setText(lyrics.getListLyricsText().at(lyricsID+2));
        }
        lyricsID++;
    }
}
