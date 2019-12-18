#ifndef _MAX32LIST_H_
#define _MAX32LIST_H_

#include <stdint.h>

struct data_linklist_s{
    struct data_linklist_s *next;
    uint16_t spayload;
};

static int sortDataMax32(struct data_linklist_s **maxlist, uint16_t *lastlist, uint8_t *datain, int len);

#endif

