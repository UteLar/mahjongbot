

#include "stringify.h"
#include <string.h>
#include <algorithm>
#include <iterator>

namespace mahjong {

    // ������ʵ�ֺ���
    static intptr_t parse_tiles_impl(const char* str, tile_t* tiles, intptr_t max_cnt, intptr_t* out_tile_cnt) {
        //if (strspn(str, "123456789mpsESWNCFP") != strlen(str)) {
        //    return PARSE_ERROR_ILLEGAL_CHARACTER;
        //}

        intptr_t tile_cnt = 0;

#define SET_SUIT_FOR_NUMBERED(value_)       \
    for (intptr_t i = tile_cnt; i > 0;) {   \
        if (tiles[--i] & 0xF0) break;       \
        tiles[i] |= value_;                 \
        } (void)0

#define SET_SUIT_FOR_CHARACTERS()   SET_SUIT_FOR_NUMBERED(0x10)
#define SET_SUIT_FOR_BAMBOO()       SET_SUIT_FOR_NUMBERED(0x20)
#define SET_SUIT_FOR_DOTS()         SET_SUIT_FOR_NUMBERED(0x30)

#define SET_SUIT_FOR_HONOR() \
    for (intptr_t i = tile_cnt; i > 0;) {   \
        if (tiles[--i] & 0xF0) break;       \
        if (tiles[i] > 7) return PARSE_ERROR_ILLEGAL_CHARACTER; \
        tiles[i] |= 0x40;                   \
        } (void)0

#define NO_SUFFIX_AFTER_DIGIT() (tile_cnt > 0 && !(tiles[tile_cnt - 1] & 0xF0))
#define CHECK_SUFFIX() if (NO_SUFFIX_AFTER_DIGIT()) return PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT

        const char* p = str;
        for (; tile_cnt < max_cnt && *p != '\0'; ++p) {
            char c = *p;
            switch (c) {
            case '0': tiles[tile_cnt++] = 5; break;
            case '1': tiles[tile_cnt++] = 1; break;
            case '2': tiles[tile_cnt++] = 2; break;
            case '3': tiles[tile_cnt++] = 3; break;
            case '4': tiles[tile_cnt++] = 4; break;
            case '5': tiles[tile_cnt++] = 5; break;
            case '6': tiles[tile_cnt++] = 6; break;
            case '7': tiles[tile_cnt++] = 7; break;
            case '8': tiles[tile_cnt++] = 8; break;
            case '9': tiles[tile_cnt++] = 9; break;
            case 'm': SET_SUIT_FOR_CHARACTERS(); break;
            case 's': SET_SUIT_FOR_BAMBOO(); break;
            case 'p': SET_SUIT_FOR_DOTS(); break;
            case 'z': SET_SUIT_FOR_HONOR(); break;
            case 'E': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_E; break;
            case 'S': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_S; break;
            case 'W': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_W; break;
            case 'N': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_N; break;
            case 'C': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_C; break;
            case 'F': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_F; break;
            case 'P': CHECK_SUFFIX(); tiles[tile_cnt++] = TILE_P; break;
            default: goto finish_parse;
            }
        }

    finish_parse:
        // һ��������+��׺�����Ѿ�����������˵���ƹ���
        if (NO_SUFFIX_AFTER_DIGIT()) {
            // ������߼�Ϊ�������м�һ�������֣�ֱ�ӽ�������ĺ�׺
            const char* p1 = strpbrk(p, "mspz");
            if (p1 == nullptr) {
                return PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT;
            }

            switch (*p1) {
            case 'm': SET_SUIT_FOR_CHARACTERS(); break;
            case 's': SET_SUIT_FOR_BAMBOO(); break;
            case 'p': SET_SUIT_FOR_DOTS(); break;
            case 'z': SET_SUIT_FOR_HONOR(); break;
            default: return PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT;
            }

            if (p1 != p) {  // �������м������
                return PARSE_ERROR_TOO_MANY_TILES;
            }

            p = p1 + 1;
        }

#undef SET_SUIT_FOR_NUMBERED
#undef SET_SUIT_FOR_CHARACTERS
#undef SET_SUIT_FOR_BAMBOO
#undef SET_SUIT_FOR_DOTS
#undef SET_SUIT_FOR_HONOR
#undef NO_SUFFIX_AFTER_DIGIT
#undef CHECK_SUFFIX

        * out_tile_cnt = tile_cnt;
        return static_cast<intptr_t>(p - str);
    }

    // ������
    intptr_t parse_tiles(const char* str, tile_t* tiles, intptr_t max_cnt) {
        intptr_t tile_cnt;
        if (parse_tiles_impl(str, tiles, max_cnt, &tile_cnt) > 0) {
            return tile_cnt;
        }
        return 0;
    }

    // ���ɸ�¶
    static intptr_t make_fixed_pack(const tile_t* tiles, intptr_t tile_cnt, pack_t* pack, uint8_t offer) {
        if (tile_cnt > 0) {
            if (tile_cnt != 3 && tile_cnt != 4) {
                return PARSE_ERROR_WRONG_TILES_COUNT_FOR_FIXED_PACK;
            }
            if (tile_cnt == 3) {
                if (offer == 0) {
                    offer = 1;
                }
                if (tiles[0] == tiles[1] && tiles[1] == tiles[2]) {
                    *pack = make_pack(offer, PACK_TYPE_PUNG, tiles[0]);
                }
                else {
                    if (tiles[0] + 1 == tiles[1] && tiles[1] + 1 == tiles[2]) {
                        *pack = make_pack(offer, PACK_TYPE_CHOW, tiles[1]);
                    }
                    else if (tiles[0] + 1 == tiles[2] && tiles[2] + 1 == tiles[1]) {
                        *pack = make_pack(offer, PACK_TYPE_CHOW, tiles[2]);
                    }
                    else if (tiles[1] + 1 == tiles[0] && tiles[0] + 1 == tiles[2]) {
                        *pack = make_pack(offer, PACK_TYPE_CHOW, tiles[0]);
                    }
                    else if (tiles[1] + 1 == tiles[2] && tiles[2] + 1 == tiles[0]) {
                        *pack = make_pack(offer, PACK_TYPE_CHOW, tiles[2]);
                    }
                    else if (tiles[2] + 1 == tiles[0] && tiles[0] + 1 == tiles[1]) {
                        *pack = make_pack(offer, PACK_TYPE_CHOW, tiles[0]);
                    }
                    else if (tiles[2] + 1 == tiles[1] && tiles[1] + 1 == tiles[0]) {
                        *pack = make_pack(offer, PACK_TYPE_CHOW, tiles[1]);
                    }
                    else {
                        return PARSE_ERROR_CANNOT_MAKE_FIXED_PACK;
                    }
                }
            }
            else {
                if (tiles[0] != tiles[1] || tiles[1] != tiles[2] || tiles[2] != tiles[3]) {
                    return PARSE_ERROR_CANNOT_MAKE_FIXED_PACK;
                }
                *pack = make_pack(offer, PACK_TYPE_KONG, tiles[0]);
            }
            return 1;
        }
        return 0;
    }

    // �ַ���ת��Ϊ���ƽṹ������
    intptr_t string_to_tiles(const char* str, hand_tiles_t* hand_tiles, tile_t* serving_tile) {
        size_t len = strlen(str);
        if (strspn(str, "0123456789mpszESWNCFP,[]") != len) {
            return PARSE_ERROR_ILLEGAL_CHARACTER;
        }

        pack_t packs[4];
        intptr_t pack_cnt = 0;
        tile_t standing_tiles[14];
        intptr_t standing_cnt = 0;

        bool in_brackets = false;
        tile_t temp_tiles[14];
        intptr_t temp_cnt = 0;
        intptr_t max_cnt = 14;
        uint8_t offer = 0;

        tile_table_t cnt_table = { 0 };

        const char* p = str;
        while (char c = *p) {
            const char* q;
            switch (c) {
            case ',': {  // ��¶��Դ
                if (!in_brackets) {
                    return PARSE_ERROR_ILLEGAL_CHARACTER;
                }
                offer = static_cast<uint8_t>(*++p - '0');
                q = ++p;
                if (*p != ']') {
                    return PARSE_ERROR_ILLEGAL_CHARACTER;
                }
                break;
            }
            case '[': {  // ��ʼһ�鸱¶
                if (in_brackets) {
                    return PARSE_ERROR_ILLEGAL_CHARACTER;
                }
                if (pack_cnt > 4) {
                    return PARSE_ERROR_TOO_MANY_FIXED_PACKS;
                }
                if (temp_cnt > 0) {  // ����[]�����������
                    if (standing_cnt + temp_cnt >= max_cnt) {
                        return PARSE_ERROR_TOO_MANY_TILES;
                    }
                    // �ŵ�������
                    memcpy(&standing_tiles[standing_cnt], temp_tiles, temp_cnt * sizeof(tile_t));
                    standing_cnt += temp_cnt;
                    temp_cnt = 0;
                }

                q = ++p;
                in_brackets = true;
                offer = 0;
                max_cnt = 4;  // ��¶������������4����
                break;
            }
            case ']': {  // ����һ����¶
                if (!in_brackets) {
                    return PARSE_ERROR_ILLEGAL_CHARACTER;
                }
                // ���ɸ�¶
                intptr_t ret = make_fixed_pack(temp_tiles, temp_cnt, &packs[pack_cnt], offer);
                if (ret < 0) {
                    return ret;
                }

                q = ++p;
                temp_cnt = 0;
                in_brackets = false;
                ++pack_cnt;
                max_cnt = 14 - standing_cnt - pack_cnt * 3;  // ���������������ֵ
                break;
            }
            default: {  // ��
                if (temp_cnt != 0) {  // �ظ�����
                    return PARSE_ERROR_TOO_MANY_TILES;
                }
                // ����max_cnt����
                intptr_t ret = parse_tiles_impl(p, temp_tiles, max_cnt, &temp_cnt);
                if (ret < 0) {  // ����
                    return ret;
                }
                if (ret == 0) {
                    return PARSE_ERROR_ILLEGAL_CHARACTER;
                }
                // ���ƴ��
                for (intptr_t i = 0; i < temp_cnt; ++i) {
                    ++cnt_table[temp_tiles[i]];
                }
                q = p + ret;
                break;
            }
            }
            p = q;
        }

        max_cnt = 14 - pack_cnt * 3;
        if (temp_cnt > 0) {  // ����[]�����������
            if (standing_cnt + temp_cnt > max_cnt) {
                return PARSE_ERROR_TOO_MANY_TILES;
            }
            // �ŵ�������
            memcpy(&standing_tiles[standing_cnt], temp_tiles, temp_cnt * sizeof(tile_t));
            standing_cnt += temp_cnt;
        }

        if (standing_cnt > max_cnt) {
            return PARSE_ERROR_TOO_MANY_TILES;
        }

        // ���ĳ���Ƴ���4
        if (std::any_of(std::begin(cnt_table), std::end(cnt_table), [](int cnt) { return cnt > 4; })) {
            return PARSE_ERROR_TILE_COUNT_GREATER_THAN_4;
        }

        // �޴���ʱ��д������
        tile_t last_tile = 0;
        if (standing_cnt == max_cnt) {
            memcpy(hand_tiles->standing_tiles, standing_tiles, (max_cnt - 1) * sizeof(tile_t));
            hand_tiles->tile_count = max_cnt - 1;
            last_tile = standing_tiles[max_cnt - 1];
        }
        else {
            memcpy(hand_tiles->standing_tiles, standing_tiles, standing_cnt * sizeof(tile_t));
            hand_tiles->tile_count = standing_cnt;
        }

        memcpy(hand_tiles->fixed_packs, packs, pack_cnt * sizeof(pack_t));
        hand_tiles->pack_count = pack_cnt;
        *serving_tile = last_tile;

        return PARSE_NO_ERROR;
    }

    // ��ת��Ϊ�ַ���
    intptr_t tiles_to_string(const tile_t* tiles, intptr_t tile_cnt, char* str, intptr_t max_size) {
        bool tenhon = false;
        char* p = str, * end = str + max_size;

        static const char suffix[] = "mspz";
        static const char honor_text[] = "ESWNCFP";
        suit_t last_suit = 0;
        for (intptr_t i = 0; i < tile_cnt && p < end; ++i) {
            tile_t t = tiles[i];
            suit_t s = tile_get_suit(t);
            rank_t r = tile_get_rank(t);
            if (s == 1 || s == 2 || s == 3) {  // ����
                if (r >= 1 && r <= 9) {  // ��Ч��Χ1-9
                    if (last_suit != s && last_suit != 0) {  // ��ɫ���ˣ��Ӻ�׺
                        if (last_suit != 4 || tenhon) {
                            *p++ = suffix[last_suit - 1];
                        }
                    }
                    if (p < end) {
                        *p++ = '0' + r;  // д��һ�������ַ�
                    }
                    last_suit = s;  // ��¼��ɫ
                }
            }
            else if (s == 4) {  // ����
                if (r >= 1 && r <= 7) {  // ��Ч��Χ1-7
                    if (last_suit != s && last_suit != 0) {  // ��ɫ���ˣ��Ӻ�׺
                        if (last_suit != 4) {
                            *p++ = suffix[last_suit - 1];
                        }
                    }
                    if (p < end) {
                        if (tenhon) {  // ���ʽ��׺
                            *p++ = '0' + r;  // д��һ�������ַ�
                        }
                        else {
                            *p++ = honor_text[r - 1];  // ֱ��д��������Ӧ��ĸ
                        }
                        last_suit = s;
                    }
                }
            }
        }

        // д����һ��пռ䣬�����׺
        if (p != str && p < end && (last_suit != 4 || tenhon)) {
            *p++ = suffix[last_suit - 1];
        }

        if (p < end) {
            *p = '\0';
        }
        return static_cast<intptr_t>(p - str);
    }

    // ����ת��Ϊ�ַ���
    intptr_t packs_to_string(const pack_t* packs, intptr_t pack_cnt, char* str, intptr_t max_size) {
        char* p = str, * end = str + max_size;
        tile_t temp[4];
        for (intptr_t i = 0; i < pack_cnt && p < end; ++i) {
            pack_t pack = packs[i];
            uint8_t o = pack_get_offer(pack);
            tile_t t = pack_get_tile(pack);
            uint8_t pt = pack_get_type(pack);
            switch (pt) {
            case PACK_TYPE_CHOW:
                if (p >= end) break;
                *p++ = '[';
                temp[0] = static_cast<tile_t>(t - 1); temp[1] = t; temp[2] = static_cast<tile_t>(t + 1);
                p += tiles_to_string(temp, 3, p, static_cast<intptr_t>(end - p));
                if (p >= end) break;
                *p++ = ',';
                if (p >= end) break;
                *p++ = '0' + o;
                if (p >= end) break;
                *p++ = ']';
                break;
            case PACK_TYPE_PUNG:
                if (p >= end) break;
                *p++ = '[';
                temp[0] = t; temp[1] = t; temp[2] = t;
                p += tiles_to_string(temp, 3, p, static_cast<intptr_t>(end - p));
                if (p >= end) break;
                *p++ = ',';
                if (p >= end) break;
                *p++ = '0' + o;
                if (p >= end) break;
                *p++ = ']';
                break;
            case PACK_TYPE_KONG:
                if (p >= end) break;
                *p++ = '[';
                temp[0] = t; temp[1] = t; temp[2] = t; temp[3] = t;
                p += tiles_to_string(temp, 4, p, static_cast<intptr_t>(end - p));
                if (p >= end) break;
                *p++ = ',';
                if (p >= end) break;
                *p++ = '0' + (is_promoted_kong(pack) ? o | 0x4 : o);
                if (p >= end) break;
                *p++ = ']';
                break;
            case PACK_TYPE_PAIR:
                temp[0] = t; temp[1] = t;
                p += tiles_to_string(temp, 2, p, static_cast<intptr_t>(end - p));
                break;
            default: break;
            }
        }

        if (p < end) {
            *p = '\0';
        }
        return static_cast<intptr_t>(p - str);
    }

    // ���ƽṹת��Ϊ�ַ���
    intptr_t hand_tiles_to_string(const hand_tiles_t* hand_tiles, char* str, intptr_t max_size) {
        char* p = str, * end = str + max_size;
        p += packs_to_string(hand_tiles->fixed_packs, hand_tiles->pack_count, str, max_size);
        if (p < end) p += tiles_to_string(hand_tiles->standing_tiles, hand_tiles->tile_count, p, static_cast<intptr_t>(end - p));
        return static_cast<intptr_t>(p - str);
    }

}