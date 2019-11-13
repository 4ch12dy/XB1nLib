//
// Created by xia0 on 2019/11/7.
//
#include "arm64.h"

// convert ins bytes(4 bytes) to uint32
uint32_t bytes_2_uint32(uint8_t* bytes){
    uint32_t value_uint32 = 0;

    value_uint32 = bytes[0] | (uint32_t)bytes[1] << 8 | (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;

    return value_uint32;
}

uint32_t arm64_insn_from_addr(void* addr){
    uint32_t insn = *((uint32_t*)addr);
    return insn;
}

// arm64 add
/*   arm64 add imm
 *
 *  | 31   30   29   28   27   26   25   24   23   22   21            10  9       5  4       0|
 *  +----+----+----+----+----+----+----+----+----+----+-----------------+----------+----------+
 *  | sf | 0  | 0  | 1  | 0  | 0  | 0  | 1  | 0  | sh |    imm12        |   Rn     |    Rd    |
 *  +----+----+----+----+----+----+----+----+----+----+-----------------+----------+----------+
*/

int arm64_is_add_imm(uint32_t insn){
    return (insn & 0xFF800000) == 0x91000000;
}

int64_t arm64_add_decode_imm(uint32_t insn){
    assert(arm64_is_add_imm(insn));
    const  int mask12 = (1 << 12) - 1;
    int sh = (insn >> 22) & 1;

    return sh ? (uint64_t)(((insn >> 10) & mask12 ) << 12) : (uint64_t)((insn >> 10) & mask12);
}

// arm64 adr
/*   arm64 adr
 *   | 31   30   29   28   27   26   25   24   23                             5   4            0 |
 *   +----+----+----+----+----+----+----+----+----------------------------------+----------------+
 *   | 0  |  immlo  | 1  | 0  | 0  | 0  | 0  |             immhhi               |         Rd     |
 *   +----+----+----+----+----+----+----+----+----------------------------------+----------------+
 *
 */
int arm64_is_adr(uint32_t insn){
    return (insn & 0x9F000000) == 0x10000000;
}

int arm64_adr_decode_imm(uint32_t insn){
    assert(arm64_is_adr(insn));
    const int mask19 = (1 << 19) -1;
    const int mask2 = 3;

    uint64_t imm = ((insn >> 5) & mask19) << 2 | (insn >> 29) &mask2;
    uint64_t msbt = (imm >> 20) & 1;

    // -1 = 0xFFFFFFFF
    return ((((uint64_t)1 << 20) - msbt) << 21) | (int64_t)imm;
}

void* arm64_adr_decode(void* pc, uint32_t insn){
    int64_t imm = arm64_adr_decode_imm(insn);
    int64_t adr_dst_value = (uint64_t)pc + imm;
    return (void*)adr_dst_value;
}

// arm64 adrp
/* C3.4.6 PC-rel. addressing
 *   31  30   29 28       24 23                5 4    0
 * +----+-------+-----------+-------------------+------+
 * | 1  | immlo | 1 0 0 0 0 |       immhi       |  Rd  |
 * +----+-------+-----------+-------------------+------+
 */
int arm64_is_adrp(uint32_t insn) {
    return (insn & 0x9F000000) == 0x90000000;
}

// https://android.googlesource.com/toolchain/binutils/+/nougat-dev/binutils-2.25/gold/aarch64.cc aarch64_adrp_decode_imm
// https://denolib.github.io/v8-docs/constants-arm64_8h_source.html
// http://armconverter.com/hextoarm/
// http://kitoslab-eng.blogspot.com/2012/10/armv8-aarch64-instruction-encoding.html

int64_t arm64_adrp_decode_imm(uint32_t insn){
    const int mask19 = (1 << 19) - 1;
    const int mask2 = 3;
    assert(arm64_is_adrp(insn));
    // 21-bit imm encoded in adrp.
    uint64_t imm = ((insn >> 29) & mask2) | (((insn >> 5) & mask19) << 2);
    // Retrieve msb of 21-bit-signed imm for sign extension.
    uint64_t msbt = (imm >> 20) & 1;
    // Real value is imm multipled by 4k. Value now has 33-bit information.
    int64_t value = imm << 12;
    // Sign extend to 64-bit by repeating msbt 31 (64-33) times and merge it
    // with value.
    return ((((uint64_t)(1) << 32) - msbt) << 33) | value;
}

void* arm64_adrp_decode(void* addr_pc, uint32_t insn){
    int64_t adrp_imm = arm64_adrp_decode_imm(insn);
    int64_t adrp_dst_value = ((uint64_t)addr_pc & ~((1 << 12) - 1)) + adrp_imm;
    return (void*)adrp_dst_value;
}

// arm64 nop
/*   arm64 nop
 *
 *  | 31            22| 21   20 19   18  16 15     12  11     8  7    5 4         0|
 *  +-----------------+----+-------+-------+---------+---------+-------+-----------+
 *  | 1 1 0 1 0 1 0 1 | 0  | 0  0  | 0 1 1 | 0 0 1 0 | 0 0 0 0 | 0 0 0 | 1 1 1 1 1 |
 *  +-----------------+----+-------+-------+---------+---------+-------+-----------+
 *                                                      CRm       op2
*/

int arm64_is_nop(uint32_t insn){
    return (insn == 0xD503201F);
}
// arm64 movz
/*   arm64 movz
 *
 *  | 31  30 29  28        23|22 21 20                 5|4    0|
 *  +----+-----+-------------+-----+--------------------+------+
 *  | sf | 1 0 | 1 0 0 1 0 1 | hw  |       imm16        |  Rd  |
 *  +----+-----+-------------+-----+--------------------+------+
 *         opc
*/

int arm64_is_movz(uint32_t insn){
    return (insn & 0x7F800000) == 0x52800000;
}


// encode
uint32_t make_insn_adrp(int rd, int imm) {
    uint32_t insn = ~0U;
    int i = 0;

    SET_BIT(insn, 24, 0);
    SET_BIT(insn, 25, 0);
    SET_BIT(insn, 26, 0);
    SET_BIT(insn, 27, 0);

    // imm = imm >> 12;

    // make up rd
    for (i = 0; i < 5; i++) {
        int bit = GET_BIT(rd, i);
        SET_BIT(insn, i, bit);
    }
    // immhi
    int immhi = 0;
    immhi = imm >> 2;

    for (i = 0; i < 19; i++) {
        int bit = GET_BIT(immhi, i);
        SET_BIT(insn, i + 5, bit);
    }
    // immlo
    int immlo = 0;
    immlo = imm & 3;

    for (i = 0; i < 2; i++) {
        int bit = GET_BIT(immlo, i);
        SET_BIT(insn, i + 29, bit);
    }
    return insn;
}

uint32_t make_insn_add_imm(uint32_t sf, uint32_t shift, uint32_t imm,
                           uint32_t rn, uint32_t rd) {
    uint32_t insn = 0;
    int i = 0;
    if (imm > 0xfff) {

        return 0;
    }
    SET_BIT(insn, 31, sf);
    SET_BIT(insn, 28, 1);
    SET_BIT(insn, 24, 1);

    // SET_BIT(insn, 23, (shift>>1)&1);
    // SET_BIT(insn, 22, shift&1);
    // imm12
    for (i = 0; i < 12; i++) {
        int bit = GET_BIT(imm, i);
        SET_BIT(insn, i + 10, bit);
    }
    // rn
    for (i = 0; i < 5; i++) {
        int bit = GET_BIT(rn, i);
        SET_BIT(insn, i + 5, bit);
    }
    // rd
    for (i = 0; i < 5; i++) {
        int bit = GET_BIT(rd, i);
        SET_BIT(insn, i, bit);
    }
    return insn;
}

uint32_t make_adrp_add_addressing_a64(uint64_t pc, uint64_t data_addr,
                                      uint32_t reg, uint32_t insn[2]) {
    uint64_t pc_page = pc >> 12;
    uint64_t data_page = data_addr >> 12;

    uint32_t page_off = (uint32_t)(data_page - pc_page);

    uint32_t data_page_off = data_addr & 0xfff;

    insn[0] = make_insn_adrp(reg, page_off);
    insn[1] = make_insn_add_imm(1, 0, data_page_off, reg, reg);

    if (insn[0] != 0 && insn[1] != 0) {
        return 0;
    }
    return 1;
}