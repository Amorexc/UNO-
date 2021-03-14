#include<iostream>
#include<vector>
#include<iterator>
#include<string>
#include<list>
#include<random>
#include<sstream>
#include<time.h>
#include<Windows.h>
#include<stdlib.h>
using namespace std;
struct card
{
    friend bool compare(card& c1, card& c2);
public:
    const int readnum();
    const int readcol();
    int lorCard();
    int funCard();
    const string id();
    card() = default;
    card(int x, int y) :color(x), num(y) {};
    void redef();
    void redel();
    void changeNum(const int d);
    void changeCol(const int d);
    int lorColor = 0;
private:
    int color;
    int num;
};
void card::changeNum(const int d)
{
    num = d;
}
void card::changeCol(const int d)
{
    color = d;
}
const int card::readnum()
{
    return num;
}
const int card::readcol()
{
    return color;
}
//判定是否为神牌,非神牌则返回0
int card::lorCard()
{//6:+4
    if (color == 6)return 2;
    //5 选定颜色
    else if (color == 5)return 1;
    //普通牌
    else return 0;
}
int card::funCard()
{
    if (color < 5 && num>9 && num != 13 && num != 14)
    {
        switch (num)
        {
        case 10:return 1; break;//停
        case 11:return 2; break;//反转
        case 12:return 3; break;//+2
        }
    }
    else return 0;
}
//一个表明身份的函数
const string card::id()
{
    vector < string > colour{ "red", "yellow", "blue", "green","mistake","Wild","plus4" };
    string s1, s2;
    s1 = colour[color];
    if (num < 10)//1-9为普通牌
    {
        s2 = std::to_string(num);
    }
    else
    {
        switch (num) {
        case 10:s2 = "Skip"; break;
        case 11:s2 = "Reverse"; break;
        case 12:s2 = "+2(Draw Two)"; break;
        case 13:s2 = "+4(Wild Draw Four)"; break;
        case -1:s2 = "Wild"; break;
        }
    }
    const string str = "[" + s1 + " " + s2 + "]";
    return str;
}
bool compare(card& c1, card& c2)
{
    //普通牌颜色或字数相同
    if (c1.readnum() == c2.readnum() || c1.readcol() == c2.readcol())return 1;
    //如果是神牌，则都可以出
    else if (c2.lorCard() != 0)return 1;
    //其余则不能出
    else return 0;
}
struct CardData:public card
{
    friend void readit(CardData c1);
    friend void leftCards(CardData& all, CardData& rest, int i);
public:
    void set();
    void shuffle();
    size_t getSize();
    vector<card>& getVec();
private:
    vector<card> cards;
};
vector<card>& CardData::getVec()
{
    return cards;
}
size_t CardData::getSize()
{
    return (cards.size());
}
void CardData::shuffle()
{
    std::random_shuffle(cards.begin(), cards.end());
}

struct PLAY :public CardData
{
public:
    void giveCards(const int i, CardData& numCard);
    void showdown(card& c1, string& str);
    void showdown(string& str);
    void npcSearch(card& c1);
    void plusCard(CardData& rest, int i);
    void readit();
    void input();
    void input(card& c1);
    void gamepro1(int j, string str);
    void mainpro1();
    int npcGetCol();
    string showit();
    bool gamer = 0;
    int cir = 0, th;
    auto sizeo() { return player.size(); }
private:
    list<card> player;
};
//输出玩家所带卡牌信息
void PLAY::readit()//readit(players[0].player);
{
    std::ostringstream cardinfo;
    for (card& entry : player)
    {

        cardinfo << " " << entry.id();
    }
    cout << cardinfo.str() << endl;
}
//读出玩家所有牌的信息，返回string
string PLAY::showit()
{
    std::ostringstream cardinfo;
    for (card& entry : player)
    {

        cardinfo << " " << entry.id();
    }
    return cardinfo.str() + '\n';
}
//玩家环形链表
struct ListNode
{
    PLAY val;
    int order;
    ListNode* next;
    ListNode* prev;
};
void readit(card& c1);
void process(ListNode* ptr, int i, ListNode* main);
void ListClear(ListNode* head);
void rule();
ListNode*& game(vector<PLAY>& players, int i);
CardData allCard, restCard, usedCard;
vector<string> yanse{ "red", "yellow", "blue", "green" };
bool win = 0, role = 0, ex = 0;
int ver = 0;
int game();
int main()
{
    int flag = 0;
    while (1)
    {

        cout << "欢迎来到UNO小游戏" << endl;
        cout << "输入1开始游戏" << endl;
        cout << "输入2查看游戏规则" << endl;
        string str;
        cin >> str;
        if (str == "1") { flag = 1; break; }
        else if (str == "2") { cout << "游戏规则" << endl; flag = 2; break; }
        else { cout << "无法识别，请重新输入" << endl; }
    }
    system("CLS");
    if (flag == 2) {
        while (1) {
            rule(); cout << "按3返回";
            int i; cin >> i;
            if (i == 3)break;
        }
       
    }
    system("CLS");
    game();
    return 0;
}
void rule()
{
    cout << "UNO游戏规则" << endl;
    cout << "游戏中有普通牌、万能牌、功能牌三种" << '\n' << "普通牌由（红、黄、绿、蓝）四种颜色和1-9的数字组成" << '\n'<<endl;
    cout << "万能牌：" << '\n' << "【Wild牌】，可以你可以随意指定下家出牌的颜色（4色中选1）" << endl;
    cout << "【+4(Draw Four)牌】，你可以随意指定下家出牌的颜色，同时下家需从牌堆中罚摸4张牌，且不能出牌" <<'\n'<< endl;
    cout << "功能牌：" << '\n' << "【Skip牌】打出跳过后，你的下家不能出牌，轮到再下家出牌" << endl;
    cout << "【Reverse牌】打出反转后，当前出牌时针顺序将反转。" << '\n' << "eg:原出牌顺序为A→B→C→D→A，在打出此牌之后，顺序变为D→C→B→A→D" << endl;
    cout << "【+2(Draw Two)牌】打出+2后，下家将被罚摸2张牌，并且不能出牌" << '\n'<<endl;
    cout << "每次出牌需与上家出的牌数字【或】颜色一样" << endl;
    cout << "功能牌有（红黄绿蓝）四种颜色，出牌时需与上家颜色一样" << '\n' << "万能牌为黑色，都可以出" << '\n'<<endl;
    cout << "UNO的获胜条件是：先将自己手上的牌全部打完(类似于斗地主)。" << '\n' << endl;
    cout << "接下来将由您开始出牌" << '\n' << "******出牌格式请务必与显示的牌格式一致 eg: [red 3]******" << endl;
}
int game()
{
    srand(time(NULL));
    //初始化所有牌
    allCard.set();
    //随机数函数洗牌
    allCard.shuffle();
    //创建玩家，生成players[0]~[i-1]

    int i;
    while (1) {
        cout << "请输入玩家个数，按回车结束";
        cin >> i;
        if (i > 9 || i < 2)cout << "请输入2-9的整数" << endl;
        else break;
    }
    system("CLS");
    vector<PLAY> players(i);//初始化i个PLAY对象
    players[0].gamer = 1;
    //发牌
    for (int j = 0; j < i; ++j)
    {
        players[j].giveCards(j, allCard);
        players[j].th = j;
    }
    //统计剩余的牌
    leftCards(allCard, restCard, i);
    //显示本人的牌
    players[0].readit();
    //出牌
    cout << "请玩家出牌" << endl;
    getchar();//吸掉上一个'\n'
    //开始搭建循环链表
    ListNode*& main = game(players, i);//值传递：主角位置//记得最后要free
    process(main, i, main);
    ListClear(main);
    return 0;
}
//游戏进程
void process(ListNode* ptr, int i, ListNode* main)
{
    ListNode* p = nullptr;
    while (!win) {
        cout << endl;
        Sleep(1000);
        if (restCard.getSize() == 0) { cout << "没有人获胜！" << endl; break; }
        if (ptr->val.gamer && ptr->val.cir == 0)//主角第一次
        {
            ptr->val.input();
            ptr->val.readit();
            ++ptr->val.cir;
            p = ptr->next;
            ptr = p;
            p = nullptr;
        }
        else {
            if (ver % 2 == 0)//没有反转
            {
                if (ex)
                {
                    p = ptr->next->next;
                    ptr = p;
                    p = nullptr;
                    ex = 0;
                }
                else;
                if (ptr->val.gamer)//如果是主角
                {
                    ptr->val.mainpro1();

                }
                else //如果不是主角
                {
                    ptr->val.gamepro1(ptr->order, main->val.showit());
                    if (ptr == main->prev)cout << main->val.showit();
                }
                //指向下一个指针
                if (ptr->val.sizeo() == 0)
                {
                    win = 1;
                    cout << "The player" << ptr->val.th << "has winned!" << endl;
                }
                else {
                    p = ptr->next;
                    ptr = p;
                    p = nullptr;
                }
            }

            else//有反转：该第j-2个玩家出牌
            {
                if (ex)
                {
                    p = ptr->prev->prev;
                    ptr = p;
                    p = nullptr;
                    ex = 0;
                }
                else;
                if (ptr->val.gamer)//如果是主角
                {
                    if (ptr->val.cir == 0)//主角第一次
                    {
                        ptr->val.input();
                        ptr->val.readit();
                        ++ptr->val.cir;

                    }
                    else {
                        ptr->val.mainpro1();
                    }
                }
                else //如果不是主角
                {
                    ptr->val.gamepro1(ptr->order, main->val.showit());
                    if (ptr == main->next)cout << main->val.showit();
                }
                if (ptr->val.sizeo() == 0)
                {
                    win = 1;
                    cout << "The player" << ptr->val.th << "has winned!" << endl;
                }
                else {
                    p = ptr->prev;
                    ptr = p;
                    p = nullptr;
                }
            }
        }

    }

}
void ListClear(ListNode* head)
{
    ListNode* q;
    ListNode* p = head;
    while (p->next) {
        q = p;
        p = q->next;
        free(q);
    }
    free(p);
    head = NULL;
}
//构造循环链表
ListNode*& game(vector<PLAY>& players, int i)
{
    ListNode* cir = new ListNode;
    cir->val = players[0];
    cir->order = 0;
    cir->next = nullptr;
    cir->prev = nullptr;
    ListNode* p = cir;
    for (int j = 1; j < i; ++j)
    {
        ListNode* pN = new ListNode;
        pN->val = players[j];
        pN->order = j;
        pN->next = nullptr;
        cir->next = pN;
        pN->prev = cir;
        cir = pN;
    }
    cir->next = p;
    p->prev = cir;//成环
    return p;
}
void PLAY::gamepro1(int j, string str)//npc出牌
{

    npcSearch(usedCard.getVec().back());

}
void PLAY::mainpro1()//玩家非首次出牌
{
    input(usedCard.getVec().back());
    readit();
}
//输入牌函数
void PLAY::input()
{
    string s1;
    getline(cin, s1);
    showdown(s1);
}
void PLAY::input(card& c1)//接收上一个牌
{
    int flag = 0;
    if (c1.lorCard() != 0)//神牌
    {
        switch (c1.lorCard())
        {
        case 2:
        {cout << "玩家" << th << "增加四张牌" << endl;
        plusCard(restCard, 4);
        c1.redel();
        flag = 1;
        break; }//+4
        case 1://换颜色
        {c1.redel();
        cout << "是否出牌 yes or no" << endl;
        string s;
        cin >> s;
        //增加了“过”的选项
        switch (s[0])
        {
        case 'n': cout << "玩家" << th << "选择不出牌，过" << endl; plusCard(restCard, 1); flag = 1; break;
        case 'y':flag = 0; break;

        }
        getchar();
        if (flag == 0) {
            string s2;
            getline(cin, s2);
            showdown(usedCard.getVec().back(), s2);
            flag = 1;
        }break;

        }//换颜色
        }
    }
    //功能牌
    else if (c1.funCard() != 0)
    {
        switch (c1.funCard())//1停2反转3+2
        {
        case 1: cout << "该玩家" << th << "被禁止出牌，过" << endl; c1.redef(); break;
        case 2:  cout << "出牌顺序调换" << endl; break;//反转
        case 3:plusCard(restCard, 2);//+2
            c1.redef();
            cout << "该玩家" << th << "增加两张牌" << endl;
            break;
        }
    }
    else {//普通牌
        cout << "是否出牌 yes or no" << endl;
        string s;
        cin >> s;
        getchar();
        //增加了“过”的选项
        switch (s[0])
        {
        case 'n': cout << "玩家" << th << "选择不出牌，过" << endl; plusCard(restCard, 1); flag = 1; break;
        case 'y':flag = 0; break;
        }
        if (flag == 0)
        {
            if (c1.lorCard() == 0 && c1.funCard() == 0)
            {
                string s2;
                getline(cin, s2);
                showdown(usedCard.getVec().back(), s2);
            }

        }
    }


}
//npc选色用函数：生成0~3的随机数
int PLAY::npcGetCol()
{
    list<card>::iterator b = player.begin(), e = player.end();
    int red = 0, yellow = 0, blue = 0, green = 0;
    for (b; b != e; ++b)
    {
        card& c2 = *b;
        switch (c2.readcol())
        {
        case 0:++red; break;
        case 1:++yellow; break;
        case 2:++blue; break;
        case 3:++green; break;
        }
    }
    int flag = 0;
    if ((red >= yellow ? red : yellow) >= (blue >= green ? blue : green))flag = 1; else flag = 2;
    if (flag == 1) { if (red >= yellow)return 0; else return 1; }
    else { if (blue >= green)return 2; else return 3; }

}
//出牌功能判定
//npc寻找可以出的牌
void PLAY::npcSearch(card& c1)
{
    //c1是普通数字牌的情况
    if (c1.funCard() == 0 && c1.lorCard() == 0)
    {
        int flag = 0;
        card c2;
        list<card>::iterator b = player.begin(), e = player.end();
        //有神牌优先出神牌
        for (b; b != e; ++b)
        {
            if (b->lorCard() != 0)
            {
                role = 0;
                c2 = *b;
                player.erase(b);
                cout << "玩家" << th << "出牌" << c2.id() << endl;
                const int f = 5;
                srand(time(NULL));
                int i;
                int j = (rand() % 2);//选取0-1
                    if (j == 0)
                    {
                        srand(time(NULL));
                       i = (rand() % 4);//选取0~3的数
                        c2.lorColor = i;
                    }
                    else { i = npcGetCol(); c2.lorColor = i; }
                cout << "下一张出牌颜色必须为" << yanse[i] << endl;
                usedCard.getVec().push_back(c2);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            b = player.begin(), e = player.end();
            for (b; b != e; ++b)
            {
                if (compare(c1, *b))

                {
                    flag = 1;
                    c2 = *b;
                    player.erase(b);
                    usedCard.getVec().push_back(c2);
                    cout << "玩家" << th << "出牌" << c2.id() << endl;
                    if (c2.funCard() == 2) { ++ver; ex = 1; c2.redef(); }
                    break;
                }

            }
            if (flag == 0)
            {
                cout << "该玩家" << th << "没有可出的牌，过" << endl;
                plusCard(restCard, 1);
            }
        }
    }
    else if (c1.funCard() != 0)//功能牌的情况
    {
        switch (c1.funCard())//1停2反转3+2
        {
        case 1:
            cout << "该玩家" << th << "被禁止出牌，过" << endl;
            c1.redef();
            break;
        case 2:
            cout << "出牌顺序调换" << endl;
            break; //反转
        case 3:
            plusCard(restCard, 2);//+2
            c1.redef();
            cout << "该玩家" << th << "增加两张牌" << endl;
            break;
        }
    }
    else//神牌的情况
    {
        if (role) {
            int flag = 1;
            const int f = 5;
            cout << "请选择颜色 red yellow blue green" << endl;
            string s1;
            cin >> s1;
            switch (s1[0])
            {
            case'r':c1.lorColor = 0; break;
            case'y':c1.lorColor = 1; break;
            case'b':c1.lorColor = 2; break;
            case'g':c1.lorColor = 3; break;
                //
            }
        }
        else;
        if (c1.lorCard() == 2)
        {
            cout << "该玩家" << th << "增加四张牌" << endl;
            plusCard(restCard, 4);
            c1.redel();
        }
        else
        {
            c1.redel();
            npcSearch(usedCard.getVec().back());
        }

    }

}
void card::redef()
{
    if (num == 11)cout << "出牌顺序调换" << endl;
    else if (num == 12)cout << "下一玩家增加两张牌" << endl;
    this->changeNum(14);
    usedCard.getVec().push_back(*this);
}
void card::redel()
{
    if (num == 13)cout << "下一玩家增加四张牌" << endl;
    this->changeCol(lorColor);
    usedCard.getVec().push_back(*this);
}
void PLAY::plusCard(CardData& rest, int i)
{
    vector<card>::iterator b = rest.getVec().begin(), e = rest.getVec().end();
    if (b + i >= e)
    {
        for (b; b != rest.getVec().end(); ++b)
        {
            player.push_back(*b);
        }
        rest.getVec().clear();
    }
    else
    {
        for (int j = 0; j < i; ++j)
        {
            player.push_back(*b); ++b;
        }
        rest.getVec().erase(b - i, b);
    }
}
//读出单个卡牌信息
void readit(card& c1)
{
    cout << c1.id() << endl;
}
//读出一套卡牌信息
void readit(CardData c1)
{
    for (card& entry : c1.cards)
        cout << " " << entry.id();
}
void leftCards(CardData& all, CardData& rest, int i)
{//现在已经发出i*7张牌,all[i*7]
    for (int j = 0; j < (108 - i * 7); ++j)
        rest.cards.push_back(all.cards[i * 7 + j]);
}
//首次出牌的判定
void PLAY::showdown(string& str)
{//验证是否有这个牌
    int flag = 0;
    card c2;
    list<card>::iterator b = player.begin(), e = player.end();
    for (b; b != e; ++b)
    {
        if (b->id() == str) {
            flag = 1;
            if (b->lorCard() != 0 || b->funCard() != 0) {
                flag = 2; break;
            }
            else
            {
                c2 = *b;
                player.erase(b);
                break;
            }
        }
        else;
    }
    if (flag == 0) { cout << "不符规则，请重新出牌" << endl; input(); }
    else if (flag == 2) {
        cout << "首局最好不要出功能/万能牌" << '\n'; input();
    }
    else {
        cout << "玩家" << th << "出牌 " << c2.id() << endl;
        usedCard.getVec().push_back(c2);
    }
}
//非首次出牌的判定
void PLAY::showdown(card& c1, string& str)
{//验证是否有这个牌
    int flag = 0;
    card c2;
    list<card>::iterator b = player.begin(), e = player.end();
    for (b; b != e; ++b)
    {
        if (b->id() == str) {
            flag = 1;
            c2 = *b;
            player.erase(b);
            break;
        }
        else;
    }
    if (flag == 0) {
        cout << "不符规则，请重新出牌" << endl; input(usedCard.getVec().back());
    }
    else
    {
        //验证与上一个牌是否匹配
        if (!compare(c1, c2)) {
            cout << "不符规则，请重新出牌" << endl; input(usedCard.getVec().back());
        }
        else//以下肯定都是匹配的牌
        {

            if (c2.funCard() == 2) {
                ++ver; ex = 1;
                c2.redef();
                cout << "玩家0出牌 " << c2.id() << endl;
            }
            else {
                if (c2.lorCard() != 0)role = 1;
                else;
                cout << "玩家0出牌 " << c2.id() << endl;
                usedCard.getVec().push_back(c2);
            }
        }
    }
}
void PLAY::giveCards(const int i, CardData& numCard)
{
    for (int j = 0; j < 7; ++j)
        player.push_back(numCard.getVec()[i * 7 + j]);
}
void setPlayers(vector<list<card>>& player, int i)
{
    for (int j = 0; j < i; ++j)
    {
        list<card> p1;
        player.push_back(p1);
    }
}
void CardData::set()
{//初始化普通数字牌
    //0红1黄2蓝3绿
    for (int j = 0; j < 4; ++j)
    {
        card num(j, 0);
        this->cards.push_back(num);
    }
    for (int j = 0; j < 4; ++j)
        for (int i = 1; i < 10; )
        {
            card num(j, i);
            this->cards.push_back(num);
            this->cards.push_back(num);
            ++i;
        }
    //numCard[j*4+i]i:0~19
    //初始化功能牌
    //10禁止11反转12加2
    for (int j = 0; j < 4; ++j)
        for (int z = 10; z < 13; ++z)
            for (int i = 0; i < 2; ++i)
            {
                card num(j, z);
                this->cards.push_back(num);
            }
    //funCard[j][z][i]z:0~2,i:0~2
    //初始化万能牌
    //5变色6+4
    for (int j = 0; j < 4; ++j)
    {
        card num(5, -1);
        this->cards.push_back(num);
        card num2(6, 13);
        this->cards.push_back(num);

    }
}