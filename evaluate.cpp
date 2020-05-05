#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;
typedef unsigned char tile;

const int NUM_PLAYER = 4;
const int NUM_HAND = 14;
const int NUM_STEAL = 4;
const int NUM_FLOWER = 8;
const int NUM_TILE = 144;
const int NUM_SYMBOL = 35; //除开花牌有34种:27数牌+7字牌+0无效牌

tile hand[NUM_HAND]; //手牌
tile steal[NUM_PLAYER][NUM_STEAL][2];//记录各个玩家鸣牌情况，4种
int me, dealer;
int hand_count[NUM_PLAYER], steal_count[NUM_PLAYER], flower_count[NUM_PLAYER];//记录玩家手牌数，鸣牌数，补花数
int wall_count, discard_count;//牌墙数和丢牌数
tile history[NUM_TILE];//已经打出的牌，不包括被其他玩家鸣牌的牌
int remain[NUM_SYMBOL];//各种牌在牌墙剩余的牌数   ?
int turnID;

class Solution { //寻找到的胡牌方案
public:
	vector<tile> need;
	vector<tile> no_need;
	//vector<pair<Mian, tile> > mianzi; //记录我们凑出的面子
};
vector<Solution> record_solution;
void SearchHu(vector<Solution>& record_solution) {

}
int fan_calculator(tile* hand, tile last_card){
	return 0;
}
void copyCard(tile* prob_hand, const tile hand[NUM_HAND], const tile steal[NUM_PLAYER][NUM_STEAL][2]) {
	int j = 0;

	for (j = 0; hand[j] != 0; ++j) {
		prob_hand[j] = hand[j];
	}
	for (int k = 0; k < NUM_STEAL; ++k) {
		if (steal[me][k][1] == 1) {
			prob_hand[j] = prob_hand[j + 1] = prob_hand[j + 2] = prob_hand[j + 3] = steal[me][k][0];
			j += 4;
		}
		else {
			if (steal[me][k][0] == steal[me][k][1] && steal[me][k][0] != 0) {
				prob_hand[j] = prob_hand[j + 1] = prob_hand[j + 2] = steal[me][k][0];
				j += 3;
			}
			else {
				prob_hand[j] = steal[me][k][0] - 1;
				prob_hand[j + 1] = steal[me][k][0];
				prob_hand[j + 2] = steal[me][k][1];
				j += 3;
			}
		}
	}
	
}
void clearMianzi(tile* card_info) {
	bool used[35] = { 0 };
	for (int i = 1; i <= 34; ++i) {
		if (used[i]) continue;
		if (card_info[i] == 3) {
			card_info[i] = 0;
		}
		if (i <= 7 && card_info[i] == 1 && card_info[i + 1] == 1 && card_info[i + 2] == 1) {
			card_info[i] = card_info[i + 1] = card_info[i + 2] = 0;
			used[i] = used[i + 1] = used[i + 2] = 1;
		}		//万
		if (i <= 16 && i > 9 && card_info[i] == 1 && card_info[i + 1] == 1 && card_info[i + 2] == 1) {
			card_info[i] = card_info[i + 1] = card_info[i + 2] = 0;
			used[i] = used[i + 1] = used[i + 2] = 1;
		}		//筒
		if (i <= 25 && i > 18 && card_info[i] == 1 && card_info[i + 1] == 1 && card_info[i + 2] == 1) {
			card_info[i] = card_info[i + 1] = card_info[i + 2] = 0;
			used[i] = used[i + 1] = used[i + 2] = 1;
		}		//条
	}
}
bool tempai(vector<tile>& card_set) {
	tile card_info[36] = { 0 };
	bool used[36] = { 0 };
	vector<int> couple;
	bool is_tempai = 0;

	int triple = 0;
	int straight = 0;
	int single = 0;
	

	for (int i = 0; i < strlen((char*)hand); ++i) {
		card_info[hand[i]]++;
	}
	for (int i = 1; i <= 34; ++i) {		//统计对子
		if (card_info[i] == 2) {
			couple.push_back(i);
		}
		if (card_info[i] == 3) {
			if (card_info[i - 1] == 1 && card_info[i + 1] == 1) {
				couple.push_back(i);
			}
			else if (card_info[i - 2] == 1 && card_info[i - 1] == 1) {
				couple.push_back(i);
			}
			else if (card_info[i + 1] == 1 && card_info[i + 2] == 1) {
				couple.push_back(i);
			}
		}
	}
	
	//如果对字数为6，则为7小对
	if (couple.size() == 6) {			
		for (int i = 1; i <= 34; ++i) {
			if (card_info[i] == 1) {
				card_set.push_back(i);
				break;
			}
		}
		
		return true;
	}	   
	

	if (couple.size() == 2) {			//两对的听牌情况判断
		
		for (vector<int>::iterator i = couple.begin(); i != couple.end(); ++i) {
			card_info[*i] -= 2;
		}		//首先对将子牌中消除
		
		clearMianzi(card_info);

		for (int i = 1; i <= 34; ++i) {
			if (card_info[i] != 0) return false;
		}
		
		for (vector<int>::iterator i = couple.begin(); i != couple.end(); ++i) {
			tile prob_hand[NUM_HAND] = { 0 };
			copyCard(prob_hand, hand, steal);
			prob_hand[13] = *i;
			sort(prob_hand, prob_hand + 14);

			int point = fan_calculator(prob_hand, *i);

			
			if ( point >= 8) {	//符合听牌条件同时也满足胡牌条件
				is_tempai = true;
				card_set.push_back(*i);
			}
			
		}
		
		return is_tempai;

		
	}
	else if (couple.size() == 1) {
	
		if (strlen((char*)hand) == 13) {
			int index[NUM_HAND] = { 1,9,10,18,19,27,28,29,30,31,32,33,34 };
			bool thirteen_yao = false;
			vector<int>card;
			for (int i = 0; i < 13; ++i) {
				if (couple.front() == index[i]) {
					thirteen_yao = true;
					break;
				}
			}
			if (thirteen_yao) {
				for (int i = 0; i < 13; ++i) {
					if (card_info[index[i]] == 0) {
						card.push_back(index[i]);
					}
				}
				if (card.size() == 1) {
				
					card_set.push_back(card.front());
					return true;
				
				}
			}
		}

		for (vector<int>::iterator i = couple.begin(); i != couple.end(); ++i) {
			card_info[*i] -= 2;
		}		//首先对将子牌中消除

		clearMianzi(card_info);

		bool req = 0;
		vector<int> card;
		for (int i = 1; i <= 34; ++i) {
			
			if (i <= 8 && card_info[i] == 1 && card_info[i + 1] == 1) {
				if (i != 1) {
					card.push_back(i - 1);
				}
				if (i != 8) {
					card.push_back(i + 2);
				}
				req = 1;
			}
			else if (i <= 17 && i > 9 && card_info[i] == 1 && card_info[i + 1] == 1) {
				if (i != 1) { 
					card.push_back(i - 1);
				}
				if (i != 17) {
					card.push_back(i + 2);
				}
				req = 1;
			}
			else if (i <= 26 && i > 18 && card_info[i] == 1 && card_info[i + 1] == 1) {
				if (i != 1) {
					card.push_back(i - 1);
				}
				if (i != 26) {
					card.push_back(i + 2);
				}
				req = 1;
			}
			else if (i <= 7 && card_info[i] == 1 && card_info[i + 1] == 0 && card_info[i + 2] == 1) {
				card.push_back(i + 1);
				req = 1;
			}
			else if (i <= 16 && i > 9 && card_info[i] == 1 && card_info[i + 1] == 0 && card_info[i + 2] == 1) {
				card.push_back(i + 1);
				req = 1;
			}
			else if (i <= 25 && i > 18 && card_info[i] == 1 && card_info[i + 1] == 0 && card_info[i + 2] == 1) {
				card.push_back(i + 1);
				req = 1;
			}
		
		}
		if (!req) return false;
		
		
		for (vector<int>::iterator i = card.begin(); i != card.end(); ++i) {
			tile prob_hand[NUM_HAND] = { 0 };
			copyCard(prob_hand, hand, steal);
			prob_hand[13] = *i;
			sort(prob_hand, prob_hand + 14);

			int point = fan_calculator(prob_hand, *i);
			if (point >= 8) {	//符合听牌条件同时也满足胡牌条件
				is_tempai = 1;
				card_set.push_back(*i);
				
			}

		}

		return is_tempai;;

	}
	else if (couple.size() == 0) {
		//十三幺
		if (strlen((char*)hand) == 13) {
			int index[NUM_HAND] = { 1,9,10,18,19,27,28,29,30,31,32,33,34 };
			bool thirteen_yao = true;
		
			for (int i = 0; i < 13; ++i) {
				if (card_info[index[i]] == 0) {
					thirteen_yao = false;
					break;
				}
			}
			if (thirteen_yao) {
				
				for (int i = 0; i < 13; ++i) {
					card_set.push_back(index[i]);
				}

				return true;
			}
		
		}
		//七星不靠
		if (strlen((char*)hand) == 13) {
			tile prob_hand[NUM_HAND] = { 0 };
			copyCard(prob_hand, hand, steal);
			
			set<int> star;
			for (int i = 1; i <= 27; ++i) {
				if (card_info[i] == 1) {
					star.insert(i);
				}
			}
			if (star.size() <= 7) {
				vector<int> wan;
				vector<int> pin;
				vector<int> tiao;
				int num_w = 0;
				int num_p = 0;
				int num_t = 0;

				for (set<int>::iterator i = star.begin(); i != star.end(); ++i) {
					if (*i <= 9) {
						wan.push_back(*i % 9 + 1);
					}
					else if (*i <= 18) {
						pin.push_back(*i % 9 + 1);
					}
					else if (*i <= 27) {
						tiao.push_back(*i % 9 + 1);
					}
				}
				num_w = wan.size(); num_p = pin.size(); num_t = tiao.size();

				if (star.size() == 7 && num_w <= 3 && num_p <= 3 && num_t <= 3) {
					int used[10] = { 0 };
					bool ap[3] = { 1,1,1 };

					if (num_w > 1) {
						for (int i = 1; i < wan.size(); ++i) {
							if (abs(wan[i] - wan[i - 1]) % 3 != 0) {
								ap[0] = 0;
								break;
							}
							if (i == 1) {
								used[wan[i - 1]]++;
							}
							used[wan[i]]++;
						}
					}
					else if (num_w == 1) {
						used[wan[0]]++;
					}

					if (num_p > 1) {
						for (int i = 1; i < pin.size(); ++i) {
							if (abs(pin[i] - pin[i - 1]) % 3 != 0) {
								ap[1] = 0;
								break;
							}
							if (i == 1) {
								used[pin[i - 1]]++;
							}
							used[pin[i]]++;
						}
					}
					else {
						used[pin[0]]++;
					}

					if (num_t > 1) {
						for (int i = 1; i < tiao.size(); ++i) {
							if (abs(tiao[i] - tiao[i - 1]) % 3 != 0) {
								ap[2] = 0;
								break;
							}
							if (i == 1) {
								used[tiao[i - 1]]++;
							}
							used[tiao[i]]++;
						}
					}
					else {
						used[tiao[0]]++;
					}

					if (ap[0] == 1 && ap[1] == 1 && ap[2] == 1) {
						bool differ = 1;
						for (int i = 1; i <= 9; ++i) {
							if (used[i] > 1) {
								differ = 0;
								break;
							}
						}
						if (differ) {
							
							for (int i = 28; i <= 34; ++i) {
								if (card_info[i] == 0) {
									card_set.push_back(i);
									return true;
								}

							}
						}
					}

				}
				else if (star.size() == 6 && num_w <= 3 && num_p <= 3 && num_t <= 3) {
					bool zi_full = 1;
					for (int i = 28; i <= 34; ++i) {
						if (card_info[i] == 0) {
							zi_full = 0;
							break;
						}
					}
					if (zi_full) {
						printf("seven star test: zi_full has successfully entered!\n");
						int used[10] = { 0 };
						bool ap[3] = { 1,1,1 };

						if (num_w > 1) {
							for (int i = 1; i < wan.size(); ++i) {
								if (abs(wan[i] - wan[i - 1]) % 3 != 0) {
									ap[0] = 0;
									break;
								}
								if (i == 1) {
									used[wan[i - 1]]++;
								}
								used[wan[i]]++;
							}
						}
						else if (num_w == 1) {
							used[wan[0]]++;
						}

						if (num_p > 1) {
							for (int i = 1; i < pin.size(); ++i) {
								if (abs(pin[i] - pin[i - 1]) % 3 != 0) {
									ap[1] = 0;
									break;
								}
								if (i == 1) {
									used[pin[i - 1]]++;
								}
								used[pin[i]]++;
							}
						}
						else {
							used[pin[0]]++;
						}

						if (num_t > 1) {
							for (int i = 1; i < tiao.size(); ++i) {
								if (abs(tiao[i] - tiao[i - 1]) % 3 != 0) {
									ap[2] = 0;
									break;
								}
								if (i == 1) {
									used[tiao[i - 1]]++;
								}
								used[tiao[i]]++;
							}
						}
						else {
							used[tiao[0]]++;
						}

						if (ap[0] == 1 && ap[1] == 1 && ap[2] == 1) {
							bool differ = 1;
							for (int i = 1; i <= 9; ++i) {
								if (used[i] > 1) {
									differ = 0;
									break;
								}
							}
							if (differ) {
								for (int i = 1; i <= 9; ++i) {
									if (!used[i]) {
										if (abs(i - wan[0]) % 3 == 0) {
											card_set.push_back(i);
										}
										else if (abs(i - pin[0]) % 3 == 0) {
											card_set.push_back(9 + i);
										}
										else if (abs(i - tiao[0]) % 3 == 0) {
											card_set.push_back(18 + i);
										}
									}
								}
								return true;
								
							}
						}
					}
				}

				
			}
		}
		
		clearMianzi(card_info);

		vector<int> card;
		for (int i = 1; i <= 34; ++i) {
			if (card_info[i] == 1) {
				card.push_back(i);
			}
		}

		if (card.size() == 1) {

			for (vector<int>::iterator i = card.begin(); i != card.end(); ++i) {
				tile prob_hand[NUM_HAND] = { 0 };
				copyCard(prob_hand, hand, steal);
				prob_hand[13] = *i;
				sort(prob_hand, prob_hand + 14);

				if (fan_calculator(prob_hand,*i) >= 8) {	//符合听牌条件同时也满足胡牌条件
					card_set.push_back(*i);
					is_tempai = 1;
				}

			}

			
		}
		return is_tempai;
	}
	//return false;
}
double calcuPbin(tile card) {		//参数可调
	if (card > 27) {
		if (turnID < 75) {
			return ((double)remain[card] / (double)wall_count) * 4;
		}
		else {
			return (double)remain[card] / (double)wall_count;
		}
	}
	else {
		if (turnID < 75) {
			if (history[card] == 1) {
				return ((double)remain[card] / (double)wall_count) * 3.5;
			}
			else if (history[card] == 2) {
				return ((double)remain[card] / (double)wall_count)* 2;
			}
			else if (history[card] == 3) {
				return ((double)remain[card] / (double)wall_count) * 0.9;
			}
			else return 0;
		}
		else {
			return ((double)remain[card] / (double)wall_count) * 0.75;
		}
	}
}
double calcuPointout(tile card) {
	if (card > 27) {
		if (remain[card] <= 2) {
			return 2;
		}
		if (remain[card] == 3) {
			if (turnID <= 12) {
				return 1.5;
			}
			else if (turnID <= 80) {
				return 0.9;
			}
		}
	}
	else {
		if (remain[card] == 2) {
			if (turnID <= 20) {
				return 1.5;
			}
			else {
				return 1.25;
			}
		}
		if (remain[card] == 1) {
			if (turnID <= 20) {
				return 2;
			}
			else {
				return 1.15;
			}
		}
		if (remain[card] == 3) {
			return 1.3;
		}

	}
}
int calcuDist(tile* currHand, const tile* hand,tile couple,int depth[],vector<tile> total_set[]) {
	return 1;
}
double evaluateIN() {
	vector<tile> card_set;
	bool _is_tempai = tempai(card_set);
	if (_is_tempai) {
		double sum_point = 0;
		for (vector<tile>::iterator i = card_set.begin(); i != card_set.end(); ++i) {
			tile prob_hand[NUM_HAND] = { 0 };
			copyCard(prob_hand, hand, steal);
			prob_hand[13] = *i;

			int point = fan_calculator(prob_hand, *i);
			sum_point += calcuPbin(*i) * point;
		}
		return card_set.size() + sum_point + 233;
		
	}
	else {
		vector<tile> solution[100];
		int dist[100] = { 0 };
		double sum_pb = 0;
		int size = 0;
		tile currHand[NUM_HAND] = { 0 };
		copyCard(currHand, hand, steal);
		sort(currHand, currHand + 13);
		calcuDist(currHand, hand, 0, dist, solution);
		int minD = 10000;
		for (int i = 0;i = solution[i].size() != 0; ++i) {
			for (vector<tile>::iterator j = solution[i].begin(); j != solution[i].end(); ++j) {
				sum_pb += calcuPbin(*j);
			}
			size += solution[i].size();
			if (dist[i] < minD); {
				minD = dist[i];
			}
		}
		
		return (double)size / (double)minD + sum_pb;
	}

}
double evalutate(tile card_out) {
	return evaluateIN() + calcuPointout(card_out);
}
int main() {

}