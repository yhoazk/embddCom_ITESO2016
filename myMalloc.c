#include <stdio.h>
#include <stdlib.h>



typedef struct _RamMemType {
    unsigned char* start;
    unsigned char* current;
    unsigned char* end;
    unsigned blks;
}RamMem;

RamMem myAlloc;


int vfnInit(RamMem* mem){
    unsigned retval = 1;
    mem->start =(unsigned char*) malloc(0xffff);
    mem->end =(unsigned char*)( mem->start + (0xffff - 1));
    if(mem->start != NULL)
    {
        retval = 0;
    }
    return retval;
}


int main(void){
    printf("wrk");
    printf("Result: %d\n", vfnInit(&myAlloc));
    return 0;
}
