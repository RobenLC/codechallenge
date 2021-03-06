
#include "helper.h"

int mem_dump(uint8_t *src, int size)
{
    int inc=0;

    printf("memdump sz%d: \n", size);

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

int readfile(char *filename, uint8_t **data)
{
    int ret=0, len=0;
    FILE *fin=0;
    uint8_t *datain=0;
  
    fin = fopen(filename, "r");
    if (!fin) {
        printf("file [%s] does not exist\n", filename);
        return -1;
    }

    ret = fseek(fin, 0, SEEK_END);
    if(ret) {
        printf("input seek end failed ret: %d\n", ret);
        fclose(fin);
        return -2;
    }

    len = ftell(fin);
    printf("file len of input file is %d \n", len);
    
    datain = malloc(len);
    if (!datain) {
        printf("memory allocate failed size: %d \n", len);
        fclose(fin);
        return -3;
    }
    
    ret = fseek(fin, 0, SEEK_SET);
    if(ret) {
        printf("input seek begin failed ret: %d\n", ret);
        fclose(fin);
        return -4;
    }

    ret = fread(datain, 1, len, fin);
    printf("file read size %d ret: %d \n", len, ret);
  
    fclose(fin);

    *data = datain;

    return len;
}

int print2file(char *filename, struct data_linklist_s *max32list, int size, uint16_t *last32)
{
    int ix=0, len=0, cnt=0, rst=0, base=0;
    FILE *fout=0;
    char str[128]={0};
    struct data_linklist_s *tmplist=0;

    cnt = size;
  
    fout = fopen(filename, "w+");
    if (!fout) {
        printf("file [%s] can't open\n", filename);
        return -1;
    }

    sprintf(str,"--Sorted Max 32 Values--\n");
    len = strlen(str);
    fwrite(str, 1, len, fout);
    tmplist = max32list;
    while (tmplist) {
        sprintf(str, "%d\n", tmplist->spayload);
        len = strlen(str);
        fwrite(str, 1, len, fout);

        tmplist = tmplist->next;
    }

    sprintf(str,"--Last 32 Values--\n");
    len = strlen(str);
    fwrite(str, 1, len, fout);

    rst = cnt % 32;

    if (cnt > 32) {
        base = 32;
    } else {
        base = cnt;
    }

    for (ix=0; ix < base; ix++) {
        sprintf(str,"%d\n", last32[(ix+rst)%base]);
        len = strlen(str);
        fwrite(str, 1, len, fout);
    }

    return 0;
}

