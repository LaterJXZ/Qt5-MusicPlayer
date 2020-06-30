#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QMenu>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include "mylrc.h"
#include <QString>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->searchBox->setPlaceholderText(tr("请输入歌曲名称或歌手"));
    ui->verticalSlider->hide();
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    tableLrc = new myLrc(this);
    tableLrc->hide();
    currentFileName.push_back("D:/");
    currentFileName.push_back("D:/");
    currentFileName.push_back("D:/");
    currentFileName.push_back("D:/");
    init();
    init_icons();
    init_actions();
    init_lists();
    init_connects();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    player = new QMediaPlayer(this);
    playlistPlay = new QMediaPlaylist(this);
    playlistPlay->setPlaybackMode(QMediaPlaylist::Loop);
    playlistLocal = new QMediaPlaylist(this);
    playlistLocal->setPlaybackMode(QMediaPlaylist::Loop);
    playlistLike = new QMediaPlaylist(this);
    playlistLike->setPlaybackMode(QMediaPlaylist::Loop);
    playlistHistory = new QMediaPlaylist(this);
    playlistHistory->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlistLocal);
}

void Widget::init_icons()
{
    ui->playOrPauseBtn->setStyleSheet(pauseStyle());
    ui->playModeBtn->setStyleSheet(loopStyle());
    ui->markLikeBtn->setStyleSheet(markLikeStyle());
}

void Widget::init_actions()
{
    actionAddToLikeList = new QAction(this);
    actionAddToLikeList->setText(tr("添加到喜爱列表"));
    actionAddToPlayList = new QAction(this);
    actionAddToPlayList->setText(tr("添加到播放列表"));

    actionAddMusic = new QAction(this);

    actionPrev = new QAction(this);
    actionPrev->setIcon(QIcon(":/icon/images/icon/pre1.png"));
    actionPrev->setText(tr("上一曲"));
    actionNext = new QAction(this);
    actionNext->setIcon(QIcon(":/icon/images/icon/next1.png"));
    actionNext->setText(tr("下一曲"));
    actionPlay = new QAction(this);
    actionPlay->setIcon(QIcon(":/icon/images/icon/zangting.png"));
    actionPlay->setText(tr("播放"));
    actionPause = new QAction(this);
    actionPause->setIcon(QIcon(":/icon/images/icon/bf2.png"));
    actionPause->setText(tr("暂停"));
    actionPlayMode = new QAction(this);
    actionPlayMode->setText(tr("播放模式"));
    actionLoop = new QAction(this);
    actionLoop->setIcon(QIcon(":/icon/images/icon/loopall.png"));
    actionLoop->setText(tr("全部循环"));
    actionLoopOne = new QAction(this);
    actionLoopOne->setIcon(QIcon(":/icon/images/icon/loopone.png"));
    actionLoopOne->setText(tr("单曲循环"));
    actionSequence = new QAction(this);
    actionSequence->setText(tr("顺序播放"));
    actionRandom = new QAction(this);
    actionRandom->setIcon(QIcon(":/icon/images/icon/random1.png"));
    actionRandom->setText(tr("随机播放"));
    QMenu *playModeMenu = new QMenu(this);
    playModeMenu->addAction(actionLoop);
    playModeMenu->addAction(actionLoopOne);
    playModeMenu->addAction(actionSequence);
    playModeMenu->addAction(actionRandom);
    actionPlayMode->setMenu(playModeMenu);

}

void Widget::init_lists()
{
    QSqlQuery query;
    modelLocal = new QSqlTableModel(this);
    modelLocal->setTable("localMusic");
    modelLocal->select();
    modelLike = new QSqlTableModel(this);
    modelLike->setTable("likeMusic");
    modelLike->select();
    modelPlay = new QSqlTableModel(this);
    modelPlay->setTable("playMusic");
    modelPlay->select();
    query.exec("select * from localMusic");
    while (query.next()) {
        QString Name = query.value(1).toString();
        QString filePathName = query.value(2).toString();
        if(!Name.isEmpty() && !filePathName.isEmpty()){
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(":/icon/images/icon/listitem.png"));
            item->setText(Name);
            ui->listLocal->addItem(item);
            playlistLocal->addMedia(QUrl::fromLocalFile(filePathName));
            flagPlayed.push_back(0);
            filePath.push_back(filePathName);
        }
    }
    query.exec("select * from likeMusic");
    while (query.next()) {
        QString Name = query.value(1).toString();
        QString filePathName = query.value(2).toString();
        if(!Name.isEmpty() && !filePathName.isEmpty()){
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(":/icon/images/icon/likelistitem.png"));
            item->setText(Name);
            ui->listLike->addItem(item);
            playlistLike->addMedia(QUrl::fromLocalFile(filePathName));
        }
    }
    query.exec("select * from playMusic");
    while (query.next()) {
        QString Name = query.value(1).toString();
        QString filePathName = query.value(2).toString();
        if(!Name.isEmpty() && !filePathName.isEmpty()){
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(":/icon/images/icon/listitem.png"));
            item->setText(Name);
            ui->listPlay->addItem(item);
            playlistPlay->addMedia(QUrl::fromLocalFile(filePathName));
        }
    }
    player->setPlaylist(playlistLocal);
    ui->listHistory->clear();
    listGetFocus();
}

void Widget::init_connects()
{
    connect(ui->addMusicBtn,&QPushButton::clicked,this,&Widget::slotAddToLocal);
    connect(ui->prevBtn,&QPushButton::clicked,this,&Widget::slotPrevMusic);
    connect(ui->nextBtn,&QPushButton::clicked,this,&Widget::slotNextMusic);

    connect(actionPrev,&QAction::triggered,this,&Widget::slotPrevMusic);
    connect(actionNext,&QAction::triggered,this,&Widget::slotNextMusic);
    connect(actionPlay,&QAction::triggered,this,&Widget::slotPlay);
    connect(actionPause,&QAction::triggered,this,&Widget::slotPause);
    connect(actionRandom,&QAction::triggered,this,&Widget::slotRandom);
    connect(actionLoop,&QAction::triggered,this,&Widget::slotLoop);
    connect(actionLoopOne,&QAction::triggered,this,&Widget::slotLoopOne);
    connect(actionSequence,&QAction::triggered,this,&Widget::slotSequence);

    connect(player,&QMediaPlayer::metaDataAvailableChanged,this,&Widget::metaDataAvailableChanged);
    connect(player,&QMediaPlayer::positionChanged,this,&Widget::musicPositionChanged);
    connect(player,&QMediaPlayer::durationChanged,this,&Widget::musicDurationChanged);
    connect(ui->horizontalSlider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);
    connect(ui->horizontalSlider,&mySlider::clickChangePosition,player,&QMediaPlayer::setPosition);
    connect(ui->verticalSlider,&QSlider::sliderMoved,player,&QMediaPlayer::setVolume);
    connect(ui->verticalSlider,&QSlider::valueChanged,this,&Widget::slotVolumeChanged);
}

QString Widget::playStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/play.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/play-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/play.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}";
}

QString Widget::pauseStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/pase.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/pase-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/pase.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::markLikeStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/Ilike.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/Ilike-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/Ilike.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::unmarkLikeStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/Ilike1.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/Ilike1-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/Ilike1.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::randomStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/play-random.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/play-random-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/play-random.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::loopStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/loop.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/loop-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/loop.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::loopOneStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/loop-one.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/loop-one-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/loop-one.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::sequenceStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/loop-list.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/loop-list-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/loop-list.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::volumeStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/music-voice.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/music-voice-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/music-voice.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

QString Widget::muteStyle()
{
    return "QPushButton{"
            "background-image:url(:/icon/images/icon/none-music.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "border:none;"
            "}"
           "QPushButton:hover{"
            "background-image:url(:/icon-hover/images/icon-hover/none-music-hover.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
            "}"
           "QPushButton:pressed{"
            "background-image:url(:/icon/images/icon/none-music.png);"
            "background-repeat:no-repeat;"
            "background-position:center center;"
           "}";
}

void Widget::listGetFocus()
{
    if(player->playlist() == playlistLocal){
        ui->listLocal->setFocus();
    }
    else if(player->playlist() == playlistLike){
        ui->listLike->setFocus();
    }
    else if(player->playlist() == playlistPlay){
        ui->listPlay->setFocus();
    }
    else if(player->playlist() == playlistHistory){
        ui->listHistory->setFocus();
    }
}

QString Widget::Time(qint64 duration)
{
    qint64 seconds = duration / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2").arg(minutes,2,10,QLatin1Char('0')).arg(seconds,2,10,QLatin1Char('0'));
}

void Widget::readLrc(const QString &filePath)
{
    lrc.clear();
    if(filePath.isEmpty())
        return;
    QString Path = filePath;
    QString lrcPath = Path.mid(8).remove(Path.right(3)) + "lrc";
    QFile file(lrcPath);
    if(!file.open(QIODevice::ReadOnly)){
        ui->welcomLabel->setText(tr("未找到歌词文件"));
        return;
    }
    QTextStream in(&file);
    QString allText = in.readAll();
    file.close();
    QStringList lines = allText.split("\n");
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    for(QString oneLine : lines){
        QString temp = oneLine;
        temp.replace(rx,"");
        int pos = rx.indexIn(oneLine,0);
        if(pos != -1) {
            QString time = rx.cap(0);
            int minute = time.mid(1,2).toInt();
            int second = time.mid(4,2).toInt();
            int millisec = time.mid(7,2).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 + millisec * 10;
            lrc.insert(totalTime,temp);
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor);
        offset = event->globalPos() - pos();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        QPoint temp;
        temp = event->globalPos() - offset;
        move(temp);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    menu = new QMenu(this);
    menu->addAction(actionPlay);
    menu->addAction(actionPause);
    menu->addAction(actionPrev);
    menu->addAction(actionNext);
    menu->addAction(actionPlayMode);
    menu->exec(QCursor::pos());
}


void Widget::on_quitBtn_clicked()
{
    qApp->quit();
}

void Widget::on_localListBtn_clicked()
{
    ui->listLocal->setSelectionMode(QListWidget::SingleSelection);
    ui->stackedWidget->setCurrentIndex(0);
    listGetFocus();
}

void Widget::on_likeListBtn_clicked()
{
    ui->listLike->setSelectionMode(QListWidget::SingleSelection);
    ui->stackedWidget->setCurrentIndex(1);
    listGetFocus();
}

void Widget::on_listBtn_clicked()
{
    ui->listPlay->setSelectionMode(QListWidget::SingleSelection);
    ui->stackedWidget->setCurrentIndex(2);
    listGetFocus();
}

void Widget::on_recentlyListBtn_clicked()
{
    ui->listHistory->setSelectionMode(QListWidget::SingleSelection);
    ui->stackedWidget->setCurrentIndex(3);
    listGetFocus();
}

void Widget::slotPrevMusic()
{
    if(ui->listLocal->count() == 0 && ui->listLike->count() == 0 && ui->listPlay->count() == 0 && ui->listHistory->count() == 0)
        return;
    listGetFocus();
    if(player->playlist() == playlistLocal){
        ui->stackedWidget->setCurrentIndex(0);
        int prevIndex;
        if(playlistLocal->playbackMode() == QMediaPlaylist::Random){
            prevIndex = qrand() % (playlistLocal->mediaCount());
        }
        else{
            if(playlistLocal->currentIndex() == 0)
                prevIndex = playlistLocal->mediaCount() - 1;
            else
                prevIndex = playlistLocal->currentIndex() - 1;
        }
        ui->listLocal->item(prevIndex)->setSelected(true);
        playlistLocal->setCurrentIndex(prevIndex);
        ui->listLocal->scrollToItem(ui->listLocal->item(prevIndex));
        if(flagPlayed[prevIndex] == 0){
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/images/icon/nextbo.png"),ui->listLocal->item(prevIndex)->text());
            ui->listHistory->addItem(item);
            playlistHistory->addMedia(QUrl::fromLocalFile(filePath[prevIndex]));
        }
        flagPlayed[prevIndex]++;
    }
}

void Widget::slotNextMusic()
{
    if(ui->listLocal->count() == 0 && ui->listLike->count() == 0 && ui->listPlay->count() == 0 && ui->listHistory->count() == 0)
        return;
    listGetFocus();
    if(player->playlist() == playlistLocal){
        ui->stackedWidget->setCurrentIndex(0);
        int nextIndex;
        if(playlistLocal->playbackMode() == QMediaPlaylist::Random){
            nextIndex = qrand() % (playlistLocal->mediaCount());
        }
        else{
            if(playlistLocal->currentIndex() == playlistLocal->mediaCount() - 1)
                nextIndex = 0;
            else
                nextIndex = playlistLocal->currentIndex() + 1;
        }
        ui->listLocal->item(nextIndex)->setSelected(true);
        playlistLocal->setCurrentIndex(nextIndex);
        ui->listLocal->scrollToItem(ui->listLocal->item(nextIndex));
        if(flagPlayed[nextIndex] == 0){
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/images/icon/nextbo.png"),ui->listLocal->item(nextIndex)->text());
            ui->listHistory->addItem(item);
            playlistHistory->addMedia(QUrl::fromLocalFile(filePath[nextIndex]));
        }
        flagPlayed[nextIndex]++;
    }
}

void Widget::slotPlay()
{
    if(ui->listLocal->count() == 0 && ui->listLike->count() == 0 && ui->listPlay->count() == 0 && ui->listHistory->count() == 0)
        return;
    listGetFocus();
    ui->playOrPauseBtn->setStyleSheet(playStyle());
    ui->playOrPauseBtn->setToolTip(tr("暂停"));
    player->play();
}

void Widget::slotPause()
{
    if(ui->listLocal->count() == 0 && ui->listLike->count() == 0 && ui->listPlay->count() == 0 && ui->listHistory->count() == 0)
        return;
    listGetFocus();
    ui->playOrPauseBtn->setStyleSheet(pauseStyle());
    ui->playOrPauseBtn->setToolTip(tr("播放"));
    player->pause();
}

void Widget::slotRandom()
{
    ui->playModeBtn->setStyleSheet(randomStyle());
    ui->playModeBtn->setToolTip(tr("随机播放"));
    playlistLocal->setPlaybackMode(QMediaPlaylist::Random);
    playlistLike->setPlaybackMode(QMediaPlaylist::Random);
    playlistPlay->setPlaybackMode(QMediaPlaylist::Random);
    playlistHistory->setPlaybackMode(QMediaPlaylist::Random);
}

void Widget::slotLoop()
{
    ui->playModeBtn->setStyleSheet(loopStyle());
    ui->playModeBtn->setToolTip(tr("全部循环"));
    playlistLocal->setPlaybackMode(QMediaPlaylist::Loop);
    playlistLike->setPlaybackMode(QMediaPlaylist::Loop);
    playlistPlay->setPlaybackMode(QMediaPlaylist::Loop);
    playlistHistory->setPlaybackMode(QMediaPlaylist::Loop);
}

void Widget::slotLoopOne()
{
    ui->playModeBtn->setStyleSheet(loopOneStyle());
    ui->playModeBtn->setToolTip(tr("单曲循环"));
    playlistLocal->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    playlistLike->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    playlistPlay->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    playlistHistory->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}

void Widget::slotSequence()
{
    ui->playModeBtn->setStyleSheet(sequenceStyle());
    ui->playModeBtn->setToolTip(tr("顺序播放"));
    playlistLocal->setPlaybackMode(QMediaPlaylist::Sequential);
    playlistLike->setPlaybackMode(QMediaPlaylist::Sequential);
    playlistPlay->setPlaybackMode(QMediaPlaylist::Sequential);
    playlistHistory->setPlaybackMode(QMediaPlaylist::Sequential);
}

void Widget::metaDataAvailableChanged(bool available)
{
    if(available){
        QString title = player->metaData("Title").toString();
        QString author = player->metaData("Author").toString();
        ui->musicInfo->setText(title + "-" + author);
        //qDebug()<<player->currentMedia().canonicalUrl().toString();
        readLrc(player->currentMedia().canonicalUrl().toString());
    }
}

void Widget::musicPositionChanged(qint64 position)
{
    ui->horizontalSlider->setValue(position);
    ui->timeProgress->setText(Time(position) + "/" + Time(player->duration()));
    //歌词显示
    if(!lrc.isEmpty()){
        qint64 previous = 0, later = 0;
        foreach(qint64 value,lrc.keys()){
            if(position >= value)
                previous = value;
            else{
                later = value;
                break;
            }
        }
        if(later == 0)
            later = player->duration();
        QString curLrc = lrc.value(previous);
        if(curLrc != ui->welcomLabel->text()){
            ui->welcomLabel->setText(curLrc);
        }
    }
    else{
        ui->welcomLabel->setText(tr("未找到歌词文件"));
    }
}

void Widget::musicDurationChanged(qint64 duration)
{
    ui->horizontalSlider->setRange(0,duration);
    ui->horizontalSlider->setEnabled(duration > 0);
    ui->horizontalSlider->setPageStep(duration / 10);
}

void Widget::slotAddToLocal()
{
    ui->stackedWidget->setCurrentIndex(0);
    QSqlQuery query;
    query.exec("select * from localMusic");
    QStringList list = QFileDialog::getOpenFileNames(this,tr("选择歌曲文件"),currentFileName[0],tr("音频文件(* mp3)"));
    if(!list.isEmpty()){
        QProgressDialog dialog(tr("正在添加..."),tr("取消"),0,list.size() - 1,this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setWindowTitle(tr("添加歌曲"));
        dialog.show();
        for (int i = 0; i < list.size(); ++i) {
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(":/icon/images/icon/listitem.png"));
            QString path = QDir::toNativeSeparators(list.at(i));
            if(dialog.wasCanceled())
                dialog.close();
            else
                dialog.setValue(i);
            if(i == list.size() - 1)
                dialog.close();
            if(!path.isEmpty()){
                QString path1 = path;
                QString name = path.split("\\").last();
                currentFileName[0] = path1.remove(name);
                name.remove(QString(".mp3"));
                item->setText(name);
                flagPlayed.push_back(0);
                ui->listLocal->addItem(item);
                int x = qrand() % 100000;
                query.exec(QString("insert into localMusic values(%1,'%2','%3',%4)").arg(x).arg(name).arg(path).arg(0));
                filePath.push_back(path);
                playlistLocal->addMedia(QUrl::fromLocalFile(path));
            }
        }
    }
}

void Widget::on_playOrPauseBtn_clicked()
{
    if(ui->listLocal->count() == 0 && ui->listLike->count() == 0 && ui->listPlay->count() == 0 && ui->listHistory->count() == 0)
        return;
    listGetFocus();
    if(player->state() == QMediaPlayer::PlayingState){
        ui->playOrPauseBtn->setStyleSheet(pauseStyle());
        ui->playOrPauseBtn->setToolTip(tr("播放"));
        player->pause();
    }
    else{
        ui->playOrPauseBtn->setStyleSheet(playStyle());
        ui->playOrPauseBtn->setToolTip(tr("暂停"));
        player->play();
    }
}

void Widget::on_playModeBtn_clicked()
{
    listGetFocus();
    if(playlistLocal->playbackMode() == QMediaPlaylist::Loop){
        ui->playModeBtn->setStyleSheet(loopOneStyle());
        ui->playModeBtn->setToolTip(tr("单曲循环"));
        playlistLocal->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        playlistLike->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        playlistPlay->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        playlistHistory->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if(playlistLocal->playbackMode() == QMediaPlaylist::CurrentItemInLoop){
        ui->playModeBtn->setStyleSheet(randomStyle());
        ui->playModeBtn->setToolTip(tr("随机播放"));
        playlistLocal->setPlaybackMode(QMediaPlaylist::Random);
        playlistLike->setPlaybackMode(QMediaPlaylist::Random);
        playlistPlay->setPlaybackMode(QMediaPlaylist::Random);
        playlistHistory->setPlaybackMode(QMediaPlaylist::Random);
    }
    else if(playlistLocal->playbackMode() == QMediaPlaylist::Random){
        ui->playModeBtn->setStyleSheet(sequenceStyle());
        ui->playModeBtn->setToolTip(tr("顺序播放"));
        playlistLocal->setPlaybackMode(QMediaPlaylist::Sequential);
        playlistLike->setPlaybackMode(QMediaPlaylist::Sequential);
        playlistPlay->setPlaybackMode(QMediaPlaylist::Sequential);
        playlistHistory->setPlaybackMode(QMediaPlaylist::Sequential);
    }
    else if(playlistLocal->playbackMode() == QMediaPlaylist::Sequential){
        ui->playModeBtn->setStyleSheet(loopStyle());
        ui->playModeBtn->setToolTip(tr("全部循环"));
        playlistLocal->setPlaybackMode(QMediaPlaylist::Loop);
        playlistLike->setPlaybackMode(QMediaPlaylist::Loop);
        playlistPlay->setPlaybackMode(QMediaPlaylist::Loop);
        playlistHistory->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void Widget::on_listLocal_doubleClicked(const QModelIndex &index)
{
    if(ui->listLocal->selectionMode() == QListWidget::SingleSelection){
        player->setPlaylist(playlistLocal);
        int currentIndex = index.row();
        playlistLocal->setCurrentIndex(currentIndex);
        player->play();
        if(flagPlayed[currentIndex] == 0){
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/images/icon/nextbo.png"),ui->listLocal->item(currentIndex)->text());
            ui->listHistory->addItem(item);
            playlistHistory->addMedia(QUrl::fromLocalFile(filePath[currentIndex]));
        }
        flagPlayed[currentIndex]++;
    }
}

void Widget::on_volumeBtn_clicked()
{
    if(ui->verticalSlider->isVisible()){
        ui->verticalSlider->hide();
    }
    else{
        ui->verticalSlider->show();
    }
}

void Widget::slotVolumeChanged()
{
    if(ui->verticalSlider->value() == 0)
        ui->volumeBtn->setStyleSheet(muteStyle());
    else
        ui->volumeBtn->setStyleSheet(volumeStyle());
}
