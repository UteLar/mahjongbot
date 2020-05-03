
#ifndef __MAHJONG_ALGORITHM__STRINGIFY_H__
#define __MAHJONG_ALGORITHM__STRINGIFY_H__

#include "tile.h"

namespace mahjong {

    /**
     * @brief �ַ�����ʽ��
     * - ���ƣ���=m ��=s ��=p����׺ʹ��Сд��ĸ��һ����ͬ��ɫ�����ƿɺϲ�ʹ����һ����׺����123m��678s�ȵȡ�
     * - ���ƣ���������=ESWN���з���=CFP��ʹ�ô�д��ĸ������������ĺ�׺z�������й�ϰ��˳��567zΪ�з��ס�
     * - �ԡ���������Ӣ��[]����ѡ�ö���+���ֱ�ʾ������Դ�����ֵľ���������£�
     *    - �ԣ���ʾ�ڼ����������ϼҴ������[567m,2]��ʾ57���6�򣨵�2�ţ������ڲ�ָ�����ֵģ�Ĭ��Ϊ�Ե�1�š�
     *    - ������ʾ���ļҴ����1Ϊ�ϼң�2Ϊ�Լң�3Ϊ�¼ң���[999s,3]��ʾ���¼ҵ�9�������ڲ�ָ�����ֵģ�Ĭ��Ϊ���ϼҡ�
     *    - �ܣ��������ƣ������ڲ�ָ�����ֵģ�����Ϊ�ǰ��ܡ����磺[SSSS]��ʾ�����ϣ�[8888p,1]��ʾ�������ϼҵ�8����������Ϊ5��6��7ʱ����ʾ�Ӹܡ����磺[1111s,6]��ʾ���Լҵ�1����������1���Ӹܡ�
     * - ����
     *    - [EEEE][CCCC][FFFF][PPPP]NN
     *    - 1112345678999s9s
     *    - [WWWW,1][444s]45m678pFF6m
     *    - [EEEE]288s349pSCFF2p
     *    - [123p,1][345s,2][999s,3]6m6pEW1m
     *    - 356m18s1579pWNFF9p
     */

     /**
      * @addtogroup stringify
      * @{
      */

      /**
       * @name error codes
       * @{
       *  �����ƵĴ�����
       */
#define PARSE_NO_ERROR 0                                ///< �޴���
#define PARSE_ERROR_ILLEGAL_CHARACTER -1                ///< �Ƿ��ַ�
#define PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT -2            ///< ���ֺ���ȱ�ٺ�׺
#define PARSE_ERROR_WRONG_TILES_COUNT_FOR_FIXED_PACK -3 ///< ��¶�������������Ŀ
#define PARSE_ERROR_CANNOT_MAKE_FIXED_PACK -4           ///< �޷���ȷ������¶
#define PARSE_ERROR_TOO_MANY_FIXED_PACKS -5             ///< �����鸱¶��һ���Ϸ��������4��¶��
#define PARSE_ERROR_TOO_MANY_TILES -6                   ///< ������
#define PARSE_ERROR_TILE_COUNT_GREATER_THAN_4 -7        ///< ĳ���Ƴ��ֳ���4ö

       /**
        * @}
        */

        /**
         * @brief ������
         * @param [in] str �ַ���
         * @param [out] tiles ��
         * @param [in] max_cnt �Ƶ��������
         * @retval > 0 ʵ���Ƶ�����
         * @retval == 0 ʧ��
         */
    intptr_t parse_tiles(const char* str, tile_t* tiles, intptr_t max_cnt);

    /**
     * @brief �ַ���ת��Ϊ���ƽṹ������
     * @param [in] str �ַ���
     * @param [out] hand_tiles ���ƽṹ
     * @param [out] serving_tile �ϵ���
     * @retval PARSE_NO_ERROR �޴���
     * @retval PARSE_ERROR_ILLEGAL_CHARACTER �Ƿ��ַ�
     * @retval PARSE_ERROR_NO_SUFFIX_AFTER_DIGIT ���ֺ���ȱ�ٺ�׺
     * @retval PARSE_ERROR_WRONG_TILES_COUNT_FOR_FIXED_PACK ��¶�������������Ŀ
     * @retval PARSE_ERROR_CANNOT_MAKE_FIXED_PACK �޷���ȷ������¶
     * @retval PARSE_ERROR_TOO_MANY_FIXED_PACKS �����鸱¶��һ���Ϸ��������4��¶��
     * @retval PARSE_ERROR_TOO_MANY_TILES ������
     * @retval PARSE_ERROR_TILE_COUNT_GREATER_THAN_4 ĳ���Ƴ��ֳ���4ö
     */
    intptr_t string_to_tiles(const char* str, hand_tiles_t* hand_tiles, tile_t* serving_tile);

    /**
     * @brief ��ת��Ϊ�ַ���
     * @param [in] tiles ��
     * @param [in] tile_cnt �Ƶ�����
     * @param [out] str �ַ���
     * @param [in] max_size �ַ�����󳤶�
     * @return intptr_t д����ַ�����
     */
    intptr_t tiles_to_string(const tile_t* tiles, intptr_t tile_cnt, char* str, intptr_t max_size);

    /**
     * @brief ����ת��Ϊ�ַ���
     * @param [in] packs ����
     * @param [in] pack_cnt ���������
     * @param [out] str �ַ���
     * @param [in] max_size �ַ�����󳤶�
     * @return intptr_t д����ַ�����
     */
    intptr_t packs_to_string(const pack_t* packs, intptr_t pack_cnt, char* str, intptr_t max_size);

    /**
     * @brief ���ƽṹת��Ϊ�ַ���
     * @param [in] hand_tiles ���ƽṹ
     * @param [out] str �ַ���
     * @param [in] max_size �ַ�����󳤶�
     * @return intptr_t д����ַ�����
     */
    intptr_t hand_tiles_to_string(const hand_tiles_t* hand_tiles, char* str, intptr_t max_size);

    /**
     * end group
     * @}
     */

}

#endif