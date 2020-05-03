#include "data.h"

void Game::reset() //重新设置
{
    discard_count = 0;
    wall_count = NUM_TILE;
    memset(hand_count, 0, sizeof(hand_count));//手牌数
    memset(steal_count, 0, sizeof(steal_count));//鸣牌数
    memset(flower_count, 0, sizeof(flower_count));//补花数
    memset(history, 0, sizeof(history));//历史牌

    for (tile t = 1; t < NUM_SYMBOL; t++)
        remain[t] = 4;
}
void Game::add_handcard(tile t) //加入手牌
{
    int i;
    for (i = 0; i < hand_count[me]; ++i) {
        if (hand[i] > t) {
            for (int j = hand_count[me]; j > i; j--)
                hand[j] = hand[j - 1];
            hand[i] = t;
            break;
        }
    }
    if (i >= hand_count[me])hand[hand_count[me]] = t;
    ++hand_count[me];
}
void Game::remove_handcard(tile t)//移除手牌
{
    for (int k = 0; k < hand_count[me]; ++k) {
        if (hand[k] == t) {
            for (int j = k; j < hand_count[me] - 1; ++j)
                hand[j] = hand[j + 1];
            break;
        }
    }
    hand[hand_count[me] - 1] = 0;
    --hand_count[me];
}
bool Game::can_peng() const //可以碰
{
    if (!IS_PLAY(last_act) || last_player == me)
        return false;

    int t_num = 0, k;
    for (k = 0; k < hand_count[me]; ++k)
        if (hand[k] == last_card)++t_num;
    if (t_num >= 2)return true;//这？没问题吧...
    return false;
}

int Game::can_chi() const  //int的三个位返回三种吃法的可行性
{
    using namespace chi;

    if (!IS_PLAY(last_act) || last_player != SHANGJIA(me) | !IS_SHU(last_card))
        return false;

    int res = 0;
    bool exists[5] = { false };// 0 1 2(x) 3 4

    for (int k = 0; k < hand_count[me]; ++k) {
        if (hand[k] - last_card <= 2 && last_card - hand[k] <= 2)
            exists[hand[k] - last_card + 2] = true;
    }

    if (exists[3] && exists[4] && NUMBER(last_card) <= 7)
        res |= LEFT_CHI;
    if (exists[1] && exists[3] && NUMBER(last_card) >= 2 && NUMBER(last_card) <= 8)
        res |= MID_CHI;
    if (exists[0] && exists[1] && NUMBER(last_card) >= 3)
        res |= RIGHT_CHI;
    return res;
}

bool Game::can_ming_gang() const //可以明杠
{
    if (last_player == me || !IS_PLAY(last_act))return false;

    int num = 0;
    for (int k = 0; k < hand_count[me]; ++k) {
        if (hand[k] == last_card) {
            ++num;
            if (num == 3)return true;//哈？我是不是把那个地方理解错了，怎么又差1.
        }
    }

    return false;
}

tile Game::can_an_gang() const //可以暗杠？如果手里有两个杠呢？先不管吧orz
{

    if (last_act != ACT_DRAW)
        return 0;
    tile gang_card = 0;
    int num = 1;
    for (int i = 1; i < hand_count[me]; i++) {
        if (hand[i] == hand[i - 1]) {
            num++;
            if (num >= 4) {
                gang_card = hand[i];
                break;
            }
        }
        else
            num = 1;
    }
    return gang_card;
}

bool Game::can_bu_gang() const //可以补杠
{
    if (last_act != ACT_DRAW)
        return false;

    for (int i = 0; i < steal_count[me]; i++) {
        if (steal[me][i][0] == last_card && steal[me][i][1] == last_card)
            return true;
    }
    return false;
}

pair<int,double> Game::decide_play_card() {
    if (last_player != me ||
        (last_act != ACT_CHI && last_act != ACT_PENG && last_act != ACT_GANG && last_act != ACT_DRAW))return -1;//误调用

    tile tem_lastcard = this->last_act;
    int tem_lastplayer = this->last_player;
    Action tem_lastact = this->last_act;//暂存数据，大胆模拟

    int choice = -1;//choice is int from 0 to hand_count[me];
    double max_value = -1.0;
    double tem_value = -1.0;

    for (int ss = 0; ss < hand_count[me]; ++ss) {
        last_card = hand[ss];
        history[discard_count++] = last_card;
        remove_handcard(hand[ss]);
        last_act = ACT_PLAY;
        last_player = me;
        
        tem_value = evaluate();
        if (tem_value > max_value) {
            max_value = tem_value;
            choice = ss; // 更新选择
        }

        discard_count--;
        add_handcard(last_card);
        last_card = tem_lastcard; 
        last_act = ACT_DRAW;
        last_player = tem_lastplayer;
    }

    return make_pair(choice, max_value);
}