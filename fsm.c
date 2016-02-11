/**  TEST STUFF START **/
#include <stdio.h>
/**  TEST STUFF END **/



/*
The implementation should meet the following parameters:
ECU ID = 0x60
Tester ID = 0xF0
Both answers, positive and negative, shall be supported
Time in between bytes for the ECU answer, P1 = 1ms
Time in between Tester command and the beginning of the ECU
answer, P2 = 5ms
Service seed shall be totally random and shall not be the same.
Implement the XTEA cipher algorithm of 64-bits with a key of 128-bits 3 rounds

 * */


/*****************/
/** Crypto stuff start **/

/* Se tienen 64 bits de datos en v[0] y v[1] y 128 bits de clave en k[0] - k[3] */

void Cifrar(unsigned int num_rounds, unsigned int v[2], unsigned int const k[4]) {
    unsigned int i;
    unsigned int v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void vfnCyph(unsigned int rounds, unsigned int *data, unsigned int  *key ){
    unsigned int i;
    unsigned int d0 = *data;
    unsigned int d1 = *(data+1);
    unsigned int sum  = 0;
    unsigned int delta  = 0x9E3779B9;

    for(i = 0; i < rounds; i++){
        d0 += (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
        sum += delta;
        d1 += (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum>>11) & 3]);
    }

    *data=d0; *(data+1)=d1;
}

void vfnDeCyph(unsigned int rounds, unsigned int *data, unsigned int  *key ){
    unsigned int i;
    unsigned int d0 = *data;
    unsigned int d1 = *(data+1);
    unsigned int sum  = 0;
    unsigned int delta  = 0x9E3779B9;

    for (i=0; i < rounds; i++) {
        d1 -= (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        d0 -= (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
    }

     *data=d0; *(data+1)=d1;
}

void Descifrar(unsigned int num_rounds, unsigned int v[2], unsigned int const k[4]) {
    unsigned int i;
    unsigned int v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}


/** Crypto stuff start **/

/** CRC stuff start **/


/** CRC stuff start **/

/** RandomSeed  stuff **/
/* Pool of entropy */
static unsigned int pui32RndEntropy[16];

static unsigned int ui32RndIndex = 0;
/* Entropy added my api user*/
void RandomAddEntropy(unsigned int ui32Entropy)
{
    //
    // Add this byte to the entropy pool.
    //
    ((unsigned char *)pui32RndEntropy)[ui32RndIndex] = ui32Entropy & 0xff;

    //
    // Increment to the next byte of the entropy pool.
    //
    ui32RndIndex = (ui32RndIndex + 1) & 63;
}


/* Get the random Seed */


unsigned int getRndSeed(void){
    unsigned int ui32A, ui32B, ui32C, ui32D, ui32Temp, ui32Idx;

    //
    // Initialize the digest.
    //
    ui32A = 0x67452301;
    ui32B = 0xefcdab89;
    ui32C = 0x98badcfe;
    ui32D = 0x10325476;

    //
    // Perform the first round of operations.
    //
#define F(a, b, c, d, k, s)                                                   \
    {                                                                         \
        ui32Temp = a + (d ^ (b & (c ^ d))) + pui32RndEntropy[k];         \
        a = (ui32Temp << s) | (ui32Temp >> (32 - s));                         \
    }
    for(ui32Idx = 0; ui32Idx < 16; ui32Idx += 4)
    {
        F(ui32A, ui32B, ui32C, ui32D, ui32Idx + 0, 3);
        F(ui32D, ui32A, ui32B, ui32C, ui32Idx + 1, 7);
        F(ui32C, ui32D, ui32A, ui32B, ui32Idx + 2, 11);
        F(ui32B, ui32C, ui32D, ui32A, ui32Idx + 3, 19);
    }

    //
    // Perform the second round of operations.
    //
#define G(a, b, c, d, k, s)                                                   \
    {                                                                         \
        ui32Temp = (a + ((b & c) | (b & d) | (c & d)) +                       \
                    pui32RndEntropy[k] + 0x5a827999);                    \
        a = (ui32Temp << s) | (ui32Temp >> (32 - s));                         \
    }
    for(ui32Idx = 0; ui32Idx < 4; ui32Idx++)
    {
        G(ui32A, ui32B, ui32C, ui32D, ui32Idx + 0, 3);
        G(ui32D, ui32A, ui32B, ui32C, ui32Idx + 4, 5);
        G(ui32C, ui32D, ui32A, ui32B, ui32Idx + 8, 9);
        G(ui32B, ui32C, ui32D, ui32A, ui32Idx + 12, 13);
    }

    //
    // Perform the third round of operations.
    //
#define H(a, b, c, d, k, s)                                            \
    {                                                                  \
        ui32Temp = a + (b ^ c ^ d) + pui32RndEntropy[k] + 0x6ed9eba1; \
        a = (ui32Temp << s) | (ui32Temp >> (32 - s));                      \
    }
    for(ui32Idx = 0; ui32Idx < 4; ui32Idx += 2)
    {
        H(ui32A, ui32B, ui32C, ui32D, ui32Idx + 0, 3);
        H(ui32D, ui32A, ui32B, ui32C, ui32Idx + 8, 9);
        H(ui32C, ui32D, ui32A, ui32B, ui32Idx + 4, 11);
        H(ui32B, ui32C, ui32D, ui32A, ui32Idx + 12, 15);

        if(ui32Idx == 2)
        {
            ui32Idx -= 3;
        }
    }

    //
    // Use the first word of the resulting digest as the random number seed.
    //
    return(ui32A + 0x67452301);

}



/******** FSM STUFF START *********/


#define ECU_ID (0x60)
#define TESTER_ID (0xF0)


typedef enum STATES_t{
    IDLE = 'A',
    CHECK_IF_HEADER,
    HEADER_FORMAT,
    HEADER_TGT,
    HEADER_SRC,
    CHECK_IF_SID,
    GET_DATA_BYTES,
    GET_KEY,
    CHECK_SUM,
    WAIT_5MS,
    SEND
}STATES;

STATES ui32ProcessFSM( unsigned char ubMessage){
    static STATES currentState = CHECK_IF_HEADER;
    static STATES nextState = IDLE;

    switch(currentState){
        default:
        case CHECK_IF_HEADER:
            if(ubMessage == 82){
                nextState = HEADER_FORMAT;
                printf("Header Correct\n");
            }
            else{
                printf("Idle\n");
            }
            break;
        case HEADER_FORMAT:
            if(ubMessage == ECU_ID){
                nextState = HEADER_TGT;
                printf("ECU_ID Correct\n");
            }
            break;
        case HEADER_TGT:
            if(ubMessage == TESTER_ID){
                nextState = HEADER_SRC;
                printf("TESTER_ID Correct\n");
            }
            break;
        case HEADER_SRC:
            if(ubMessage == 27){
                nextState = CHECK_IF_SID;
                printf("Header Correct\n");
            }
            break;
        case CHECK_IF_SID:
            if(ubMessage == 1){
                nextState = CHECK_SUM;
                printf("Header Correct\n");
            }
            break;
        case GET_DATA_BYTES:
            if(ubMessage == 82){
                nextState = HEADER_FORMAT;
                printf("Header Correct\n");
            }
            break;
        case GET_KEY:
            if(ubMessage == 82){
                nextState = HEADER_FORMAT;
                printf("Header Correct\n");
            }
            break;
        case CHECK_SUM:
            if(ubMessage == 82){
                nextState = SEND;
                printf("Header Correct\n");
            }
            break;
        case WAIT_5MS:
            break;
        case SEND:
            break;
    }
    currentState = nextState;
    return currentState;
}


/******** FSM STUFF END *********/


/**  TEST MAIN START **/

int main(){

    unsigned char dataCyph[] = {
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        0
    };


    unsigned char keyCyph[] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F'
    };

    unsigned char input = 0;
    STATES state = IDLE;
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());
    RandomAddEntropy(getRndSeed());
    printf("%x\n", getRndSeed());

    printf("Testing FSM\n");

    printf("Testing CYPH Orig Data %s\n", dataCyph);
    vfnCyph(3, dataCyph, keyCyph);
    printf("Testing CYPH     Data %x %x %x %x %x %x %x %x\n", dataCyph[0], dataCyph[1],dataCyph[2],dataCyph[3],dataCyph[4],dataCyph[5],dataCyph[6],dataCyph[7]);

    vfnDeCyph(3, dataCyph, keyCyph);
    printf("Testing DECYPH   Data %x %x %x %x %x %x %x %x\n", dataCyph[0], dataCyph[1],dataCyph[2],dataCyph[3],dataCyph[4],dataCyph[5],dataCyph[6],dataCyph[7]);

/*
    while(state != SEND){
    input = getchar();
    (void) getchar(); // cosume the intro char
        state = ui32ProcessFSM(input);
        printf("State:%c\n", state);
    }
*/
    return 0;
}
/**  TEST MAIN END **/
