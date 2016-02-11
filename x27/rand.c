/* random gen */


/** RandomSeed  stuff **/
static unsigned int pui32RndEntropy[16];

static unsigned int ui32RndIndex = 0;
/* Entropy added my api user*/
void RandomAddEntropy(unsigned int ui32Entropy)
{
    // Add this byte to the entropy pool.
    ((unsigned char *)pui32RndEntropy)[ui32RndIndex] = ui32Entropy & 0xff;

    // Increment to the next byte of the entropy pool.
    ui32RndIndex = (ui32RndIndex + 1) & 63;
}


/* Get the random Seed */
unsigned int getRndSeed(void){
    unsigned int ui32A, ui32B, ui32C, ui32D, ui32Temp, ui32Idx;
    // Initialize the digest.
    ui32A = 0x67452301;
    ui32B = 0xefcdab89;
    ui32C = 0x98badcfe;
    ui32D = 0x10325476;

    // Perform the first round of operations.
#define F(a, b, c, d, k, s)                                                   \
    {                                                                         \
        ui32Temp = a + (d ^ (b & (c ^ d))) + pui32RndEntropy[k];              \
        a = (ui32Temp << s) | (ui32Temp >> (32 - s));                         \
    }
    for(ui32Idx = 0; ui32Idx < 16; ui32Idx += 4)
    {
        F(ui32A, ui32B, ui32C, ui32D, ui32Idx + 0, 3);
        F(ui32D, ui32A, ui32B, ui32C, ui32Idx + 1, 7);
        F(ui32C, ui32D, ui32A, ui32B, ui32Idx + 2, 11);
        F(ui32B, ui32C, ui32D, ui32A, ui32Idx + 3, 19);
    }

    // Perform the second round of operations.
#define G(a, b, c, d, k, s)                                                   \
    {                                                                         \
        ui32Temp = (a + ((b & c) | (b & d) | (c & d)) +                       \
                    pui32RndEntropy[k] + 0x5a827999);                         \
        a = (ui32Temp << s) | (ui32Temp >> (32 - s));                         \
    }
    for(ui32Idx = 0; ui32Idx < 4; ui32Idx++)
    {
        G(ui32A, ui32B, ui32C, ui32D, ui32Idx + 0, 3);
        G(ui32D, ui32A, ui32B, ui32C, ui32Idx + 4, 5);
        G(ui32C, ui32D, ui32A, ui32B, ui32Idx + 8, 9);
        G(ui32B, ui32C, ui32D, ui32A, ui32Idx + 12, 13);
    }

    // Perform the third round of operations.
#define H(a, b, c, d, k, s)                                                   \
    {                                                                         \
        ui32Temp = a + (b ^ c ^ d) + pui32RndEntropy[k] + 0x6ed9eba1;         \
        a = (ui32Temp << s) | (ui32Temp >> (32 - s));                         \
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

    // Use the first word of the resulting digest as the random number seed.
    return(ui32A + 0x67452301);

}
