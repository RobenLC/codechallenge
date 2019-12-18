#ifndef _MAX32LIST_H_
#define _MAX32LIST_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct data_linklist_s{
    struct data_linklist_s *next;
    uint16_t spayload;
};

int sortDataMax32(struct data_linklist_s **maxlist, uint16_t *lastlist, uint8_t *datain, int len);

#endif

