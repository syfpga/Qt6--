#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtMultimedia/QMediaPlayer> // 包含头文件
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QTimer>
#include <QMenu>
#include <QMap>
#include "lyrics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainWidget; }
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();
    void setPlayTime();

private slots:
    //设置初始化
    void setting_init();
    //歌词读取
    void lyrics_Display();
    //播放与暂停
    void on_btn_play_clicked();
    //添加文件
    void on_btn_AddMusic_clicked();
    //上一首
    void on_btn_Pre_clicked();
    //下一首|自动
    void on_btn_Next_clicked();
    //点击播放
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_HistorytMusic_doubleClicked(const QModelIndex &index);
    void on_listWidget_localMusic_doubleClicked(const QModelIndex &index);
    void on_listWidget_LikeMusic_doubleClicked(const QModelIndex &index);
    //播放时间
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    //静音
    void on_btn_Voice_clicked();
    //调节音量
    void on_VolumeSlider_valueChanged(int value);
    //播放方式
    void on_btn_PlayMode_clicked();
    //历史记录
    void on_btn_CurrentMusic_clicked();
    //本地歌曲
    void on_btn_LocalMusic_clicked();
    //我喜欢
    void on_btn_LikeMusic_clicked();
    //收藏(喜欢)
    void on_love_Button_clicked();
    //关于
    void on_btn_About_clicked();
    //换肤
    void on_btn_Skin_clicked();
    //最小化
    void on_btn_Min_clicked();
    //最大化
    void on_btn_Max_clicked();
    //关闭
    void on_btn_Close_clicked();
    //歌词解析
    void on_btn_lyrics_clicked();

private:
    Ui::mainWidget *ui;

    QMenu *menu_changeSkin;
    QList<QUrl> playList;
    QList<QUrl> playList1;
    QAudioOutput *audiOutput;
    QMediaPlayer *mediaPalyer;

    int curPlayIndex = 0;
    QString durationTime;
    QString positionTime;
    int flag_mode;
    QMap<int,QString> *map;

    Lyrics lyrics;
    int lyricsID = 0;
    QString playPath;
    qint64 postime = 0;
    int flag_color;
};
#endif // MAINWIDGET_H
