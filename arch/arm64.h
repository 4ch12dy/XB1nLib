//
// Created by xia0 on 2019/11/7.
//

#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define SET_BIT(insn, pos, bit) do{ \
                                        if(bit){insn |= 1<<pos;}else{insn &= ~(1<<pos);}    \
                                            }while(0)

#define GET_BIT(insn, pos) ((insn) >> (pos)&1)

#ifdef __cplusplus
extern "C"
{
#endif

    // misc
    uint32_t bytes_2_uint32(uint8_t* bytes);
    uint32_t arm64_insn_from_addr(void* addr);

    // add
    int arm64_is_add_imm(uint32_t insn);
    int64_t arm64_add_decode_imm(uint32_t insn);

    // adr
    int arm64_is_adr(uint32_t insn);
    int arm64_adr_decode_imm(uint32_t insn);
    void* arm64_adr_decode(void* pc, uint32_t insn);

    // adrp
    int arm64_is_adrp(uint32_t insn);
    int64_t arm64_adrp_decode_imm(uint32_t insn);
    void* arm64_adrp_decode(void* addr_pc, uint32_t insn);

    // nop
    int arm64_is_nop(uint32_t insn);

    // mov
    int arm64_is_movz(uint32_t insn);

    // ignore
    uint32_t make_insn_adrp(int rd, int imm);
    uint32_t make_insn_add_imm(uint32_t sf, uint32_t shift, uint32_t imm, uint32_t rn, uint32_t rd);

#ifdef __cplusplus
}
#endif
