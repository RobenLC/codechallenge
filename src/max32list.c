
#include "max32list.h"
/*
* insert the value into link-list which contains the largest 32 items
* and keep the number of link-list on 32, in this case, we dont have
* maintain total number of data which can prevent the memory issue
*/
static int max32ListInsert(struct data_linklist_s **list, uint16_t val, int number)
{
    struct data_linklist_s *head=0, *ptr=0, *newlist=0, *pre=0;

    head = *list;

    newlist = malloc(sizeof(struct data_linklist_s));
    if (!newlist) {
        printf("Error!! memory allocate for new link list item failed !!\n");
        return -1;
    }

    memset(newlist, 0, sizeof(struct data_linklist_s));
    newlist->spayload = val;

    ptr = head;

    if (!ptr) {
        *list = newlist;
        return (number + 1);
    }

    while (ptr) {
        if (val >= ptr->spayload) {
            pre = ptr;
            ptr = ptr->next;
        } else {
            break;
        }
    }

    if (ptr) {
        if (pre) {
            pre->next = newlist;
            newlist->next = ptr;
            if (number == 32) {
                *list = head->next;
                free(head);
            } else {
                return (number + 1);
            }
        } else {
            if (number < 32) {
                newlist->next = ptr;
                *list = newlist;
                return (number + 1);
            }
        }
    } else {
        pre->next = newlist;
        if (number == 32) {
            *list = head->next;
            free(head);
        } else {
            return (number + 1);
        }
    }

    return number;
}

/*
* shift the msb data into an unsigned 32 bits varible, and retrieve the lsb data 
* process 3 bytes a time, and treat the len == 1 and len == 2 as special case
*/
int sortDataMax32(struct data_linklist_s **maxlist, uint16_t *lastlist, uint8_t *datain, int len)
{
    int cnt=0, idx=0, rst=0, num=0;
    uint8_t *src=0;
    uint32_t tmp=0;
    struct data_linklist_s *ptr=0;

    src = datain;
    
    while(len) {
        if (len == 1) {
            tmp = src[0];
            //tmp <<= 4;
            lastlist[cnt%32] = tmp;
            num = max32ListInsert(maxlist, lastlist[cnt%32], num);
            cnt ++;

            len = 0;
        } else if (len == 2) {
            tmp = src[0];
            tmp <<= 8;
            tmp |= src[1];
            //tmp <<= 8;
            lastlist[cnt%32] = (tmp >> 4) & 0xfff;
            num = max32ListInsert(maxlist, lastlist[cnt%32], num);
            cnt ++;
            lastlist[cnt%32] = tmp & 0xf;
            num = max32ListInsert(maxlist, lastlist[cnt%32], num);
            cnt ++;

            len = 0;
        } else {  /* (len >= 3) */
           tmp = src[0]; 
           tmp <<= 8;
           tmp |= src[1];
           tmp <<= 8;
           tmp |= src[2];

           lastlist[cnt%32] = (tmp >> 12) & 0xfff;
           num = max32ListInsert(maxlist, lastlist[cnt%32], num);
           cnt ++;

           lastlist[cnt%32] = tmp & 0xfff;
           num = max32ListInsert(maxlist, lastlist[cnt%32], num);
           cnt ++;

           len -= 3;
           src += 3;
        }
    }

    /* print the result on screen */
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

