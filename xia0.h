//
// Created by xia0 on 2019/11/5.
//

#define XSLOG(log, ...)	printf("[+] " log, ##__VA_ARGS__)
#define XELOG(log, ...)	fprintf(stderr, "[-] " log, ##__VA_ARGS__);perror(":")
#define XILOG(log, ...)	printf("[*] " log, ##__VA_ARGS__)
#define XDLOG(log, ...)	fprintf(stderr,"[D] " log, ##__VA_ARGS__)

