#include <vector>
#include <iostream>
#include <cstring>
using namespace std;


#ifndef ORZ_DATA_H
#define ORZ_DATA_H

#endif //ORZ_DATA_H
using namespace std;
typedef unsigned char tile;
class Game;

const int NUM_PLAYER = 4;
const int NUM_HAND = 14;
const int NUM_STEAL = 4;
const int NUM_FLOWER = 8;
const int NUM_TILE = 144;
const int NUM_SYMBOL = 35; //除开花牌有34种:27数牌+7字牌+0无效牌

const string TILE_NAME[NUM_SYMBOL + 8]= { //牌名
        "Invalid",
        "W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8", "W9",
        "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9",
        "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9",
        "F1", "F2", "F3", "F4", "J1", "J2", "J3",
        "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8"
};
enum Action { //所有的动作
    ACT_QUAN,
    ACT_RECEIVE,
    ACT_DRAW,
    ACT_BU_HUA,
    ACT_OTHER_DRAW,
    ACT_PLAY,
    ACT_PENG,
    ACT_CHI,
    ACT_GANG,
    ACT_BU_GANG,
    ACT_PASS,
    ACT_HU
};
enum Mian{
    PENG,CHI,JIANG
};

#define DIGITs(s) ((s[0]) - '0')
#define DIGITc(c) (c - '0')
#define MY(field) (this->field[this->me])
#define IS_PLAY(action) \
	(action == ACT_PLAY || action == ACT_PENG || action == ACT_CHI)
#define IS_SHU(t) ((t) <= 27) //是否为数牌
#define NUMBER(t) (((t)-1) % 9 + 1) //计算数牌的具体值
#define SHANGJIA(t) ((t + NUM_PLAYER - 1) % NUM_PLAYER)

namespace chi { //不同的顺序
    const int LEFT_CHI = 0x1; // {X, X+1, X+2}
    const int MID_CHI = 0x2; // {X-1, X, X+1}
    const int RIGHT_CHI = 0x4; // {X-2, X-1, X}
    const int NEED_DELTA[5][2] = { { 0, 0 }, { 1, 2 }, { -1, 1 }, { 0, 0 }, { -2, -1 } };
    const int OUTPUT_DELTA[5] = { 0, 1, 0, 0, -1 };
}

const tile CARD_NUM[26] = { //给牌编序号
        0, 9, 0, 0, 0, 27, 0, 0, 0, 31,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 18,
        0, 0, 0, 0, 0, 0
};
#define GET_CARD(s) (tile(CARD_NUM[s[0]-'A']+DIGITc(s[1]))) //计算牌的序号

class Game {
public:
    tile hand[NUM_HAND]; //手牌
    tile steal[NUM_PLAYER][NUM_STEAL][2];//记录各个玩家鸣牌情况，4种
    int me, dealer;
    int hand_count[NUM_PLAYER], steal_count[NUM_PLAYER], flower_count[NUM_PLAYER];//记录玩家手牌数，鸣牌数，补花数
    int wall_count, discard_count;//牌墙数和丢牌数
    tile history[NUM_TILE];//已经打出的牌，不包括被其他玩家鸣牌的牌
    int remain[NUM_SYMBOL];//各种牌在牌墙剩余的牌数
    tile last_card;//上一张被打出的牌
    int last_player; //上一个操作玩家
    Action last_act; //上一个动作

    Game() { reset(); }
    void reset(); //初始化牌局
    void add_handcard(tile t); //加入手牌
    void remove_handcard(tile t); //移除手牌
    bool can_peng() const;
    int can_chi() const;
    bool can_ming_gang() const;
    tile can_an_gang() const;
    bool can_bu_gang() const;
    pair<int,double> decide_play_card();//在需要出牌时，决定出牌的下标
};

class Solution{ //寻找到的胡牌方案
public:
    vector<tile> need;
    vector<tile> no_need;
    vector<pair<Mian, tile> > mianzi; //记录我们凑出的面子
};

class Hu{ //胡牌函数模板
public:
    vector<tile> need_card; //存储我们需要的牌
    vector<pair<Mian,tile> > mian_type; //存储做出来的
    const tile *hand; //手牌
    int hand_count, ding; //手牌数，ding为最多接受的需要牌数，由我们设置
    int *remain; //未知的牌池
    bool visit[14] = {false};
    vector<Solution> record_solution;


    Hu(const tile hand_[], int hand_count_, int *remain_, int ding_):hand(hand_){
        hand_count = hand_count_; remain = remain_; ding = ding_;
    }
    void SearchHu(int mian_num,int jiang_num, int begin);
};