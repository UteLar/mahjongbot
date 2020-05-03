

#ifndef __MAHJONG_ALGORITHM__SHANTEN_H__
#define __MAHJONG_ALGORITHM__SHANTEN_H__

#include "tile.h"

namespace mahjong {

    /**
     * @brief ����ת������
     *
     * @param [in] packs ����
     * @param [in] pack_cnt ���������
     * @param [out] tiles ��
     * @param [in] tile_cnt �Ƶ��������
     * @return intptr_t �Ƶ�ʵ������
     */
    intptr_t packs_to_tiles(const pack_t* packs, intptr_t pack_cnt, tile_t* tiles, intptr_t tile_cnt);

    /**
     * @brief ���ƴ��
     *
     * @param [in] tiles ��
     * @param [in] cnt �Ƶ�����
     * @param [out] cnt_table �Ƶ�������
     */
    void map_tiles(const tile_t* tiles, intptr_t cnt, tile_table_t* cnt_table);

    /**
     * @brief �����ƴ��
     *
     * @param [in] hand_tiles ����
     * @param [out] cnt_table �Ƶ�������
     * @return bool ���ƽṹ�Ƿ���ȷ�����Ƿ���ϣ���¶����*3+������=13
     */
    bool map_hand_tiles(const hand_tiles_t* hand_tiles, tile_table_t* cnt_table);

    /**
     * @brief ����ת������
     *
     * @param [in] cnt_table �Ƶ�������
     * @param [out] tiles ��
     * @param [in] max_cnt �Ƶ��������
     * @return intptr_t �Ƶ�ʵ������
     */
    intptr_t table_to_tiles(const tile_table_t& cnt_table, tile_t* tiles, intptr_t max_cnt);

    /**
     * @brief ��Ч�Ʊ�Ǳ�����
     */
    typedef bool useful_table_t[TILE_TABLE_SIZE];

    /**
     * @addtogroup shanten
     * @{
     */

     /**
      * @addtogroup basic_form
      * @{
      */

      /**
       * @brief ��������������
       *
       * @param [in] standing_tiles ����
       * @param [in] standing_cnt ������
       * @param [out] useful_table ��Ч�Ʊ�Ǳ���Ϊnull��
       * @return int ������
       */
    int basic_form_shanten(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* useful_table);

    /**
     * @brief ���������Ƿ�����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [out] waiting_table ���Ʊ�Ǳ���Ϊnull��
     * @return bool �Ƿ�����
     */
    bool is_basic_form_wait(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* waiting_table);

    /**
     * @brief ���������Ƿ����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [in] test_tile ���Ե���
     * @return bool �Ƿ����
     */
    bool is_basic_form_win(const tile_t* standing_tiles, intptr_t standing_cnt, tile_t test_tile);

    /**
     * end group
     * @}
     */

     /**
      * @addtogroup seven_pairs
      * @{
      */

      /**
       * @brief �߶�������
       *
       * @param [in] standing_tiles ����
       * @param [in] standing_cnt ������
       * @param [out] useful_table ��Ч�Ʊ�Ǳ���Ϊnull��
       * @return int ������
       */
    int seven_pairs_shanten(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* useful_table);

    /**
     * @brief �߶��Ƿ�����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [out] waiting_table ���Ʊ�Ǳ���Ϊnull��
     * @return bool �Ƿ�����
     */
    bool is_seven_pairs_wait(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* waiting_table);

    /**
     * @brief �߶��Ƿ����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [in] test_tile ���Ե���
     * @return bool �Ƿ����
     */
    bool is_seven_pairs_win(const tile_t* standing_tiles, intptr_t standing_cnt, tile_t test_tile);

    /**
     * end group
     * @}
     */

     /**
      * @addtogroup thirteen_orphans
      * @{
      */

      /**
       * @brief ʮ����������
       *
       * @param [in] standing_tiles ����
       * @param [in] standing_cnt ������
       * @param [out] useful_table ��Ч�Ʊ�Ǳ���Ϊnull��
       * @return int ������
       */
    int thirteen_orphans_shanten(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* useful_table);

    /**
     * @brief ʮ�����Ƿ�����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [out] waiting_table ���Ʊ�Ǳ���Ϊnull��
     * @return bool �Ƿ�����
     */
    bool is_thirteen_orphans_wait(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* waiting_table);

    /**
     * @brief ʮ�����Ƿ����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [in] test_tile ���Ե���
     * @return bool �Ƿ����
     */
    bool is_thirteen_orphans_win(const tile_t* standing_tiles, intptr_t standing_cnt, tile_t test_tile);

    /**
     * end group
     * @}
     */

     /**
      * @addtogroup knitted_straight
      * @{
      */

      /**
       * @brief �����������
       *
       * @param [in] standing_tiles ����
       * @param [in] standing_cnt ������
       * @param [out] useful_table ��Ч�Ʊ�Ǳ���Ϊnull��
       * @return int ������
       */
    int knitted_straight_shanten(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* useful_table);

    /**
     * @brief ������Ƿ�����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [out] waiting_table ���Ʊ�Ǳ���Ϊnull��
     * @return bool �Ƿ�����
     */
    bool is_knitted_straight_wait(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* waiting_table);

    /**
     * @brief ������Ƿ����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [in] test_tile ���Ե���
     * @return bool �Ƿ����
     */
    bool is_knitted_straight_win(const tile_t* standing_tiles, intptr_t standing_cnt, tile_t test_tile);

    /**
     * end group
     * @}
     */

     /**
      * @addtogroup honors_and_knitted_tiles
      * @{
      */

      /**
       * @brief ȫ����������
       *
       * @param [in] standing_tiles ����
       * @param [in] standing_cnt ������
       * @param [out] useful_table ��Ч�Ʊ�Ǳ���Ϊnull��
       * @return int ������
       */
    int honors_and_knitted_tiles_shanten(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* useful_table);

    /**
     * @brief ȫ�����Ƿ�����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [out] waiting_table ���Ʊ�Ǳ���Ϊnull��
     * @return bool �Ƿ�����
     */
    bool is_honors_and_knitted_tiles_wait(const tile_t* standing_tiles, intptr_t standing_cnt, useful_table_t* waiting_table);

    /**
     * @brief ȫ�����Ƿ����
     *
     * @param [in] standing_tiles ����
     * @param [in] standing_cnt ������
     * @param [in] test_tile ���Ե���
     * @return bool �Ƿ����
     */
    bool is_honors_and_knitted_tiles_win(const tile_t* standing_tiles, intptr_t standing_cnt, tile_t test_tile);

    /**
     * end group
     * @}
     */

     /**
      * @brief �Ƿ�����
      *
      * @param [in] hand_tiles ���ƽṹ
      * @param [out] useful_table ��Ч�Ʊ�Ǳ���Ϊnull��
      * @return bool �Ƿ�����
      */
    bool is_waiting(const hand_tiles_t& hand_tiles, useful_table_t* useful_table);

    /**
     * end group
     * @}
     */

     /**
      * @name form flags
      * @{
      *  ����
      */
#define FORM_FLAG_BASIC_FORM                0x01  ///< ��������
#define FORM_FLAG_SEVEN_PAIRS               0x02  ///< �߶�
#define FORM_FLAG_THIRTEEN_ORPHANS          0x04  ///< ʮ����
#define FORM_FLAG_HONORS_AND_KNITTED_TILES  0x08  ///< ȫ����
#define FORM_FLAG_KNITTED_STRAIGHT          0x10  ///< �����
#define FORM_FLAG_ALL                       0xFF  ///< ȫ������
      /**
       * @}
       */

       /**
        * @brief ö�ٴ������Ƶļ�������Ϣ
        */
    struct enum_result_t {
        tile_t discard_tile;                    ///< ��������
        uint8_t form_flag;                      ///< ������ʽ
        int shanten;                            ///< ������
        useful_table_t useful_table;            ///< ��Ч�Ʊ�Ǳ�
    };

    /**
     * @brief ö�ٴ������Ƶļ���ص�����
     *
     * @param [in] context ��enum_discard_tile��������contextԭ������
     * @param [in] result ������
     * @retval true ����ö��
     * @retval false ����ö��
     */
    typedef bool (*enum_callback_t)(void* context, const enum_result_t* result);

    /**
     * @brief ö�ٴ�������
     *
     * @param [in] hand_tiles ���ƽṹ
     * @param [in] serving_tile ���ƣ���Ϊ0����ʱ���������Ƶ���Ϣ��
     * @param [in] form_flag ������Щ����
     * @param [in] context �û��Զ����������ԭ���ӻص���������
     * @param [in] enum_callback �ص�����
     */
    void enum_discard_tile(const hand_tiles_t* hand_tiles, tile_t serving_tile, uint8_t form_flag,
        void* context, enum_callback_t enum_callback);

}

/**
 * end group
 * @}
 */

#endif