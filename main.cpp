#include <memory.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "jsoncpp/json.h"
#include "data.h"




vector<string> request, response;
int main(){
    int turnID;
    string stmp;
    Game game;

    Json::Value input;
    cin >> input;
    turnID = input["responses"].size();

    int type, player;
    tile card, me_draw_card, last_card;
    Action last_act;
    for(int i = 0; i <= turnID; i++) {
        istringstream in(input["requests"][i].asString()); //没有用到response
        string tmp;

        in >> type;
        switch (type) {
            case '0':
                in >> tmp; game.me = DIGITs(tmp); //玩家编号
                in >> tmp; game.dealer = DIGITs(tmp); //圈风
                last_act = ACT_QUAN;
            case '1':
                for(int j = 0; j < 4; ++j){
                    in >> tmp;
                    game.flower_count[j] = DIGITs(tmp); //四个玩家的补花数
                }
                for(int j = 0; j < 13; ++j){
                    in >> tmp;
                    game.hand[j] = GET_CARD(tmp); //加入手牌
                }
                last_act = ACT_RECEIVE;
                //花牌就不用管了，没啥用
            case '2':
                in >> tmp;
                game.add_handcard(GET_CARD(tmp));
                me_draw_card = GET_CARD(tmp);
                last_act = ACT_DRAW;
            case '3':
                in >> tmp;
                player = DIGITs(tmp); //当前操作的玩家
                in >> tmp;
                switch (tmp[0]) {
                    case 'B':
                        if(tmp[2]=='H'){ //补花
                            game.flower_count[player]++; //玩家补花，但是摸到什么花牌并不重要
                            last_act = ACT_BU_HUA;
                        }
                        else if(tmp[2]=='G'){ //补杠
                            in >> tmp;
                            card = GET_CARD(tmp);
                            if (player == game.me) //我补杠
                                game.remove_handcard(card); //把这张牌从手牌移出
                            else { //其他玩家补杠
                                game.hand_count[player]--; //玩家手牌-1
                                game.remain[card]--; //牌池-1
                            }
                            for (int j = 0; j < game.steal_count[player]; j++) {//补杠操作
                                if (game.steal[player][j][0] == card) {
                                    game.steal[player][j][1] = 0; //杠牌这个value就是1
                                }
                            }
                            last_act = ACT_BU_GANG;
                        }
                    case 'D': //其他玩家摸牌
                        game.hand_count[player]++;
                        game.wall_count--;
                        last_act = ACT_OTHER_DRAW;
                    case 'P':
                        if(tmp[1] == 'L'){ //打牌
                            in >> tmp;
                            last_card = card = GET_CARD(tmp);
                            if (player == game.me) //我打出的牌
                                game.remove_handcard(card);
                            else { //其他玩家打出的牌
                                game.hand_count[player]--;
                                game.remain[card]--;
                            }
                            game.history[game.discard_count++] = card;//打出的牌加入记录
                            last_act = ACT_PLAY;
                        }
                        else if(tmp[1] == 'E'){ //碰
                            in >> tmp;
                            card = GET_CARD(tmp);
                            if (player == game.me) { //自己碰
                                for(int k = 0; k < 2; ++k) game.remove_handcard(last_card); //把碰的牌从手牌移出
                                game.remove_handcard(card); //移除打出的牌
                            }
                            else { //别人碰
                                game.hand_count[player] -= 3;
                                game.remain[last_card] -= 2;
                                game.remain[card]--;
                            }
                            game.steal[player][game.steal_count[player]][0] = last_card;
                            game.steal[player][game.steal_count[player]++][1] = last_card;
                            game.history[game.discard_count - 1] = last_card = card;
                            last_act = ACT_PENG;
                        }
                    case 'C': //吃牌
                        tile target, playout;//被丢弃的牌和打出的牌
                        in >> tmp; card = GET_CARD(tmp);
                        in >> tmp; playout = GET_CARD(tmp);
                        target = last_card; //上家打出的牌
                        if (player == game.me) { //自己吃
                            for (int j = -1; j <= 1; j++) {
                                if (card + j != target)
                                    game.remove_handcard(card + j); //从手牌移除非上家打出的顺子牌
                            }
                            game.remove_handcard(playout); //打出的牌
                        }
                        else { //别人吃
                            game.hand_count[player] -= 3;
                            for (int j = -1; j <= 1; j++) {
                                if (card + j != target)
                                    game.remain[card + j]--;
                            }
                            game.remain[playout]--;
                        }
                        game.steal[player][game.steal_count[player]][0] = card;
                        game.steal[player][game.steal_count[player]++][1] = card + 1;
                        game.history[game.discard_count - 1] = playout;
                        last_card = playout;
                        last_act = ACT_CHI;
                    case 'G':
                        bool angang = false;
                        tile gang_card = 0; //如果是别人暗杠，value一定为0
                        if(last_act == ACT_DRAW || last_act == ACT_OTHER_DRAW){ //从上回合动作判断是否暗杠
                            angang = true;
                        }
                        if (player == game.me) { //自己杠
                            if (angang) gang_card = me_draw_card;
                            else gang_card = game.last_card; 
                            for(int k = 0; k < 3; ++k) game.remove_handcard(gang_card);
                            if (angang) // 暗杠
                                game.remove_handcard(gang_card);
                            else // 明杠
                                game.discard_count--; //因为把上回合打出的牌杠了
                        }
                        else { // 别人杠
                            game.hand_count[player] -= 3;
                            if (angang) // 暗杠
                                game.hand_count[player]--;
                            else { // 明杠
                                game.remain[last_card] -= 3;
                                gang_card = last_card;
                                game.discard_count--;
                            }
                        }
                        game.steal[player][game.steal_count[player]][0] = gang_card;
                        game.steal[player][game.steal_count[player]++][1] = 0; //注意杠牌的1值为0

                }
        }
    } //处理历史数据到此结束
    game.last_card = last_card;
    game.last_player = player;
    game.last_act = last_act;
}