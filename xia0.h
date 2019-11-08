//
// Created by xia0 on 2019/11/5.
//

#ifndef XIL2CPPDUMPER_XIA0_H
#define XIL2CPPDUMPER_XIA0_H

#define XSLOG(log, ...)	printf("[+] " log, ##__VA_ARGS__)
#define XELOG(log, ...)	fprintf(stderr, "[-] " log, ##__VA_ARGS__);perror(":")
#define XILOG(log, ...)	printf("[*] " log, ##__VA_ARGS__)
#define XDLOG(log, ...)	fprintf(stderr,"[D] " log, ##__VA_ARGS__)
#endif //XIL2CPPDUMPER_XIA0_H
