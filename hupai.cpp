//
// Created by 22325 on 2020/4/23.
//
#include<vector>
#include "data.h"
#include "MahjongGB.h"


void Hu::SearchHu(int mian_num,int jiang_num, int begin){
    if(need_card.size() > ding) return; //需要的牌超过接受范围，不考虑

    if(mian_num == hand_count / 3 + 1){
        //已经找到了一种胡牌的牌型
        if(record_solution.size() == 85){
            cout<<"use for test\n";
        }
        Solution solution;
        solution.need = need_card;
        solution.mianzi = mian_type;
        for(int i = 0; i < hand_count; ++i){
            if(!visit[i]){ //之前凑面子的时候就是以此为基准的，所以可以不用再弄一次了
                bool in_need = false;
                for(int j = 0; j < solution.need.size();++j){ //看看手牌中有没有我们需要的牌
                    if(hand[i]==solution.need[j]){
                        solution.need.erase(solution.need.begin()+j); //手牌中有就可以从need中去除
                        in_need = true;
                        break;
                    }
                }
                if(!in_need){ //这张手牌我们不需要
                    solution.no_need.push_back(hand[i]); //手牌中不需要的牌，也就是和牌用不到的牌
                }
            }
        }
        record_solution.push_back(solution); //记录这种胡牌方案
        return;
    }
    else if(mian_num < hand_count/3) {
        //从hand[begin]开始来做顺子或者刻子
        //先考虑刻子的情况，也就是AAA情形
        for (int i = begin; i < hand_count; ++i) {
            //之前做面子已经用到的牌不考虑了,字牌不考虑，同样的牌也不考虑了
            if (!visit[i] && (i < 1 || hand[i] != hand[i - 1])) {
                visit[i] = true; //征用这张牌

                int que = 2; //凑刻子缺的牌数
                for (int j = i + 1; j < hand_count; ++j) {
                    if (visit[j]) continue; //已经被征用了
                    if (hand[j] == hand[i] && que >= 0) { //找到可以用的，如果找到两张就够了
                        visit[j] = true; //征用这张牌
                        que--;
                        if (que == 0) break; //找到了两张一样的牌，结束循环
                    }
                }
                if (que == 0 || que <= remain[hand[i]]) { //我们已经有刻子或者这张牌还可能出现

                    for (int j = 0; j < que; ++j) { //还缺的牌加入need
                        need_card.push_back(hand[i]);
                    }
                    mian_type.emplace_back(PENG, hand[i]); //加入面子
                    SearchHu(mian_num + 1, jiang_num, i + 1); //寻找下一个面子
                    for (int j = 0; j < que; ++j) {
                        need_card.pop_back();
                    }
                    mian_type.pop_back(); //搜过了，可以弹出
                }
                for (int j = i + 1; j < hand_count && que < 2; ++j) {
                    if (visit[j] && hand[i] == hand[j]) {
                        visit[j] = false; //搜过了，visit变为false
                        que++;
                    }
                }

                //现在考虑顺子的情况，顺子分为三种，我们依次搜索 x x+1 x+2的情况
                //x,x+1,x+2
                //用过了，不是数牌，数值>7不考虑
                if ( IS_SHU(hand[i]) && NUMBER(hand[i]) < 8) {
                    int jiayi = 0, jiaer = 0; //如果在手牌中找到，记录下标
                    bool findyi = false, finder = false;
                    for (int j = i + 1; j < hand_count; ++j) { //寻找手牌中i后能凑顺子的牌
                        if (findyi && finder) break;//都找到了结束循环
                        if (!visit[j] && hand[j] == hand[i] + 1 && !findyi) {
                            visit[j] = true;
                            findyi = true;
                            jiayi = j;
                        }
                        if (!visit[j] && hand[j] == hand[i] + 2 && !finder) {
                            visit[j] = true;
                            finder = true;
                            jiaer = j;
                        }
                    }
                    if ((!findyi && remain[hand[i] + 1] <= 0) || (!finder && remain[hand[i] + 2] <= 0)) {
                        if (findyi) visit[jiayi] = false;
                        if (finder) visit[jiaer] = false; //缺牌并且牌池中没有就不用搜索了
                    } else { //可以凑这个面子
                        if (!findyi) { //没有找到x+1
                            need_card.push_back(hand[i] + 1); //加入需要的牌
                            remain[hand[i] + 1]--; //默认征用这张牌，牌池--
                        }
                        if (!finder) { //没有找到x+2
                            need_card.push_back(hand[i] + 2);
                            remain[hand[i] + 2]--;
                        }
                        mian_type.emplace_back(make_pair(CHI, hand[i] + 1)); //加入面子列
                        SearchHu(mian_num + 1, jiang_num, i + 1); //寻找下一个面子
                        if (!finder) {
                            need_card.pop_back(); //弹出
                            remain[hand[i] + 2]++; //恢复牌池状态
                        } else visit[jiayi] = false;
                        if (!findyi) {
                            need_card.pop_back();
                            remain[hand[i] + 1]++;
                        } else visit[jiaer] = false;
                        mian_type.pop_back();
                    }
                }

                //第二种顺子,x-1,x,x+1,因为之前已经考虑了x,x+1,x+2，所以x-1不应该存在
                if (IS_SHU(hand[i]) && NUMBER(hand[i]) > 1 && NUMBER(hand[i]) < 9) {
                    bool exist = false;
                    for (int j = i - 1; j > 0 && hand[j] >= hand[i] - 1; j--) {
                        if (!visit[j] && hand[j] != hand[i]) {
                            exist = true;
                            break;
                        }
                    }
                    if (!exist) {
                        int after = 0;
                        bool find_after = false;
                        for (int j = i + 1; j < hand_count; ++j) { //寻找手牌中i后能凑顺子的牌
                            if (find_after) break;
                            if (!visit[j] && hand[j] == hand[i] + 1 && !find_after) {
                                visit[j] = true;
                                find_after = true;
                                after = j;
                            }
                        }
                        if (!find_after && remain[hand[i] + 1] <= 0 || remain[hand[i] - 1] <= 0) {
                            if (find_after) visit[after] = false; //缺牌并且牌池中没有就不用搜索了
                        } else {
                            need_card.push_back(hand[i] - 1); //加入需要的牌
                            remain[hand[i] - 1]--; //默认征用这张牌，牌池--
                            if (!find_after) { //没有找到x+1
                                need_card.push_back(hand[i] + 1);
                                remain[hand[i] + 1]--;
                            }
                            mian_type.emplace_back(make_pair(CHI, hand[i])); //加入面子列
                            SearchHu(mian_num + 1, jiang_num, i + 1); //寻找下一个面子

                            need_card.pop_back(); //弹出
                            remain[hand[i] - 1]++; //恢复牌池状态

                            if (!find_after) {
                                need_card.pop_back();
                                remain[hand[i] + 1]++;
                            } else visit[after] = false;
                            mian_type.pop_back();
                        }
                    }
                }

                //第三种 x-2，x-1，x，和前一种情况同理
                if ( IS_SHU(hand[i]) && NUMBER(hand[i]) > 2) {
                    bool exit = false;
                    for (int j = i - 1; j > 0 && hand[j] >= hand[i] - 2; j--) {
                        if (!visit[j] && hand[j] != hand[i]) {
                            exit = true;
                            break;
                        }
                    }
                    if (!exit) {
                        if (remain[hand[i] - 2] <= 0 || remain[hand[i] - 1] <= 0) {

                        } else {
                            need_card.push_back(hand[i] - 1);
                            remain[hand[i] - 1]--;
                            need_card.push_back(hand[i] - 2);
                            remain[hand[i] - 2]--;
                            mian_type.emplace_back(make_pair(CHI, hand[i] - 1));
                            SearchHu(mian_num + 1, jiang_num, i + 1);
                            mian_type.pop_back();
                            need_card.pop_back();
                            remain[hand[i] - 1]++;
                            need_card.pop_back();
                            remain[hand[i] - 2]++;
                        }
                    }
                }
                visit[i] = false;
            }
        }
    }
    else if (jiang_num == 0){ //决定将牌
        for(int i = 0; i < hand_count; ++i){
            if(visit[i] || (i>0&&hand[i]==hand[i-1])) continue;
            visit[i] = true;
            bool find_another = false;
            int another = 0;
            for(int j = i + 1; j < hand_count; ++j){
                if(find_another) break;
                if(hand[i]==hand[j]){
                    visit[j] = true;
                    find_another = true;
                    another = j;
                }
            }
            if(!find_another && remain[hand[i]]<=0){
                visit[i] = false;
                continue; //搜索下一个i
            }
            if(!find_another){
                need_card.push_back(hand[i]);
                remain[hand[i]]--;
            }
            mian_type.emplace_back(make_pair(JIANG,hand[i]));
            SearchHu(mian_num+1, jiang_num+1, i+1);
            if(!find_another){
                need_card.pop_back();
                remain[hand[i]]++;
            }
            else visit[another] = false;
            visit[i] = false;
            mian_type.pop_back();
        }
    }
}

#define MY(field) (game.field[game.me])
typedef vector<pair<int ,string> > FanScore;
vector<FanScore > GetScores(const Game& game, const vector<Solution> &solutions){
    int flower_count = MY(flower_count); //补花数
    int men_feng = game.me; //门风
    int quan_feng  = game.dealer; //圈风
    int hand_count = MY(hand_count);
    int card_from = game.last_player; //供牌人
    string win_tile;
    vector<FanScore > scores;

    vector<pair<string, pair<string, int> > > pack;
    vector<string> hand_card;
    int steal_count = MY(steal_count); //我的鸣牌数
    for(int i = 0; i < steal_count; ++i){
        const tile *steal = MY(steal)[i];
        if (steal[0] == steal[1]) // 碰
            pack.emplace_back(make_pair("PENG", make_pair(TILE_NAME[steal[0]], card_from)));//供牌参数有点神秘，我先放着
        else if (steal[0] + 1 == steal[1]) // 吃
            pack.emplace_back(make_pair("CHI", make_pair(TILE_NAME[steal[0]], card_from)));
        else if (steal[1] == 0) // 杠
            pack.emplace_back(make_pair("GANG", make_pair(TILE_NAME[steal[0]], card_from)));
    }
    //处理完已有的鸣牌，如果是模拟胡牌方案，还要处理剩下的
    for (int i = 0; i < solutions.size(); i++) {
        int isZIMO = 0, isJUEZHANG = 0, isGANG = 0, isLAST = 0;
        if (solutions[i].need.size() == 0) { // 不需要牌，该我们胡了
            if (game.last_act == ACT_DRAW) isZIMO = 1; //自摸和
            if (game.remain[game.last_card] == 0) isJUEZHANG = 1; //和绝张，计4番
            if (game.wall_count == 0) isLAST = 1; //是否为牌墙的最后一张
            if (game.last_act == ACT_GANG || game.last_act == ACT_BU_GANG) isGANG; //复合杠
            bool remove = 0;
            for(int j = 0; j < hand_count; ++j){
                if(!remove && game.hand[j] == game.last_card){
                    remove = true;
                }
                else hand_card.push_back(TILE_NAME[game.hand[j]]); //复制手牌，但不用复制制胜牌
            }
            win_tile = game.last_card;
        }
        else { // 模拟的胡牌方案
            for(int j = 0; j < solutions[i].mianzi.size(); ++j){
                if(solutions[i].mianzi[j].first == 0){
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second]);
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second]);
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second]);
                }
                else if(solutions[i].mianzi[j].first == 1){
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second-1]);
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second]);
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second+1]);
                }
                else if(solutions[i].mianzi[j].first == 2){
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second]);
                    hand_card.push_back(TILE_NAME[solutions[i].mianzi[j].second]);
                }
            }
            win_tile = TILE_NAME[solutions[i].need.back()]; //用需要的最后一张作为制胜牌
            vector<string>::iterator iter = find(hand_card.begin(), hand_card.end(), win_tile);
            hand_card.erase(iter); //把制胜牌从手牌去掉
        }
        vector<pair<int, string> > score =  MahjongFanCalculator(pack, hand_card,win_tile,flower_count,
                                                                 isZIMO, isJUEZHANG, isGANG, isLAST,
                                                                 men_feng, quan_feng);
        scores.push_back(score);
    }
    return scores;
}