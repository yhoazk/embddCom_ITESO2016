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
    if ((addrs != 0)&&(((int)addrs & 0x03) == 0)){
        r = 1;
    }
    return r;
}

int main(void){
    printf("wrk");
    printf("Result: %d\n", vfnInit(&myAlloc));
    int   x = 0xffff;
    int accptd = 0;

    accptd = is4bAligned(myMalloc(0x04));
    x -= (accptd)? 0x04:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,0x04,x);
    accptd = is4bAligned(myMalloc(0x08));
    x -= (accptd)? 0x08:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,0x08,x);
    accptd = is4bAligned(myMalloc(0x0c));
    x -= (accptd)? 0x0c:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,0x0c,x);

    accptd = is4bAligned(myMalloc(65432));
    x -= (accptd)? 65432:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,65432,x);

    accptd = is4bAligned(myMalloc(64));
    x -= (accptd)? 64:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,64,x);

    accptd = is4bAligned(myMalloc(66));
    x -= (accptd)? 66:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,66,x);

    accptd = is4bAligned(myMalloc(67));
    x -= (accptd)? 67:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,67,x);

    accptd = is4bAligned(myMalloc(64));
    x -= (accptd)? 64:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,64,x);

    accptd = is4bAligned(myMalloc(4));
    x -= (accptd)? 4:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,4,x);

    accptd = is4bAligned(myMalloc(5));
    x -= (accptd)? 5:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,5,x);

    accptd = is4bAligned(myMalloc(68));
    x -= (accptd)? 68:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,68,x);

    accptd = is4bAligned(myMalloc(136));
    x -= (accptd)? 136:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,136,x);

    accptd = is4bAligned(myMalloc(77));
    x -= (accptd)? 77:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,77,x);

    accptd = is4bAligned(myMalloc(89));
    x -= (accptd)? 89:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,89,x);

    accptd = is4bAligned(myMalloc(1));
    x -= (accptd)? 1:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,1,x);

    accptd = is4bAligned(myMalloc(9));
    x -= (accptd)? 9:0;
    printf("Result: %x Rq: %d Rem: %d\n", accptd,9,x);

    return 0;
}
