#include <MD5.h>

unsigned f0( unsigned abcd[] ){
	return ( abcd[1] & abcd[2] ) | ( ~abcd[1] & abcd[3] );
}

unsigned f1( unsigned abcd[] ){
	return ( abcd[3] & abcd[1] ) | ( ~abcd[3] & abcd[2] );
}

unsigned f2( unsigned abcd[] ){
	return abcd[1] ^  abcd[2] ^ abcd[3];
}

unsigned f3( unsigned abcd[]){
	return abcd[2] ^ ( abcd[1] |~ abcd[3] );
}

//typedef unsigned (*DgstFctn)(unsigned a[]);

unsigned *calcKs( unsigned *k)
{
	double s, pwr;
	int i;
	
	pwr = pow(2, 32);
	for(i=0; i<64; i++){
		s = fabs( sin(i+1) );
		k[i] = (unsigned)( s*pwr );
	}
	return k;
}


unsigned rol( unsigned v, short amt)
{
	unsigned msk1 = ( 1<<amt) -1;
	return ( (v >>(32-amt)) & msk1) | ((v<<amt) & ~msk1);
}

unsigned *md5( const char *msg, int mlen)
{
	static Digest h0 = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
	static DgstFctn ff[] = { &f0, &f1, &f2, &f3};
	static short M[] = {1, 5, 3, 7};
	static short O[] = {0, 1, 5, 0};
	static short rot0[] = {7, 12, 17, 22};
	static short rot1[] = {5, 9, 14, 20};
	static short rot2[] = {4, 11, 16, 23};
	static short rot3[] = {6, 10, 15, 21};
	static short *rots[] = {rot0, rot1, rot2, rot3};
	static unsigned kspace[64];
	static unsigned *k;

	static Digest h;
	Digest abcd;
	DgstFctn fctn;
	short m, o,g;
	unsigned f;
	short *rotn;
	union{
		unsigned w[16];
		char b[64];
	}mm;
	int os = 0;
	int grp, grps, q, p;
	unsigned char *msg2;

	if (k == NULL) k= calcKs(kspace);
	
	for(q=0; q<4; q++) h[q] = h0[q];

	{
		grps = 1 + (mlen + 8)/64;
		msg2 = malloc( 64*grps);
		memcpy( msg2, msg, mlen);
		msg2[mlen] = (unsigned char)0x80;
		q = mlen +1;
		while(q <  64*grps){ msg2[q] = 0; q++;}
		{
			WBunion u;
			u.w = 8*mlen;
			q -=8;
			memcpy(msg2+q, &u.w, 4);
		}
	}

	for(grp=0; grp<grps; grp++){
		memcpy( mm.b, msg2+os, 64);
		for(q=0; q<4; q++) abcd[q] = h[q];
		for(p=0; p<4; p++){
			fctn = ff[p];
			rotn = rots[p];
			m = M[p]; o = O[p];
			for(q=0; q<16; q++){
				g = ( m*q +o ) % 16;
				f = abcd[1] + rol( abcd[0] + fctn(abcd) + k[q+16*p] + mm.w[g], rotn[q%4]);

				abcd[0] = abcd[3];
				abcd[3] = abcd[2];
				abcd[2] = abcd[1];
				abcd[1] = f;
			}
		}
		for(p=0; p<4; p++)
			h[p] += abcd[p];
		os += 64;
	}
	if(msg2)
		free(msg2);
	return h;
}

int *md5_int(char *key){
  int j, k;
  char *msg = key;
  unsigned *d = md5(msg, strlen(msg));
  
  WBunion u;
  int *result = (int *)malloc(16*sizeof(int));

  int count =0;
  for(j=0; j<4; j++){
    u.w = d[j];
    for(k=0; k<4;k++){
      result[count++] = u.b[k]- '\0';
    }
  }
  return result;
}


int HexaToDecimal_Bit(char msg){// convert each bit from Haxa to Decimal
    int value;
	switch(msg){
		case '0': value=0; break;
		case '1': value=1; break;
		case '2': value=2; break;
		case '3': value=3; break;
		case '4': value=4; break;
		case '5': value=5; break;
		case '6': value=6; break;
		case '7': value=7; break;
		case '8': value=8; break;
		case '9': value=9; break;
		case 'a': value=10; break;
		case 'b': value=11; break;
		case 'c': value=12; break;
		case 'd': value=13; break;
		case 'e': value=14; break;
		case 'f': value=15; break;		
	}
	return value;
}

int *HexaToDecimal_Msg(char *msg){// convert Message Digest from Hexa format to Decimal format
	int *MD5_hash = (int *)malloc( 16*sizeof(int));
	int i;
	for(i=0; i<16; i++){
		int a = HexaToDecimal_Bit( msg[2*i] ) * 16;
		int b = HexaToDecimal_Bit( msg[2*i +1] );
		MD5_hash[i] = a + b;
	}
	return MD5_hash;
}