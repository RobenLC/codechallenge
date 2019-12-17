
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct lastitem_s{
    uint16_t	lpayload;
};

struct sort12bit_linklist_s{
    struct sort12bit_linklist_s *next;
    uint16_t spayload;
};

static int mem_dump(char *src, int size)
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

static int slinklistInsert(struct sort12bit_linklist_s **list, uint16_t val)
{
    struct sort12bit_linklist_s *head=0, *ptr=0, *new=0, *lst=0;

    head = *list;

    new = malloc(sizeof(struct sort12bit_linklist_s));
    if (!new) {
        printf("Error!! memory allocate for new link list item failed !!\n");
        return -1;
    }

    memset(new, 0, sizeof(struct sort12bit_linklist_s));
    new->spayload = val;

    ptr = head;
    while(ptr) {

        if (ptr->spayload >= val) {
            
            if (ptr == head) {
                new->spayload = head->spayload;
                head->spayload = val;
            }
            new->next = head->next;
            head->next = new;
            break;
        }
        lst = ptr;
        ptr = ptr->next; 
    }
 
    if (!ptr) {
        if (!lst) {
            *list = new;
        } else {
            lst->next = new;
        }
    }

    return 0;
}

static int sort12bits(struct sort12bit_linklist_s **maxlist, struct lastitem_s *lastlist, uint8_t *datain, int len)
{
    int cnt=0, idx=0, rst=0;
    uint8_t *src=0;
    uint32_t tmp=0;
    struct sort12bit_linklist_s *ptr=0;

    src = datain;
    
    while(len) {
        if (len >= 3) {
           tmp = src[0]; 
           tmp <<= 8;
           tmp |= src[1];
           tmp <<= 8;
           tmp |= src[2];

           lastlist[cnt%32].lpayload = (tmp >> 12) & 0xfff;
           slinklistInsert(maxlist, lastlist[cnt%32].lpayload);
           cnt ++;

           lastlist[cnt%32].lpayload = tmp & 0xfff;
           slinklistInsert(maxlist, lastlist[cnt%32].lpayload);
           cnt ++;

           len -= 3;
           src += 3;
        } else {
            switch(len) {
                case 1:
                    tmp = src[0];
                    tmp <<= 4;
                    lastlist[cnt%32].lpayload = tmp;
                    slinklistInsert(maxlist, lastlist[cnt%32].lpayload);
                    cnt ++;
                    break;
                case 2:
                    tmp = src[0];
                    tmp <<= 8;
                    tmp = src[1];
                    tmp <<= 8;
                    lastlist[cnt%32].lpayload = (tmp >> 12) & 0xfff;
                    slinklistInsert(maxlist, lastlist[cnt%32].lpayload);
                    cnt ++;
                    lastlist[cnt%32].lpayload = tmp & 0xfff;
                    slinklistInsert(maxlist, lastlist[cnt%32].lpayload);
                    cnt ++;
                    break;
                default:
                    break;
            }
            
            len = 0;
        }
    }

    printf("total: %d\n", cnt);
    idx = 0;
    ptr = *maxlist;

    while(ptr) {
        printf("%d.%d \n", idx, ptr->spayload);
        idx++;
        ptr = ptr->next;
    }
    
    rst = cnt % 32;
    for(idx=0; idx < 32; idx++) {
        printf("last %d.%d\n", idx, lastlist[(idx+rst)%32].lpayload);
    }

    return cnt;
}

void main(int argc, char *argv[])
{    
    char str[128];
    int ix=0, ret=0, len=0, rst=0, cnt=0;
    FILE *fin=0, *fout=0;
    char *datin=0;
    struct sort12bit_linklist_s *maxlist=0, *tmplist=0;
    struct lastitem_s lastlist[32];
    
    printf("argc: %d \n", argc);

    if (argc < 3) {
        printf("Warning!!! usage: executable file + input file + output file \n");
        return;
    }

    printf("input: %s\n", argv[1]);
    printf("output: %s\n", argv[2]);

    fout = fopen(argv[2], "w+");
    if (!fout) {
        printf("file [%s] can't open\n", argv[2]);
        return;
    }

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
    fclose(fin);

    mem_dump(datin, len);
    /* start */
    memset(lastlist, 0, sizeof(struct lastitem_s)*32);

    ret = sort12bits(&maxlist, lastlist, datin, len);
    if (ret < 0) {
        printf("Error!! sort ret: %d \n", ret);
    } else {
        printf("sort ret: %d \n", ret);
        cnt = ret;

        sprintf(str,"--Sorted Max 32 Values--\n");
        len = strlen(str);
        fwrite(str, 1, len, fout);

        tmplist = maxlist;
        if (cnt > 32) {
            len = cnt - 32;
            while(len) {
                if (tmplist) {
                    tmplist = tmplist->next;
                } else {
                    printf("Error!! max list not match len %d \n", len);
                    break;
                }
                len --;
            }
        }

        while (tmplist) {
            sprintf(str, "%d\n", tmplist->spayload);
            len = strlen(str);
            fwrite(str, 1, len, fout);

            tmplist = tmplist->next;
        }

        sprintf(str,"--Last 32 Values--\n");
        len = strlen(str);
        fwrite(str, 1, len, fout);

        if (cnt < 32) {
            for (ix=0; ix < cnt; ix++) {
                sprintf(str,"%d\n", lastlist[ix].lpayload);
                len = strlen(str);
                fwrite(str, 1, len, fout);
            }
        } else {
            rst = cnt % 32;
            for (ix=0; ix < 32; ix++) {
                sprintf(str,"%d\n", lastlist[(ix+rst)%32].lpayload);
                len = strlen(str);
                fwrite(str, 1, len, fout);
            }
        }
    }

    while(maxlist) {
        tmplist = maxlist;
        maxlist = maxlist->next;
        free(tmplist);
    }

    if (datin) {
        free(datin);
    }

    fclose(fout);

}
