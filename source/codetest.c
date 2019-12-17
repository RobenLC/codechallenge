
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static int mem_dump(char *src, int size)
{
    int inc=0;

    printf("memdump[0x%.8x] sz%d: \n", (uint32_t)src, size);

    while (inc < size) {
        printf("%.2x ", *src & 0xff);

        if (!((inc+1) % 16)) {
            printf(" %d \n", inc+1);
        }
        inc++;
        src++;
    }

    printf("\n");

    return inc;
}



void main(int argc, char *argv[])
{    
    int ix=0, ret=0, len=0;
    FILE *fin=0, *fout=0;
    char *datin=0;
    
    printf("argc: %d \n", argc);

    if (argc < 3) {
        printf("Warning!!! usage: executable file + input file + output file \n");
        return;
    }

    printf("input: %s\n", argv[1]);
    printf("output: %s\n", argv[2]);


    fin = fopen(argv[1], "r");
    if (!fin) {
        printf("file [%s] does not exist\n", argv[1]);
        return;
    }

    ret = fseek(fin, 0, SEEK_END);
    if(ret) {
        printf("input seek end failed ret: %d\n", ret);
    }

    len = ftell(fin);
    printf("file len of input file is %d \n", len);
    
    datin = malloc(len);
    if (!datin) {
        printf("memory allocate failed size: %d \n", len);
    }
    
    ret = fseek(fin, 0, SEEK_SET);
    if(ret) {
        printf("input seek begin failed ret: %d\n", ret);
    }

    ret = fread(datin, 1, len, fin);
    printf("file read size %d ret: %d \n", len, ret);

    mem_dump(datin, len);

    if (datin) {
        free(datin);
    }

    fclose(fin);
}
