
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "max32list.h"
#include "helper.h"

void main(int argc, char *argv[])
{    
    int ret=0, len=0, rst=0, cnt=0;
    char *datin=0;
    struct data_linklist_s *maxlist=0, *tmplist=0;
    uint16_t lastlist[32]={0};
    
    printf("argc: %d \n", argc);

    if (argc < 3) {
        printf("Warning!!! usage: executable file + input file + output file \n");
        return;
    }

    printf("input: %s\n", argv[1]);
    printf("output: %s\n", argv[2]);

    len = readfile(argv[1], &datin);
	if (len < 0) {
		goto end;
	}
    mem_dump(datin, len);

    /* start data processing */

    ret = sortDataMax32(&maxlist, lastlist, datin, len);
    if (ret < 0) {
        printf("Error!! sort ret: %d \n", ret);
		goto end;
    }
	
    printf("sort ret: %d \n", ret);
    cnt = ret;

	ret = print2file(argv[2],maxlist,cnt,lastlist);
    if (ret < 0) {
        printf("Error!! print result to file failed, ret: %d \n", ret);
    }
end:
    while(maxlist) {
        tmplist = maxlist;
        maxlist = maxlist->next;
        free(tmplist);
    }

    if (datin) {
        free(datin);
    }

}


