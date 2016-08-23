/* TEAN implementation according to original spec */

tean( int * v, int * k, int N) {
	unsigned int y=v[0], z=v[1], DELTA=0x9e3779b9 ;
	if (N>0) {
/* coding */
		unsigned int limit=DELTA*N, sum=0 ;
		while (sum!=limit)
			y+= (z<<4 ^ z>>5) + z ^ sum + k[sum&3],
		sum+=DELTA,
		z+= (y<<4 ^ y>>5) + y ^ sum + k[sum>>11 &3] ;
	}
	else {
/* decoding */
		unsigned int sum=DELTA*(-N) ;
		while (sum)
			z-= (y<<4 ^ y>>5) + y ^ sum + k[sum>>11 &3],
		sum-=DELTA,
		y-= (z<<4 ^ z>>5) + z ^ sum + k[sum&3] ;
	}
	v[0]=y, v[1]=z ;
	return ;
}

/**
v gives the plain text of 2 words,
k gives the key of 4 words,
N gives the number of cycles, 32 are recommended,
if negative causes decoding, N must be the same as for coding,
if zero causes no coding or decoding.
assumes 32 bit long and same endian coding or decoding
**/
