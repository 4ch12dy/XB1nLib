//
// Created by xia0 on 2019/11/7.
//

#ifndef XIL2CPPDUMPER_ARM_H
#define XIL2CPPDUMPER_ARM_H


#define SET_BIT(insn, pos, bit) do{ \
                                        if(bit){insn |= 1<<pos;}else{insn &= ~(1<<pos);}    \
                                            }while(0)

#define GET_BIT(insn, pos) ((insn) >> (pos)&1)

#endif //XIL2CPPDUMPER_ARM_H
