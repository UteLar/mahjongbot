#include "data.h"
#include "jsoncpp/json.h"
#include <iostream>
using namespace std;


bool can_hu();
Game game;
double evaluate();
void Msg_out()
{
    string smsg;
    switch (game.last_act)
    {
    case ACT_QUAN:case ACT_RECEIVE:case ACT_BU_HUA:
    case ACT_OTHER_DRAW:case ACT_PENG:case ACT_CHI:
    case ACT_GANG:
        smsg = "PASS";
        break;
    case ACT_DRAW:    //可以暗杠或打牌或胡牌
        if (can_hu()) { smsg = "HU"; break; }

        int choice = -1;//choice is int from 0 to 14, where 0 means gang while 1-14 means play card.
        double max_value = -1.0;

        tile tem_lastcard = game.last_card;//暂存数据
 
        tile tem_card = game.can_an_gang();
        if (tem_card!=0) { //模拟杠牌，假定下一步抓打
            for (int i = 0; i < 4; ++i)game.remove_handcard(game.last_card);
            game.steal[game.me][game.steal_count[game.me]][0] = game.last_card;
            game.steal[game.me][game.steal_count[game.me]++][1] = 0;
            game.last_act = ACT_GANG;
            game.last_card = tem_card;
            max_value = evaluate();
            choice = 0;
            //回复牌型
            for (int i = 0; i < 4; ++i)game.add_handcard(game.last_card);
            game.steal_count[game.me]--;
            game.last_act = ACT_DRAW;
            game.last_card = tem_lastcard;

        }
        pair<int, double> P_playcard = game.decide_play_card();
        if (P_playcard.second > max_value) 
            choice = P_playcard.first + 1;//下标加1
        if (choice == 0)  smsg = "GANG";
        else smsg = "PLAY " + TILE_NAME[game.hand[choice - 1]];
        break;


    case ACT_PLAY:    //是否胡牌或吃或碰或杠
        if (can_hu()) { smsg = "HU"; break; }
        if (game.last_player == game.me) { smsg = "PASS"; break; }

        using namespace chi;
        tile tem_lastcard = game.last_act;
        int tem_lastplayer = game.last_player;

        
        int choice = -1; //0-2 左中右吃，3碰 4杠 ， -1 means pass
        tile tem_cardplay = 0;//如果打，怎么打？
        double max_value = evaluate(); //do nothing

        int tem_canchi = game.can_chi();

        if (tem_canchi != 0) {
            for (int ii = -1; ii <= 1; ++ii) {//left mid right eat
                if (tem_canchi & (1 << (ii + 1))) {

                    game.steal[game.me][game.steal_count[game.me]][0] = game.last_card - ii;//1 0  -1
                    game.steal[game.me][game.steal_count[game.me]++][1] = game.last_card - ii + 1;//2 +1 0
                    game.remove_handcard(game.last_card + DELTA_THREE_EAT[ii + 1][0]);//
                    game.remove_handcard(game.last_card + DELTA_THREE_EAT[ii + 1][1]);//(1,2)(-1,1)(-1,-2)
                    game.discard_count--;
                    game.last_act = ACT_CHI;
                    game.last_player = game.me;
                    pair<int, double> P_solution = game.decide_play_card();
                    if (P_solution.second > max_value) {
                        choice = ii + 1;
                        max_value = P_solution.second;
                        tem_cardplay = game.hand[P_solution.first];
                    }

                    game.steal_count[game.me]--;
                    for(int sss=0;sss<2;++sss) game.add_handcard(game.last_card+DELTA_THREE_EAT[ii + 1][sss]);
                    game.discard_count++;
                    game.last_act = ACT_PLAY;
                    game.last_player = tem_lastplayer;
                    
                }
            }//for
        }//if

        if (game.can_peng()) { 
            
            for (int m = 0; m < 2; ++m) {
                game.steal[game.me][game.steal_count[game.me]][m] = game.last_card;
                game.remove_handcard(game.last_card);
            }
            game.steal_count[game.me]++;
            --game.discard_count;
            game.last_act = ACT_PENG;
            game.last_player = game.me;
            
            pair<int, double> P_solution = game.decide_play_card();

            if (P_solution.second > max_value) {
                choice = 3;
                max_value = P_solution.second;
                tem_cardplay = game.hand[P_solution.first];
            }
            game.steal_count[game.me]--;
            for (int mm = 0; mm < 2; ++mm)game.add_handcard(game.last_card);
            ++game.discard_count;
            game.last_act = ACT_PLAY;
            game.last_player = tem_lastplayer;

        }
        if (game.can_ming_gang()) {
            for (int j = 0; j < 3; ++j)game.remove_handcard(game.last_card);
            game.steal[game.me][game.steal_count[game.me]][0] = game.last_card;
            game.steal[game.me][game.steal_count[game.me]++][1] = 0;
            game.discard_count--;
            game.last_act = ACT_GANG;
            game.last_player = game.me;//假设抓打，进行评估
            double tem_value = evaluate();
            if (tem_value > max_value) { choice = 4; max_value = tem_value; }

            game.steal_count[game.me]--;
            for (int j = 0; j < 3; ++j)game.add_handcard(game.last_card);
            game.discard_count++;
            game.last_act = ACT_PLAY;
            game.last_player = tem_lastplayer;
        }
        if (choice == -1) { smsg = "PASS"; }
        else if (choice >= 0 && choice <= 2) {//+1 0 -1
            smsg = "CHI ";
            smsg += TILE_NAME[game.last_card - choice + 1];
            smsg += " ";
            smsg += TILE_NAME[tem_cardplay];
        }
        else if (choice == 3) {
            smsg = "PENG ";
            smsg += TILE_NAME[tem_cardplay];
        }
        else if (choice == 4) {
            smsg = "GANG";
        }
        else { cout << "This is impossible 2333." << endl; }
        break;
    case ACT_BU_GANG: //是否抢杠胡
        if (game.last_player != game.me) { smsg = "PASS"; break; }
        if (can_hu())smsg = "HU";
        else smsg = "PASS";
        break;
    }
    Json::Value outputJSON;
    outputJSON["response"] = smsg;
    cout << outputJSON << endl;
}