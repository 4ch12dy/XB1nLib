//
// Created by xia0 on 2019/11/7.
//

#include "xia0-bin-lib.h"

char* hex_dump(void* target_addr, uint64_t size){
    uint64_t hex_buffer_size = size*3 + 1;
    char* hex_buffer = (char*)malloc((unsigned long)hex_buffer_size);
    memset(hex_buffer, 0x0, hex_buffer_size);

    uint8_t* p = (uint8_t*)target_addr;
    char* q = hex_buffer;

    for(int  i = 0; i < size ;i++ ){
        sprintf(q, "%02X ", *p);
        q += 3;
        p ++;
    };
    return hex_buffer;
}

void show_hex_dump(void* target_addr, uint64_t size){
    char* hex_str = hex_dump(target_addr, size);
    XILOG("%s", hex_str);
    free(hex_str);
}

void* map_file_2_mem(const char* file_path){
    int fd;
    int ret;
    struct stat st;
    size_t len_file;
    void *p;

    if ((fd = open(file_path, O_RDWR | O_CREAT,  S_IRWXU | S_IRGRP | S_IROTH)) < 0){
        XELOG("failed to open file:%s", file_path);
        perror("");
        return NULL;
    }
    if ((ret = fstat(fd,&st)) < 0){
        XELOG("error in fstat");
        return NULL;
    }
    len_file = st.st_size;

    if ((p = mmap(NULL,len_file,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED){
        XELOG("error in mmap");
        return NULL;
    }

    if (close(fd)){
        XELOG("error in close");
    }

    return p;
}

XRange* macho64_get_sec_range_by_name(struct mach_header_64* mh, const char* seg_name, const char* sec_name){
    int offset = 0;
    struct mach_header_64* header = (struct mach_header_64*)mh;

    if(header->magic != MH_MAGIC_64) {
        return NULL;
    }

    XRange* the_segment = malloc(sizeof(XRange));

    offset = sizeof(struct mach_header_64);
    int ncmds = header->ncmds;

    while(ncmds--) {
        /* go through all load command to find __TEXT segment*/
        struct load_command * lcp = (struct load_command *)((uint8_t*)header + offset);
        offset += lcp->cmdsize;

        if(lcp->cmd == LC_SEGMENT_64) {
            struct segment_command_64 * curSegment = (struct segment_command_64 *)lcp;
            struct section_64* curSection = NULL;

            if(strcmp(curSegment->segname, seg_name)){
                continue;
            }

            if (!sec_name || curSegment->nsects == 0 ) {
                if(!strcmp(curSegment->segname, seg_name)){
                    uint64_t memAddr = curSegment->vmaddr;
                    the_segment->start = memAddr;
                    the_segment->end = the_segment->start + curSegment->vmsize;
                    return the_segment;
                }
            }

            curSection = (struct section_64*)((uint8_t*)curSegment + sizeof(struct segment_command_64));
            for (int i = 0; i < curSegment->nsects; i++) {
                if (!strcmp(curSection->sectname, sec_name)) {
                    uint64_t memAddr = curSection->addr;
                    the_segment->start = memAddr;
                    the_segment->end = the_segment->start + curSection->size;
                    return the_segment;
                }

                curSection = (struct section_64*)((uint8_t*)curSection + sizeof(struct section_64));
            }
        }
    }
    return the_segment;
}

// ==================================================================arm64 lib==================================================================

