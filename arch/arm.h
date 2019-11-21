//
// Created by xia0 on 2019/11/7.
//
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t    Arm_Addr;
typedef uint32_t    Arm_Insn;

// misc
Arm_Addr arm_addr_add(Arm_Addr addr, int32_t insn_count);
uint32_t arm_insn_from_addr(void* addr);

// add
int arm_is_add_rsr(uint32_t insn);
int arm_is_add_r(uint32_t insn);

// adr
int arm_is_adr(uint32_t insn);
int arm_adr_decode_imm(uint32_t insn);
void* arm_adr_decode(void* pc, uint32_t insn);

// adrp
int arm_is_adrp(uint32_t insn);
int64_t arm_adrp_decode_imm(uint32_t insn);
void* arm_adrp_decode(void* addr_pc, uint32_t insn);

// nop
int arm_is_nop(uint32_t insn);

// mov
int arm_is_mov_imm(uint32_t insn);

//ldr
int arm_is_ldr_literal(uint32_t insn);
int32_t arm_ldr_literal_decode_imm(uint32_t insn);

int arm_is_ldr_r(Arm_Insn insn);

#ifdef __cplusplus
}
#endif

