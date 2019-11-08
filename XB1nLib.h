//
// Created by xia0 on 2019/11/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>

#include "xia0.h"
#include "macho64.h"
#include "arch/arm64.h"


typedef struct XRange
{
    uint64_t start;
    uint64_t end;
}XRange;


#ifdef __cplusplus
extern "C"
{
#endif

char* hex_dump(void* target_addr, uint64_t size);

void show_hex_dump(void* target_addr, uint64_t size);

void* map_file_2_mem(const char* file_path);

XRange* macho64_get_sec_range_by_name(struct mach_header_64* mh, const char* seg_name, const char* sec_name);

#ifdef __cplusplus
}
#endif

