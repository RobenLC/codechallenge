#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "max32list.h"

int mem_dump(uint8_t *src, int size);
int readfile(char *filename, uint8_t **data);
int print2file(char *filename, struct data_linklist_s *max32list, int size, uint16_t *last32);

#endif
