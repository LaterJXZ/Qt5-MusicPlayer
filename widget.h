#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
class QMediaPlayer;
class QMediaPlaylist;
class QSqlTableModel;
class QMenu;
class QListWidgetItem;
class myLrc;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    //初始化
    void init();
    void init_icons();
    void init_actions();
    void init_lists();
    void init_connects();
    //样式表相关
    QString playStyle();
    QString pauseStyle();
    QString markLikeStyle();
    QString unmarkLikeStyle();
    QString randomStyle();
    QString loopStyle();
    QString loopOneStyle();
    QString sequenceStyle();
    QString volumeStyle();
    QString muteStyle();

    void listGetFocus();
    //歌曲时间转换
    static QString Time(qint64 duration);

    void readLrc(const QString &filePath);

protected:
    //拖动窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_quitBtn_clicked();  //按下退出按钮
    void on_localListBtn_clicked(); //点击本地列表
    void on_likeListBtn_clicked();  //点击喜爱列表
    void on_listBtn_clicked();
    void on_recentlyListBtn_clicked();  //点击最近播放
    //右键菜单里面的动作需要触发的槽
    void slotPrevMusic();   //上一曲
    void slotNextMusic();   //下一曲
    void slotPlay();        //播放
    void slotPause();       //暂停
    void slotRandom();      //随机播放
    void slotLoop();        //全部循环
    void slotLoopOne();     //单曲循环
    void slotSequence();    //顺序播放
    //歌曲变化相关
    void metaDataAvailableChanged(bool available);  //歌曲变了
    void musicPositionChanged(qint64 position);     //播放进度改变
    void musicDurationChanged(qint64 duration);     //歌曲时长改变
    void slotAddToLocal();  //从本地导入音乐文件
    void on_playOrPauseBtn_clicked();   //播放/暂停按钮
    void on_playModeBtn_clicked();      //播放模式按钮

    void on_listLocal_doubleClicked(const QModelIndex &index);  //双击播放本地列表

    void on_volumeBtn_clicked();    //音量调节条显示与隐藏
    void slotVolumeChanged();       //静音与否图标显示格式

private:
    Ui::Widget *ui;
    //播放列表和播放器
    QMediaPlaylist *playlistPlay;
    QMediaPlaylist *playlistLike;
    QMediaPlaylist *playlistHistory;
    QMediaPlaylist *playlistLocal;
    QMediaPlayer *player;
    QSqlTableModel *modelLocal;
    QSqlTableModel *modelLike;
    QSqlTableModel *modelPlay;
    //鼠标与窗口位置差值
    QPoint offset;
    //动作
    QAction *actionAddToLikeList;   //添加到喜欢列表
    QAction *actionAddToPlayList;   //添加到播放列表
    QAction *actionAddMusic;        //从本地导入音乐
    QAction *actionPrev;            //上一曲
    QAction *actionNext;            //下一曲
    QAction *actionPause;           //暂停
    QAction *actionPlay;            //播放
    QAction *actionPlayMode;        //播放模式
    QAction *actionRandom;          //随机播放
    QAction *actionLoop;            //全部循环
    QAction *actionLoopOne;         //单曲循环
    QAction *actionSequence;        //顺序播放
    QMenu *menu;                    //右键菜单

    myLrc *tableLrc;    //桌面歌词
    QMap<qint64,QString> lrc;

    QVector<QString> currentFileName;//各个列表的当前文件
    QVector<int> flagPlayed;  //标记每首歌曲是否播放过
    QVector<QString> filePath;//储存每首歌和文件路径
};
#endif // WIDGET_H
