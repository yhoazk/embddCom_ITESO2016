#include <stdio.h>
#include <stdlib.h>



typedef struct _RamMemType {
    unsigned char* start;
    unsigned char* current;
    unsigned char* end;
    unsigned blks;
}RamMem;

static RamMem myAlloc;


int vfnInit(RamMem* mem){
    unsigned retval = 1;
    mem->start =(unsigned char*) malloc(0xffff);
    mem->end =(unsigned char*)( mem->start + (0xffff - 1));
    if(mem->start != NULL)
    {
        mem->current = mem->start;
        retval = 0;
    }
    return retval;
}

/*
 * At this point we take as assumption that current is aligned
 * */
unsigned char* myMalloc(int size){
    unsigned char* retPtr = NULL;
    size += (size&0x03)? 0x04:0; // add padding
    if(size <= (myAlloc.end - myAlloc.current)){
        // the remaining memory is bigger or equal to the requested
        retPtr = myAlloc.current;
        myAlloc.current = (unsigned char*) ((int)(myAlloc.current + size) & ~0x03); // align the memory again
    }
    return retPtr;
}

int memInit(int* start, int size){
}


int is4bAligned(int* addrs){
    int r = 0;
    if (((int)addrs & 0x03) == 0){
        r = 1;
    }
    return r;
}

int main(void){
    printf("wrk");
    printf("Result: %d\n", vfnInit(&myAlloc));
    printf("Result: %x\n", is4bAligned(myMalloc(0x04)));
    printf("Result: %x\n", is4bAligned(myMalloc(0x08)));
    printf("Result: %x\n", is4bAligned(myMalloc(0x0c)));
    printf("Result: %x\n", is4bAligned(myMalloc(65432)));
    printf("Result: %x\n", is4bAligned(myMalloc(64)));
    printf("Result: %x\n", is4bAligned(myMalloc(66)));
    printf("Result: %x\n", is4bAligned(myMalloc(67)));
    printf("Result: %x\n", is4bAligned(myMalloc(64)));
    printf("Result: %x\n", is4bAligned(myMalloc(4)));
    printf("Result: %x\n", is4bAligned(myMalloc(5)));
    printf("Result: %x\n", is4bAligned(myMalloc(68)));
    printf("Result: %x\n", is4bAligned(myMalloc(136)));
    printf("Result: %x\n", is4bAligned(myMalloc(77)));
    printf("Result: %x\n", is4bAligned(myMalloc(89)));
    printf("Result: %x\n", is4bAligned(myMalloc(1)));
    printf("Result: %x\n", is4bAligned(myMalloc(9)));
    return 0;
}
