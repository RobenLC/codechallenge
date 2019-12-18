
#include "max32list.h"

static int max32ListInsert(struct data_linklist_s **list, uint16_t val)
{
    struct data_linklist_s *head=0, *ptr=0, *new=0, *lst=0;

    head = *list;

    new = malloc(sizeof(struct data_linklist_s));
    if (!new) {
        printf("Error!! memory allocate for new link list item failed !!\n");
        return -1;
    }

    memset(new, 0, sizeof(struct data_linklist_s));
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

int sortDataMax32(struct data_linklist_s **maxlist, uint16_t *lastlist, uint8_t *datain, int len)
{
    int cnt=0, idx=0, rst=0;
    uint8_t *src=0;
    uint32_t tmp=0;
    struct data_linklist_s *ptr=0;

    src = datain;
    
    while(len) {
        if (len >= 3) {
           tmp = src[0]; 
           tmp <<= 8;
           tmp |= src[1];
           tmp <<= 8;
           tmp |= src[2];

           lastlist[cnt%32] = (tmp >> 12) & 0xfff;
           max32ListInsert(maxlist, lastlist[cnt%32]);
           cnt ++;

           lastlist[cnt%32] = tmp & 0xfff;
           max32ListInsert(maxlist, lastlist[cnt%32]);
           cnt ++;

           len -= 3;
           src += 3;
        } else {
            switch(len) {
                case 1:
                    tmp = src[0];
                    tmp <<= 4;
                    lastlist[cnt%32] = tmp;
                    max32ListInsert(maxlist, lastlist[cnt%32]);
                    cnt ++;
                    break;
                case 2:
                    tmp = src[0];
                    tmp <<= 8;
                    tmp = src[1];
                    tmp <<= 8;
                    lastlist[cnt%32] = (tmp >> 12) & 0xfff;
                    max32ListInsert(maxlist, lastlist[cnt%32]);
                    cnt ++;
                    lastlist[cnt%32] = tmp & 0xfff;
                    max32ListInsert(maxlist, lastlist[cnt%32]);
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
        printf("last %d.%d\n", idx, lastlist[(idx+rst)%32]);
    }

    return cnt;
}

