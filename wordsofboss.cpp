#include "wordsofboss.h"
/*本类包括Boss的开场白以及发动技能时的对话框
总体上是QLabel的延伸，亮点是让其实现了自动操作。
*/

WordsOfBoss::WordsOfBoss(QWidget *parent)
{
   setParent(parent);
   setAlignment(Qt::AlignCenter);
   wordsList<<u8"今天真是美好的一天";
   wordsList<<u8"鸟儿在歌唱、花朵绽放...";
   wordsList<<u8"这么美丽的日子，你这样的家伙...";
   wordsList<<u8"应该在地狱里燃烧！";
   wordsList<<u8"你的挣扎只是给我增添笑料...";
   wordsList<<u8"因为你实在是微不足道";
   wordsList<<u8"回去吧，你在自找苦吃";
   wordsList<<u8"用尽你的浑身解数吧...";
   wordsList<<u8"如果你觉得你做得到的话";
   wordsList<<u8"在越线之前你最好想清楚了...";
   wordsList<<u8"否则你会吃苦头的";
   wordsList<<u8"为什么你这家伙...";
   wordsList<<u8"总是不懂得什么叫规矩？";
   wordsList<<u8"难道你真的狂妄自大到以为";
   wordsList<<u8"你能拿那些蹩脚伎俩糊弄我吗？";
   wordsList<<u8"是时候让你吃点苦头了...";
   wordsList<<u8"你似乎充满了决心...";
   wordsList<<u8"徒劳的决心。";
   wordsList<<u8"我不耐烦了...";
   timer = new QTimer;
   timer->setInterval(2000);
   connect(timer, &QTimer::timeout, this, &WordsOfBoss::toContinue);
   connect(this, &WordsOfBoss::finalOver, this, &WordsOfBoss::stop);
}

void WordsOfBoss::mousePressEvent(QMouseEvent *ev)
{
    if(!isHidden() && ev->buttons()&Qt::LeftButton)
        toContinue();
}
void WordsOfBoss::keyPressEvent(QKeyEvent *ev)
{
    if(!isHidden() && (ev->key()==Qt::Key_Enter || ev->key()==Qt::Key_Space))
        toContinue();
}

void WordsOfBoss::say()
{
    timer->start();
}

void WordsOfBoss::stop()
{
    timer->stop();
    sequence = 0;
}

int WordsOfBoss::currentSequece()
{
    return sequence;
}

void WordsOfBoss::notFirstTime()
{
    this->hide();
    emit over();
}

void WordsOfBoss::loadSequence(int value)
{
    sequence = value;
    notFirstTime();
}
void WordsOfBoss::toContinue()
{
    if(isHidden()) show();
    if(currentNumOfWords == seq[sequence]){
        timer->stop();
        currentNumOfWords = 0;
        this->hide();
        sequence == 9 ?emit finalOver():emit over();
        sequence++;
        return;
    }
    else{
        loadSentence();
    }
}

void WordsOfBoss::loadSentence()
{
    if(sequence >= 9) {
        sequence = 9;
        sentences = 18;
    }
    setText(wordsList[sentences]);
    sentences++;
    currentNumOfWords++;
}

int WordsOfBoss::currentSentences(){
    return sentences;
}

void WordsOfBoss::loadCurrentSentences(int value){
    sentences = value;
}
