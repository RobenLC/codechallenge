#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int mem_dump(char *src, int size);
static int readfile(char *filename, char **data);
static int print2file(char *filename, struct data_linklist_s *max32list, int size, uint16_t *last32);

#endif