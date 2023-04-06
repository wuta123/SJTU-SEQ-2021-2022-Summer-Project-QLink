#include "singlemodet.h"
#include "ui_singlemodet.h"
#include "clock.h"
#include <QTextBrowser>
#include <cmath>
#include <QQueue>
#include <QPen>
#include <QColor>
/*本类为游戏答辩模式的主窗体，所有归属于singleModeT的类和变量都在该类中得到妥善的处理*/
SingleModeT::SingleModeT(int x, int y, int firstTime):ui(new Ui::SingleModeT)
{
    /*构造函数主要完成ui的配置，graphicsView为本程序中所有graphicsItem的活动空间
    */
    setWindowTitle(QStringLiteral("连连看"));
    setWindowIcon(QIcon(":/icon/1x1/darkzaitzev/chameleon-glyph.svg"));
    ui->setupUi(this);
    ui->clock->restart();
    connect(ui->clock, &Clock::isPaused, this, [=](){
        pause();
    });
    ui->clock->resize(ui->clock->size()*2);
    ui->clock->move(ui->clock->x()-20,ui->clock->y()-8);
    ui->pauseWidget->hide();
    connect(ui->pauseWidget, &PauseWidget::resume, this, &SingleModeT::startProgress);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    background = new QPixmap(":/test/test/backGroundT.png");
    QGraphicsPixmapItem *backGround = new QGraphicsPixmapItem(*background);
    scene = new QGraphicsScene(0,0,800,650);
    scene->addItem(backGround);
    connect(ui->matrixButton, &QPushButton::clicked, this, &SingleModeT::showMatrix);
    ui->graphicsView->setScene(scene);
    ui->statusLabel->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    ui->statusLabel->setFixedSize(ui->statusLabel->size());
    ui->statusLabel->setWordWrap(true);
    Blocksnum = x*y;
    step = player.step;
    ui->marksLabel->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    ui->marksLabel->setAlignment(Qt::AlignCenter);
    ui->marksLabel->setText("0");
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    ui->statusLabel->setPalette(pa);
    connect(ui->hintButton, &QPushButton::clicked, this, &SingleModeT::findTheSame);
    scene->addItem(&player);
    player.setPos(0,0);
    setRules();
    FlashTimer = new QTimer;
    FlashTimer->setInterval(5000);
    connect(FlashTimer, &QTimer::timeout, this, [=](){
        FlashTimer->stop();
        ableToFlash = false;
    });//如果本次游戏是通过文件加载得来的，运行loadGame
    if(firstTime) setMap(x, y, firstTime);
    else loadGame();

}

SingleModeT::~SingleModeT()
{
    delete ui;
}

void SingleModeT::showMatrix(){//一个小功能，用来在程序中随时查看当前页面上的矩阵状况
    QWidget *matrixWidget = new QWidget(nullptr);
    matrixWidget->resize(600,600);
    QTextBrowser *browser = new QTextBrowser(matrixWidget);
    browser->resize(600,600);
    browser->move(0,0);
    for(int i = 0; i < Y+2; i++){
        QString literal;
        for(int j = 0; j < X+2; j++){
            literal.append(QString::number(matrix[j][i]));
        }
        literal.append("/n");
        browser->append(literal);
    }
    matrixWidget->show();
    browser->show();
}

void SingleModeT::pause()
{
    //暂停，paused变量表示处于暂停状态*/
    for(int i = 0; i < X*Y; i++){
        blocks[i]->paused = true;
    }
    paused = true;
    ui->clock->pause();
    ui->pauseWidget->show();
}

void SingleModeT::startProgress()
{
    /*从暂停中恢复后会调用此函数，将场景中的所有元素恢复原本的运行状态*/
    for(int i = 0; i < X*Y; i++){
        blocks[i]->paused = false;
    }
    paused = false;
    ui->clock->restart();
}

void SingleModeT::paintEvent(QPaintEvent *event)
{
    /*留空，避免update对界面进行误操作*/
}

void SingleModeT::keyPressEvent(QKeyEvent *event)
{
    /*重点程序，键盘响应函数，分别对按键为上下左右键、以及z键进行处理
        设置currentPOS保存玩家现在的位置，而newPos保存玩家视图移动到的位置
        先进行移动，然后利用Qt的碰撞检测来判断移动是否合法，如果合法，保留新的pos
        不合法则回到原pos的位置
    */
    if(paused) return;
    QPoint *currentPos = new QPoint(player.x(),player.y());
    QPoint *newPos = new QPoint(player.x(),player.y());
    if(event -> key() == Qt::Key_D){
            if(player.x()+step+player.width <= 800) newPos = new QPoint(player.x()+step,player.y());
            else newPos = new QPoint(800-player.width,player.y());
            player.setCurrentDirection(1);
            player.update();
    }
    if(event -> key() == Qt::Key_A){
            if(player.x() - step >= 0) newPos = new QPoint(player.x()-step,player.y());
            else newPos = new QPoint(0,player.y());
            player.setCurrentDirection(2);
            player.update();
    }
    if(event -> key() == Qt::Key_W){
            if(player.y()-step >= 0) newPos = new QPoint(player.x(), player.y()-step);
            else newPos = new QPoint(player.x(), 0);
            player.setCurrentDirection(3);
            player.update();
    }
    if(event -> key() == Qt::Key_S){
            if(player.y()+player.height + step<= 650) newPos = new QPoint(player.x(), player.y()+step);
            else newPos = new QPoint(player.x(),650-player.height);
            player.setCurrentDirection(4);
            player.update();
    }
    player.setPos(*newPos);

    if(judgeCollide())
        player.setPos(*currentPos);
    return;
}

void SingleModeT::judgeEliminate(int code)
{
    /*这里主要判断方块和玩家之间的距离是否控制在九宫格的范围内，通过二者的pos函数可知。
     * 然后判断两种方块是否相等，如果不相等就直接返回，相等的话就调用ableTolink函数来判断
     * 路线的合法性*/
    if(abs((player.x()+15)-(blocks[code]->x()+23)) >= 55 || abs((player.y()+24)-(blocks[code]->y()+23)) >= 55)
            return;
    else{
        if(carriedState == 999){
            carriedState = blocks[code]->checkState();
            carriedCode = code;
            blocks[code]->beenLighted();
            ui->statusLabel->setText(u8"激活了方块！");
            return;
        }else if(carriedState != 999){
            if(carriedCode == code){
                blocks[carriedCode]->shadow();
                carriedCode = 999;
                carriedState = 999;
                ui->statusLabel->setText(u8"你点击了同一个方块！");
                return;
            }
            if(blocks[carriedCode]->checkState() != blocks[code]->checkState()){
                blocks[carriedCode]->shadow();
                carriedCode = 999;
                carriedState =999;
                ui->statusLabel->setText(u8"方块种类不一样！");
                return;
            }else if(blocks[carriedCode]->checkState() == blocks[code]->checkState()){
                if(ableToLink(carriedCode,code)){
                    marks += 15;
                    ui->marksLabel->setText(QString::number(marks));
                    matrix[blocks[carriedCode]->getX()][blocks[carriedCode]->getY()] = 0;
                    matrix[blocks[code]->getX()][blocks[code]->getY()] = 0;
                    Blocksnum -= 2;
                    if(Blocksnum == 0) {
                        showVictory();
                    }
                    ui->statusLabel->setText(u8"成功合成！");
                    drawLine();
                    blocks[carriedCode]->beenCarried();
                    blocks[code]->beenCarried();
                    blocks[carriedCode]->beenMerged();
                    blocks[carriedCode]->hide();
                    blocks[code]->hide();
                    carriedCode = 999;
                    carriedState =999;
                    return;
                }
                else{
                    ui->statusLabel->setText(u8"这一对方块不能连通！");
                    blocks[carriedCode]->shadow();
                    carriedCode = 999;
                    carriedState =999;
                }
            }
        }
    }
}

bool SingleModeT::ableToLink(int carriedCode, int code)
{
    /*主函数，通过调用多个分函数来完成自己的功能*/
    int x1 = blocks[carriedCode]->getX();
    int y1 = blocks[carriedCode]->getY();
    int x2 = blocks[code]->getX();
    int y2 = blocks[code]->getY();
    //垂直链接
    bool success = false;
    success = horizontal(x1, y1, x2, y2);
    if(success) return true;
    success = vertical(x1, y1, x2, y2);
    if(success) return true;
    //拐弯链接
    success = oneTurn(x1,y1,x2,y2);
    if(success) return true;
    success = twoTurn(x1,y1,x2,y2);
    if(success) return true;
    return false;
}

bool SingleModeT::vertical(int x1, int y1, int x2, int y2)
{
    /*判断是否可以通过直线垂直相连*/
    if(x1 != x2) return false;
    int y_min = min(y1,y2);
    int y_max = max(y1,y2);
    for(int i = y_min+1; i <= y_max; i++){
            if(i == y_max){
                keypoints.clear();
                keypoints.push_back(physicalPoint(x1,y1));
                keypoints.push_back(physicalPoint(x2,y2));
                return true;
            }
            if(isBlock(x1,i)){
                break;
            }
            if(i == y_max-1){
                keypoints.clear();
                keypoints.push_back(physicalPoint(x1,y1));
                keypoints.push_back(physicalPoint(x2,y2));
                return true;
            }
        }
    return false;
}

bool SingleModeT::horizontal(int x1, int y1, int x2, int y2)
{
    /*判断是否可以直接水平相连*/
    if(y1 != y2) return false;
    int x_min = min(x1,x2);
    int x_max = max(x1,x2);
    for(int i = x_min+1; i <= x_max; i++){
            if(i == x_max){
                keypoints.clear();
                keypoints.push_back(physicalPoint(x1,y1));
                keypoints.push_back(physicalPoint(x2,y2));
                return true;
            }
            if(isBlock(i,y1)){
                break;
            }
            if(i == x_max-1){
                keypoints.clear();
                keypoints.push_back(physicalPoint(x1,y1));
                keypoints.push_back(physicalPoint(x2,y2));
                return true;
            }
        }
    return false;
}

bool SingleModeT::oneTurn(int x1, int y1, int x2, int y2)
{
    //拐一个弯，两种情况，先水平链接到x2,y1，然后垂直链接到x2,y2
    //或者先垂直链接到x1,y2,然后水平链接到x2,y2
    //其实也是通过调用horizontal和vertical来达到目的
    if(!isBlock(x2,y1) && horizontal(x1,y1,x2,y1) && vertical(x2,y1,x2,y2)){
        keypoints.clear();
        keypoints.push_back(physicalPoint(x1,y1));
        keypoints.push_back(physicalPoint(x2,y1));
        keypoints.push_back(physicalPoint(x2,y2));
        return true;
    }
    if(!isBlock(x1,y2) && vertical(x1,y1,x1,y2) && horizontal(x1,y2,x2,y2)){
        keypoints.clear();
        keypoints.push_back(physicalPoint(x1,y1));
        keypoints.push_back(physicalPoint(x1,y2));
        keypoints.push_back(physicalPoint(x2,y2));
        return true;
    }
    return false;
}

bool SingleModeT::twoTurn(int x1, int y1, int x2, int y2)
{
    /*利用前面已经完成的oneTurn和Horizontal、Vertical函数来完成拐两个
     * 弯的功能*/
    //1.向左扫描*/
    for(int i = x1-1; i >= 0; i--){
        if(isBlock(i,y2)) break;
        if(horizontal(x2,y2,i,y2) && oneTurn(i,y2,x1,y1)){
            keypoints.push_front(physicalPoint(x2,y2));
            return true;
        }
    }
    //2.向右扫描
    for(int i = x1+1; i <= X+1; i++){
        if(isBlock(i,y2)) break;
        if(horizontal(x2,y2,i,y2) && oneTurn(i,y2,x1,y1)){
            keypoints.push_front(physicalPoint(x2,y2));
            return true;
        }
    }

    //水平扫描失败，垂直扫描
    //1.向上扫描*/
    for(int i = y1-1; i >= 0; i--){
        if(isBlock(x1,i)) break;
        if(vertical(x1,y1,x1,i) && oneTurn(x1,i,x2,y2)){
            keypoints.push_front(physicalPoint(x1,y1));
            return true;
        }
    }//2.向下扫描
    for(int i = y1+1; i <= Y+1; i++){
        if(isBlock(x1,i)) break;
        if(vertical(x1,y1,x1,i) && oneTurn(x1,i,x2,y2)){
            keypoints.push_front(physicalPoint(x1,y1));
            return true;
        }
    }
    return false;
}



void SingleModeT::drawLine()
{
    /*沿着在keypoints内保存好的起点、终点、拐点信息，在graphicsScene上添加
     * 直线item，并创建tmp作为定时器，在1s之后将item和tmp删除，释放空间*/
    QPointF point1 = keypoints.front();
    keypoints.pop_front();
    QPointF point2 = keypoints.front();
    keypoints.pop_front();
    QGraphicsLineItem *lineItem = new QGraphicsLineItem;
    QTimer *tmp = new QTimer;
    tmp -> start(1000);
    connect(tmp, &QTimer::timeout,[=](){
        tmp -> stop();
        delete lineItem;
        delete tmp;
    });
    lineItem->setLine(point1.x(),point1.y(),point2.x(),point2.y());
    scene->addItem(lineItem);
    while(!keypoints.isEmpty()){
        point1 = point2;
        point2 = keypoints.front();
        keypoints.pop_front();
        QGraphicsLineItem *lineItem = new QGraphicsLineItem;
        lineItem->setLine(point1.x(),point1.y(),point2.x(),point2.y());
        scene->addItem(lineItem);
        QTimer *tmp = new QTimer;
        tmp -> start(1000);
        connect(tmp, &QTimer::timeout,[=](){
            tmp -> stop();
            delete lineItem;
            delete tmp;
        });
    }
}

bool SingleModeT::judgeCollide()
{
    /*判断人物是否和item相碰撞，限制人物的移动范围*/
    bool collide = false;
    for(int i = 0; i < X*Y; i++){
        bool a = blocks[i]->checkCarried();
        bool b = player.collidesWithItem(blocks[i]);
        if((!a)&&b){
            collide = true;
        }
        if(collide) break;
    }
    return collide;
}

void SingleModeT::setMap(int x, int y, int firstTime)
{
    /*设置地图，其中x，y代表横纵方块数，firstTime为是否第一次游玩的标记*/
    X = x;
    Y = y;
    int topLeftX = 80;
    int topLeftY = 55;
    if(firstTime){
        matrix = new int*[x+2];
        for(int i = 0; i < x+2; i++){
            matrix[i] = new int[y+2]{0};
        }
        blocks = new block[x*y];
        int half = x*y/2;
        bool change = false;

        for(int i = 0; i < x*y; i++){
            int theX = topLeftX + 45*(i%x);
            int theY = topLeftY + 45*(i/x);
            int X = i%x+1;
            int Y = i/x+1;
            if(i == half) change = true;
            if(change){
                blocks[i] = new BlocksT(*blocks[(i+half)%(x*y)], X, Y, i);
                scene->addItem(blocks[i]);
                blocks[i]->setPos(theX,theY);
                blocks[i]->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
            }
            if(!change) {
                blocks[i] = new BlocksT(X, Y, i);
                scene->addItem(blocks[i]);
                blocks[i]->setPos(theX,theY);
                blocks[i]->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
            }
            matrix[X][Y] = 1;
            connect(blocks[i], &BlocksT::beenClicked, [=](int code){
                judgeEliminate(code);
            });
            connect(blocks[i], &BlocksT::causeEffects,[=](int effects){
                switch (effects) {
                    case 7:
                        shuffle();
                        break;
                    case 8:
                        flash();
                        break;
                    case 9:
                        addTime();
                        break;
                    default:
                        break;
                }
            });
        }
    }else{

    }
}

void SingleModeT::mousePressEvent(QMouseEvent *event){
    /*配合flash使用，实现角色位移*/
    if(!ableToFlash) return;
    if(event -> buttons() & Qt::LeftButton){
        QPointF newPos = event->pos();
        QPointF currentPos = player.pos();
        if(newPos.x()+player.width <= 795 && newPos.x() >= 5
           && newPos.y() >= 5 && newPos.y()+player.height <= 645){
            player.setPos(newPos);
            if(judgeCollide())
                    player.setPos(currentPos);
            else{
                FlashTimer->stop();
                ableToFlash = false;
                flashCount = 5;
            }
        }
    }
}

void SingleModeT::shuffle()
{
    /*重新分配方块*/
    int i =0;
    int j = X*Y-1;
    for(; i < j; i++){
        if(!blocks[i] -> checkCarried()){
            blocks[i]->randomState();
            blocks[i]->loadState(blocks[i]->checkState());
            for(; j > i; j--){
                if(!blocks[j] -> checkCarried()){
                    blocks[j]->loadState(blocks[i]->checkState());
                    j--;
                    break;
                }
            }
        }
    }
    ui->statusLabel->setText(ui->statusLabel->text().append(u8"激活了洗牌道具，方块重新生成"));
    marks += 20;

}

void SingleModeT::flash(){
    /*闪现*/
    marks += 15;
    ableToFlash = true;
    FlashTimer->start();
    ui->statusLabel->setText(ui->statusLabel->text().append(u8"激活了瞬移道具，5秒内可以移动到附近没有方块的地方"));
}

void SingleModeT::addTime(){
    /*加时道具，实现非常简单*/
    ui->clock->setRemainTime(ui->clock->remainTime()+6);
    ui->statusLabel->setText(ui->statusLabel->text().append(u8"激活了加时道具，剩余时间加6秒"));
}

void SingleModeT::setRules()
{
    /*一些简单的游戏规则和ui设计*/
    connect(ui->pauseWidget, &PauseWidget::resume, this, &SingleModeT::startProgress);
    connect(ui->pauseWidget, &PauseWidget::returnToBackground, this, [=](){
        this->close();
        emit returnToHomepage();
    });
    connect(ui->pauseWidget, &PauseWidget::savegame, [=]{
        QMessageBox::about(NULL,u8"保存游戏", u8"游戏已保存！");
        saveGame();
    });
    connect(ui->clock, &Clock::timesUp, this, [=](){
        showFail();
    });
}



QPointF SingleModeT::physicalPoint(int originPointx, int originPointy)
{
    /*一个简单的mapping函数，通过方块在方块堆中的位置获得其在主界面上的位置*/
    int theX = 80 + 45*originPointx-22;
    int theY = 55 + 45*originPointy-22;
    return QPointF(theX, theY);
}


void SingleModeT::showFail()
{
    /*显示失败窗口*/
    pause();
    ui->pauseWidget->setMarksForFailure(marks);
}

void SingleModeT::showVictory()
{
    /*显示胜利窗口*/
    pause();
    ui->pauseWidget->setMarksForVictory(marks);
}

void SingleModeT::saveGame(){
    /*保存游戏*/
    QFile *saver = new QFile("C:\\temp\\gameT.txt");
    saver->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
    QTextStream in(saver);
    in << QString::number(player.x())+","+QString::number(player.y())<<endl;
    in << QString::number(ui->clock->remainTime())<<endl;
    in << QString::number(X)+","+QString::number(Y)<<endl;
    in << QString::number(Blocksnum) << endl;
    for(int i = 0; i < X*Y; i++){
        in << QString::number(blocks[i]->checkState()) << "," << QString::number(blocks[i]->checkCarried()) << endl;
    }
    in << ui->marksLabel->text() << endl;
    saver->close();
}

bool SingleModeT::isBlock(int x, int y){
    /*判断在x，y位置上是否有方块*/
    return matrix[x][y];
}

void SingleModeT::loadGame(){
    /*从文件当中恢复游戏状态，并加入异常处理：当这个文件不存在的时候弹出提示框*/
    QFile *reader = new QFile("C:\\temp\\gameT.txt");
    if(!reader->open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(NULL,u8"加载失败", u8"游戏未能加载！");
        setMap(6,6,true);
        return;
    }
    QTextStream out(reader);
    QString playerx, playery;
    QString playerpos = out.readLine();
    bool change = false;
    for(int i = 0; i < playerpos.length() && playerpos[i] != '\n'; i++){
        if(playerpos[i] == ','){
            change = true;
            continue;
        }
        if(change) playery.append(playerpos[i]);
        else playerx.append(playerpos[i]);
    }
    player.setPos(playerx.toInt(), playery.toInt());
    int remainTime = (out.readLine().toInt());
    ui->clock->setRemainTime(remainTime);

    QString xString, yString;
    QString scale = out.readLine();
    bool change2 = false;
    for(int i = 0; i < scale.length() && scale[i] != '\n'; i++){
        if(scale[i] == ','){
            change2 = true;
            continue;
        }
        if(change2) yString.append(scale[i]);
        else xString.append(scale[i]);
    }
    setMap(xString.toInt(), yString.toInt(),true);
    QString numOfBlocks = out.readLine();
    this->Blocksnum = numOfBlocks.toInt();
    for(int i = 0; i < X*Y; i++){
        QString stateString, carriedString;
        QString itemStateString = out.readLine();
        bool change = false;
        for(int i = 0; i < itemStateString.length() && itemStateString[i] != '\n'; i++){
            if(itemStateString[i] == ','){
                change = true;
                continue;
            }
            if(change) carriedString.append(itemStateString[i]);
            else stateString.append(itemStateString[i]);
        }
        blocks[i]->loadState(stateString.toInt());
        if(carriedString.toInt()){
            matrix[blocks[i]->getX()][blocks[i]->getY()] = 0;
            blocks[i]->beenCarried();
            blocks[i]->hide();
        }
    }
    QString marks = out.readLine();
    ui->marksLabel->setText(marks);
    this->marks = marks.toInt();
}

void SingleModeT::findTheSame(){
    /*找一找有没有能够和人物相接触，而且可以被消除的方块*/
    for(int i = 0; i < X*Y; i++){
        if(blocks[i]->checkCarried()||(isBlock(blocks[i]->getX()-1,blocks[i]->getY()-1)&&
                                       isBlock(blocks[i]->getX()-1,blocks[i]->getY())&&
                                       isBlock(blocks[i]->getX(),blocks[i]->getY()-1)&&
                                       isBlock(blocks[i]->getX()-1,blocks[i]->getY()+1)&&
                                       isBlock(blocks[i]->getX(),blocks[i]->getY()+1)&&
                                       isBlock(blocks[i]->getX()+1,blocks[i]->getY()+1)&&
                                       isBlock(blocks[i]->getX()+1,blocks[i]->getY()-1)&&
                                       isBlock(blocks[i]->getX()+1,blocks[i]->getY())))
            continue;
        for(int j = X*Y-1; j > i; j--){
            if(blocks[j]->checkCarried()||blocks[i]->checkState() != blocks[j]->checkState()||
                    !ableToLink(i,j)||(isBlock(blocks[j]->getX()-1,blocks[j]->getY()-1)&&
                                     isBlock(blocks[j]->getX()-1,blocks[j]->getY())&&
                                     isBlock(blocks[j]->getX(),blocks[j]->getY()-1)&&
                                     isBlock(blocks[j]->getX()-1,blocks[j]->getY()+1)&&
                                     isBlock(blocks[j]->getX(),blocks[j]->getY()+1)&&
                                     isBlock(blocks[j]->getX()+1,blocks[j]->getY()+1)&&
                                     isBlock(blocks[j]->getX()+1,blocks[j]->getY()-1)&&
                                     isBlock(blocks[j]->getX()+1,blocks[j]->getY())))
                continue;
            else{
                blocks[i]->beenLighted();
                blocks[j]->beenLighted();
                ui->statusLabel->setText(u8"已经找到一对可以链接的方块，高亮中");
                return;
            }
        }
    }
    ui->statusLabel->setText(u8"当前地图上没有可以链接的方块，重新生成方块中");
    shuffle();
}
