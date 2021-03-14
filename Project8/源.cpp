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
//�ж��Ƿ�Ϊ����,�������򷵻�0
int card::lorCard()
{//6:+4
    if (color == 6)return 2;
    //5 ѡ����ɫ
    else if (color == 5)return 1;
    //��ͨ��
    else return 0;
}
int card::funCard()
{
    if (color < 5 && num>9 && num != 13 && num != 14)
    {
        switch (num)
        {
        case 10:return 1; break;//ͣ
        case 11:return 2; break;//��ת
        case 12:return 3; break;//+2
        }
    }
    else return 0;
}
//һ��������ݵĺ���
const string card::id()
{
    vector < string > colour{ "red", "yellow", "blue", "green","mistake","Wild","plus4" };
    string s1, s2;
    s1 = colour[color];
    if (num < 10)//1-9Ϊ��ͨ��
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
    //��ͨ����ɫ��������ͬ
    if (c1.readnum() == c2.readnum() || c1.readcol() == c2.readcol())return 1;
    //��������ƣ��򶼿��Գ�
    else if (c2.lorCard() != 0)return 1;
    //�������ܳ�
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
//����������������Ϣ
void PLAY::readit()//readit(players[0].player);
{
    std::ostringstream cardinfo;
    for (card& entry : player)
    {

        cardinfo << " " << entry.id();
    }
    cout << cardinfo.str() << endl;
}
//������������Ƶ���Ϣ������string
string PLAY::showit()
{
    std::ostringstream cardinfo;
    for (card& entry : player)
    {

        cardinfo << " " << entry.id();
    }
    return cardinfo.str() + '\n';
}
//��һ�������
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

        cout << "��ӭ����UNOС��Ϸ" << endl;
        cout << "����1��ʼ��Ϸ" << endl;
        cout << "����2�鿴��Ϸ����" << endl;
        string str;
        cin >> str;
        if (str == "1") { flag = 1; break; }
        else if (str == "2") { cout << "��Ϸ����" << endl; flag = 2; break; }
        else { cout << "�޷�ʶ������������" << endl; }
    }
    system("CLS");
    if (flag == 2) {
        while (1) {
            rule(); cout << "��3����";
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
    cout << "UNO��Ϸ����" << endl;
    cout << "��Ϸ������ͨ�ơ������ơ�����������" << '\n' << "��ͨ���ɣ��졢�ơ��̡�����������ɫ��1-9���������" << '\n'<<endl;
    cout << "�����ƣ�" << '\n' << "��Wild�ơ����������������ָ���¼ҳ��Ƶ���ɫ��4ɫ��ѡ1��" << endl;
    cout << "��+4(Draw Four)�ơ������������ָ���¼ҳ��Ƶ���ɫ��ͬʱ�¼�����ƶ��з���4���ƣ��Ҳ��ܳ���" <<'\n'<< endl;
    cout << "�����ƣ�" << '\n' << "��Skip�ơ��������������¼Ҳ��ܳ��ƣ��ֵ����¼ҳ���" << endl;
    cout << "��Reverse�ơ������ת�󣬵�ǰ����ʱ��˳�򽫷�ת��" << '\n' << "eg:ԭ����˳��ΪA��B��C��D��A���ڴ������֮��˳���ΪD��C��B��A��D" << endl;
    cout << "��+2(Draw Two)�ơ����+2���¼ҽ�������2���ƣ����Ҳ��ܳ���" << '\n'<<endl;
    cout << "ÿ�γ��������ϼҳ��������֡�����ɫһ��" << endl;
    cout << "�������У����������������ɫ������ʱ�����ϼ���ɫһ��" << '\n' << "������Ϊ��ɫ�������Գ�" << '\n'<<endl;
    cout << "UNO�Ļ�ʤ�����ǣ��Ƚ��Լ����ϵ���ȫ������(�����ڶ�����)��" << '\n' << endl;
    cout << "��������������ʼ����" << '\n' << "******���Ƹ�ʽ���������ʾ���Ƹ�ʽһ�� eg: [red 3]******" << endl;
}
int game()
{
    srand(time(NULL));
    //��ʼ��������
    allCard.set();
    //���������ϴ��
    allCard.shuffle();
    //������ң�����players[0]~[i-1]

    int i;
    while (1) {
        cout << "��������Ҹ��������س�����";
        cin >> i;
        if (i > 9 || i < 2)cout << "������2-9������" << endl;
        else break;
    }
    system("CLS");
    vector<PLAY> players(i);//��ʼ��i��PLAY����
    players[0].gamer = 1;
    //����
    for (int j = 0; j < i; ++j)
    {
        players[j].giveCards(j, allCard);
        players[j].th = j;
    }
    //ͳ��ʣ�����
    leftCards(allCard, restCard, i);
    //��ʾ���˵���
    players[0].readit();
    //����
    cout << "����ҳ���" << endl;
    getchar();//������һ��'\n'
    //��ʼ�ѭ������
    ListNode*& main = game(players, i);//ֵ���ݣ�����λ��//�ǵ����Ҫfree
    process(main, i, main);
    ListClear(main);
    return 0;
}
//��Ϸ����
void process(ListNode* ptr, int i, ListNode* main)
{
    ListNode* p = nullptr;
    while (!win) {
        cout << endl;
        Sleep(1000);
        if (restCard.getSize() == 0) { cout << "û���˻�ʤ��" << endl; break; }
        if (ptr->val.gamer && ptr->val.cir == 0)//���ǵ�һ��
        {
            ptr->val.input();
            ptr->val.readit();
            ++ptr->val.cir;
            p = ptr->next;
            ptr = p;
            p = nullptr;
        }
        else {
            if (ver % 2 == 0)//û�з�ת
            {
                if (ex)
                {
                    p = ptr->next->next;
                    ptr = p;
                    p = nullptr;
                    ex = 0;
                }
                else;
                if (ptr->val.gamer)//���������
                {
                    ptr->val.mainpro1();

                }
                else //�����������
                {
                    ptr->val.gamepro1(ptr->order, main->val.showit());
                    if (ptr == main->prev)cout << main->val.showit();
                }
                //ָ����һ��ָ��
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

            else//�з�ת���õ�j-2����ҳ���
            {
                if (ex)
                {
                    p = ptr->prev->prev;
                    ptr = p;
                    p = nullptr;
                    ex = 0;
                }
                else;
                if (ptr->val.gamer)//���������
                {
                    if (ptr->val.cir == 0)//���ǵ�һ��
                    {
                        ptr->val.input();
                        ptr->val.readit();
                        ++ptr->val.cir;

                    }
                    else {
                        ptr->val.mainpro1();
                    }
                }
                else //�����������
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
//����ѭ������
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
    p->prev = cir;//�ɻ�
    return p;
}
void PLAY::gamepro1(int j, string str)//npc����
{

    npcSearch(usedCard.getVec().back());

}
void PLAY::mainpro1()//��ҷ��״γ���
{
    input(usedCard.getVec().back());
    readit();
}
//�����ƺ���
void PLAY::input()
{
    string s1;
    getline(cin, s1);
    showdown(s1);
}
void PLAY::input(card& c1)//������һ����
{
    int flag = 0;
    if (c1.lorCard() != 0)//����
    {
        switch (c1.lorCard())
        {
        case 2:
        {cout << "���" << th << "����������" << endl;
        plusCard(restCard, 4);
        c1.redel();
        flag = 1;
        break; }//+4
        case 1://����ɫ
        {c1.redel();
        cout << "�Ƿ���� yes or no" << endl;
        string s;
        cin >> s;
        //�����ˡ�������ѡ��
        switch (s[0])
        {
        case 'n': cout << "���" << th << "ѡ�񲻳��ƣ���" << endl; plusCard(restCard, 1); flag = 1; break;
        case 'y':flag = 0; break;

        }
        getchar();
        if (flag == 0) {
            string s2;
            getline(cin, s2);
            showdown(usedCard.getVec().back(), s2);
            flag = 1;
        }break;

        }//����ɫ
        }
    }
    //������
    else if (c1.funCard() != 0)
    {
        switch (c1.funCard())//1ͣ2��ת3+2
        {
        case 1: cout << "�����" << th << "����ֹ���ƣ���" << endl; c1.redef(); break;
        case 2:  cout << "����˳�����" << endl; break;//��ת
        case 3:plusCard(restCard, 2);//+2
            c1.redef();
            cout << "�����" << th << "����������" << endl;
            break;
        }
    }
    else {//��ͨ��
        cout << "�Ƿ���� yes or no" << endl;
        string s;
        cin >> s;
        getchar();
        //�����ˡ�������ѡ��
        switch (s[0])
        {
        case 'n': cout << "���" << th << "ѡ�񲻳��ƣ���" << endl; plusCard(restCard, 1); flag = 1; break;
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
//npcѡɫ�ú���������0~3�������
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
//���ƹ����ж�
//npcѰ�ҿ��Գ�����
void PLAY::npcSearch(card& c1)
{
    //c1����ͨ�����Ƶ����
    if (c1.funCard() == 0 && c1.lorCard() == 0)
    {
        int flag = 0;
        card c2;
        list<card>::iterator b = player.begin(), e = player.end();
        //���������ȳ�����
        for (b; b != e; ++b)
        {
            if (b->lorCard() != 0)
            {
                role = 0;
                c2 = *b;
                player.erase(b);
                cout << "���" << th << "����" << c2.id() << endl;
                const int f = 5;
                srand(time(NULL));
                int i;
                int j = (rand() % 2);//ѡȡ0-1
                    if (j == 0)
                    {
                        srand(time(NULL));
                       i = (rand() % 4);//ѡȡ0~3����
                        c2.lorColor = i;
                    }
                    else { i = npcGetCol(); c2.lorColor = i; }
                cout << "��һ�ų�����ɫ����Ϊ" << yanse[i] << endl;
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
                    cout << "���" << th << "����" << c2.id() << endl;
                    if (c2.funCard() == 2) { ++ver; ex = 1; c2.redef(); }
                    break;
                }

            }
            if (flag == 0)
            {
                cout << "�����" << th << "û�пɳ����ƣ���" << endl;
                plusCard(restCard, 1);
            }
        }
    }
    else if (c1.funCard() != 0)//�����Ƶ����
    {
        switch (c1.funCard())//1ͣ2��ת3+2
        {
        case 1:
            cout << "�����" << th << "����ֹ���ƣ���" << endl;
            c1.redef();
            break;
        case 2:
            cout << "����˳�����" << endl;
            break; //��ת
        case 3:
            plusCard(restCard, 2);//+2
            c1.redef();
            cout << "�����" << th << "����������" << endl;
            break;
        }
    }
    else//���Ƶ����
    {
        if (role) {
            int flag = 1;
            const int f = 5;
            cout << "��ѡ����ɫ red yellow blue green" << endl;
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
            cout << "�����" << th << "����������" << endl;
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
    if (num == 11)cout << "����˳�����" << endl;
    else if (num == 12)cout << "��һ�������������" << endl;
    this->changeNum(14);
    usedCard.getVec().push_back(*this);
}
void card::redel()
{
    if (num == 13)cout << "��һ�������������" << endl;
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
//��������������Ϣ
void readit(card& c1)
{
    cout << c1.id() << endl;
}
//����һ�׿�����Ϣ
void readit(CardData c1)
{
    for (card& entry : c1.cards)
        cout << " " << entry.id();
}
void leftCards(CardData& all, CardData& rest, int i)
{//�����Ѿ�����i*7����,all[i*7]
    for (int j = 0; j < (108 - i * 7); ++j)
        rest.cards.push_back(all.cards[i * 7 + j]);
}
//�״γ��Ƶ��ж�
void PLAY::showdown(string& str)
{//��֤�Ƿ��������
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
    if (flag == 0) { cout << "�������������³���" << endl; input(); }
    else if (flag == 2) {
        cout << "�׾���ò�Ҫ������/������" << '\n'; input();
    }
    else {
        cout << "���" << th << "���� " << c2.id() << endl;
        usedCard.getVec().push_back(c2);
    }
}
//���״γ��Ƶ��ж�
void PLAY::showdown(card& c1, string& str)
{//��֤�Ƿ��������
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
        cout << "�������������³���" << endl; input(usedCard.getVec().back());
    }
    else
    {
        //��֤����һ�����Ƿ�ƥ��
        if (!compare(c1, c2)) {
            cout << "�������������³���" << endl; input(usedCard.getVec().back());
        }
        else//���¿϶�����ƥ�����
        {

            if (c2.funCard() == 2) {
                ++ver; ex = 1;
                c2.redef();
                cout << "���0���� " << c2.id() << endl;
            }
            else {
                if (c2.lorCard() != 0)role = 1;
                else;
                cout << "���0���� " << c2.id() << endl;
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
{//��ʼ����ͨ������
    //0��1��2��3��
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
    //��ʼ��������
    //10��ֹ11��ת12��2
    for (int j = 0; j < 4; ++j)
        for (int z = 10; z < 13; ++z)
            for (int i = 0; i < 2; ++i)
            {
                card num(j, z);
                this->cards.push_back(num);
            }
    //funCard[j][z][i]z:0~2,i:0~2
    //��ʼ��������
    //5��ɫ6+4
    for (int j = 0; j < 4; ++j)
    {
        card num(5, -1);
        this->cards.push_back(num);
        card num2(6, 13);
        this->cards.push_back(num);

    }
}