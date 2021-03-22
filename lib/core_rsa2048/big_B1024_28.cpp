/*
 * Copyright (c) 2012-2020 MIRACL UK Ltd.
 *
 * This file is part of MIRACL Core
 * (see https://github.com/miracl/core).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* CORE basic functions for BIG type */
/* SU=m, SU is Stack Usage */

#include "big_B1024_28.h"

//int B1024_28::BIGMULS=0;
//int B1024_28::BIGSQRS=0;

/* test a=0? */
int B1024_28::BIG_iszilch(BIG a)
{
    int i;
    chunk d=0;
    for (i = 0; i < NLEN_B1024_28; i++)
        d|=a[i];
    return (1 & ((d-1)>>BASEBITS_B1024_28));
}

/* test a=1? */
int B1024_28::BIG_isunity(BIG a)
{
    int i;
    chunk d=0;
    for (i = 1; i < NLEN_B1024_28; i++)
        d|=a[i];
    return (1 & ((d-1)>>BASEBITS_B1024_28) & (((a[0]^1)-1)>>BASEBITS_B1024_28));
}

/* test a=0? */
int B1024_28::BIG_diszilch(DBIG a)
{
    int i;
    chunk d=0;
    for (i = 0; i < DNLEN_B1024_28; i++)
        d|=a[i];
    return (1 & ((d-1)>>BASEBITS_B1024_28));
}

/* SU= 56 */
/* output a */
void B1024_28::BIG_output(BIG a)
{
    BIG b;
    int i, len;
    len = BIG_nbits(a);
    if (len % 4 == 0) len /= 4;
    else
    {
        len /= 4;
        len++;
    }
    if (len < MODBYTES_B1024_28 * 2) len = MODBYTES_B1024_28 * 2;

    for (i = len - 1; i >= 0; i--)
    {
        BIG_copy(b, a);
        BIG_shr(b, i * 4);
        printf("%01x", (unsigned int) b[0] & 15);
    }
}

/* SU= 16 */
void B1024_28::BIG_rawoutput(BIG a)
{
    int i;
    printf("(");
    for (i = 0; i < NLEN_B1024_28 - 1; i++)
#if CHUNK==64
        printf("%jx,", (uintmax_t) a[i]);
    printf("%jx)", (uintmax_t) a[NLEN_B1024_28 - 1]);
#else
        printf("%x,", (unsigned int) a[i]);
    printf("%x)", (unsigned int) a[NLEN_B1024_28 - 1]);
#endif
}

/* Swap a and b if d=1 */
void B1024_28::BIG_cswap(BIG a, BIG b, int d)
{
    int i;
    chunk t, c = d;
    c = ~(c - 1);
#ifdef DEBUG_NORM
    for (i = 0; i < NLEN_B1024_28 + 2; i++)
#else
    for (i = 0; i < NLEN_B1024_28; i++)
#endif
    {
        t = c & (a[i] ^ b[i]);
        a[i] ^= t;
        b[i] ^= t;
    }
}

/* Move g to f if d=1 */
void B1024_28::BIG_cmove(BIG f, BIG g, int d)
{
    int i;
    chunk b = (chunk) - d;
#ifdef DEBUG_NORM
    for (i = 0; i < NLEN_B1024_28 + 2; i++)
#else
    for (i = 0; i < NLEN_B1024_28; i++)
#endif
    {
        f[i] ^= (f[i] ^ g[i])&b;
    }
}

/* Move g to f if d=1 */
void B1024_28::BIG_dcmove(DBIG f, DBIG g, int d)
{
    int i;
    chunk b = (chunk) - d;
#ifdef DEBUG_NORM
    for (i = 0; i < DNLEN_B1024_28 + 2; i++)
#else
    for (i = 0; i < DNLEN_B1024_28; i++)
#endif
    {
        f[i] ^= (f[i] ^ g[i])&b;
    }
}

/* convert BIG to/from bytes */
/* SU= 64 */
void B1024_28::BIG_toBytes(char *b, BIG a)
{
    int i;
    BIG c;
    BIG_copy(c, a);
    BIG_norm(c);
    for (i = MODBYTES_B1024_28 - 1; i >= 0; i--)
    {
        b[i] = c[0] & 0xff;
        BIG_fshr(c, 8);
    }
}

/* SU= 16 */
void B1024_28::BIG_fromBytes(BIG a, char *b)
{
    int i;
    BIG_zero(a);
    for (i = 0; i < MODBYTES_B1024_28; i++)
    {
        BIG_fshl(a, 8);
        a[0] += (int)(unsigned char)b[i];
    }
#ifdef DEBUG_NORM
    a[MPV_B1024_28] = 1; a[MNV_B1024_28] = 0;
#endif
}

void B1024_28::BIG_fromBytesLen(BIG a, char *b, int s)
{
    int i, len = s;
    BIG_zero(a);

    if (len > MODBYTES_B1024_28) len = MODBYTES_B1024_28;
    for (i = 0; i < len; i++)
    {
        BIG_fshl(a, 8);
        a[0] += (int)(unsigned char)b[i];
    }
#ifdef DEBUG_NORM
    a[MPV_B1024_28] = 1; a[MNV_B1024_28] = 0;
#endif
}



/* SU= 88 */
void B1024_28::BIG_doutput(DBIG a)
{
    DBIG b;
    int i, len;
    BIG_dnorm(a);
    len = BIG_dnbits(a);
    if (len % 4 == 0) len /= 4;
    else
    {
        len /= 4;
        len++;
    }

    for (i = len - 1; i >= 0; i--)
    {
        BIG_dcopy(b, a);
        BIG_dshr(b, i * 4);
        printf("%01x", (unsigned int) b[0] & 15);
    }
}


void B1024_28::BIG_drawoutput(DBIG a)
{
    int i;
    printf("(");
    for (i = 0; i < DNLEN_B1024_28 - 1; i++)
#if CHUNK==64
        printf("%jx,", (uintmax_t) a[i]);
    printf("%jx)", (uintmax_t) a[DNLEN_B1024_28 - 1]);
#else
        printf("%x,", (unsigned int) a[i]);
    printf("%x)", (unsigned int) a[DNLEN_B1024_28 - 1]);
#endif
}

/* Copy b=a */
void B1024_28::BIG_copy(BIG b, BIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        b[i] = a[i];
#ifdef DEBUG_NORM
    b[MPV_B1024_28] = a[MPV_B1024_28];
    b[MNV_B1024_28] = a[MNV_B1024_28];
#endif
}

/* Copy from ROM b=a */
void B1024_28::BIG_rcopy(BIG b, const BIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        b[i] = a[i];
#ifdef DEBUG_NORM
    b[MPV_B1024_28] = 1; b[MNV_B1024_28] = 0;
#endif
}

/* double length DBIG copy b=a */
void B1024_28::BIG_dcopy(DBIG b, DBIG a)
{
    int i;
    for (i = 0; i < DNLEN_B1024_28; i++)
        b[i] = a[i];
#ifdef DEBUG_NORM
    b[DMPV_B1024_28] = a[DMPV_B1024_28];
    b[DMNV_B1024_28] = a[DMNV_B1024_28];
#endif
}

/* Copy BIG to bottom half of DBIG */
void B1024_28::BIG_dscopy(DBIG b, BIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28 - 1; i++)
        b[i] = a[i];

    b[NLEN_B1024_28 - 1] = a[NLEN_B1024_28 - 1] & BMASK_B1024_28; /* top word normalized */
    b[NLEN_B1024_28] = a[NLEN_B1024_28 - 1] >> BASEBITS_B1024_28;

    for (i = NLEN_B1024_28 + 1; i < DNLEN_B1024_28; i++) b[i] = 0;
#ifdef DEBUG_NORM
    b[DMPV_B1024_28] = a[MPV_B1024_28];
    b[DMNV_B1024_28] = a[MNV_B1024_28];
#endif
}

/* Copy BIG to top half of DBIG */
void B1024_28::BIG_dsucopy(DBIG b, BIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        b[i] = 0;
    for (i = NLEN_B1024_28; i < DNLEN_B1024_28; i++)
        b[i] = a[i - NLEN_B1024_28];
#ifdef DEBUG_NORM
    b[DMPV_B1024_28] = a[MPV_B1024_28];
    b[DMNV_B1024_28] = a[MNV_B1024_28];
#endif
}

/* Copy bottom half of DBIG to BIG */
void B1024_28::BIG_sdcopy(BIG b, DBIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        b[i] = a[i];
#ifdef DEBUG_NORM
    b[MPV_B1024_28] = a[DMPV_B1024_28];
    b[MNV_B1024_28] = a[DMNV_B1024_28];
#endif
}

/* Copy top half of DBIG to BIG */
void B1024_28::BIG_sducopy(BIG b, DBIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        b[i] = a[i + NLEN_B1024_28];
#ifdef DEBUG_NORM
    b[MPV_B1024_28] = a[DMPV_B1024_28];
    b[MNV_B1024_28] = a[DMNV_B1024_28];

#endif
}

/* Set a=0 */
void B1024_28::BIG_zero(BIG a)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        a[i] = 0;
#ifdef DEBUG_NORM
    a[MPV_B1024_28] = a[MNV_B1024_28] = 0;
#endif
}

void B1024_28::BIG_dzero(DBIG a)
{
    int i;
    for (i = 0; i < DNLEN_B1024_28; i++)
        a[i] = 0;
#ifdef DEBUG_NORM
    a[DMPV_B1024_28] = a[DMNV_B1024_28] = 0;
#endif
}

/* set a=1 */
void B1024_28::BIG_one(BIG a)
{
    int i;
    a[0] = 1;
    for (i = 1; i < NLEN_B1024_28; i++)
        a[i] = 0;
#ifdef DEBUG_NORM
    a[MPV_B1024_28] = 1;
    a[MNV_B1024_28] = 0;
#endif
}

/* Set c=a+b */
/* SU= 8 */
void B1024_28::BIG_add(BIG c, BIG a, BIG b)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        c[i] = a[i] + b[i];
#ifdef DEBUG_NORM
    c[MPV_B1024_28] = a[MPV_B1024_28] + b[MPV_B1024_28];
    c[MNV_B1024_28] = a[MNV_B1024_28] + b[MNV_B1024_28];
    if (c[MPV_B1024_28] > NEXCESS_B1024_28)  printf("add problem - positive digit overflow %d\n", (int)c[MPV_B1024_28]);
    if (c[MNV_B1024_28] > NEXCESS_B1024_28)  printf("add problem - negative digit overflow %d\n", (int)c[MNV_B1024_28]);

#endif
}

/* Set c=a or b */
/* SU= 8 */
void B1024_28::BIG_or(BIG c, BIG a, BIG b)
{
    int i;
    BIG_norm(a);
    BIG_norm(b);
    for (i = 0; i < NLEN_B1024_28; i++)
        c[i] = a[i] | b[i];
#ifdef DEBUG_NORM
    c[MPV_B1024_28] = 1;
    c[MNV_B1024_28] = 0;
#endif

}


/* Set c=c+d */
void B1024_28::BIG_inc(BIG c, int d)
{
    BIG_norm(c);
    c[0] += (chunk)d;
#ifdef DEBUG_NORM
    c[MPV_B1024_28] += 1;
#endif
}

/* Set c=a-b */
/* SU= 8 */
void B1024_28::BIG_sub(BIG c, BIG a, BIG b)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++)
        c[i] = a[i] - b[i];
#ifdef DEBUG_NORM
    c[MPV_B1024_28] = a[MPV_B1024_28] + b[MNV_B1024_28];
    c[MNV_B1024_28] = a[MNV_B1024_28] + b[MPV_B1024_28];
    if (c[MPV_B1024_28] > NEXCESS_B1024_28)  printf("sub problem - positive digit overflow %d\n", (int)c[MPV_B1024_28]);
    if (c[MNV_B1024_28] > NEXCESS_B1024_28)  printf("sub problem - negative digit overflow %d\n", (int)c[MNV_B1024_28]);

#endif
}

/* SU= 8 */

void B1024_28::BIG_dsub(DBIG c, DBIG a, DBIG b)
{
    int i;
    for (i = 0; i < DNLEN_B1024_28; i++)
        c[i] = a[i] - b[i];
#ifdef DEBUG_NORM
    c[DMPV_B1024_28] = a[DMPV_B1024_28] + b[DMNV_B1024_28];
    c[DMNV_B1024_28] = a[DMNV_B1024_28] + b[DMPV_B1024_28];
    if (c[DMPV_B1024_28] > NEXCESS_B1024_28)  printf("double sub problem - positive digit overflow %d\n", (int)c[DMPV_B1024_28]);
    if (c[DMNV_B1024_28] > NEXCESS_B1024_28)  printf("double sub problem - negative digit overflow %d\n", (int)c[DMNV_B1024_28]);
#endif
}

void B1024_28::BIG_dadd(DBIG c, DBIG a, DBIG b)
{
    int i;
    for (i = 0; i < DNLEN_B1024_28; i++)
        c[i] = a[i] + b[i];
#ifdef DEBUG_NORM
    c[DMPV_B1024_28] = a[DMPV_B1024_28] + b[DMNV_B1024_28];
    c[DMNV_B1024_28] = a[DMNV_B1024_28] + b[DMPV_B1024_28];
    if (c[DMPV_B1024_28] > NEXCESS_B1024_28)  printf("double add problem - positive digit overflow %d\n", (int)c[DMPV_B1024_28]);
    if (c[DMNV_B1024_28] > NEXCESS_B1024_28)  printf("double add problem - negative digit overflow %d\n", (int)c[DMNV_B1024_28]);
#endif
}

/* Set c=c-1 */
void B1024_28::BIG_dec(BIG c, int d)
{
    BIG_norm(c);
    c[0] -= (chunk)d;
#ifdef DEBUG_NORM
    c[MNV_B1024_28] += 1;
#endif
}

/* multiplication r=a*c by c<=NEXCESS_B1024_28 */
void B1024_28::BIG_imul(BIG r, BIG a, int c)
{
    int i;
    for (i = 0; i < NLEN_B1024_28; i++) r[i] = a[i] * c;
#ifdef DEBUG_NORM
    r[MPV_B1024_28] = a[MPV_B1024_28] * c;
    r[MNV_B1024_28] = a[MNV_B1024_28] * c;
    if (r[MPV_B1024_28] > NEXCESS_B1024_28)  printf("int mul problem - positive digit overflow %d\n", (int)r[MPV_B1024_28]);
    if (r[MNV_B1024_28] > NEXCESS_B1024_28)  printf("int mul problem - negative digit overflow %d\n", (int)r[MNV_B1024_28]);

#endif
}

/* multiplication r=a*c by larger integer - c<=FEXCESS */
/* SU= 24 */
chunk B1024_28::BIG_pmul(BIG r, BIG a, int c)
{
    int i;
    chunk ak, carry = 0;
    for (i = 0; i < NLEN_B1024_28; i++)
    {
        ak = a[i];
        r[i] = 0;
        carry = muladd(ak, (chunk)c, carry, &r[i]);
    }
#ifdef DEBUG_NORM
    r[MPV_B1024_28] = 1;
    r[MNV_B1024_28] = 0;
#endif
    return carry;
}

/* r/=3 */
/* SU= 16 */
/*
int B1024_28::BIG_div3(BIG r)
{
    int i;
    chunk ak,base,carry=0;
    BIG_norm(r);
    base=((chunk)1<<BASEBITS_B1024_28);
    for (i=NLEN_B1024_28-1; i>=0; i--)
    {
        ak=(carry*base+r[i]);
        r[i]=ak/3;
        carry=ak%3;
    }
    return (int)carry;
}
*/
/* multiplication c=a*b by even larger integer b>FEXCESS, resulting in DBIG */
/* SU= 24 */
void B1024_28::BIG_pxmul(DBIG c, BIG a, int b)
{
    int j;
    chunk carry;
    BIG_dzero(c);
    carry = 0;
    for (j = 0; j < NLEN_B1024_28; j++)
        carry = muladd(a[j], (chunk)b, carry, &c[j]);
    c[NLEN_B1024_28] = carry;
#ifdef DEBUG_NORM
    c[DMPV_B1024_28] = 1;
    c[DMNV_B1024_28] = 0;
#endif
}

/* .. if you know the result will fit in a BIG, c must be distinct from a and b */
/* SU= 40 */
void B1024_28::BIG_smul(BIG c, BIG a, BIG b)
{
    int i, j;
    chunk carry;

    BIG_zero(c);
    for (i = 0; i < NLEN_B1024_28; i++)
    {
        carry = 0;
        for (j = 0; j < NLEN_B1024_28; j++)
        {
            if (i + j < NLEN_B1024_28)
                carry = muladd(a[i], b[j], carry, &c[i + j]);
        }
    }
#ifdef DEBUG_NORM
    c[MPV_B1024_28] = 1;
    c[MNV_B1024_28] = 0;
#endif

}

/* Set c=a*b */
/* SU= 72 */
void B1024_28::BIG_mul(DBIG c, BIG a, BIG b)
{
    int i,k;
#ifdef dchunk
    dchunk co,t;
    dchunk s;
    dchunk d[NLEN_B1024_28];
    int m;
#endif

//B1024_28::BIGMULS++;

#ifdef DEBUG_NORM
    if ((a[MPV_B1024_28] != 1 && a[MPV_B1024_28] != 0) || a[MNV_B1024_28] != 0) printf("First input to mul not normed\n");
    if ((b[MPV_B1024_28] != 1 && b[MPV_B1024_28] != 0) || b[MNV_B1024_28] != 0) printf("Second input to mul not normed\n");
#endif

    /* Faster to Combafy it.. Let the compiler unroll the loops! */

#ifdef COMBA

    /* faster psuedo-Karatsuba method */
#ifdef UNWOUND

#ifdef USE_KARATSUBA

    	d[0]=(dchunk)a[0]*b[0];
	d[1]=(dchunk)a[1]*b[1];
	d[2]=(dchunk)a[2]*b[2];
	d[3]=(dchunk)a[3]*b[3];
	d[4]=(dchunk)a[4]*b[4];
	d[5]=(dchunk)a[5]*b[5];
	d[6]=(dchunk)a[6]*b[6];
	d[7]=(dchunk)a[7]*b[7];
	d[8]=(dchunk)a[8]*b[8];
	d[9]=(dchunk)a[9]*b[9];
	d[10]=(dchunk)a[10]*b[10];
	d[11]=(dchunk)a[11]*b[11];
	d[12]=(dchunk)a[12]*b[12];
	d[13]=(dchunk)a[13]*b[13];
	d[14]=(dchunk)a[14]*b[14];
	d[15]=(dchunk)a[15]*b[15];
	d[16]=(dchunk)a[16]*b[16];
	d[17]=(dchunk)a[17]*b[17];
	d[18]=(dchunk)a[18]*b[18];
	d[19]=(dchunk)a[19]*b[19];
	d[20]=(dchunk)a[20]*b[20];
	d[21]=(dchunk)a[21]*b[21];
	d[22]=(dchunk)a[22]*b[22];
	d[23]=(dchunk)a[23]*b[23];
	d[24]=(dchunk)a[24]*b[24];
	d[25]=(dchunk)a[25]*b[25];
	d[26]=(dchunk)a[26]*b[26];
	d[27]=(dchunk)a[27]*b[27];
	d[28]=(dchunk)a[28]*b[28];
	d[29]=(dchunk)a[29]*b[29];
	d[30]=(dchunk)a[30]*b[30];
	d[31]=(dchunk)a[31]*b[31];
	d[32]=(dchunk)a[32]*b[32];
	d[33]=(dchunk)a[33]*b[33];
	d[34]=(dchunk)a[34]*b[34];
	d[35]=(dchunk)a[35]*b[35];
	d[36]=(dchunk)a[36]*b[36];

	s=d[0];
	t = s; c[0]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28;
	s+=d[1]; t=co+s +(dchunk)(a[1]-a[0])*(b[0]-b[1]); c[1]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[2]; t=co+s +(dchunk)(a[2]-a[0])*(b[0]-b[2]); c[2]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[3]; t=co+s +(dchunk)(a[3]-a[0])*(b[0]-b[3])+(dchunk)(a[2]-a[1])*(b[1]-b[2]); c[3]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[4]; t=co+s +(dchunk)(a[4]-a[0])*(b[0]-b[4])+(dchunk)(a[3]-a[1])*(b[1]-b[3]); c[4]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[5]; t=co+s +(dchunk)(a[5]-a[0])*(b[0]-b[5])+(dchunk)(a[4]-a[1])*(b[1]-b[4])+(dchunk)(a[3]-a[2])*(b[2]-b[3]); c[5]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[6]; t=co+s +(dchunk)(a[6]-a[0])*(b[0]-b[6])+(dchunk)(a[5]-a[1])*(b[1]-b[5])+(dchunk)(a[4]-a[2])*(b[2]-b[4]); c[6]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[7]; t=co+s +(dchunk)(a[7]-a[0])*(b[0]-b[7])+(dchunk)(a[6]-a[1])*(b[1]-b[6])+(dchunk)(a[5]-a[2])*(b[2]-b[5])+(dchunk)(a[4]-a[3])*(b[3]-b[4]); c[7]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[8]; t=co+s +(dchunk)(a[8]-a[0])*(b[0]-b[8])+(dchunk)(a[7]-a[1])*(b[1]-b[7])+(dchunk)(a[6]-a[2])*(b[2]-b[6])+(dchunk)(a[5]-a[3])*(b[3]-b[5]); c[8]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[9]; t=co+s +(dchunk)(a[9]-a[0])*(b[0]-b[9])+(dchunk)(a[8]-a[1])*(b[1]-b[8])+(dchunk)(a[7]-a[2])*(b[2]-b[7])+(dchunk)(a[6]-a[3])*(b[3]-b[6])+(dchunk)(a[5]-a[4])*(b[4]-b[5]); c[9]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[10]; t=co+s +(dchunk)(a[10]-a[0])*(b[0]-b[10])+(dchunk)(a[9]-a[1])*(b[1]-b[9])+(dchunk)(a[8]-a[2])*(b[2]-b[8])+(dchunk)(a[7]-a[3])*(b[3]-b[7])+(dchunk)(a[6]-a[4])*(b[4]-b[6]); c[10]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[11]; t=co+s +(dchunk)(a[11]-a[0])*(b[0]-b[11])+(dchunk)(a[10]-a[1])*(b[1]-b[10])+(dchunk)(a[9]-a[2])*(b[2]-b[9])+(dchunk)(a[8]-a[3])*(b[3]-b[8])+(dchunk)(a[7]-a[4])*(b[4]-b[7])+(dchunk)(a[6]-a[5])*(b[5]-b[6]); c[11]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[12]; t=co+s +(dchunk)(a[12]-a[0])*(b[0]-b[12])+(dchunk)(a[11]-a[1])*(b[1]-b[11])+(dchunk)(a[10]-a[2])*(b[2]-b[10])+(dchunk)(a[9]-a[3])*(b[3]-b[9])+(dchunk)(a[8]-a[4])*(b[4]-b[8])+(dchunk)(a[7]-a[5])*(b[5]-b[7]); c[12]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[13]; t=co+s +(dchunk)(a[13]-a[0])*(b[0]-b[13])+(dchunk)(a[12]-a[1])*(b[1]-b[12])+(dchunk)(a[11]-a[2])*(b[2]-b[11])+(dchunk)(a[10]-a[3])*(b[3]-b[10])+(dchunk)(a[9]-a[4])*(b[4]-b[9])+(dchunk)(a[8]-a[5])*(b[5]-b[8])+(dchunk)(a[7]-a[6])*(b[6]-b[7]); c[13]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[14]; t=co+s +(dchunk)(a[14]-a[0])*(b[0]-b[14])+(dchunk)(a[13]-a[1])*(b[1]-b[13])+(dchunk)(a[12]-a[2])*(b[2]-b[12])+(dchunk)(a[11]-a[3])*(b[3]-b[11])+(dchunk)(a[10]-a[4])*(b[4]-b[10])+(dchunk)(a[9]-a[5])*(b[5]-b[9])+(dchunk)(a[8]-a[6])*(b[6]-b[8]); c[14]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[15]; t=co+s +(dchunk)(a[15]-a[0])*(b[0]-b[15])+(dchunk)(a[14]-a[1])*(b[1]-b[14])+(dchunk)(a[13]-a[2])*(b[2]-b[13])+(dchunk)(a[12]-a[3])*(b[3]-b[12])+(dchunk)(a[11]-a[4])*(b[4]-b[11])+(dchunk)(a[10]-a[5])*(b[5]-b[10])+(dchunk)(a[9]-a[6])*(b[6]-b[9])+(dchunk)(a[8]-a[7])*(b[7]-b[8]); c[15]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[16]; t=co+s +(dchunk)(a[16]-a[0])*(b[0]-b[16])+(dchunk)(a[15]-a[1])*(b[1]-b[15])+(dchunk)(a[14]-a[2])*(b[2]-b[14])+(dchunk)(a[13]-a[3])*(b[3]-b[13])+(dchunk)(a[12]-a[4])*(b[4]-b[12])+(dchunk)(a[11]-a[5])*(b[5]-b[11])+(dchunk)(a[10]-a[6])*(b[6]-b[10])+(dchunk)(a[9]-a[7])*(b[7]-b[9]); c[16]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[17]; t=co+s +(dchunk)(a[17]-a[0])*(b[0]-b[17])+(dchunk)(a[16]-a[1])*(b[1]-b[16])+(dchunk)(a[15]-a[2])*(b[2]-b[15])+(dchunk)(a[14]-a[3])*(b[3]-b[14])+(dchunk)(a[13]-a[4])*(b[4]-b[13])+(dchunk)(a[12]-a[5])*(b[5]-b[12])+(dchunk)(a[11]-a[6])*(b[6]-b[11])+(dchunk)(a[10]-a[7])*(b[7]-b[10])+(dchunk)(a[9]-a[8])*(b[8]-b[9]); c[17]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[18]; t=co+s +(dchunk)(a[18]-a[0])*(b[0]-b[18])+(dchunk)(a[17]-a[1])*(b[1]-b[17])+(dchunk)(a[16]-a[2])*(b[2]-b[16])+(dchunk)(a[15]-a[3])*(b[3]-b[15])+(dchunk)(a[14]-a[4])*(b[4]-b[14])+(dchunk)(a[13]-a[5])*(b[5]-b[13])+(dchunk)(a[12]-a[6])*(b[6]-b[12])+(dchunk)(a[11]-a[7])*(b[7]-b[11])+(dchunk)(a[10]-a[8])*(b[8]-b[10]); c[18]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[19]; t=co+s +(dchunk)(a[19]-a[0])*(b[0]-b[19])+(dchunk)(a[18]-a[1])*(b[1]-b[18])+(dchunk)(a[17]-a[2])*(b[2]-b[17])+(dchunk)(a[16]-a[3])*(b[3]-b[16])+(dchunk)(a[15]-a[4])*(b[4]-b[15])+(dchunk)(a[14]-a[5])*(b[5]-b[14])+(dchunk)(a[13]-a[6])*(b[6]-b[13])+(dchunk)(a[12]-a[7])*(b[7]-b[12])+(dchunk)(a[11]-a[8])*(b[8]-b[11])+(dchunk)(a[10]-a[9])*(b[9]-b[10]); c[19]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[20]; t=co+s +(dchunk)(a[20]-a[0])*(b[0]-b[20])+(dchunk)(a[19]-a[1])*(b[1]-b[19])+(dchunk)(a[18]-a[2])*(b[2]-b[18])+(dchunk)(a[17]-a[3])*(b[3]-b[17])+(dchunk)(a[16]-a[4])*(b[4]-b[16])+(dchunk)(a[15]-a[5])*(b[5]-b[15])+(dchunk)(a[14]-a[6])*(b[6]-b[14])+(dchunk)(a[13]-a[7])*(b[7]-b[13])+(dchunk)(a[12]-a[8])*(b[8]-b[12])+(dchunk)(a[11]-a[9])*(b[9]-b[11]); c[20]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[21]; t=co+s +(dchunk)(a[21]-a[0])*(b[0]-b[21])+(dchunk)(a[20]-a[1])*(b[1]-b[20])+(dchunk)(a[19]-a[2])*(b[2]-b[19])+(dchunk)(a[18]-a[3])*(b[3]-b[18])+(dchunk)(a[17]-a[4])*(b[4]-b[17])+(dchunk)(a[16]-a[5])*(b[5]-b[16])+(dchunk)(a[15]-a[6])*(b[6]-b[15])+(dchunk)(a[14]-a[7])*(b[7]-b[14])+(dchunk)(a[13]-a[8])*(b[8]-b[13])+(dchunk)(a[12]-a[9])*(b[9]-b[12])+(dchunk)(a[11]-a[10])*(b[10]-b[11]); c[21]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[22]; t=co+s +(dchunk)(a[22]-a[0])*(b[0]-b[22])+(dchunk)(a[21]-a[1])*(b[1]-b[21])+(dchunk)(a[20]-a[2])*(b[2]-b[20])+(dchunk)(a[19]-a[3])*(b[3]-b[19])+(dchunk)(a[18]-a[4])*(b[4]-b[18])+(dchunk)(a[17]-a[5])*(b[5]-b[17])+(dchunk)(a[16]-a[6])*(b[6]-b[16])+(dchunk)(a[15]-a[7])*(b[7]-b[15])+(dchunk)(a[14]-a[8])*(b[8]-b[14])+(dchunk)(a[13]-a[9])*(b[9]-b[13])+(dchunk)(a[12]-a[10])*(b[10]-b[12]); c[22]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[23]; t=co+s +(dchunk)(a[23]-a[0])*(b[0]-b[23])+(dchunk)(a[22]-a[1])*(b[1]-b[22])+(dchunk)(a[21]-a[2])*(b[2]-b[21])+(dchunk)(a[20]-a[3])*(b[3]-b[20])+(dchunk)(a[19]-a[4])*(b[4]-b[19])+(dchunk)(a[18]-a[5])*(b[5]-b[18])+(dchunk)(a[17]-a[6])*(b[6]-b[17])+(dchunk)(a[16]-a[7])*(b[7]-b[16])+(dchunk)(a[15]-a[8])*(b[8]-b[15])+(dchunk)(a[14]-a[9])*(b[9]-b[14])+(dchunk)(a[13]-a[10])*(b[10]-b[13])+(dchunk)(a[12]-a[11])*(b[11]-b[12]); c[23]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[24]; t=co+s +(dchunk)(a[24]-a[0])*(b[0]-b[24])+(dchunk)(a[23]-a[1])*(b[1]-b[23])+(dchunk)(a[22]-a[2])*(b[2]-b[22])+(dchunk)(a[21]-a[3])*(b[3]-b[21])+(dchunk)(a[20]-a[4])*(b[4]-b[20])+(dchunk)(a[19]-a[5])*(b[5]-b[19])+(dchunk)(a[18]-a[6])*(b[6]-b[18])+(dchunk)(a[17]-a[7])*(b[7]-b[17])+(dchunk)(a[16]-a[8])*(b[8]-b[16])+(dchunk)(a[15]-a[9])*(b[9]-b[15])+(dchunk)(a[14]-a[10])*(b[10]-b[14])+(dchunk)(a[13]-a[11])*(b[11]-b[13]); c[24]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[25]; t=co+s +(dchunk)(a[25]-a[0])*(b[0]-b[25])+(dchunk)(a[24]-a[1])*(b[1]-b[24])+(dchunk)(a[23]-a[2])*(b[2]-b[23])+(dchunk)(a[22]-a[3])*(b[3]-b[22])+(dchunk)(a[21]-a[4])*(b[4]-b[21])+(dchunk)(a[20]-a[5])*(b[5]-b[20])+(dchunk)(a[19]-a[6])*(b[6]-b[19])+(dchunk)(a[18]-a[7])*(b[7]-b[18])+(dchunk)(a[17]-a[8])*(b[8]-b[17])+(dchunk)(a[16]-a[9])*(b[9]-b[16])+(dchunk)(a[15]-a[10])*(b[10]-b[15])+(dchunk)(a[14]-a[11])*(b[11]-b[14])+(dchunk)(a[13]-a[12])*(b[12]-b[13]); c[25]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[26]; t=co+s +(dchunk)(a[26]-a[0])*(b[0]-b[26])+(dchunk)(a[25]-a[1])*(b[1]-b[25])+(dchunk)(a[24]-a[2])*(b[2]-b[24])+(dchunk)(a[23]-a[3])*(b[3]-b[23])+(dchunk)(a[22]-a[4])*(b[4]-b[22])+(dchunk)(a[21]-a[5])*(b[5]-b[21])+(dchunk)(a[20]-a[6])*(b[6]-b[20])+(dchunk)(a[19]-a[7])*(b[7]-b[19])+(dchunk)(a[18]-a[8])*(b[8]-b[18])+(dchunk)(a[17]-a[9])*(b[9]-b[17])+(dchunk)(a[16]-a[10])*(b[10]-b[16])+(dchunk)(a[15]-a[11])*(b[11]-b[15])+(dchunk)(a[14]-a[12])*(b[12]-b[14]); c[26]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[27]; t=co+s +(dchunk)(a[27]-a[0])*(b[0]-b[27])+(dchunk)(a[26]-a[1])*(b[1]-b[26])+(dchunk)(a[25]-a[2])*(b[2]-b[25])+(dchunk)(a[24]-a[3])*(b[3]-b[24])+(dchunk)(a[23]-a[4])*(b[4]-b[23])+(dchunk)(a[22]-a[5])*(b[5]-b[22])+(dchunk)(a[21]-a[6])*(b[6]-b[21])+(dchunk)(a[20]-a[7])*(b[7]-b[20])+(dchunk)(a[19]-a[8])*(b[8]-b[19])+(dchunk)(a[18]-a[9])*(b[9]-b[18])+(dchunk)(a[17]-a[10])*(b[10]-b[17])+(dchunk)(a[16]-a[11])*(b[11]-b[16])+(dchunk)(a[15]-a[12])*(b[12]-b[15])+(dchunk)(a[14]-a[13])*(b[13]-b[14]); c[27]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[28]; t=co+s +(dchunk)(a[28]-a[0])*(b[0]-b[28])+(dchunk)(a[27]-a[1])*(b[1]-b[27])+(dchunk)(a[26]-a[2])*(b[2]-b[26])+(dchunk)(a[25]-a[3])*(b[3]-b[25])+(dchunk)(a[24]-a[4])*(b[4]-b[24])+(dchunk)(a[23]-a[5])*(b[5]-b[23])+(dchunk)(a[22]-a[6])*(b[6]-b[22])+(dchunk)(a[21]-a[7])*(b[7]-b[21])+(dchunk)(a[20]-a[8])*(b[8]-b[20])+(dchunk)(a[19]-a[9])*(b[9]-b[19])+(dchunk)(a[18]-a[10])*(b[10]-b[18])+(dchunk)(a[17]-a[11])*(b[11]-b[17])+(dchunk)(a[16]-a[12])*(b[12]-b[16])+(dchunk)(a[15]-a[13])*(b[13]-b[15]); c[28]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[29]; t=co+s +(dchunk)(a[29]-a[0])*(b[0]-b[29])+(dchunk)(a[28]-a[1])*(b[1]-b[28])+(dchunk)(a[27]-a[2])*(b[2]-b[27])+(dchunk)(a[26]-a[3])*(b[3]-b[26])+(dchunk)(a[25]-a[4])*(b[4]-b[25])+(dchunk)(a[24]-a[5])*(b[5]-b[24])+(dchunk)(a[23]-a[6])*(b[6]-b[23])+(dchunk)(a[22]-a[7])*(b[7]-b[22])+(dchunk)(a[21]-a[8])*(b[8]-b[21])+(dchunk)(a[20]-a[9])*(b[9]-b[20])+(dchunk)(a[19]-a[10])*(b[10]-b[19])+(dchunk)(a[18]-a[11])*(b[11]-b[18])+(dchunk)(a[17]-a[12])*(b[12]-b[17])+(dchunk)(a[16]-a[13])*(b[13]-b[16])+(dchunk)(a[15]-a[14])*(b[14]-b[15]); c[29]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[30]; t=co+s +(dchunk)(a[30]-a[0])*(b[0]-b[30])+(dchunk)(a[29]-a[1])*(b[1]-b[29])+(dchunk)(a[28]-a[2])*(b[2]-b[28])+(dchunk)(a[27]-a[3])*(b[3]-b[27])+(dchunk)(a[26]-a[4])*(b[4]-b[26])+(dchunk)(a[25]-a[5])*(b[5]-b[25])+(dchunk)(a[24]-a[6])*(b[6]-b[24])+(dchunk)(a[23]-a[7])*(b[7]-b[23])+(dchunk)(a[22]-a[8])*(b[8]-b[22])+(dchunk)(a[21]-a[9])*(b[9]-b[21])+(dchunk)(a[20]-a[10])*(b[10]-b[20])+(dchunk)(a[19]-a[11])*(b[11]-b[19])+(dchunk)(a[18]-a[12])*(b[12]-b[18])+(dchunk)(a[17]-a[13])*(b[13]-b[17])+(dchunk)(a[16]-a[14])*(b[14]-b[16]); c[30]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[31]; t=co+s +(dchunk)(a[31]-a[0])*(b[0]-b[31])+(dchunk)(a[30]-a[1])*(b[1]-b[30])+(dchunk)(a[29]-a[2])*(b[2]-b[29])+(dchunk)(a[28]-a[3])*(b[3]-b[28])+(dchunk)(a[27]-a[4])*(b[4]-b[27])+(dchunk)(a[26]-a[5])*(b[5]-b[26])+(dchunk)(a[25]-a[6])*(b[6]-b[25])+(dchunk)(a[24]-a[7])*(b[7]-b[24])+(dchunk)(a[23]-a[8])*(b[8]-b[23])+(dchunk)(a[22]-a[9])*(b[9]-b[22])+(dchunk)(a[21]-a[10])*(b[10]-b[21])+(dchunk)(a[20]-a[11])*(b[11]-b[20])+(dchunk)(a[19]-a[12])*(b[12]-b[19])+(dchunk)(a[18]-a[13])*(b[13]-b[18])+(dchunk)(a[17]-a[14])*(b[14]-b[17])+(dchunk)(a[16]-a[15])*(b[15]-b[16]); c[31]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[32]; t=co+s +(dchunk)(a[32]-a[0])*(b[0]-b[32])+(dchunk)(a[31]-a[1])*(b[1]-b[31])+(dchunk)(a[30]-a[2])*(b[2]-b[30])+(dchunk)(a[29]-a[3])*(b[3]-b[29])+(dchunk)(a[28]-a[4])*(b[4]-b[28])+(dchunk)(a[27]-a[5])*(b[5]-b[27])+(dchunk)(a[26]-a[6])*(b[6]-b[26])+(dchunk)(a[25]-a[7])*(b[7]-b[25])+(dchunk)(a[24]-a[8])*(b[8]-b[24])+(dchunk)(a[23]-a[9])*(b[9]-b[23])+(dchunk)(a[22]-a[10])*(b[10]-b[22])+(dchunk)(a[21]-a[11])*(b[11]-b[21])+(dchunk)(a[20]-a[12])*(b[12]-b[20])+(dchunk)(a[19]-a[13])*(b[13]-b[19])+(dchunk)(a[18]-a[14])*(b[14]-b[18])+(dchunk)(a[17]-a[15])*(b[15]-b[17]); c[32]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[33]; t=co+s +(dchunk)(a[33]-a[0])*(b[0]-b[33])+(dchunk)(a[32]-a[1])*(b[1]-b[32])+(dchunk)(a[31]-a[2])*(b[2]-b[31])+(dchunk)(a[30]-a[3])*(b[3]-b[30])+(dchunk)(a[29]-a[4])*(b[4]-b[29])+(dchunk)(a[28]-a[5])*(b[5]-b[28])+(dchunk)(a[27]-a[6])*(b[6]-b[27])+(dchunk)(a[26]-a[7])*(b[7]-b[26])+(dchunk)(a[25]-a[8])*(b[8]-b[25])+(dchunk)(a[24]-a[9])*(b[9]-b[24])+(dchunk)(a[23]-a[10])*(b[10]-b[23])+(dchunk)(a[22]-a[11])*(b[11]-b[22])+(dchunk)(a[21]-a[12])*(b[12]-b[21])+(dchunk)(a[20]-a[13])*(b[13]-b[20])+(dchunk)(a[19]-a[14])*(b[14]-b[19])+(dchunk)(a[18]-a[15])*(b[15]-b[18])+(dchunk)(a[17]-a[16])*(b[16]-b[17]); c[33]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[34]; t=co+s +(dchunk)(a[34]-a[0])*(b[0]-b[34])+(dchunk)(a[33]-a[1])*(b[1]-b[33])+(dchunk)(a[32]-a[2])*(b[2]-b[32])+(dchunk)(a[31]-a[3])*(b[3]-b[31])+(dchunk)(a[30]-a[4])*(b[4]-b[30])+(dchunk)(a[29]-a[5])*(b[5]-b[29])+(dchunk)(a[28]-a[6])*(b[6]-b[28])+(dchunk)(a[27]-a[7])*(b[7]-b[27])+(dchunk)(a[26]-a[8])*(b[8]-b[26])+(dchunk)(a[25]-a[9])*(b[9]-b[25])+(dchunk)(a[24]-a[10])*(b[10]-b[24])+(dchunk)(a[23]-a[11])*(b[11]-b[23])+(dchunk)(a[22]-a[12])*(b[12]-b[22])+(dchunk)(a[21]-a[13])*(b[13]-b[21])+(dchunk)(a[20]-a[14])*(b[14]-b[20])+(dchunk)(a[19]-a[15])*(b[15]-b[19])+(dchunk)(a[18]-a[16])*(b[16]-b[18]); c[34]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[35]; t=co+s +(dchunk)(a[35]-a[0])*(b[0]-b[35])+(dchunk)(a[34]-a[1])*(b[1]-b[34])+(dchunk)(a[33]-a[2])*(b[2]-b[33])+(dchunk)(a[32]-a[3])*(b[3]-b[32])+(dchunk)(a[31]-a[4])*(b[4]-b[31])+(dchunk)(a[30]-a[5])*(b[5]-b[30])+(dchunk)(a[29]-a[6])*(b[6]-b[29])+(dchunk)(a[28]-a[7])*(b[7]-b[28])+(dchunk)(a[27]-a[8])*(b[8]-b[27])+(dchunk)(a[26]-a[9])*(b[9]-b[26])+(dchunk)(a[25]-a[10])*(b[10]-b[25])+(dchunk)(a[24]-a[11])*(b[11]-b[24])+(dchunk)(a[23]-a[12])*(b[12]-b[23])+(dchunk)(a[22]-a[13])*(b[13]-b[22])+(dchunk)(a[21]-a[14])*(b[14]-b[21])+(dchunk)(a[20]-a[15])*(b[15]-b[20])+(dchunk)(a[19]-a[16])*(b[16]-b[19])+(dchunk)(a[18]-a[17])*(b[17]-b[18]); c[35]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s+=d[36]; t=co+s +(dchunk)(a[36]-a[0])*(b[0]-b[36])+(dchunk)(a[35]-a[1])*(b[1]-b[35])+(dchunk)(a[34]-a[2])*(b[2]-b[34])+(dchunk)(a[33]-a[3])*(b[3]-b[33])+(dchunk)(a[32]-a[4])*(b[4]-b[32])+(dchunk)(a[31]-a[5])*(b[5]-b[31])+(dchunk)(a[30]-a[6])*(b[6]-b[30])+(dchunk)(a[29]-a[7])*(b[7]-b[29])+(dchunk)(a[28]-a[8])*(b[8]-b[28])+(dchunk)(a[27]-a[9])*(b[9]-b[27])+(dchunk)(a[26]-a[10])*(b[10]-b[26])+(dchunk)(a[25]-a[11])*(b[11]-b[25])+(dchunk)(a[24]-a[12])*(b[12]-b[24])+(dchunk)(a[23]-a[13])*(b[13]-b[23])+(dchunk)(a[22]-a[14])*(b[14]-b[22])+(dchunk)(a[21]-a[15])*(b[15]-b[21])+(dchunk)(a[20]-a[16])*(b[16]-b[20])+(dchunk)(a[19]-a[17])*(b[17]-b[19]); c[36]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 

	s-=d[0]; t=co+s +(dchunk)(a[36]-a[1])*(b[1]-b[36])+(dchunk)(a[35]-a[2])*(b[2]-b[35])+(dchunk)(a[34]-a[3])*(b[3]-b[34])+(dchunk)(a[33]-a[4])*(b[4]-b[33])+(dchunk)(a[32]-a[5])*(b[5]-b[32])+(dchunk)(a[31]-a[6])*(b[6]-b[31])+(dchunk)(a[30]-a[7])*(b[7]-b[30])+(dchunk)(a[29]-a[8])*(b[8]-b[29])+(dchunk)(a[28]-a[9])*(b[9]-b[28])+(dchunk)(a[27]-a[10])*(b[10]-b[27])+(dchunk)(a[26]-a[11])*(b[11]-b[26])+(dchunk)(a[25]-a[12])*(b[12]-b[25])+(dchunk)(a[24]-a[13])*(b[13]-b[24])+(dchunk)(a[23]-a[14])*(b[14]-b[23])+(dchunk)(a[22]-a[15])*(b[15]-b[22])+(dchunk)(a[21]-a[16])*(b[16]-b[21])+(dchunk)(a[20]-a[17])*(b[17]-b[20])+(dchunk)(a[19]-a[18])*(b[18]-b[19]); c[37]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[1]; t=co+s +(dchunk)(a[36]-a[2])*(b[2]-b[36])+(dchunk)(a[35]-a[3])*(b[3]-b[35])+(dchunk)(a[34]-a[4])*(b[4]-b[34])+(dchunk)(a[33]-a[5])*(b[5]-b[33])+(dchunk)(a[32]-a[6])*(b[6]-b[32])+(dchunk)(a[31]-a[7])*(b[7]-b[31])+(dchunk)(a[30]-a[8])*(b[8]-b[30])+(dchunk)(a[29]-a[9])*(b[9]-b[29])+(dchunk)(a[28]-a[10])*(b[10]-b[28])+(dchunk)(a[27]-a[11])*(b[11]-b[27])+(dchunk)(a[26]-a[12])*(b[12]-b[26])+(dchunk)(a[25]-a[13])*(b[13]-b[25])+(dchunk)(a[24]-a[14])*(b[14]-b[24])+(dchunk)(a[23]-a[15])*(b[15]-b[23])+(dchunk)(a[22]-a[16])*(b[16]-b[22])+(dchunk)(a[21]-a[17])*(b[17]-b[21])+(dchunk)(a[20]-a[18])*(b[18]-b[20]); c[38]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[2]; t=co+s +(dchunk)(a[36]-a[3])*(b[3]-b[36])+(dchunk)(a[35]-a[4])*(b[4]-b[35])+(dchunk)(a[34]-a[5])*(b[5]-b[34])+(dchunk)(a[33]-a[6])*(b[6]-b[33])+(dchunk)(a[32]-a[7])*(b[7]-b[32])+(dchunk)(a[31]-a[8])*(b[8]-b[31])+(dchunk)(a[30]-a[9])*(b[9]-b[30])+(dchunk)(a[29]-a[10])*(b[10]-b[29])+(dchunk)(a[28]-a[11])*(b[11]-b[28])+(dchunk)(a[27]-a[12])*(b[12]-b[27])+(dchunk)(a[26]-a[13])*(b[13]-b[26])+(dchunk)(a[25]-a[14])*(b[14]-b[25])+(dchunk)(a[24]-a[15])*(b[15]-b[24])+(dchunk)(a[23]-a[16])*(b[16]-b[23])+(dchunk)(a[22]-a[17])*(b[17]-b[22])+(dchunk)(a[21]-a[18])*(b[18]-b[21])+(dchunk)(a[20]-a[19])*(b[19]-b[20]); c[39]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[3]; t=co+s +(dchunk)(a[36]-a[4])*(b[4]-b[36])+(dchunk)(a[35]-a[5])*(b[5]-b[35])+(dchunk)(a[34]-a[6])*(b[6]-b[34])+(dchunk)(a[33]-a[7])*(b[7]-b[33])+(dchunk)(a[32]-a[8])*(b[8]-b[32])+(dchunk)(a[31]-a[9])*(b[9]-b[31])+(dchunk)(a[30]-a[10])*(b[10]-b[30])+(dchunk)(a[29]-a[11])*(b[11]-b[29])+(dchunk)(a[28]-a[12])*(b[12]-b[28])+(dchunk)(a[27]-a[13])*(b[13]-b[27])+(dchunk)(a[26]-a[14])*(b[14]-b[26])+(dchunk)(a[25]-a[15])*(b[15]-b[25])+(dchunk)(a[24]-a[16])*(b[16]-b[24])+(dchunk)(a[23]-a[17])*(b[17]-b[23])+(dchunk)(a[22]-a[18])*(b[18]-b[22])+(dchunk)(a[21]-a[19])*(b[19]-b[21]); c[40]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[4]; t=co+s +(dchunk)(a[36]-a[5])*(b[5]-b[36])+(dchunk)(a[35]-a[6])*(b[6]-b[35])+(dchunk)(a[34]-a[7])*(b[7]-b[34])+(dchunk)(a[33]-a[8])*(b[8]-b[33])+(dchunk)(a[32]-a[9])*(b[9]-b[32])+(dchunk)(a[31]-a[10])*(b[10]-b[31])+(dchunk)(a[30]-a[11])*(b[11]-b[30])+(dchunk)(a[29]-a[12])*(b[12]-b[29])+(dchunk)(a[28]-a[13])*(b[13]-b[28])+(dchunk)(a[27]-a[14])*(b[14]-b[27])+(dchunk)(a[26]-a[15])*(b[15]-b[26])+(dchunk)(a[25]-a[16])*(b[16]-b[25])+(dchunk)(a[24]-a[17])*(b[17]-b[24])+(dchunk)(a[23]-a[18])*(b[18]-b[23])+(dchunk)(a[22]-a[19])*(b[19]-b[22])+(dchunk)(a[21]-a[20])*(b[20]-b[21]); c[41]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[5]; t=co+s +(dchunk)(a[36]-a[6])*(b[6]-b[36])+(dchunk)(a[35]-a[7])*(b[7]-b[35])+(dchunk)(a[34]-a[8])*(b[8]-b[34])+(dchunk)(a[33]-a[9])*(b[9]-b[33])+(dchunk)(a[32]-a[10])*(b[10]-b[32])+(dchunk)(a[31]-a[11])*(b[11]-b[31])+(dchunk)(a[30]-a[12])*(b[12]-b[30])+(dchunk)(a[29]-a[13])*(b[13]-b[29])+(dchunk)(a[28]-a[14])*(b[14]-b[28])+(dchunk)(a[27]-a[15])*(b[15]-b[27])+(dchunk)(a[26]-a[16])*(b[16]-b[26])+(dchunk)(a[25]-a[17])*(b[17]-b[25])+(dchunk)(a[24]-a[18])*(b[18]-b[24])+(dchunk)(a[23]-a[19])*(b[19]-b[23])+(dchunk)(a[22]-a[20])*(b[20]-b[22]); c[42]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[6]; t=co+s +(dchunk)(a[36]-a[7])*(b[7]-b[36])+(dchunk)(a[35]-a[8])*(b[8]-b[35])+(dchunk)(a[34]-a[9])*(b[9]-b[34])+(dchunk)(a[33]-a[10])*(b[10]-b[33])+(dchunk)(a[32]-a[11])*(b[11]-b[32])+(dchunk)(a[31]-a[12])*(b[12]-b[31])+(dchunk)(a[30]-a[13])*(b[13]-b[30])+(dchunk)(a[29]-a[14])*(b[14]-b[29])+(dchunk)(a[28]-a[15])*(b[15]-b[28])+(dchunk)(a[27]-a[16])*(b[16]-b[27])+(dchunk)(a[26]-a[17])*(b[17]-b[26])+(dchunk)(a[25]-a[18])*(b[18]-b[25])+(dchunk)(a[24]-a[19])*(b[19]-b[24])+(dchunk)(a[23]-a[20])*(b[20]-b[23])+(dchunk)(a[22]-a[21])*(b[21]-b[22]); c[43]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[7]; t=co+s +(dchunk)(a[36]-a[8])*(b[8]-b[36])+(dchunk)(a[35]-a[9])*(b[9]-b[35])+(dchunk)(a[34]-a[10])*(b[10]-b[34])+(dchunk)(a[33]-a[11])*(b[11]-b[33])+(dchunk)(a[32]-a[12])*(b[12]-b[32])+(dchunk)(a[31]-a[13])*(b[13]-b[31])+(dchunk)(a[30]-a[14])*(b[14]-b[30])+(dchunk)(a[29]-a[15])*(b[15]-b[29])+(dchunk)(a[28]-a[16])*(b[16]-b[28])+(dchunk)(a[27]-a[17])*(b[17]-b[27])+(dchunk)(a[26]-a[18])*(b[18]-b[26])+(dchunk)(a[25]-a[19])*(b[19]-b[25])+(dchunk)(a[24]-a[20])*(b[20]-b[24])+(dchunk)(a[23]-a[21])*(b[21]-b[23]); c[44]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[8]; t=co+s +(dchunk)(a[36]-a[9])*(b[9]-b[36])+(dchunk)(a[35]-a[10])*(b[10]-b[35])+(dchunk)(a[34]-a[11])*(b[11]-b[34])+(dchunk)(a[33]-a[12])*(b[12]-b[33])+(dchunk)(a[32]-a[13])*(b[13]-b[32])+(dchunk)(a[31]-a[14])*(b[14]-b[31])+(dchunk)(a[30]-a[15])*(b[15]-b[30])+(dchunk)(a[29]-a[16])*(b[16]-b[29])+(dchunk)(a[28]-a[17])*(b[17]-b[28])+(dchunk)(a[27]-a[18])*(b[18]-b[27])+(dchunk)(a[26]-a[19])*(b[19]-b[26])+(dchunk)(a[25]-a[20])*(b[20]-b[25])+(dchunk)(a[24]-a[21])*(b[21]-b[24])+(dchunk)(a[23]-a[22])*(b[22]-b[23]); c[45]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[9]; t=co+s +(dchunk)(a[36]-a[10])*(b[10]-b[36])+(dchunk)(a[35]-a[11])*(b[11]-b[35])+(dchunk)(a[34]-a[12])*(b[12]-b[34])+(dchunk)(a[33]-a[13])*(b[13]-b[33])+(dchunk)(a[32]-a[14])*(b[14]-b[32])+(dchunk)(a[31]-a[15])*(b[15]-b[31])+(dchunk)(a[30]-a[16])*(b[16]-b[30])+(dchunk)(a[29]-a[17])*(b[17]-b[29])+(dchunk)(a[28]-a[18])*(b[18]-b[28])+(dchunk)(a[27]-a[19])*(b[19]-b[27])+(dchunk)(a[26]-a[20])*(b[20]-b[26])+(dchunk)(a[25]-a[21])*(b[21]-b[25])+(dchunk)(a[24]-a[22])*(b[22]-b[24]); c[46]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[10]; t=co+s +(dchunk)(a[36]-a[11])*(b[11]-b[36])+(dchunk)(a[35]-a[12])*(b[12]-b[35])+(dchunk)(a[34]-a[13])*(b[13]-b[34])+(dchunk)(a[33]-a[14])*(b[14]-b[33])+(dchunk)(a[32]-a[15])*(b[15]-b[32])+(dchunk)(a[31]-a[16])*(b[16]-b[31])+(dchunk)(a[30]-a[17])*(b[17]-b[30])+(dchunk)(a[29]-a[18])*(b[18]-b[29])+(dchunk)(a[28]-a[19])*(b[19]-b[28])+(dchunk)(a[27]-a[20])*(b[20]-b[27])+(dchunk)(a[26]-a[21])*(b[21]-b[26])+(dchunk)(a[25]-a[22])*(b[22]-b[25])+(dchunk)(a[24]-a[23])*(b[23]-b[24]); c[47]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[11]; t=co+s +(dchunk)(a[36]-a[12])*(b[12]-b[36])+(dchunk)(a[35]-a[13])*(b[13]-b[35])+(dchunk)(a[34]-a[14])*(b[14]-b[34])+(dchunk)(a[33]-a[15])*(b[15]-b[33])+(dchunk)(a[32]-a[16])*(b[16]-b[32])+(dchunk)(a[31]-a[17])*(b[17]-b[31])+(dchunk)(a[30]-a[18])*(b[18]-b[30])+(dchunk)(a[29]-a[19])*(b[19]-b[29])+(dchunk)(a[28]-a[20])*(b[20]-b[28])+(dchunk)(a[27]-a[21])*(b[21]-b[27])+(dchunk)(a[26]-a[22])*(b[22]-b[26])+(dchunk)(a[25]-a[23])*(b[23]-b[25]); c[48]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[12]; t=co+s +(dchunk)(a[36]-a[13])*(b[13]-b[36])+(dchunk)(a[35]-a[14])*(b[14]-b[35])+(dchunk)(a[34]-a[15])*(b[15]-b[34])+(dchunk)(a[33]-a[16])*(b[16]-b[33])+(dchunk)(a[32]-a[17])*(b[17]-b[32])+(dchunk)(a[31]-a[18])*(b[18]-b[31])+(dchunk)(a[30]-a[19])*(b[19]-b[30])+(dchunk)(a[29]-a[20])*(b[20]-b[29])+(dchunk)(a[28]-a[21])*(b[21]-b[28])+(dchunk)(a[27]-a[22])*(b[22]-b[27])+(dchunk)(a[26]-a[23])*(b[23]-b[26])+(dchunk)(a[25]-a[24])*(b[24]-b[25]); c[49]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[13]; t=co+s +(dchunk)(a[36]-a[14])*(b[14]-b[36])+(dchunk)(a[35]-a[15])*(b[15]-b[35])+(dchunk)(a[34]-a[16])*(b[16]-b[34])+(dchunk)(a[33]-a[17])*(b[17]-b[33])+(dchunk)(a[32]-a[18])*(b[18]-b[32])+(dchunk)(a[31]-a[19])*(b[19]-b[31])+(dchunk)(a[30]-a[20])*(b[20]-b[30])+(dchunk)(a[29]-a[21])*(b[21]-b[29])+(dchunk)(a[28]-a[22])*(b[22]-b[28])+(dchunk)(a[27]-a[23])*(b[23]-b[27])+(dchunk)(a[26]-a[24])*(b[24]-b[26]); c[50]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[14]; t=co+s +(dchunk)(a[36]-a[15])*(b[15]-b[36])+(dchunk)(a[35]-a[16])*(b[16]-b[35])+(dchunk)(a[34]-a[17])*(b[17]-b[34])+(dchunk)(a[33]-a[18])*(b[18]-b[33])+(dchunk)(a[32]-a[19])*(b[19]-b[32])+(dchunk)(a[31]-a[20])*(b[20]-b[31])+(dchunk)(a[30]-a[21])*(b[21]-b[30])+(dchunk)(a[29]-a[22])*(b[22]-b[29])+(dchunk)(a[28]-a[23])*(b[23]-b[28])+(dchunk)(a[27]-a[24])*(b[24]-b[27])+(dchunk)(a[26]-a[25])*(b[25]-b[26]); c[51]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[15]; t=co+s +(dchunk)(a[36]-a[16])*(b[16]-b[36])+(dchunk)(a[35]-a[17])*(b[17]-b[35])+(dchunk)(a[34]-a[18])*(b[18]-b[34])+(dchunk)(a[33]-a[19])*(b[19]-b[33])+(dchunk)(a[32]-a[20])*(b[20]-b[32])+(dchunk)(a[31]-a[21])*(b[21]-b[31])+(dchunk)(a[30]-a[22])*(b[22]-b[30])+(dchunk)(a[29]-a[23])*(b[23]-b[29])+(dchunk)(a[28]-a[24])*(b[24]-b[28])+(dchunk)(a[27]-a[25])*(b[25]-b[27]); c[52]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[16]; t=co+s +(dchunk)(a[36]-a[17])*(b[17]-b[36])+(dchunk)(a[35]-a[18])*(b[18]-b[35])+(dchunk)(a[34]-a[19])*(b[19]-b[34])+(dchunk)(a[33]-a[20])*(b[20]-b[33])+(dchunk)(a[32]-a[21])*(b[21]-b[32])+(dchunk)(a[31]-a[22])*(b[22]-b[31])+(dchunk)(a[30]-a[23])*(b[23]-b[30])+(dchunk)(a[29]-a[24])*(b[24]-b[29])+(dchunk)(a[28]-a[25])*(b[25]-b[28])+(dchunk)(a[27]-a[26])*(b[26]-b[27]); c[53]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[17]; t=co+s +(dchunk)(a[36]-a[18])*(b[18]-b[36])+(dchunk)(a[35]-a[19])*(b[19]-b[35])+(dchunk)(a[34]-a[20])*(b[20]-b[34])+(dchunk)(a[33]-a[21])*(b[21]-b[33])+(dchunk)(a[32]-a[22])*(b[22]-b[32])+(dchunk)(a[31]-a[23])*(b[23]-b[31])+(dchunk)(a[30]-a[24])*(b[24]-b[30])+(dchunk)(a[29]-a[25])*(b[25]-b[29])+(dchunk)(a[28]-a[26])*(b[26]-b[28]); c[54]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[18]; t=co+s +(dchunk)(a[36]-a[19])*(b[19]-b[36])+(dchunk)(a[35]-a[20])*(b[20]-b[35])+(dchunk)(a[34]-a[21])*(b[21]-b[34])+(dchunk)(a[33]-a[22])*(b[22]-b[33])+(dchunk)(a[32]-a[23])*(b[23]-b[32])+(dchunk)(a[31]-a[24])*(b[24]-b[31])+(dchunk)(a[30]-a[25])*(b[25]-b[30])+(dchunk)(a[29]-a[26])*(b[26]-b[29])+(dchunk)(a[28]-a[27])*(b[27]-b[28]); c[55]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[19]; t=co+s +(dchunk)(a[36]-a[20])*(b[20]-b[36])+(dchunk)(a[35]-a[21])*(b[21]-b[35])+(dchunk)(a[34]-a[22])*(b[22]-b[34])+(dchunk)(a[33]-a[23])*(b[23]-b[33])+(dchunk)(a[32]-a[24])*(b[24]-b[32])+(dchunk)(a[31]-a[25])*(b[25]-b[31])+(dchunk)(a[30]-a[26])*(b[26]-b[30])+(dchunk)(a[29]-a[27])*(b[27]-b[29]); c[56]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[20]; t=co+s +(dchunk)(a[36]-a[21])*(b[21]-b[36])+(dchunk)(a[35]-a[22])*(b[22]-b[35])+(dchunk)(a[34]-a[23])*(b[23]-b[34])+(dchunk)(a[33]-a[24])*(b[24]-b[33])+(dchunk)(a[32]-a[25])*(b[25]-b[32])+(dchunk)(a[31]-a[26])*(b[26]-b[31])+(dchunk)(a[30]-a[27])*(b[27]-b[30])+(dchunk)(a[29]-a[28])*(b[28]-b[29]); c[57]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[21]; t=co+s +(dchunk)(a[36]-a[22])*(b[22]-b[36])+(dchunk)(a[35]-a[23])*(b[23]-b[35])+(dchunk)(a[34]-a[24])*(b[24]-b[34])+(dchunk)(a[33]-a[25])*(b[25]-b[33])+(dchunk)(a[32]-a[26])*(b[26]-b[32])+(dchunk)(a[31]-a[27])*(b[27]-b[31])+(dchunk)(a[30]-a[28])*(b[28]-b[30]); c[58]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[22]; t=co+s +(dchunk)(a[36]-a[23])*(b[23]-b[36])+(dchunk)(a[35]-a[24])*(b[24]-b[35])+(dchunk)(a[34]-a[25])*(b[25]-b[34])+(dchunk)(a[33]-a[26])*(b[26]-b[33])+(dchunk)(a[32]-a[27])*(b[27]-b[32])+(dchunk)(a[31]-a[28])*(b[28]-b[31])+(dchunk)(a[30]-a[29])*(b[29]-b[30]); c[59]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[23]; t=co+s +(dchunk)(a[36]-a[24])*(b[24]-b[36])+(dchunk)(a[35]-a[25])*(b[25]-b[35])+(dchunk)(a[34]-a[26])*(b[26]-b[34])+(dchunk)(a[33]-a[27])*(b[27]-b[33])+(dchunk)(a[32]-a[28])*(b[28]-b[32])+(dchunk)(a[31]-a[29])*(b[29]-b[31]); c[60]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[24]; t=co+s +(dchunk)(a[36]-a[25])*(b[25]-b[36])+(dchunk)(a[35]-a[26])*(b[26]-b[35])+(dchunk)(a[34]-a[27])*(b[27]-b[34])+(dchunk)(a[33]-a[28])*(b[28]-b[33])+(dchunk)(a[32]-a[29])*(b[29]-b[32])+(dchunk)(a[31]-a[30])*(b[30]-b[31]); c[61]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[25]; t=co+s +(dchunk)(a[36]-a[26])*(b[26]-b[36])+(dchunk)(a[35]-a[27])*(b[27]-b[35])+(dchunk)(a[34]-a[28])*(b[28]-b[34])+(dchunk)(a[33]-a[29])*(b[29]-b[33])+(dchunk)(a[32]-a[30])*(b[30]-b[32]); c[62]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[26]; t=co+s +(dchunk)(a[36]-a[27])*(b[27]-b[36])+(dchunk)(a[35]-a[28])*(b[28]-b[35])+(dchunk)(a[34]-a[29])*(b[29]-b[34])+(dchunk)(a[33]-a[30])*(b[30]-b[33])+(dchunk)(a[32]-a[31])*(b[31]-b[32]); c[63]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[27]; t=co+s +(dchunk)(a[36]-a[28])*(b[28]-b[36])+(dchunk)(a[35]-a[29])*(b[29]-b[35])+(dchunk)(a[34]-a[30])*(b[30]-b[34])+(dchunk)(a[33]-a[31])*(b[31]-b[33]); c[64]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[28]; t=co+s +(dchunk)(a[36]-a[29])*(b[29]-b[36])+(dchunk)(a[35]-a[30])*(b[30]-b[35])+(dchunk)(a[34]-a[31])*(b[31]-b[34])+(dchunk)(a[33]-a[32])*(b[32]-b[33]); c[65]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[29]; t=co+s +(dchunk)(a[36]-a[30])*(b[30]-b[36])+(dchunk)(a[35]-a[31])*(b[31]-b[35])+(dchunk)(a[34]-a[32])*(b[32]-b[34]); c[66]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[30]; t=co+s +(dchunk)(a[36]-a[31])*(b[31]-b[36])+(dchunk)(a[35]-a[32])*(b[32]-b[35])+(dchunk)(a[34]-a[33])*(b[33]-b[34]); c[67]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[31]; t=co+s +(dchunk)(a[36]-a[32])*(b[32]-b[36])+(dchunk)(a[35]-a[33])*(b[33]-b[35]); c[68]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[32]; t=co+s +(dchunk)(a[36]-a[33])*(b[33]-b[36])+(dchunk)(a[35]-a[34])*(b[34]-b[35]); c[69]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[33]; t=co+s +(dchunk)(a[36]-a[34])*(b[34]-b[36]); c[70]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[34]; t=co+s +(dchunk)(a[36]-a[35])*(b[35]-b[36]); c[71]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	s-=d[35]; t=co+s ; c[72]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	c[73]=(chunk)co;


#else

    	t=(dchunk)a[0]*b[0]; c[0]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[1]+(dchunk)a[1]*b[0]; c[1]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[2]+(dchunk)a[1]*b[1]+(dchunk)a[2]*b[0]; c[2]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[3]+(dchunk)a[1]*b[2]+(dchunk)a[2]*b[1]+(dchunk)a[3]*b[0]; c[3]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[4]+(dchunk)a[1]*b[3]+(dchunk)a[2]*b[2]+(dchunk)a[3]*b[1]+(dchunk)a[4]*b[0]; c[4]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[5]+(dchunk)a[1]*b[4]+(dchunk)a[2]*b[3]+(dchunk)a[3]*b[2]+(dchunk)a[4]*b[1]+(dchunk)a[5]*b[0]; c[5]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[6]+(dchunk)a[1]*b[5]+(dchunk)a[2]*b[4]+(dchunk)a[3]*b[3]+(dchunk)a[4]*b[2]+(dchunk)a[5]*b[1]+(dchunk)a[6]*b[0]; c[6]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[7]+(dchunk)a[1]*b[6]+(dchunk)a[2]*b[5]+(dchunk)a[3]*b[4]+(dchunk)a[4]*b[3]+(dchunk)a[5]*b[2]+(dchunk)a[6]*b[1]+(dchunk)a[7]*b[0]; c[7]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[8]+(dchunk)a[1]*b[7]+(dchunk)a[2]*b[6]+(dchunk)a[3]*b[5]+(dchunk)a[4]*b[4]+(dchunk)a[5]*b[3]+(dchunk)a[6]*b[2]+(dchunk)a[7]*b[1]+(dchunk)a[8]*b[0]; c[8]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[9]+(dchunk)a[1]*b[8]+(dchunk)a[2]*b[7]+(dchunk)a[3]*b[6]+(dchunk)a[4]*b[5]+(dchunk)a[5]*b[4]+(dchunk)a[6]*b[3]+(dchunk)a[7]*b[2]+(dchunk)a[8]*b[1]+(dchunk)a[9]*b[0]; c[9]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[10]+(dchunk)a[1]*b[9]+(dchunk)a[2]*b[8]+(dchunk)a[3]*b[7]+(dchunk)a[4]*b[6]+(dchunk)a[5]*b[5]+(dchunk)a[6]*b[4]+(dchunk)a[7]*b[3]+(dchunk)a[8]*b[2]+(dchunk)a[9]*b[1]+(dchunk)a[10]*b[0]; c[10]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[11]+(dchunk)a[1]*b[10]+(dchunk)a[2]*b[9]+(dchunk)a[3]*b[8]+(dchunk)a[4]*b[7]+(dchunk)a[5]*b[6]+(dchunk)a[6]*b[5]+(dchunk)a[7]*b[4]+(dchunk)a[8]*b[3]+(dchunk)a[9]*b[2]+(dchunk)a[10]*b[1]+(dchunk)a[11]*b[0]; c[11]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[12]+(dchunk)a[1]*b[11]+(dchunk)a[2]*b[10]+(dchunk)a[3]*b[9]+(dchunk)a[4]*b[8]+(dchunk)a[5]*b[7]+(dchunk)a[6]*b[6]+(dchunk)a[7]*b[5]+(dchunk)a[8]*b[4]+(dchunk)a[9]*b[3]+(dchunk)a[10]*b[2]+(dchunk)a[11]*b[1]+(dchunk)a[12]*b[0]; c[12]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[13]+(dchunk)a[1]*b[12]+(dchunk)a[2]*b[11]+(dchunk)a[3]*b[10]+(dchunk)a[4]*b[9]+(dchunk)a[5]*b[8]+(dchunk)a[6]*b[7]+(dchunk)a[7]*b[6]+(dchunk)a[8]*b[5]+(dchunk)a[9]*b[4]+(dchunk)a[10]*b[3]+(dchunk)a[11]*b[2]+(dchunk)a[12]*b[1]+(dchunk)a[13]*b[0]; c[13]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[14]+(dchunk)a[1]*b[13]+(dchunk)a[2]*b[12]+(dchunk)a[3]*b[11]+(dchunk)a[4]*b[10]+(dchunk)a[5]*b[9]+(dchunk)a[6]*b[8]+(dchunk)a[7]*b[7]+(dchunk)a[8]*b[6]+(dchunk)a[9]*b[5]+(dchunk)a[10]*b[4]+(dchunk)a[11]*b[3]+(dchunk)a[12]*b[2]+(dchunk)a[13]*b[1]+(dchunk)a[14]*b[0]; c[14]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[15]+(dchunk)a[1]*b[14]+(dchunk)a[2]*b[13]+(dchunk)a[3]*b[12]+(dchunk)a[4]*b[11]+(dchunk)a[5]*b[10]+(dchunk)a[6]*b[9]+(dchunk)a[7]*b[8]+(dchunk)a[8]*b[7]+(dchunk)a[9]*b[6]+(dchunk)a[10]*b[5]+(dchunk)a[11]*b[4]+(dchunk)a[12]*b[3]+(dchunk)a[13]*b[2]+(dchunk)a[14]*b[1]+(dchunk)a[15]*b[0]; c[15]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[16]+(dchunk)a[1]*b[15]+(dchunk)a[2]*b[14]+(dchunk)a[3]*b[13]+(dchunk)a[4]*b[12]+(dchunk)a[5]*b[11]+(dchunk)a[6]*b[10]+(dchunk)a[7]*b[9]+(dchunk)a[8]*b[8]+(dchunk)a[9]*b[7]+(dchunk)a[10]*b[6]+(dchunk)a[11]*b[5]+(dchunk)a[12]*b[4]+(dchunk)a[13]*b[3]+(dchunk)a[14]*b[2]+(dchunk)a[15]*b[1]+(dchunk)a[16]*b[0]; c[16]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[17]+(dchunk)a[1]*b[16]+(dchunk)a[2]*b[15]+(dchunk)a[3]*b[14]+(dchunk)a[4]*b[13]+(dchunk)a[5]*b[12]+(dchunk)a[6]*b[11]+(dchunk)a[7]*b[10]+(dchunk)a[8]*b[9]+(dchunk)a[9]*b[8]+(dchunk)a[10]*b[7]+(dchunk)a[11]*b[6]+(dchunk)a[12]*b[5]+(dchunk)a[13]*b[4]+(dchunk)a[14]*b[3]+(dchunk)a[15]*b[2]+(dchunk)a[16]*b[1]+(dchunk)a[17]*b[0]; c[17]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[18]+(dchunk)a[1]*b[17]+(dchunk)a[2]*b[16]+(dchunk)a[3]*b[15]+(dchunk)a[4]*b[14]+(dchunk)a[5]*b[13]+(dchunk)a[6]*b[12]+(dchunk)a[7]*b[11]+(dchunk)a[8]*b[10]+(dchunk)a[9]*b[9]+(dchunk)a[10]*b[8]+(dchunk)a[11]*b[7]+(dchunk)a[12]*b[6]+(dchunk)a[13]*b[5]+(dchunk)a[14]*b[4]+(dchunk)a[15]*b[3]+(dchunk)a[16]*b[2]+(dchunk)a[17]*b[1]+(dchunk)a[18]*b[0]; c[18]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[19]+(dchunk)a[1]*b[18]+(dchunk)a[2]*b[17]+(dchunk)a[3]*b[16]+(dchunk)a[4]*b[15]+(dchunk)a[5]*b[14]+(dchunk)a[6]*b[13]+(dchunk)a[7]*b[12]+(dchunk)a[8]*b[11]+(dchunk)a[9]*b[10]+(dchunk)a[10]*b[9]+(dchunk)a[11]*b[8]+(dchunk)a[12]*b[7]+(dchunk)a[13]*b[6]+(dchunk)a[14]*b[5]+(dchunk)a[15]*b[4]+(dchunk)a[16]*b[3]+(dchunk)a[17]*b[2]+(dchunk)a[18]*b[1]+(dchunk)a[19]*b[0]; c[19]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[20]+(dchunk)a[1]*b[19]+(dchunk)a[2]*b[18]+(dchunk)a[3]*b[17]+(dchunk)a[4]*b[16]+(dchunk)a[5]*b[15]+(dchunk)a[6]*b[14]+(dchunk)a[7]*b[13]+(dchunk)a[8]*b[12]+(dchunk)a[9]*b[11]+(dchunk)a[10]*b[10]+(dchunk)a[11]*b[9]+(dchunk)a[12]*b[8]+(dchunk)a[13]*b[7]+(dchunk)a[14]*b[6]+(dchunk)a[15]*b[5]+(dchunk)a[16]*b[4]+(dchunk)a[17]*b[3]+(dchunk)a[18]*b[2]+(dchunk)a[19]*b[1]+(dchunk)a[20]*b[0]; c[20]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[21]+(dchunk)a[1]*b[20]+(dchunk)a[2]*b[19]+(dchunk)a[3]*b[18]+(dchunk)a[4]*b[17]+(dchunk)a[5]*b[16]+(dchunk)a[6]*b[15]+(dchunk)a[7]*b[14]+(dchunk)a[8]*b[13]+(dchunk)a[9]*b[12]+(dchunk)a[10]*b[11]+(dchunk)a[11]*b[10]+(dchunk)a[12]*b[9]+(dchunk)a[13]*b[8]+(dchunk)a[14]*b[7]+(dchunk)a[15]*b[6]+(dchunk)a[16]*b[5]+(dchunk)a[17]*b[4]+(dchunk)a[18]*b[3]+(dchunk)a[19]*b[2]+(dchunk)a[20]*b[1]+(dchunk)a[21]*b[0]; c[21]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[22]+(dchunk)a[1]*b[21]+(dchunk)a[2]*b[20]+(dchunk)a[3]*b[19]+(dchunk)a[4]*b[18]+(dchunk)a[5]*b[17]+(dchunk)a[6]*b[16]+(dchunk)a[7]*b[15]+(dchunk)a[8]*b[14]+(dchunk)a[9]*b[13]+(dchunk)a[10]*b[12]+(dchunk)a[11]*b[11]+(dchunk)a[12]*b[10]+(dchunk)a[13]*b[9]+(dchunk)a[14]*b[8]+(dchunk)a[15]*b[7]+(dchunk)a[16]*b[6]+(dchunk)a[17]*b[5]+(dchunk)a[18]*b[4]+(dchunk)a[19]*b[3]+(dchunk)a[20]*b[2]+(dchunk)a[21]*b[1]+(dchunk)a[22]*b[0]; c[22]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[23]+(dchunk)a[1]*b[22]+(dchunk)a[2]*b[21]+(dchunk)a[3]*b[20]+(dchunk)a[4]*b[19]+(dchunk)a[5]*b[18]+(dchunk)a[6]*b[17]+(dchunk)a[7]*b[16]+(dchunk)a[8]*b[15]+(dchunk)a[9]*b[14]+(dchunk)a[10]*b[13]+(dchunk)a[11]*b[12]+(dchunk)a[12]*b[11]+(dchunk)a[13]*b[10]+(dchunk)a[14]*b[9]+(dchunk)a[15]*b[8]+(dchunk)a[16]*b[7]+(dchunk)a[17]*b[6]+(dchunk)a[18]*b[5]+(dchunk)a[19]*b[4]+(dchunk)a[20]*b[3]+(dchunk)a[21]*b[2]+(dchunk)a[22]*b[1]+(dchunk)a[23]*b[0]; c[23]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[24]+(dchunk)a[1]*b[23]+(dchunk)a[2]*b[22]+(dchunk)a[3]*b[21]+(dchunk)a[4]*b[20]+(dchunk)a[5]*b[19]+(dchunk)a[6]*b[18]+(dchunk)a[7]*b[17]+(dchunk)a[8]*b[16]+(dchunk)a[9]*b[15]+(dchunk)a[10]*b[14]+(dchunk)a[11]*b[13]+(dchunk)a[12]*b[12]+(dchunk)a[13]*b[11]+(dchunk)a[14]*b[10]+(dchunk)a[15]*b[9]+(dchunk)a[16]*b[8]+(dchunk)a[17]*b[7]+(dchunk)a[18]*b[6]+(dchunk)a[19]*b[5]+(dchunk)a[20]*b[4]+(dchunk)a[21]*b[3]+(dchunk)a[22]*b[2]+(dchunk)a[23]*b[1]+(dchunk)a[24]*b[0]; c[24]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[25]+(dchunk)a[1]*b[24]+(dchunk)a[2]*b[23]+(dchunk)a[3]*b[22]+(dchunk)a[4]*b[21]+(dchunk)a[5]*b[20]+(dchunk)a[6]*b[19]+(dchunk)a[7]*b[18]+(dchunk)a[8]*b[17]+(dchunk)a[9]*b[16]+(dchunk)a[10]*b[15]+(dchunk)a[11]*b[14]+(dchunk)a[12]*b[13]+(dchunk)a[13]*b[12]+(dchunk)a[14]*b[11]+(dchunk)a[15]*b[10]+(dchunk)a[16]*b[9]+(dchunk)a[17]*b[8]+(dchunk)a[18]*b[7]+(dchunk)a[19]*b[6]+(dchunk)a[20]*b[5]+(dchunk)a[21]*b[4]+(dchunk)a[22]*b[3]+(dchunk)a[23]*b[2]+(dchunk)a[24]*b[1]+(dchunk)a[25]*b[0]; c[25]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[26]+(dchunk)a[1]*b[25]+(dchunk)a[2]*b[24]+(dchunk)a[3]*b[23]+(dchunk)a[4]*b[22]+(dchunk)a[5]*b[21]+(dchunk)a[6]*b[20]+(dchunk)a[7]*b[19]+(dchunk)a[8]*b[18]+(dchunk)a[9]*b[17]+(dchunk)a[10]*b[16]+(dchunk)a[11]*b[15]+(dchunk)a[12]*b[14]+(dchunk)a[13]*b[13]+(dchunk)a[14]*b[12]+(dchunk)a[15]*b[11]+(dchunk)a[16]*b[10]+(dchunk)a[17]*b[9]+(dchunk)a[18]*b[8]+(dchunk)a[19]*b[7]+(dchunk)a[20]*b[6]+(dchunk)a[21]*b[5]+(dchunk)a[22]*b[4]+(dchunk)a[23]*b[3]+(dchunk)a[24]*b[2]+(dchunk)a[25]*b[1]+(dchunk)a[26]*b[0]; c[26]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[27]+(dchunk)a[1]*b[26]+(dchunk)a[2]*b[25]+(dchunk)a[3]*b[24]+(dchunk)a[4]*b[23]+(dchunk)a[5]*b[22]+(dchunk)a[6]*b[21]+(dchunk)a[7]*b[20]+(dchunk)a[8]*b[19]+(dchunk)a[9]*b[18]+(dchunk)a[10]*b[17]+(dchunk)a[11]*b[16]+(dchunk)a[12]*b[15]+(dchunk)a[13]*b[14]+(dchunk)a[14]*b[13]+(dchunk)a[15]*b[12]+(dchunk)a[16]*b[11]+(dchunk)a[17]*b[10]+(dchunk)a[18]*b[9]+(dchunk)a[19]*b[8]+(dchunk)a[20]*b[7]+(dchunk)a[21]*b[6]+(dchunk)a[22]*b[5]+(dchunk)a[23]*b[4]+(dchunk)a[24]*b[3]+(dchunk)a[25]*b[2]+(dchunk)a[26]*b[1]+(dchunk)a[27]*b[0]; c[27]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[28]+(dchunk)a[1]*b[27]+(dchunk)a[2]*b[26]+(dchunk)a[3]*b[25]+(dchunk)a[4]*b[24]+(dchunk)a[5]*b[23]+(dchunk)a[6]*b[22]+(dchunk)a[7]*b[21]+(dchunk)a[8]*b[20]+(dchunk)a[9]*b[19]+(dchunk)a[10]*b[18]+(dchunk)a[11]*b[17]+(dchunk)a[12]*b[16]+(dchunk)a[13]*b[15]+(dchunk)a[14]*b[14]+(dchunk)a[15]*b[13]+(dchunk)a[16]*b[12]+(dchunk)a[17]*b[11]+(dchunk)a[18]*b[10]+(dchunk)a[19]*b[9]+(dchunk)a[20]*b[8]+(dchunk)a[21]*b[7]+(dchunk)a[22]*b[6]+(dchunk)a[23]*b[5]+(dchunk)a[24]*b[4]+(dchunk)a[25]*b[3]+(dchunk)a[26]*b[2]+(dchunk)a[27]*b[1]+(dchunk)a[28]*b[0]; c[28]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[29]+(dchunk)a[1]*b[28]+(dchunk)a[2]*b[27]+(dchunk)a[3]*b[26]+(dchunk)a[4]*b[25]+(dchunk)a[5]*b[24]+(dchunk)a[6]*b[23]+(dchunk)a[7]*b[22]+(dchunk)a[8]*b[21]+(dchunk)a[9]*b[20]+(dchunk)a[10]*b[19]+(dchunk)a[11]*b[18]+(dchunk)a[12]*b[17]+(dchunk)a[13]*b[16]+(dchunk)a[14]*b[15]+(dchunk)a[15]*b[14]+(dchunk)a[16]*b[13]+(dchunk)a[17]*b[12]+(dchunk)a[18]*b[11]+(dchunk)a[19]*b[10]+(dchunk)a[20]*b[9]+(dchunk)a[21]*b[8]+(dchunk)a[22]*b[7]+(dchunk)a[23]*b[6]+(dchunk)a[24]*b[5]+(dchunk)a[25]*b[4]+(dchunk)a[26]*b[3]+(dchunk)a[27]*b[2]+(dchunk)a[28]*b[1]+(dchunk)a[29]*b[0]; c[29]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[30]+(dchunk)a[1]*b[29]+(dchunk)a[2]*b[28]+(dchunk)a[3]*b[27]+(dchunk)a[4]*b[26]+(dchunk)a[5]*b[25]+(dchunk)a[6]*b[24]+(dchunk)a[7]*b[23]+(dchunk)a[8]*b[22]+(dchunk)a[9]*b[21]+(dchunk)a[10]*b[20]+(dchunk)a[11]*b[19]+(dchunk)a[12]*b[18]+(dchunk)a[13]*b[17]+(dchunk)a[14]*b[16]+(dchunk)a[15]*b[15]+(dchunk)a[16]*b[14]+(dchunk)a[17]*b[13]+(dchunk)a[18]*b[12]+(dchunk)a[19]*b[11]+(dchunk)a[20]*b[10]+(dchunk)a[21]*b[9]+(dchunk)a[22]*b[8]+(dchunk)a[23]*b[7]+(dchunk)a[24]*b[6]+(dchunk)a[25]*b[5]+(dchunk)a[26]*b[4]+(dchunk)a[27]*b[3]+(dchunk)a[28]*b[2]+(dchunk)a[29]*b[1]+(dchunk)a[30]*b[0]; c[30]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[31]+(dchunk)a[1]*b[30]+(dchunk)a[2]*b[29]+(dchunk)a[3]*b[28]+(dchunk)a[4]*b[27]+(dchunk)a[5]*b[26]+(dchunk)a[6]*b[25]+(dchunk)a[7]*b[24]+(dchunk)a[8]*b[23]+(dchunk)a[9]*b[22]+(dchunk)a[10]*b[21]+(dchunk)a[11]*b[20]+(dchunk)a[12]*b[19]+(dchunk)a[13]*b[18]+(dchunk)a[14]*b[17]+(dchunk)a[15]*b[16]+(dchunk)a[16]*b[15]+(dchunk)a[17]*b[14]+(dchunk)a[18]*b[13]+(dchunk)a[19]*b[12]+(dchunk)a[20]*b[11]+(dchunk)a[21]*b[10]+(dchunk)a[22]*b[9]+(dchunk)a[23]*b[8]+(dchunk)a[24]*b[7]+(dchunk)a[25]*b[6]+(dchunk)a[26]*b[5]+(dchunk)a[27]*b[4]+(dchunk)a[28]*b[3]+(dchunk)a[29]*b[2]+(dchunk)a[30]*b[1]+(dchunk)a[31]*b[0]; c[31]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[32]+(dchunk)a[1]*b[31]+(dchunk)a[2]*b[30]+(dchunk)a[3]*b[29]+(dchunk)a[4]*b[28]+(dchunk)a[5]*b[27]+(dchunk)a[6]*b[26]+(dchunk)a[7]*b[25]+(dchunk)a[8]*b[24]+(dchunk)a[9]*b[23]+(dchunk)a[10]*b[22]+(dchunk)a[11]*b[21]+(dchunk)a[12]*b[20]+(dchunk)a[13]*b[19]+(dchunk)a[14]*b[18]+(dchunk)a[15]*b[17]+(dchunk)a[16]*b[16]+(dchunk)a[17]*b[15]+(dchunk)a[18]*b[14]+(dchunk)a[19]*b[13]+(dchunk)a[20]*b[12]+(dchunk)a[21]*b[11]+(dchunk)a[22]*b[10]+(dchunk)a[23]*b[9]+(dchunk)a[24]*b[8]+(dchunk)a[25]*b[7]+(dchunk)a[26]*b[6]+(dchunk)a[27]*b[5]+(dchunk)a[28]*b[4]+(dchunk)a[29]*b[3]+(dchunk)a[30]*b[2]+(dchunk)a[31]*b[1]+(dchunk)a[32]*b[0]; c[32]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[33]+(dchunk)a[1]*b[32]+(dchunk)a[2]*b[31]+(dchunk)a[3]*b[30]+(dchunk)a[4]*b[29]+(dchunk)a[5]*b[28]+(dchunk)a[6]*b[27]+(dchunk)a[7]*b[26]+(dchunk)a[8]*b[25]+(dchunk)a[9]*b[24]+(dchunk)a[10]*b[23]+(dchunk)a[11]*b[22]+(dchunk)a[12]*b[21]+(dchunk)a[13]*b[20]+(dchunk)a[14]*b[19]+(dchunk)a[15]*b[18]+(dchunk)a[16]*b[17]+(dchunk)a[17]*b[16]+(dchunk)a[18]*b[15]+(dchunk)a[19]*b[14]+(dchunk)a[20]*b[13]+(dchunk)a[21]*b[12]+(dchunk)a[22]*b[11]+(dchunk)a[23]*b[10]+(dchunk)a[24]*b[9]+(dchunk)a[25]*b[8]+(dchunk)a[26]*b[7]+(dchunk)a[27]*b[6]+(dchunk)a[28]*b[5]+(dchunk)a[29]*b[4]+(dchunk)a[30]*b[3]+(dchunk)a[31]*b[2]+(dchunk)a[32]*b[1]+(dchunk)a[33]*b[0]; c[33]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[34]+(dchunk)a[1]*b[33]+(dchunk)a[2]*b[32]+(dchunk)a[3]*b[31]+(dchunk)a[4]*b[30]+(dchunk)a[5]*b[29]+(dchunk)a[6]*b[28]+(dchunk)a[7]*b[27]+(dchunk)a[8]*b[26]+(dchunk)a[9]*b[25]+(dchunk)a[10]*b[24]+(dchunk)a[11]*b[23]+(dchunk)a[12]*b[22]+(dchunk)a[13]*b[21]+(dchunk)a[14]*b[20]+(dchunk)a[15]*b[19]+(dchunk)a[16]*b[18]+(dchunk)a[17]*b[17]+(dchunk)a[18]*b[16]+(dchunk)a[19]*b[15]+(dchunk)a[20]*b[14]+(dchunk)a[21]*b[13]+(dchunk)a[22]*b[12]+(dchunk)a[23]*b[11]+(dchunk)a[24]*b[10]+(dchunk)a[25]*b[9]+(dchunk)a[26]*b[8]+(dchunk)a[27]*b[7]+(dchunk)a[28]*b[6]+(dchunk)a[29]*b[5]+(dchunk)a[30]*b[4]+(dchunk)a[31]*b[3]+(dchunk)a[32]*b[2]+(dchunk)a[33]*b[1]+(dchunk)a[34]*b[0]; c[34]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[35]+(dchunk)a[1]*b[34]+(dchunk)a[2]*b[33]+(dchunk)a[3]*b[32]+(dchunk)a[4]*b[31]+(dchunk)a[5]*b[30]+(dchunk)a[6]*b[29]+(dchunk)a[7]*b[28]+(dchunk)a[8]*b[27]+(dchunk)a[9]*b[26]+(dchunk)a[10]*b[25]+(dchunk)a[11]*b[24]+(dchunk)a[12]*b[23]+(dchunk)a[13]*b[22]+(dchunk)a[14]*b[21]+(dchunk)a[15]*b[20]+(dchunk)a[16]*b[19]+(dchunk)a[17]*b[18]+(dchunk)a[18]*b[17]+(dchunk)a[19]*b[16]+(dchunk)a[20]*b[15]+(dchunk)a[21]*b[14]+(dchunk)a[22]*b[13]+(dchunk)a[23]*b[12]+(dchunk)a[24]*b[11]+(dchunk)a[25]*b[10]+(dchunk)a[26]*b[9]+(dchunk)a[27]*b[8]+(dchunk)a[28]*b[7]+(dchunk)a[29]*b[6]+(dchunk)a[30]*b[5]+(dchunk)a[31]*b[4]+(dchunk)a[32]*b[3]+(dchunk)a[33]*b[2]+(dchunk)a[34]*b[1]+(dchunk)a[35]*b[0]; c[35]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[0]*b[36]+(dchunk)a[1]*b[35]+(dchunk)a[2]*b[34]+(dchunk)a[3]*b[33]+(dchunk)a[4]*b[32]+(dchunk)a[5]*b[31]+(dchunk)a[6]*b[30]+(dchunk)a[7]*b[29]+(dchunk)a[8]*b[28]+(dchunk)a[9]*b[27]+(dchunk)a[10]*b[26]+(dchunk)a[11]*b[25]+(dchunk)a[12]*b[24]+(dchunk)a[13]*b[23]+(dchunk)a[14]*b[22]+(dchunk)a[15]*b[21]+(dchunk)a[16]*b[20]+(dchunk)a[17]*b[19]+(dchunk)a[18]*b[18]+(dchunk)a[19]*b[17]+(dchunk)a[20]*b[16]+(dchunk)a[21]*b[15]+(dchunk)a[22]*b[14]+(dchunk)a[23]*b[13]+(dchunk)a[24]*b[12]+(dchunk)a[25]*b[11]+(dchunk)a[26]*b[10]+(dchunk)a[27]*b[9]+(dchunk)a[28]*b[8]+(dchunk)a[29]*b[7]+(dchunk)a[30]*b[6]+(dchunk)a[31]*b[5]+(dchunk)a[32]*b[4]+(dchunk)a[33]*b[3]+(dchunk)a[34]*b[2]+(dchunk)a[35]*b[1]+(dchunk)a[36]*b[0]; c[36]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[1]*b[36]+(dchunk)a[2]*b[35]+(dchunk)a[3]*b[34]+(dchunk)a[4]*b[33]+(dchunk)a[5]*b[32]+(dchunk)a[6]*b[31]+(dchunk)a[7]*b[30]+(dchunk)a[8]*b[29]+(dchunk)a[9]*b[28]+(dchunk)a[10]*b[27]+(dchunk)a[11]*b[26]+(dchunk)a[12]*b[25]+(dchunk)a[13]*b[24]+(dchunk)a[14]*b[23]+(dchunk)a[15]*b[22]+(dchunk)a[16]*b[21]+(dchunk)a[17]*b[20]+(dchunk)a[18]*b[19]+(dchunk)a[19]*b[18]+(dchunk)a[20]*b[17]+(dchunk)a[21]*b[16]+(dchunk)a[22]*b[15]+(dchunk)a[23]*b[14]+(dchunk)a[24]*b[13]+(dchunk)a[25]*b[12]+(dchunk)a[26]*b[11]+(dchunk)a[27]*b[10]+(dchunk)a[28]*b[9]+(dchunk)a[29]*b[8]+(dchunk)a[30]*b[7]+(dchunk)a[31]*b[6]+(dchunk)a[32]*b[5]+(dchunk)a[33]*b[4]+(dchunk)a[34]*b[3]+(dchunk)a[35]*b[2]+(dchunk)a[36]*b[1]; c[37]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[2]*b[36]+(dchunk)a[3]*b[35]+(dchunk)a[4]*b[34]+(dchunk)a[5]*b[33]+(dchunk)a[6]*b[32]+(dchunk)a[7]*b[31]+(dchunk)a[8]*b[30]+(dchunk)a[9]*b[29]+(dchunk)a[10]*b[28]+(dchunk)a[11]*b[27]+(dchunk)a[12]*b[26]+(dchunk)a[13]*b[25]+(dchunk)a[14]*b[24]+(dchunk)a[15]*b[23]+(dchunk)a[16]*b[22]+(dchunk)a[17]*b[21]+(dchunk)a[18]*b[20]+(dchunk)a[19]*b[19]+(dchunk)a[20]*b[18]+(dchunk)a[21]*b[17]+(dchunk)a[22]*b[16]+(dchunk)a[23]*b[15]+(dchunk)a[24]*b[14]+(dchunk)a[25]*b[13]+(dchunk)a[26]*b[12]+(dchunk)a[27]*b[11]+(dchunk)a[28]*b[10]+(dchunk)a[29]*b[9]+(dchunk)a[30]*b[8]+(dchunk)a[31]*b[7]+(dchunk)a[32]*b[6]+(dchunk)a[33]*b[5]+(dchunk)a[34]*b[4]+(dchunk)a[35]*b[3]+(dchunk)a[36]*b[2]; c[38]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[3]*b[36]+(dchunk)a[4]*b[35]+(dchunk)a[5]*b[34]+(dchunk)a[6]*b[33]+(dchunk)a[7]*b[32]+(dchunk)a[8]*b[31]+(dchunk)a[9]*b[30]+(dchunk)a[10]*b[29]+(dchunk)a[11]*b[28]+(dchunk)a[12]*b[27]+(dchunk)a[13]*b[26]+(dchunk)a[14]*b[25]+(dchunk)a[15]*b[24]+(dchunk)a[16]*b[23]+(dchunk)a[17]*b[22]+(dchunk)a[18]*b[21]+(dchunk)a[19]*b[20]+(dchunk)a[20]*b[19]+(dchunk)a[21]*b[18]+(dchunk)a[22]*b[17]+(dchunk)a[23]*b[16]+(dchunk)a[24]*b[15]+(dchunk)a[25]*b[14]+(dchunk)a[26]*b[13]+(dchunk)a[27]*b[12]+(dchunk)a[28]*b[11]+(dchunk)a[29]*b[10]+(dchunk)a[30]*b[9]+(dchunk)a[31]*b[8]+(dchunk)a[32]*b[7]+(dchunk)a[33]*b[6]+(dchunk)a[34]*b[5]+(dchunk)a[35]*b[4]+(dchunk)a[36]*b[3]; c[39]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[4]*b[36]+(dchunk)a[5]*b[35]+(dchunk)a[6]*b[34]+(dchunk)a[7]*b[33]+(dchunk)a[8]*b[32]+(dchunk)a[9]*b[31]+(dchunk)a[10]*b[30]+(dchunk)a[11]*b[29]+(dchunk)a[12]*b[28]+(dchunk)a[13]*b[27]+(dchunk)a[14]*b[26]+(dchunk)a[15]*b[25]+(dchunk)a[16]*b[24]+(dchunk)a[17]*b[23]+(dchunk)a[18]*b[22]+(dchunk)a[19]*b[21]+(dchunk)a[20]*b[20]+(dchunk)a[21]*b[19]+(dchunk)a[22]*b[18]+(dchunk)a[23]*b[17]+(dchunk)a[24]*b[16]+(dchunk)a[25]*b[15]+(dchunk)a[26]*b[14]+(dchunk)a[27]*b[13]+(dchunk)a[28]*b[12]+(dchunk)a[29]*b[11]+(dchunk)a[30]*b[10]+(dchunk)a[31]*b[9]+(dchunk)a[32]*b[8]+(dchunk)a[33]*b[7]+(dchunk)a[34]*b[6]+(dchunk)a[35]*b[5]+(dchunk)a[36]*b[4]; c[40]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[5]*b[36]+(dchunk)a[6]*b[35]+(dchunk)a[7]*b[34]+(dchunk)a[8]*b[33]+(dchunk)a[9]*b[32]+(dchunk)a[10]*b[31]+(dchunk)a[11]*b[30]+(dchunk)a[12]*b[29]+(dchunk)a[13]*b[28]+(dchunk)a[14]*b[27]+(dchunk)a[15]*b[26]+(dchunk)a[16]*b[25]+(dchunk)a[17]*b[24]+(dchunk)a[18]*b[23]+(dchunk)a[19]*b[22]+(dchunk)a[20]*b[21]+(dchunk)a[21]*b[20]+(dchunk)a[22]*b[19]+(dchunk)a[23]*b[18]+(dchunk)a[24]*b[17]+(dchunk)a[25]*b[16]+(dchunk)a[26]*b[15]+(dchunk)a[27]*b[14]+(dchunk)a[28]*b[13]+(dchunk)a[29]*b[12]+(dchunk)a[30]*b[11]+(dchunk)a[31]*b[10]+(dchunk)a[32]*b[9]+(dchunk)a[33]*b[8]+(dchunk)a[34]*b[7]+(dchunk)a[35]*b[6]+(dchunk)a[36]*b[5]; c[41]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[6]*b[36]+(dchunk)a[7]*b[35]+(dchunk)a[8]*b[34]+(dchunk)a[9]*b[33]+(dchunk)a[10]*b[32]+(dchunk)a[11]*b[31]+(dchunk)a[12]*b[30]+(dchunk)a[13]*b[29]+(dchunk)a[14]*b[28]+(dchunk)a[15]*b[27]+(dchunk)a[16]*b[26]+(dchunk)a[17]*b[25]+(dchunk)a[18]*b[24]+(dchunk)a[19]*b[23]+(dchunk)a[20]*b[22]+(dchunk)a[21]*b[21]+(dchunk)a[22]*b[20]+(dchunk)a[23]*b[19]+(dchunk)a[24]*b[18]+(dchunk)a[25]*b[17]+(dchunk)a[26]*b[16]+(dchunk)a[27]*b[15]+(dchunk)a[28]*b[14]+(dchunk)a[29]*b[13]+(dchunk)a[30]*b[12]+(dchunk)a[31]*b[11]+(dchunk)a[32]*b[10]+(dchunk)a[33]*b[9]+(dchunk)a[34]*b[8]+(dchunk)a[35]*b[7]+(dchunk)a[36]*b[6]; c[42]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[7]*b[36]+(dchunk)a[8]*b[35]+(dchunk)a[9]*b[34]+(dchunk)a[10]*b[33]+(dchunk)a[11]*b[32]+(dchunk)a[12]*b[31]+(dchunk)a[13]*b[30]+(dchunk)a[14]*b[29]+(dchunk)a[15]*b[28]+(dchunk)a[16]*b[27]+(dchunk)a[17]*b[26]+(dchunk)a[18]*b[25]+(dchunk)a[19]*b[24]+(dchunk)a[20]*b[23]+(dchunk)a[21]*b[22]+(dchunk)a[22]*b[21]+(dchunk)a[23]*b[20]+(dchunk)a[24]*b[19]+(dchunk)a[25]*b[18]+(dchunk)a[26]*b[17]+(dchunk)a[27]*b[16]+(dchunk)a[28]*b[15]+(dchunk)a[29]*b[14]+(dchunk)a[30]*b[13]+(dchunk)a[31]*b[12]+(dchunk)a[32]*b[11]+(dchunk)a[33]*b[10]+(dchunk)a[34]*b[9]+(dchunk)a[35]*b[8]+(dchunk)a[36]*b[7]; c[43]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[8]*b[36]+(dchunk)a[9]*b[35]+(dchunk)a[10]*b[34]+(dchunk)a[11]*b[33]+(dchunk)a[12]*b[32]+(dchunk)a[13]*b[31]+(dchunk)a[14]*b[30]+(dchunk)a[15]*b[29]+(dchunk)a[16]*b[28]+(dchunk)a[17]*b[27]+(dchunk)a[18]*b[26]+(dchunk)a[19]*b[25]+(dchunk)a[20]*b[24]+(dchunk)a[21]*b[23]+(dchunk)a[22]*b[22]+(dchunk)a[23]*b[21]+(dchunk)a[24]*b[20]+(dchunk)a[25]*b[19]+(dchunk)a[26]*b[18]+(dchunk)a[27]*b[17]+(dchunk)a[28]*b[16]+(dchunk)a[29]*b[15]+(dchunk)a[30]*b[14]+(dchunk)a[31]*b[13]+(dchunk)a[32]*b[12]+(dchunk)a[33]*b[11]+(dchunk)a[34]*b[10]+(dchunk)a[35]*b[9]+(dchunk)a[36]*b[8]; c[44]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[9]*b[36]+(dchunk)a[10]*b[35]+(dchunk)a[11]*b[34]+(dchunk)a[12]*b[33]+(dchunk)a[13]*b[32]+(dchunk)a[14]*b[31]+(dchunk)a[15]*b[30]+(dchunk)a[16]*b[29]+(dchunk)a[17]*b[28]+(dchunk)a[18]*b[27]+(dchunk)a[19]*b[26]+(dchunk)a[20]*b[25]+(dchunk)a[21]*b[24]+(dchunk)a[22]*b[23]+(dchunk)a[23]*b[22]+(dchunk)a[24]*b[21]+(dchunk)a[25]*b[20]+(dchunk)a[26]*b[19]+(dchunk)a[27]*b[18]+(dchunk)a[28]*b[17]+(dchunk)a[29]*b[16]+(dchunk)a[30]*b[15]+(dchunk)a[31]*b[14]+(dchunk)a[32]*b[13]+(dchunk)a[33]*b[12]+(dchunk)a[34]*b[11]+(dchunk)a[35]*b[10]+(dchunk)a[36]*b[9]; c[45]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[10]*b[36]+(dchunk)a[11]*b[35]+(dchunk)a[12]*b[34]+(dchunk)a[13]*b[33]+(dchunk)a[14]*b[32]+(dchunk)a[15]*b[31]+(dchunk)a[16]*b[30]+(dchunk)a[17]*b[29]+(dchunk)a[18]*b[28]+(dchunk)a[19]*b[27]+(dchunk)a[20]*b[26]+(dchunk)a[21]*b[25]+(dchunk)a[22]*b[24]+(dchunk)a[23]*b[23]+(dchunk)a[24]*b[22]+(dchunk)a[25]*b[21]+(dchunk)a[26]*b[20]+(dchunk)a[27]*b[19]+(dchunk)a[28]*b[18]+(dchunk)a[29]*b[17]+(dchunk)a[30]*b[16]+(dchunk)a[31]*b[15]+(dchunk)a[32]*b[14]+(dchunk)a[33]*b[13]+(dchunk)a[34]*b[12]+(dchunk)a[35]*b[11]+(dchunk)a[36]*b[10]; c[46]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[11]*b[36]+(dchunk)a[12]*b[35]+(dchunk)a[13]*b[34]+(dchunk)a[14]*b[33]+(dchunk)a[15]*b[32]+(dchunk)a[16]*b[31]+(dchunk)a[17]*b[30]+(dchunk)a[18]*b[29]+(dchunk)a[19]*b[28]+(dchunk)a[20]*b[27]+(dchunk)a[21]*b[26]+(dchunk)a[22]*b[25]+(dchunk)a[23]*b[24]+(dchunk)a[24]*b[23]+(dchunk)a[25]*b[22]+(dchunk)a[26]*b[21]+(dchunk)a[27]*b[20]+(dchunk)a[28]*b[19]+(dchunk)a[29]*b[18]+(dchunk)a[30]*b[17]+(dchunk)a[31]*b[16]+(dchunk)a[32]*b[15]+(dchunk)a[33]*b[14]+(dchunk)a[34]*b[13]+(dchunk)a[35]*b[12]+(dchunk)a[36]*b[11]; c[47]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[12]*b[36]+(dchunk)a[13]*b[35]+(dchunk)a[14]*b[34]+(dchunk)a[15]*b[33]+(dchunk)a[16]*b[32]+(dchunk)a[17]*b[31]+(dchunk)a[18]*b[30]+(dchunk)a[19]*b[29]+(dchunk)a[20]*b[28]+(dchunk)a[21]*b[27]+(dchunk)a[22]*b[26]+(dchunk)a[23]*b[25]+(dchunk)a[24]*b[24]+(dchunk)a[25]*b[23]+(dchunk)a[26]*b[22]+(dchunk)a[27]*b[21]+(dchunk)a[28]*b[20]+(dchunk)a[29]*b[19]+(dchunk)a[30]*b[18]+(dchunk)a[31]*b[17]+(dchunk)a[32]*b[16]+(dchunk)a[33]*b[15]+(dchunk)a[34]*b[14]+(dchunk)a[35]*b[13]+(dchunk)a[36]*b[12]; c[48]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[13]*b[36]+(dchunk)a[14]*b[35]+(dchunk)a[15]*b[34]+(dchunk)a[16]*b[33]+(dchunk)a[17]*b[32]+(dchunk)a[18]*b[31]+(dchunk)a[19]*b[30]+(dchunk)a[20]*b[29]+(dchunk)a[21]*b[28]+(dchunk)a[22]*b[27]+(dchunk)a[23]*b[26]+(dchunk)a[24]*b[25]+(dchunk)a[25]*b[24]+(dchunk)a[26]*b[23]+(dchunk)a[27]*b[22]+(dchunk)a[28]*b[21]+(dchunk)a[29]*b[20]+(dchunk)a[30]*b[19]+(dchunk)a[31]*b[18]+(dchunk)a[32]*b[17]+(dchunk)a[33]*b[16]+(dchunk)a[34]*b[15]+(dchunk)a[35]*b[14]+(dchunk)a[36]*b[13]; c[49]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[14]*b[36]+(dchunk)a[15]*b[35]+(dchunk)a[16]*b[34]+(dchunk)a[17]*b[33]+(dchunk)a[18]*b[32]+(dchunk)a[19]*b[31]+(dchunk)a[20]*b[30]+(dchunk)a[21]*b[29]+(dchunk)a[22]*b[28]+(dchunk)a[23]*b[27]+(dchunk)a[24]*b[26]+(dchunk)a[25]*b[25]+(dchunk)a[26]*b[24]+(dchunk)a[27]*b[23]+(dchunk)a[28]*b[22]+(dchunk)a[29]*b[21]+(dchunk)a[30]*b[20]+(dchunk)a[31]*b[19]+(dchunk)a[32]*b[18]+(dchunk)a[33]*b[17]+(dchunk)a[34]*b[16]+(dchunk)a[35]*b[15]+(dchunk)a[36]*b[14]; c[50]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[15]*b[36]+(dchunk)a[16]*b[35]+(dchunk)a[17]*b[34]+(dchunk)a[18]*b[33]+(dchunk)a[19]*b[32]+(dchunk)a[20]*b[31]+(dchunk)a[21]*b[30]+(dchunk)a[22]*b[29]+(dchunk)a[23]*b[28]+(dchunk)a[24]*b[27]+(dchunk)a[25]*b[26]+(dchunk)a[26]*b[25]+(dchunk)a[27]*b[24]+(dchunk)a[28]*b[23]+(dchunk)a[29]*b[22]+(dchunk)a[30]*b[21]+(dchunk)a[31]*b[20]+(dchunk)a[32]*b[19]+(dchunk)a[33]*b[18]+(dchunk)a[34]*b[17]+(dchunk)a[35]*b[16]+(dchunk)a[36]*b[15]; c[51]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[16]*b[36]+(dchunk)a[17]*b[35]+(dchunk)a[18]*b[34]+(dchunk)a[19]*b[33]+(dchunk)a[20]*b[32]+(dchunk)a[21]*b[31]+(dchunk)a[22]*b[30]+(dchunk)a[23]*b[29]+(dchunk)a[24]*b[28]+(dchunk)a[25]*b[27]+(dchunk)a[26]*b[26]+(dchunk)a[27]*b[25]+(dchunk)a[28]*b[24]+(dchunk)a[29]*b[23]+(dchunk)a[30]*b[22]+(dchunk)a[31]*b[21]+(dchunk)a[32]*b[20]+(dchunk)a[33]*b[19]+(dchunk)a[34]*b[18]+(dchunk)a[35]*b[17]+(dchunk)a[36]*b[16]; c[52]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[17]*b[36]+(dchunk)a[18]*b[35]+(dchunk)a[19]*b[34]+(dchunk)a[20]*b[33]+(dchunk)a[21]*b[32]+(dchunk)a[22]*b[31]+(dchunk)a[23]*b[30]+(dchunk)a[24]*b[29]+(dchunk)a[25]*b[28]+(dchunk)a[26]*b[27]+(dchunk)a[27]*b[26]+(dchunk)a[28]*b[25]+(dchunk)a[29]*b[24]+(dchunk)a[30]*b[23]+(dchunk)a[31]*b[22]+(dchunk)a[32]*b[21]+(dchunk)a[33]*b[20]+(dchunk)a[34]*b[19]+(dchunk)a[35]*b[18]+(dchunk)a[36]*b[17]; c[53]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[18]*b[36]+(dchunk)a[19]*b[35]+(dchunk)a[20]*b[34]+(dchunk)a[21]*b[33]+(dchunk)a[22]*b[32]+(dchunk)a[23]*b[31]+(dchunk)a[24]*b[30]+(dchunk)a[25]*b[29]+(dchunk)a[26]*b[28]+(dchunk)a[27]*b[27]+(dchunk)a[28]*b[26]+(dchunk)a[29]*b[25]+(dchunk)a[30]*b[24]+(dchunk)a[31]*b[23]+(dchunk)a[32]*b[22]+(dchunk)a[33]*b[21]+(dchunk)a[34]*b[20]+(dchunk)a[35]*b[19]+(dchunk)a[36]*b[18]; c[54]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[19]*b[36]+(dchunk)a[20]*b[35]+(dchunk)a[21]*b[34]+(dchunk)a[22]*b[33]+(dchunk)a[23]*b[32]+(dchunk)a[24]*b[31]+(dchunk)a[25]*b[30]+(dchunk)a[26]*b[29]+(dchunk)a[27]*b[28]+(dchunk)a[28]*b[27]+(dchunk)a[29]*b[26]+(dchunk)a[30]*b[25]+(dchunk)a[31]*b[24]+(dchunk)a[32]*b[23]+(dchunk)a[33]*b[22]+(dchunk)a[34]*b[21]+(dchunk)a[35]*b[20]+(dchunk)a[36]*b[19]; c[55]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[20]*b[36]+(dchunk)a[21]*b[35]+(dchunk)a[22]*b[34]+(dchunk)a[23]*b[33]+(dchunk)a[24]*b[32]+(dchunk)a[25]*b[31]+(dchunk)a[26]*b[30]+(dchunk)a[27]*b[29]+(dchunk)a[28]*b[28]+(dchunk)a[29]*b[27]+(dchunk)a[30]*b[26]+(dchunk)a[31]*b[25]+(dchunk)a[32]*b[24]+(dchunk)a[33]*b[23]+(dchunk)a[34]*b[22]+(dchunk)a[35]*b[21]+(dchunk)a[36]*b[20]; c[56]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[21]*b[36]+(dchunk)a[22]*b[35]+(dchunk)a[23]*b[34]+(dchunk)a[24]*b[33]+(dchunk)a[25]*b[32]+(dchunk)a[26]*b[31]+(dchunk)a[27]*b[30]+(dchunk)a[28]*b[29]+(dchunk)a[29]*b[28]+(dchunk)a[30]*b[27]+(dchunk)a[31]*b[26]+(dchunk)a[32]*b[25]+(dchunk)a[33]*b[24]+(dchunk)a[34]*b[23]+(dchunk)a[35]*b[22]+(dchunk)a[36]*b[21]; c[57]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[22]*b[36]+(dchunk)a[23]*b[35]+(dchunk)a[24]*b[34]+(dchunk)a[25]*b[33]+(dchunk)a[26]*b[32]+(dchunk)a[27]*b[31]+(dchunk)a[28]*b[30]+(dchunk)a[29]*b[29]+(dchunk)a[30]*b[28]+(dchunk)a[31]*b[27]+(dchunk)a[32]*b[26]+(dchunk)a[33]*b[25]+(dchunk)a[34]*b[24]+(dchunk)a[35]*b[23]+(dchunk)a[36]*b[22]; c[58]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[23]*b[36]+(dchunk)a[24]*b[35]+(dchunk)a[25]*b[34]+(dchunk)a[26]*b[33]+(dchunk)a[27]*b[32]+(dchunk)a[28]*b[31]+(dchunk)a[29]*b[30]+(dchunk)a[30]*b[29]+(dchunk)a[31]*b[28]+(dchunk)a[32]*b[27]+(dchunk)a[33]*b[26]+(dchunk)a[34]*b[25]+(dchunk)a[35]*b[24]+(dchunk)a[36]*b[23]; c[59]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[24]*b[36]+(dchunk)a[25]*b[35]+(dchunk)a[26]*b[34]+(dchunk)a[27]*b[33]+(dchunk)a[28]*b[32]+(dchunk)a[29]*b[31]+(dchunk)a[30]*b[30]+(dchunk)a[31]*b[29]+(dchunk)a[32]*b[28]+(dchunk)a[33]*b[27]+(dchunk)a[34]*b[26]+(dchunk)a[35]*b[25]+(dchunk)a[36]*b[24]; c[60]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[25]*b[36]+(dchunk)a[26]*b[35]+(dchunk)a[27]*b[34]+(dchunk)a[28]*b[33]+(dchunk)a[29]*b[32]+(dchunk)a[30]*b[31]+(dchunk)a[31]*b[30]+(dchunk)a[32]*b[29]+(dchunk)a[33]*b[28]+(dchunk)a[34]*b[27]+(dchunk)a[35]*b[26]+(dchunk)a[36]*b[25]; c[61]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[26]*b[36]+(dchunk)a[27]*b[35]+(dchunk)a[28]*b[34]+(dchunk)a[29]*b[33]+(dchunk)a[30]*b[32]+(dchunk)a[31]*b[31]+(dchunk)a[32]*b[30]+(dchunk)a[33]*b[29]+(dchunk)a[34]*b[28]+(dchunk)a[35]*b[27]+(dchunk)a[36]*b[26]; c[62]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[27]*b[36]+(dchunk)a[28]*b[35]+(dchunk)a[29]*b[34]+(dchunk)a[30]*b[33]+(dchunk)a[31]*b[32]+(dchunk)a[32]*b[31]+(dchunk)a[33]*b[30]+(dchunk)a[34]*b[29]+(dchunk)a[35]*b[28]+(dchunk)a[36]*b[27]; c[63]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[28]*b[36]+(dchunk)a[29]*b[35]+(dchunk)a[30]*b[34]+(dchunk)a[31]*b[33]+(dchunk)a[32]*b[32]+(dchunk)a[33]*b[31]+(dchunk)a[34]*b[30]+(dchunk)a[35]*b[29]+(dchunk)a[36]*b[28]; c[64]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[29]*b[36]+(dchunk)a[30]*b[35]+(dchunk)a[31]*b[34]+(dchunk)a[32]*b[33]+(dchunk)a[33]*b[32]+(dchunk)a[34]*b[31]+(dchunk)a[35]*b[30]+(dchunk)a[36]*b[29]; c[65]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[30]*b[36]+(dchunk)a[31]*b[35]+(dchunk)a[32]*b[34]+(dchunk)a[33]*b[33]+(dchunk)a[34]*b[32]+(dchunk)a[35]*b[31]+(dchunk)a[36]*b[30]; c[66]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[31]*b[36]+(dchunk)a[32]*b[35]+(dchunk)a[33]*b[34]+(dchunk)a[34]*b[33]+(dchunk)a[35]*b[32]+(dchunk)a[36]*b[31]; c[67]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[32]*b[36]+(dchunk)a[33]*b[35]+(dchunk)a[34]*b[34]+(dchunk)a[35]*b[33]+(dchunk)a[36]*b[32]; c[68]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[33]*b[36]+(dchunk)a[34]*b[35]+(dchunk)a[35]*b[34]+(dchunk)a[36]*b[33]; c[69]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[34]*b[36]+(dchunk)a[35]*b[35]+(dchunk)a[36]*b[34]; c[70]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[35]*b[36]+(dchunk)a[36]*b[35]; c[71]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	t=t+(dchunk)a[36]*b[36]; c[72]=(chunk)t & BMASK_B1024_28; t=t>>BASEBITS_B1024_28;
	c[73]=(chunk)t;


#endif

#else

#ifndef USE_KARATSUBA

    t=(dchunk)a[0]*b[0];
    c[0]=(chunk)t & BMASK_B1024_28;
    t = t >> BASEBITS_B1024_28;
    for (i=1;i<NLEN_B1024_28;i++)
    {
        k=0; 
        while (k<=i) {t+=(dchunk)a[k]*b[i-k]; k++;}
        c[i]=(chunk)t & BMASK_B1024_28;
        t = t >> BASEBITS_B1024_28;
    }

    for (i=NLEN_B1024_28;i<2*NLEN_B1024_28-1;i++)
    {
        k=i-(NLEN_B1024_28-1);
        while (k<=NLEN_B1024_28-1) {t+=(dchunk)a[k]*b[i-k]; k++;}
        c[i]=(chunk)t & BMASK_B1024_28;
        t = t >> BASEBITS_B1024_28;
    }

    c[2 * NLEN_B1024_28 - 1] = (chunk)t;
#else

    for (i = 0; i < NLEN_B1024_28; i++)
        d[i] = (dchunk)a[i] * b[i];

    s = d[0];
    t = s;
    c[0] = (chunk)t & BMASK_B1024_28;
    t = t >> BASEBITS_B1024_28;

    for (k = 1; k < NLEN_B1024_28; k++)
    {
        s += d[k];
        t += s;
        /*for (i = k; i >= 1 + k / 2; i--) This causes a huge slow down! gcc/g++ optimizer problem (I think) */
        for (i=1+k/2;i<=k;i++) t += (dchunk)(a[i] - a[k - i]) * (b[k - i] - b[i]);
        c[k] = (chunk)t & BMASK_B1024_28;
        t = t >> BASEBITS_B1024_28;
    }
    for (k = NLEN_B1024_28; k < 2 * NLEN_B1024_28 - 1; k++)
    {
        s -= d[k - NLEN_B1024_28];
        t += s;
        for (i=1+k/2;i<NLEN_B1024_28;i++) t += (dchunk)(a[i] - a[k - i]) * (b[k - i] - b[i]);
        c[k] = (chunk)t & BMASK_B1024_28;
        t = t >> BASEBITS_B1024_28;
    }
    c[2 * NLEN_B1024_28 - 1] = (chunk)t;
#endif
#endif

#else
    int j;
    chunk carry;
    BIG_dzero(c);
    for (i = 0; i < NLEN_B1024_28; i++)
    {
        carry = 0;
        for (j = 0; j < NLEN_B1024_28; j++)
            carry = muladd(a[i], b[j], carry, &c[i + j]);

        c[NLEN_B1024_28 + i] = carry;
    }

#endif

#ifdef DEBUG_NORM
    c[DMPV_B1024_28] = 1;
    c[DMNV_B1024_28] = 0;
#endif
}

/* Set c=a*a */
/* SU= 80 */
void B1024_28::BIG_sqr(DBIG c, BIG a)
{
    int i, j;
#ifdef dchunk
    dchunk t, co;
#endif
//B1024_28::BIGSQRS++;
#ifdef DEBUG_NORM
    if ((a[MPV_B1024_28] != 1 && a[MPV_B1024_28] != 0) || a[MNV_B1024_28] != 0) printf("Input to sqr not normed\n");
#endif
    /* Note 2*a[i] in loop below and extra addition */

#ifdef COMBA

#ifdef UNWOUND

    
	t=(dchunk)a[0]*a[0]; c[0]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28;
	t= +(dchunk)a[1]*a[0]; t+=t; t+=co; c[1]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[2]*a[0]; t+=t; t+=co; t+=(dchunk)a[1]*a[1]; c[2]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[3]*a[0]+(dchunk)a[2]*a[1]; t+=t; t+=co; c[3]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[4]*a[0]+(dchunk)a[3]*a[1]; t+=t; t+=co; t+=(dchunk)a[2]*a[2]; c[4]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[5]*a[0]+(dchunk)a[4]*a[1]+(dchunk)a[3]*a[2]; t+=t; t+=co; c[5]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[6]*a[0]+(dchunk)a[5]*a[1]+(dchunk)a[4]*a[2]; t+=t; t+=co; t+=(dchunk)a[3]*a[3]; c[6]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[7]*a[0]+(dchunk)a[6]*a[1]+(dchunk)a[5]*a[2]+(dchunk)a[4]*a[3]; t+=t; t+=co; c[7]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[8]*a[0]+(dchunk)a[7]*a[1]+(dchunk)a[6]*a[2]+(dchunk)a[5]*a[3]; t+=t; t+=co; t+=(dchunk)a[4]*a[4]; c[8]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[9]*a[0]+(dchunk)a[8]*a[1]+(dchunk)a[7]*a[2]+(dchunk)a[6]*a[3]+(dchunk)a[5]*a[4]; t+=t; t+=co; c[9]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[10]*a[0]+(dchunk)a[9]*a[1]+(dchunk)a[8]*a[2]+(dchunk)a[7]*a[3]+(dchunk)a[6]*a[4]; t+=t; t+=co; t+=(dchunk)a[5]*a[5]; c[10]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[11]*a[0]+(dchunk)a[10]*a[1]+(dchunk)a[9]*a[2]+(dchunk)a[8]*a[3]+(dchunk)a[7]*a[4]+(dchunk)a[6]*a[5]; t+=t; t+=co; c[11]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[12]*a[0]+(dchunk)a[11]*a[1]+(dchunk)a[10]*a[2]+(dchunk)a[9]*a[3]+(dchunk)a[8]*a[4]+(dchunk)a[7]*a[5]; t+=t; t+=co; t+=(dchunk)a[6]*a[6]; c[12]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[13]*a[0]+(dchunk)a[12]*a[1]+(dchunk)a[11]*a[2]+(dchunk)a[10]*a[3]+(dchunk)a[9]*a[4]+(dchunk)a[8]*a[5]+(dchunk)a[7]*a[6]; t+=t; t+=co; c[13]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[14]*a[0]+(dchunk)a[13]*a[1]+(dchunk)a[12]*a[2]+(dchunk)a[11]*a[3]+(dchunk)a[10]*a[4]+(dchunk)a[9]*a[5]+(dchunk)a[8]*a[6]; t+=t; t+=co; t+=(dchunk)a[7]*a[7]; c[14]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[15]*a[0]+(dchunk)a[14]*a[1]+(dchunk)a[13]*a[2]+(dchunk)a[12]*a[3]+(dchunk)a[11]*a[4]+(dchunk)a[10]*a[5]+(dchunk)a[9]*a[6]+(dchunk)a[8]*a[7]; t+=t; t+=co; c[15]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[16]*a[0]+(dchunk)a[15]*a[1]+(dchunk)a[14]*a[2]+(dchunk)a[13]*a[3]+(dchunk)a[12]*a[4]+(dchunk)a[11]*a[5]+(dchunk)a[10]*a[6]+(dchunk)a[9]*a[7]; t+=t; t+=co; t+=(dchunk)a[8]*a[8]; c[16]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[17]*a[0]+(dchunk)a[16]*a[1]+(dchunk)a[15]*a[2]+(dchunk)a[14]*a[3]+(dchunk)a[13]*a[4]+(dchunk)a[12]*a[5]+(dchunk)a[11]*a[6]+(dchunk)a[10]*a[7]+(dchunk)a[9]*a[8]; t+=t; t+=co; c[17]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[18]*a[0]+(dchunk)a[17]*a[1]+(dchunk)a[16]*a[2]+(dchunk)a[15]*a[3]+(dchunk)a[14]*a[4]+(dchunk)a[13]*a[5]+(dchunk)a[12]*a[6]+(dchunk)a[11]*a[7]+(dchunk)a[10]*a[8]; t+=t; t+=co; t+=(dchunk)a[9]*a[9]; c[18]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[19]*a[0]+(dchunk)a[18]*a[1]+(dchunk)a[17]*a[2]+(dchunk)a[16]*a[3]+(dchunk)a[15]*a[4]+(dchunk)a[14]*a[5]+(dchunk)a[13]*a[6]+(dchunk)a[12]*a[7]+(dchunk)a[11]*a[8]+(dchunk)a[10]*a[9]; t+=t; t+=co; c[19]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[20]*a[0]+(dchunk)a[19]*a[1]+(dchunk)a[18]*a[2]+(dchunk)a[17]*a[3]+(dchunk)a[16]*a[4]+(dchunk)a[15]*a[5]+(dchunk)a[14]*a[6]+(dchunk)a[13]*a[7]+(dchunk)a[12]*a[8]+(dchunk)a[11]*a[9]; t+=t; t+=co; t+=(dchunk)a[10]*a[10]; c[20]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[21]*a[0]+(dchunk)a[20]*a[1]+(dchunk)a[19]*a[2]+(dchunk)a[18]*a[3]+(dchunk)a[17]*a[4]+(dchunk)a[16]*a[5]+(dchunk)a[15]*a[6]+(dchunk)a[14]*a[7]+(dchunk)a[13]*a[8]+(dchunk)a[12]*a[9]+(dchunk)a[11]*a[10]; t+=t; t+=co; c[21]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[22]*a[0]+(dchunk)a[21]*a[1]+(dchunk)a[20]*a[2]+(dchunk)a[19]*a[3]+(dchunk)a[18]*a[4]+(dchunk)a[17]*a[5]+(dchunk)a[16]*a[6]+(dchunk)a[15]*a[7]+(dchunk)a[14]*a[8]+(dchunk)a[13]*a[9]+(dchunk)a[12]*a[10]; t+=t; t+=co; t+=(dchunk)a[11]*a[11]; c[22]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[23]*a[0]+(dchunk)a[22]*a[1]+(dchunk)a[21]*a[2]+(dchunk)a[20]*a[3]+(dchunk)a[19]*a[4]+(dchunk)a[18]*a[5]+(dchunk)a[17]*a[6]+(dchunk)a[16]*a[7]+(dchunk)a[15]*a[8]+(dchunk)a[14]*a[9]+(dchunk)a[13]*a[10]+(dchunk)a[12]*a[11]; t+=t; t+=co; c[23]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[24]*a[0]+(dchunk)a[23]*a[1]+(dchunk)a[22]*a[2]+(dchunk)a[21]*a[3]+(dchunk)a[20]*a[4]+(dchunk)a[19]*a[5]+(dchunk)a[18]*a[6]+(dchunk)a[17]*a[7]+(dchunk)a[16]*a[8]+(dchunk)a[15]*a[9]+(dchunk)a[14]*a[10]+(dchunk)a[13]*a[11]; t+=t; t+=co; t+=(dchunk)a[12]*a[12]; c[24]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[25]*a[0]+(dchunk)a[24]*a[1]+(dchunk)a[23]*a[2]+(dchunk)a[22]*a[3]+(dchunk)a[21]*a[4]+(dchunk)a[20]*a[5]+(dchunk)a[19]*a[6]+(dchunk)a[18]*a[7]+(dchunk)a[17]*a[8]+(dchunk)a[16]*a[9]+(dchunk)a[15]*a[10]+(dchunk)a[14]*a[11]+(dchunk)a[13]*a[12]; t+=t; t+=co; c[25]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[26]*a[0]+(dchunk)a[25]*a[1]+(dchunk)a[24]*a[2]+(dchunk)a[23]*a[3]+(dchunk)a[22]*a[4]+(dchunk)a[21]*a[5]+(dchunk)a[20]*a[6]+(dchunk)a[19]*a[7]+(dchunk)a[18]*a[8]+(dchunk)a[17]*a[9]+(dchunk)a[16]*a[10]+(dchunk)a[15]*a[11]+(dchunk)a[14]*a[12]; t+=t; t+=co; t+=(dchunk)a[13]*a[13]; c[26]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[27]*a[0]+(dchunk)a[26]*a[1]+(dchunk)a[25]*a[2]+(dchunk)a[24]*a[3]+(dchunk)a[23]*a[4]+(dchunk)a[22]*a[5]+(dchunk)a[21]*a[6]+(dchunk)a[20]*a[7]+(dchunk)a[19]*a[8]+(dchunk)a[18]*a[9]+(dchunk)a[17]*a[10]+(dchunk)a[16]*a[11]+(dchunk)a[15]*a[12]+(dchunk)a[14]*a[13]; t+=t; t+=co; c[27]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[28]*a[0]+(dchunk)a[27]*a[1]+(dchunk)a[26]*a[2]+(dchunk)a[25]*a[3]+(dchunk)a[24]*a[4]+(dchunk)a[23]*a[5]+(dchunk)a[22]*a[6]+(dchunk)a[21]*a[7]+(dchunk)a[20]*a[8]+(dchunk)a[19]*a[9]+(dchunk)a[18]*a[10]+(dchunk)a[17]*a[11]+(dchunk)a[16]*a[12]+(dchunk)a[15]*a[13]; t+=t; t+=co; t+=(dchunk)a[14]*a[14]; c[28]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[29]*a[0]+(dchunk)a[28]*a[1]+(dchunk)a[27]*a[2]+(dchunk)a[26]*a[3]+(dchunk)a[25]*a[4]+(dchunk)a[24]*a[5]+(dchunk)a[23]*a[6]+(dchunk)a[22]*a[7]+(dchunk)a[21]*a[8]+(dchunk)a[20]*a[9]+(dchunk)a[19]*a[10]+(dchunk)a[18]*a[11]+(dchunk)a[17]*a[12]+(dchunk)a[16]*a[13]+(dchunk)a[15]*a[14]; t+=t; t+=co; c[29]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[30]*a[0]+(dchunk)a[29]*a[1]+(dchunk)a[28]*a[2]+(dchunk)a[27]*a[3]+(dchunk)a[26]*a[4]+(dchunk)a[25]*a[5]+(dchunk)a[24]*a[6]+(dchunk)a[23]*a[7]+(dchunk)a[22]*a[8]+(dchunk)a[21]*a[9]+(dchunk)a[20]*a[10]+(dchunk)a[19]*a[11]+(dchunk)a[18]*a[12]+(dchunk)a[17]*a[13]+(dchunk)a[16]*a[14]; t+=t; t+=co; t+=(dchunk)a[15]*a[15]; c[30]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[31]*a[0]+(dchunk)a[30]*a[1]+(dchunk)a[29]*a[2]+(dchunk)a[28]*a[3]+(dchunk)a[27]*a[4]+(dchunk)a[26]*a[5]+(dchunk)a[25]*a[6]+(dchunk)a[24]*a[7]+(dchunk)a[23]*a[8]+(dchunk)a[22]*a[9]+(dchunk)a[21]*a[10]+(dchunk)a[20]*a[11]+(dchunk)a[19]*a[12]+(dchunk)a[18]*a[13]+(dchunk)a[17]*a[14]+(dchunk)a[16]*a[15]; t+=t; t+=co; c[31]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[32]*a[0]+(dchunk)a[31]*a[1]+(dchunk)a[30]*a[2]+(dchunk)a[29]*a[3]+(dchunk)a[28]*a[4]+(dchunk)a[27]*a[5]+(dchunk)a[26]*a[6]+(dchunk)a[25]*a[7]+(dchunk)a[24]*a[8]+(dchunk)a[23]*a[9]+(dchunk)a[22]*a[10]+(dchunk)a[21]*a[11]+(dchunk)a[20]*a[12]+(dchunk)a[19]*a[13]+(dchunk)a[18]*a[14]+(dchunk)a[17]*a[15]; t+=t; t+=co; t+=(dchunk)a[16]*a[16]; c[32]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[33]*a[0]+(dchunk)a[32]*a[1]+(dchunk)a[31]*a[2]+(dchunk)a[30]*a[3]+(dchunk)a[29]*a[4]+(dchunk)a[28]*a[5]+(dchunk)a[27]*a[6]+(dchunk)a[26]*a[7]+(dchunk)a[25]*a[8]+(dchunk)a[24]*a[9]+(dchunk)a[23]*a[10]+(dchunk)a[22]*a[11]+(dchunk)a[21]*a[12]+(dchunk)a[20]*a[13]+(dchunk)a[19]*a[14]+(dchunk)a[18]*a[15]+(dchunk)a[17]*a[16]; t+=t; t+=co; c[33]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[34]*a[0]+(dchunk)a[33]*a[1]+(dchunk)a[32]*a[2]+(dchunk)a[31]*a[3]+(dchunk)a[30]*a[4]+(dchunk)a[29]*a[5]+(dchunk)a[28]*a[6]+(dchunk)a[27]*a[7]+(dchunk)a[26]*a[8]+(dchunk)a[25]*a[9]+(dchunk)a[24]*a[10]+(dchunk)a[23]*a[11]+(dchunk)a[22]*a[12]+(dchunk)a[21]*a[13]+(dchunk)a[20]*a[14]+(dchunk)a[19]*a[15]+(dchunk)a[18]*a[16]; t+=t; t+=co; t+=(dchunk)a[17]*a[17]; c[34]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[35]*a[0]+(dchunk)a[34]*a[1]+(dchunk)a[33]*a[2]+(dchunk)a[32]*a[3]+(dchunk)a[31]*a[4]+(dchunk)a[30]*a[5]+(dchunk)a[29]*a[6]+(dchunk)a[28]*a[7]+(dchunk)a[27]*a[8]+(dchunk)a[26]*a[9]+(dchunk)a[25]*a[10]+(dchunk)a[24]*a[11]+(dchunk)a[23]*a[12]+(dchunk)a[22]*a[13]+(dchunk)a[21]*a[14]+(dchunk)a[20]*a[15]+(dchunk)a[19]*a[16]+(dchunk)a[18]*a[17]; t+=t; t+=co; c[35]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[0]+(dchunk)a[35]*a[1]+(dchunk)a[34]*a[2]+(dchunk)a[33]*a[3]+(dchunk)a[32]*a[4]+(dchunk)a[31]*a[5]+(dchunk)a[30]*a[6]+(dchunk)a[29]*a[7]+(dchunk)a[28]*a[8]+(dchunk)a[27]*a[9]+(dchunk)a[26]*a[10]+(dchunk)a[25]*a[11]+(dchunk)a[24]*a[12]+(dchunk)a[23]*a[13]+(dchunk)a[22]*a[14]+(dchunk)a[21]*a[15]+(dchunk)a[20]*a[16]+(dchunk)a[19]*a[17]; t+=t; t+=co; t+=(dchunk)a[18]*a[18]; c[36]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 

	t= +(dchunk)a[36]*a[1]+(dchunk)a[35]*a[2]+(dchunk)a[34]*a[3]+(dchunk)a[33]*a[4]+(dchunk)a[32]*a[5]+(dchunk)a[31]*a[6]+(dchunk)a[30]*a[7]+(dchunk)a[29]*a[8]+(dchunk)a[28]*a[9]+(dchunk)a[27]*a[10]+(dchunk)a[26]*a[11]+(dchunk)a[25]*a[12]+(dchunk)a[24]*a[13]+(dchunk)a[23]*a[14]+(dchunk)a[22]*a[15]+(dchunk)a[21]*a[16]+(dchunk)a[20]*a[17]+(dchunk)a[19]*a[18]; t+=t; t+=co; c[37]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[2]+(dchunk)a[35]*a[3]+(dchunk)a[34]*a[4]+(dchunk)a[33]*a[5]+(dchunk)a[32]*a[6]+(dchunk)a[31]*a[7]+(dchunk)a[30]*a[8]+(dchunk)a[29]*a[9]+(dchunk)a[28]*a[10]+(dchunk)a[27]*a[11]+(dchunk)a[26]*a[12]+(dchunk)a[25]*a[13]+(dchunk)a[24]*a[14]+(dchunk)a[23]*a[15]+(dchunk)a[22]*a[16]+(dchunk)a[21]*a[17]+(dchunk)a[20]*a[18]; t+=t; t+=co; t+=(dchunk)a[19]*a[19]; c[38]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[3]+(dchunk)a[35]*a[4]+(dchunk)a[34]*a[5]+(dchunk)a[33]*a[6]+(dchunk)a[32]*a[7]+(dchunk)a[31]*a[8]+(dchunk)a[30]*a[9]+(dchunk)a[29]*a[10]+(dchunk)a[28]*a[11]+(dchunk)a[27]*a[12]+(dchunk)a[26]*a[13]+(dchunk)a[25]*a[14]+(dchunk)a[24]*a[15]+(dchunk)a[23]*a[16]+(dchunk)a[22]*a[17]+(dchunk)a[21]*a[18]+(dchunk)a[20]*a[19]; t+=t; t+=co; c[39]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[4]+(dchunk)a[35]*a[5]+(dchunk)a[34]*a[6]+(dchunk)a[33]*a[7]+(dchunk)a[32]*a[8]+(dchunk)a[31]*a[9]+(dchunk)a[30]*a[10]+(dchunk)a[29]*a[11]+(dchunk)a[28]*a[12]+(dchunk)a[27]*a[13]+(dchunk)a[26]*a[14]+(dchunk)a[25]*a[15]+(dchunk)a[24]*a[16]+(dchunk)a[23]*a[17]+(dchunk)a[22]*a[18]+(dchunk)a[21]*a[19]; t+=t; t+=co; t+=(dchunk)a[20]*a[20]; c[40]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[5]+(dchunk)a[35]*a[6]+(dchunk)a[34]*a[7]+(dchunk)a[33]*a[8]+(dchunk)a[32]*a[9]+(dchunk)a[31]*a[10]+(dchunk)a[30]*a[11]+(dchunk)a[29]*a[12]+(dchunk)a[28]*a[13]+(dchunk)a[27]*a[14]+(dchunk)a[26]*a[15]+(dchunk)a[25]*a[16]+(dchunk)a[24]*a[17]+(dchunk)a[23]*a[18]+(dchunk)a[22]*a[19]+(dchunk)a[21]*a[20]; t+=t; t+=co; c[41]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[6]+(dchunk)a[35]*a[7]+(dchunk)a[34]*a[8]+(dchunk)a[33]*a[9]+(dchunk)a[32]*a[10]+(dchunk)a[31]*a[11]+(dchunk)a[30]*a[12]+(dchunk)a[29]*a[13]+(dchunk)a[28]*a[14]+(dchunk)a[27]*a[15]+(dchunk)a[26]*a[16]+(dchunk)a[25]*a[17]+(dchunk)a[24]*a[18]+(dchunk)a[23]*a[19]+(dchunk)a[22]*a[20]; t+=t; t+=co; t+=(dchunk)a[21]*a[21]; c[42]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[7]+(dchunk)a[35]*a[8]+(dchunk)a[34]*a[9]+(dchunk)a[33]*a[10]+(dchunk)a[32]*a[11]+(dchunk)a[31]*a[12]+(dchunk)a[30]*a[13]+(dchunk)a[29]*a[14]+(dchunk)a[28]*a[15]+(dchunk)a[27]*a[16]+(dchunk)a[26]*a[17]+(dchunk)a[25]*a[18]+(dchunk)a[24]*a[19]+(dchunk)a[23]*a[20]+(dchunk)a[22]*a[21]; t+=t; t+=co; c[43]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[8]+(dchunk)a[35]*a[9]+(dchunk)a[34]*a[10]+(dchunk)a[33]*a[11]+(dchunk)a[32]*a[12]+(dchunk)a[31]*a[13]+(dchunk)a[30]*a[14]+(dchunk)a[29]*a[15]+(dchunk)a[28]*a[16]+(dchunk)a[27]*a[17]+(dchunk)a[26]*a[18]+(dchunk)a[25]*a[19]+(dchunk)a[24]*a[20]+(dchunk)a[23]*a[21]; t+=t; t+=co; t+=(dchunk)a[22]*a[22]; c[44]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[9]+(dchunk)a[35]*a[10]+(dchunk)a[34]*a[11]+(dchunk)a[33]*a[12]+(dchunk)a[32]*a[13]+(dchunk)a[31]*a[14]+(dchunk)a[30]*a[15]+(dchunk)a[29]*a[16]+(dchunk)a[28]*a[17]+(dchunk)a[27]*a[18]+(dchunk)a[26]*a[19]+(dchunk)a[25]*a[20]+(dchunk)a[24]*a[21]+(dchunk)a[23]*a[22]; t+=t; t+=co; c[45]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[10]+(dchunk)a[35]*a[11]+(dchunk)a[34]*a[12]+(dchunk)a[33]*a[13]+(dchunk)a[32]*a[14]+(dchunk)a[31]*a[15]+(dchunk)a[30]*a[16]+(dchunk)a[29]*a[17]+(dchunk)a[28]*a[18]+(dchunk)a[27]*a[19]+(dchunk)a[26]*a[20]+(dchunk)a[25]*a[21]+(dchunk)a[24]*a[22]; t+=t; t+=co; t+=(dchunk)a[23]*a[23]; c[46]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[11]+(dchunk)a[35]*a[12]+(dchunk)a[34]*a[13]+(dchunk)a[33]*a[14]+(dchunk)a[32]*a[15]+(dchunk)a[31]*a[16]+(dchunk)a[30]*a[17]+(dchunk)a[29]*a[18]+(dchunk)a[28]*a[19]+(dchunk)a[27]*a[20]+(dchunk)a[26]*a[21]+(dchunk)a[25]*a[22]+(dchunk)a[24]*a[23]; t+=t; t+=co; c[47]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[12]+(dchunk)a[35]*a[13]+(dchunk)a[34]*a[14]+(dchunk)a[33]*a[15]+(dchunk)a[32]*a[16]+(dchunk)a[31]*a[17]+(dchunk)a[30]*a[18]+(dchunk)a[29]*a[19]+(dchunk)a[28]*a[20]+(dchunk)a[27]*a[21]+(dchunk)a[26]*a[22]+(dchunk)a[25]*a[23]; t+=t; t+=co; t+=(dchunk)a[24]*a[24]; c[48]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[13]+(dchunk)a[35]*a[14]+(dchunk)a[34]*a[15]+(dchunk)a[33]*a[16]+(dchunk)a[32]*a[17]+(dchunk)a[31]*a[18]+(dchunk)a[30]*a[19]+(dchunk)a[29]*a[20]+(dchunk)a[28]*a[21]+(dchunk)a[27]*a[22]+(dchunk)a[26]*a[23]+(dchunk)a[25]*a[24]; t+=t; t+=co; c[49]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[14]+(dchunk)a[35]*a[15]+(dchunk)a[34]*a[16]+(dchunk)a[33]*a[17]+(dchunk)a[32]*a[18]+(dchunk)a[31]*a[19]+(dchunk)a[30]*a[20]+(dchunk)a[29]*a[21]+(dchunk)a[28]*a[22]+(dchunk)a[27]*a[23]+(dchunk)a[26]*a[24]; t+=t; t+=co; t+=(dchunk)a[25]*a[25]; c[50]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[15]+(dchunk)a[35]*a[16]+(dchunk)a[34]*a[17]+(dchunk)a[33]*a[18]+(dchunk)a[32]*a[19]+(dchunk)a[31]*a[20]+(dchunk)a[30]*a[21]+(dchunk)a[29]*a[22]+(dchunk)a[28]*a[23]+(dchunk)a[27]*a[24]+(dchunk)a[26]*a[25]; t+=t; t+=co; c[51]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[16]+(dchunk)a[35]*a[17]+(dchunk)a[34]*a[18]+(dchunk)a[33]*a[19]+(dchunk)a[32]*a[20]+(dchunk)a[31]*a[21]+(dchunk)a[30]*a[22]+(dchunk)a[29]*a[23]+(dchunk)a[28]*a[24]+(dchunk)a[27]*a[25]; t+=t; t+=co; t+=(dchunk)a[26]*a[26]; c[52]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[17]+(dchunk)a[35]*a[18]+(dchunk)a[34]*a[19]+(dchunk)a[33]*a[20]+(dchunk)a[32]*a[21]+(dchunk)a[31]*a[22]+(dchunk)a[30]*a[23]+(dchunk)a[29]*a[24]+(dchunk)a[28]*a[25]+(dchunk)a[27]*a[26]; t+=t; t+=co; c[53]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[18]+(dchunk)a[35]*a[19]+(dchunk)a[34]*a[20]+(dchunk)a[33]*a[21]+(dchunk)a[32]*a[22]+(dchunk)a[31]*a[23]+(dchunk)a[30]*a[24]+(dchunk)a[29]*a[25]+(dchunk)a[28]*a[26]; t+=t; t+=co; t+=(dchunk)a[27]*a[27]; c[54]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[19]+(dchunk)a[35]*a[20]+(dchunk)a[34]*a[21]+(dchunk)a[33]*a[22]+(dchunk)a[32]*a[23]+(dchunk)a[31]*a[24]+(dchunk)a[30]*a[25]+(dchunk)a[29]*a[26]+(dchunk)a[28]*a[27]; t+=t; t+=co; c[55]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[20]+(dchunk)a[35]*a[21]+(dchunk)a[34]*a[22]+(dchunk)a[33]*a[23]+(dchunk)a[32]*a[24]+(dchunk)a[31]*a[25]+(dchunk)a[30]*a[26]+(dchunk)a[29]*a[27]; t+=t; t+=co; t+=(dchunk)a[28]*a[28]; c[56]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[21]+(dchunk)a[35]*a[22]+(dchunk)a[34]*a[23]+(dchunk)a[33]*a[24]+(dchunk)a[32]*a[25]+(dchunk)a[31]*a[26]+(dchunk)a[30]*a[27]+(dchunk)a[29]*a[28]; t+=t; t+=co; c[57]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[22]+(dchunk)a[35]*a[23]+(dchunk)a[34]*a[24]+(dchunk)a[33]*a[25]+(dchunk)a[32]*a[26]+(dchunk)a[31]*a[27]+(dchunk)a[30]*a[28]; t+=t; t+=co; t+=(dchunk)a[29]*a[29]; c[58]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[23]+(dchunk)a[35]*a[24]+(dchunk)a[34]*a[25]+(dchunk)a[33]*a[26]+(dchunk)a[32]*a[27]+(dchunk)a[31]*a[28]+(dchunk)a[30]*a[29]; t+=t; t+=co; c[59]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[24]+(dchunk)a[35]*a[25]+(dchunk)a[34]*a[26]+(dchunk)a[33]*a[27]+(dchunk)a[32]*a[28]+(dchunk)a[31]*a[29]; t+=t; t+=co; t+=(dchunk)a[30]*a[30]; c[60]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[25]+(dchunk)a[35]*a[26]+(dchunk)a[34]*a[27]+(dchunk)a[33]*a[28]+(dchunk)a[32]*a[29]+(dchunk)a[31]*a[30]; t+=t; t+=co; c[61]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[26]+(dchunk)a[35]*a[27]+(dchunk)a[34]*a[28]+(dchunk)a[33]*a[29]+(dchunk)a[32]*a[30]; t+=t; t+=co; t+=(dchunk)a[31]*a[31]; c[62]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[27]+(dchunk)a[35]*a[28]+(dchunk)a[34]*a[29]+(dchunk)a[33]*a[30]+(dchunk)a[32]*a[31]; t+=t; t+=co; c[63]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[28]+(dchunk)a[35]*a[29]+(dchunk)a[34]*a[30]+(dchunk)a[33]*a[31]; t+=t; t+=co; t+=(dchunk)a[32]*a[32]; c[64]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[29]+(dchunk)a[35]*a[30]+(dchunk)a[34]*a[31]+(dchunk)a[33]*a[32]; t+=t; t+=co; c[65]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[30]+(dchunk)a[35]*a[31]+(dchunk)a[34]*a[32]; t+=t; t+=co; t+=(dchunk)a[33]*a[33]; c[66]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[31]+(dchunk)a[35]*a[32]+(dchunk)a[34]*a[33]; t+=t; t+=co; c[67]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[32]+(dchunk)a[35]*a[33]; t+=t; t+=co; t+=(dchunk)a[34]*a[34]; c[68]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[33]+(dchunk)a[35]*a[34]; t+=t; t+=co; c[69]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[34]; t+=t; t+=co; t+=(dchunk)a[35]*a[35]; c[70]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t= +(dchunk)a[36]*a[35]; t+=t; t+=co; c[71]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
	t=co; t+=(dchunk)a[36]*a[36]; c[72]=(chunk)t&BMASK_B1024_28; co=t>>BASEBITS_B1024_28; 
 	c[73]=(chunk)co;


#else


    t = (dchunk)a[0] * a[0];
    c[0] = (chunk)t & BMASK_B1024_28;
    co = t >> BASEBITS_B1024_28;

    for (j = 1; j < NLEN_B1024_28 - 1; )
    {
        t = (dchunk)a[j] * a[0];
        for (i = 1; i < (j + 1) / 2; i++) {t += (dchunk)a[j - i] * a[i]; }
        t += t; t += co;
        c[j] = (chunk)t & BMASK_B1024_28;
        co = t >> BASEBITS_B1024_28;
        j++;
        t = (dchunk)a[j] * a[0];
        for (i = 1; i < (j + 1) / 2; i++) {t += (dchunk)a[j - i] * a[i]; }
        t += t; t += co;
        t += (dchunk)a[j / 2] * a[j / 2];
        c[j] = (chunk)t & BMASK_B1024_28;
        co = t >> BASEBITS_B1024_28;
        j++;
    }

    for (j = NLEN_B1024_28 - 1 + NLEN_B1024_28 % 2; j < DNLEN_B1024_28 - 3; )
    {
        t = (dchunk)a[NLEN_B1024_28 - 1] * a[j - NLEN_B1024_28 + 1];
        for (i = j - NLEN_B1024_28 + 2; i < (j + 1) / 2; i++) {t += (dchunk)a[j - i] * a[i];  }
        t += t; t += co;
        c[j] = (chunk)t & BMASK_B1024_28;
        co = t >> BASEBITS_B1024_28;
        j++;
        t = (dchunk)a[NLEN_B1024_28 - 1] * a[j - NLEN_B1024_28 + 1];
        for (i = j - NLEN_B1024_28 + 2; i < (j + 1) / 2; i++) {t += (dchunk)a[j - i] * a[i];  }
        t += t; t += co;
        t += (dchunk)a[j / 2] * a[j / 2];
        c[j] = (chunk)t & BMASK_B1024_28;
        co = t >> BASEBITS_B1024_28;
        j++;
    }

    t = (dchunk)a[NLEN_B1024_28 - 2] * a[NLEN_B1024_28 - 1];
    t += t; t += co;
    c[DNLEN_B1024_28 - 3] = (chunk)t & BMASK_B1024_28;
    co = t >> BASEBITS_B1024_28;

    t = (dchunk)a[NLEN_B1024_28 - 1] * a[NLEN_B1024_28 - 1] + co;
    c[DNLEN_B1024_28 - 2] = (chunk)t & BMASK_B1024_28;
    co = t >> BASEBITS_B1024_28;
    c[DNLEN_B1024_28 - 1] = (chunk)co;


#endif

#else
    chunk carry;
    BIG_dzero(c);
    for (i = 0; i < NLEN_B1024_28; i++)
    {
        carry = 0;
        for (j = i + 1; j < NLEN_B1024_28; j++)
            carry = muladd(a[i], a[j], carry, &c[i + j]);
        c[NLEN_B1024_28 + i] = carry;
    }

    for (i = 0; i < DNLEN_B1024_28; i++) c[i] *= 2;

    for (i = 0; i < NLEN_B1024_28; i++)
        c[2 * i + 1] += muladd(a[i], a[i], 0, &c[2 * i]);

    BIG_dnorm(c); 
#endif


#ifdef DEBUG_NORM
    c[DMPV_B1024_28] = 1;
    c[DMNV_B1024_28] = 0;
#endif

}

/* Montgomery reduction */
void B1024_28::BIG_monty(BIG a, BIG md, chunk MC, DBIG d)
{
    int i, k;

#ifdef dchunk
    dchunk c,t,s;
    dchunk dd[NLEN_B1024_28];
    chunk v[NLEN_B1024_28];
#endif


#ifdef DEBUG_NORM
    if ((d[DMPV_B1024_28] != 1 && d[DMPV_B1024_28] != 0) || d[DMNV_B1024_28] != 0) printf("Input to redc not normed\n");
#endif

#ifdef COMBA

#ifdef UNWOUND

#ifdef USE_KARATSUBA

    	t=d[0]; v[0]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[0]*md[0];  s=0; c=(t>>BASEBITS_B1024_28);

	t=d[1]+c+s+(dchunk)v[0]*md[1]; v[1]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[1]*md[0];  dd[1]=(dchunk)v[1]*md[1]; s+=dd[1]; c=(t>>BASEBITS_B1024_28); 
	t=d[2]+c+s+(dchunk)v[0]*md[2]; v[2]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[2]*md[0];  dd[2]=(dchunk)v[2]*md[2]; s+=dd[2]; c=(t>>BASEBITS_B1024_28); 
	t=d[3]+c+s+(dchunk)v[0]*md[3]+(dchunk)(v[1]-v[2])*(md[2]-md[1]); v[3]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[3]*md[0];  dd[3]=(dchunk)v[3]*md[3]; s+=dd[3]; c=(t>>BASEBITS_B1024_28); 
	t=d[4]+c+s+(dchunk)v[0]*md[4]+(dchunk)(v[1]-v[3])*(md[3]-md[1]); v[4]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[4]*md[0];  dd[4]=(dchunk)v[4]*md[4]; s+=dd[4]; c=(t>>BASEBITS_B1024_28); 
	t=d[5]+c+s+(dchunk)v[0]*md[5]+(dchunk)(v[1]-v[4])*(md[4]-md[1])+(dchunk)(v[2]-v[3])*(md[3]-md[2]); v[5]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[5]*md[0];  dd[5]=(dchunk)v[5]*md[5]; s+=dd[5]; c=(t>>BASEBITS_B1024_28); 
	t=d[6]+c+s+(dchunk)v[0]*md[6]+(dchunk)(v[1]-v[5])*(md[5]-md[1])+(dchunk)(v[2]-v[4])*(md[4]-md[2]); v[6]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[6]*md[0];  dd[6]=(dchunk)v[6]*md[6]; s+=dd[6]; c=(t>>BASEBITS_B1024_28); 
	t=d[7]+c+s+(dchunk)v[0]*md[7]+(dchunk)(v[1]-v[6])*(md[6]-md[1])+(dchunk)(v[2]-v[5])*(md[5]-md[2])+(dchunk)(v[3]-v[4])*(md[4]-md[3]); v[7]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[7]*md[0];  dd[7]=(dchunk)v[7]*md[7]; s+=dd[7]; c=(t>>BASEBITS_B1024_28); 
	t=d[8]+c+s+(dchunk)v[0]*md[8]+(dchunk)(v[1]-v[7])*(md[7]-md[1])+(dchunk)(v[2]-v[6])*(md[6]-md[2])+(dchunk)(v[3]-v[5])*(md[5]-md[3]); v[8]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[8]*md[0];  dd[8]=(dchunk)v[8]*md[8]; s+=dd[8]; c=(t>>BASEBITS_B1024_28); 
	t=d[9]+c+s+(dchunk)v[0]*md[9]+(dchunk)(v[1]-v[8])*(md[8]-md[1])+(dchunk)(v[2]-v[7])*(md[7]-md[2])+(dchunk)(v[3]-v[6])*(md[6]-md[3])+(dchunk)(v[4]-v[5])*(md[5]-md[4]); v[9]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[9]*md[0];  dd[9]=(dchunk)v[9]*md[9]; s+=dd[9]; c=(t>>BASEBITS_B1024_28); 
	t=d[10]+c+s+(dchunk)v[0]*md[10]+(dchunk)(v[1]-v[9])*(md[9]-md[1])+(dchunk)(v[2]-v[8])*(md[8]-md[2])+(dchunk)(v[3]-v[7])*(md[7]-md[3])+(dchunk)(v[4]-v[6])*(md[6]-md[4]); v[10]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[10]*md[0];  dd[10]=(dchunk)v[10]*md[10]; s+=dd[10]; c=(t>>BASEBITS_B1024_28); 
	t=d[11]+c+s+(dchunk)v[0]*md[11]+(dchunk)(v[1]-v[10])*(md[10]-md[1])+(dchunk)(v[2]-v[9])*(md[9]-md[2])+(dchunk)(v[3]-v[8])*(md[8]-md[3])+(dchunk)(v[4]-v[7])*(md[7]-md[4])+(dchunk)(v[5]-v[6])*(md[6]-md[5]); v[11]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[11]*md[0];  dd[11]=(dchunk)v[11]*md[11]; s+=dd[11]; c=(t>>BASEBITS_B1024_28); 
	t=d[12]+c+s+(dchunk)v[0]*md[12]+(dchunk)(v[1]-v[11])*(md[11]-md[1])+(dchunk)(v[2]-v[10])*(md[10]-md[2])+(dchunk)(v[3]-v[9])*(md[9]-md[3])+(dchunk)(v[4]-v[8])*(md[8]-md[4])+(dchunk)(v[5]-v[7])*(md[7]-md[5]); v[12]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[12]*md[0];  dd[12]=(dchunk)v[12]*md[12]; s+=dd[12]; c=(t>>BASEBITS_B1024_28); 
	t=d[13]+c+s+(dchunk)v[0]*md[13]+(dchunk)(v[1]-v[12])*(md[12]-md[1])+(dchunk)(v[2]-v[11])*(md[11]-md[2])+(dchunk)(v[3]-v[10])*(md[10]-md[3])+(dchunk)(v[4]-v[9])*(md[9]-md[4])+(dchunk)(v[5]-v[8])*(md[8]-md[5])+(dchunk)(v[6]-v[7])*(md[7]-md[6]); v[13]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[13]*md[0];  dd[13]=(dchunk)v[13]*md[13]; s+=dd[13]; c=(t>>BASEBITS_B1024_28); 
	t=d[14]+c+s+(dchunk)v[0]*md[14]+(dchunk)(v[1]-v[13])*(md[13]-md[1])+(dchunk)(v[2]-v[12])*(md[12]-md[2])+(dchunk)(v[3]-v[11])*(md[11]-md[3])+(dchunk)(v[4]-v[10])*(md[10]-md[4])+(dchunk)(v[5]-v[9])*(md[9]-md[5])+(dchunk)(v[6]-v[8])*(md[8]-md[6]); v[14]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[14]*md[0];  dd[14]=(dchunk)v[14]*md[14]; s+=dd[14]; c=(t>>BASEBITS_B1024_28); 
	t=d[15]+c+s+(dchunk)v[0]*md[15]+(dchunk)(v[1]-v[14])*(md[14]-md[1])+(dchunk)(v[2]-v[13])*(md[13]-md[2])+(dchunk)(v[3]-v[12])*(md[12]-md[3])+(dchunk)(v[4]-v[11])*(md[11]-md[4])+(dchunk)(v[5]-v[10])*(md[10]-md[5])+(dchunk)(v[6]-v[9])*(md[9]-md[6])+(dchunk)(v[7]-v[8])*(md[8]-md[7]); v[15]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[15]*md[0];  dd[15]=(dchunk)v[15]*md[15]; s+=dd[15]; c=(t>>BASEBITS_B1024_28); 
	t=d[16]+c+s+(dchunk)v[0]*md[16]+(dchunk)(v[1]-v[15])*(md[15]-md[1])+(dchunk)(v[2]-v[14])*(md[14]-md[2])+(dchunk)(v[3]-v[13])*(md[13]-md[3])+(dchunk)(v[4]-v[12])*(md[12]-md[4])+(dchunk)(v[5]-v[11])*(md[11]-md[5])+(dchunk)(v[6]-v[10])*(md[10]-md[6])+(dchunk)(v[7]-v[9])*(md[9]-md[7]); v[16]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[16]*md[0];  dd[16]=(dchunk)v[16]*md[16]; s+=dd[16]; c=(t>>BASEBITS_B1024_28); 
	t=d[17]+c+s+(dchunk)v[0]*md[17]+(dchunk)(v[1]-v[16])*(md[16]-md[1])+(dchunk)(v[2]-v[15])*(md[15]-md[2])+(dchunk)(v[3]-v[14])*(md[14]-md[3])+(dchunk)(v[4]-v[13])*(md[13]-md[4])+(dchunk)(v[5]-v[12])*(md[12]-md[5])+(dchunk)(v[6]-v[11])*(md[11]-md[6])+(dchunk)(v[7]-v[10])*(md[10]-md[7])+(dchunk)(v[8]-v[9])*(md[9]-md[8]); v[17]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[17]*md[0];  dd[17]=(dchunk)v[17]*md[17]; s+=dd[17]; c=(t>>BASEBITS_B1024_28); 
	t=d[18]+c+s+(dchunk)v[0]*md[18]+(dchunk)(v[1]-v[17])*(md[17]-md[1])+(dchunk)(v[2]-v[16])*(md[16]-md[2])+(dchunk)(v[3]-v[15])*(md[15]-md[3])+(dchunk)(v[4]-v[14])*(md[14]-md[4])+(dchunk)(v[5]-v[13])*(md[13]-md[5])+(dchunk)(v[6]-v[12])*(md[12]-md[6])+(dchunk)(v[7]-v[11])*(md[11]-md[7])+(dchunk)(v[8]-v[10])*(md[10]-md[8]); v[18]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[18]*md[0];  dd[18]=(dchunk)v[18]*md[18]; s+=dd[18]; c=(t>>BASEBITS_B1024_28); 
	t=d[19]+c+s+(dchunk)v[0]*md[19]+(dchunk)(v[1]-v[18])*(md[18]-md[1])+(dchunk)(v[2]-v[17])*(md[17]-md[2])+(dchunk)(v[3]-v[16])*(md[16]-md[3])+(dchunk)(v[4]-v[15])*(md[15]-md[4])+(dchunk)(v[5]-v[14])*(md[14]-md[5])+(dchunk)(v[6]-v[13])*(md[13]-md[6])+(dchunk)(v[7]-v[12])*(md[12]-md[7])+(dchunk)(v[8]-v[11])*(md[11]-md[8])+(dchunk)(v[9]-v[10])*(md[10]-md[9]); v[19]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[19]*md[0];  dd[19]=(dchunk)v[19]*md[19]; s+=dd[19]; c=(t>>BASEBITS_B1024_28); 
	t=d[20]+c+s+(dchunk)v[0]*md[20]+(dchunk)(v[1]-v[19])*(md[19]-md[1])+(dchunk)(v[2]-v[18])*(md[18]-md[2])+(dchunk)(v[3]-v[17])*(md[17]-md[3])+(dchunk)(v[4]-v[16])*(md[16]-md[4])+(dchunk)(v[5]-v[15])*(md[15]-md[5])+(dchunk)(v[6]-v[14])*(md[14]-md[6])+(dchunk)(v[7]-v[13])*(md[13]-md[7])+(dchunk)(v[8]-v[12])*(md[12]-md[8])+(dchunk)(v[9]-v[11])*(md[11]-md[9]); v[20]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[20]*md[0];  dd[20]=(dchunk)v[20]*md[20]; s+=dd[20]; c=(t>>BASEBITS_B1024_28); 
	t=d[21]+c+s+(dchunk)v[0]*md[21]+(dchunk)(v[1]-v[20])*(md[20]-md[1])+(dchunk)(v[2]-v[19])*(md[19]-md[2])+(dchunk)(v[3]-v[18])*(md[18]-md[3])+(dchunk)(v[4]-v[17])*(md[17]-md[4])+(dchunk)(v[5]-v[16])*(md[16]-md[5])+(dchunk)(v[6]-v[15])*(md[15]-md[6])+(dchunk)(v[7]-v[14])*(md[14]-md[7])+(dchunk)(v[8]-v[13])*(md[13]-md[8])+(dchunk)(v[9]-v[12])*(md[12]-md[9])+(dchunk)(v[10]-v[11])*(md[11]-md[10]); v[21]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[21]*md[0];  dd[21]=(dchunk)v[21]*md[21]; s+=dd[21]; c=(t>>BASEBITS_B1024_28); 
	t=d[22]+c+s+(dchunk)v[0]*md[22]+(dchunk)(v[1]-v[21])*(md[21]-md[1])+(dchunk)(v[2]-v[20])*(md[20]-md[2])+(dchunk)(v[3]-v[19])*(md[19]-md[3])+(dchunk)(v[4]-v[18])*(md[18]-md[4])+(dchunk)(v[5]-v[17])*(md[17]-md[5])+(dchunk)(v[6]-v[16])*(md[16]-md[6])+(dchunk)(v[7]-v[15])*(md[15]-md[7])+(dchunk)(v[8]-v[14])*(md[14]-md[8])+(dchunk)(v[9]-v[13])*(md[13]-md[9])+(dchunk)(v[10]-v[12])*(md[12]-md[10]); v[22]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[22]*md[0];  dd[22]=(dchunk)v[22]*md[22]; s+=dd[22]; c=(t>>BASEBITS_B1024_28); 
	t=d[23]+c+s+(dchunk)v[0]*md[23]+(dchunk)(v[1]-v[22])*(md[22]-md[1])+(dchunk)(v[2]-v[21])*(md[21]-md[2])+(dchunk)(v[3]-v[20])*(md[20]-md[3])+(dchunk)(v[4]-v[19])*(md[19]-md[4])+(dchunk)(v[5]-v[18])*(md[18]-md[5])+(dchunk)(v[6]-v[17])*(md[17]-md[6])+(dchunk)(v[7]-v[16])*(md[16]-md[7])+(dchunk)(v[8]-v[15])*(md[15]-md[8])+(dchunk)(v[9]-v[14])*(md[14]-md[9])+(dchunk)(v[10]-v[13])*(md[13]-md[10])+(dchunk)(v[11]-v[12])*(md[12]-md[11]); v[23]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[23]*md[0];  dd[23]=(dchunk)v[23]*md[23]; s+=dd[23]; c=(t>>BASEBITS_B1024_28); 
	t=d[24]+c+s+(dchunk)v[0]*md[24]+(dchunk)(v[1]-v[23])*(md[23]-md[1])+(dchunk)(v[2]-v[22])*(md[22]-md[2])+(dchunk)(v[3]-v[21])*(md[21]-md[3])+(dchunk)(v[4]-v[20])*(md[20]-md[4])+(dchunk)(v[5]-v[19])*(md[19]-md[5])+(dchunk)(v[6]-v[18])*(md[18]-md[6])+(dchunk)(v[7]-v[17])*(md[17]-md[7])+(dchunk)(v[8]-v[16])*(md[16]-md[8])+(dchunk)(v[9]-v[15])*(md[15]-md[9])+(dchunk)(v[10]-v[14])*(md[14]-md[10])+(dchunk)(v[11]-v[13])*(md[13]-md[11]); v[24]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[24]*md[0];  dd[24]=(dchunk)v[24]*md[24]; s+=dd[24]; c=(t>>BASEBITS_B1024_28); 
	t=d[25]+c+s+(dchunk)v[0]*md[25]+(dchunk)(v[1]-v[24])*(md[24]-md[1])+(dchunk)(v[2]-v[23])*(md[23]-md[2])+(dchunk)(v[3]-v[22])*(md[22]-md[3])+(dchunk)(v[4]-v[21])*(md[21]-md[4])+(dchunk)(v[5]-v[20])*(md[20]-md[5])+(dchunk)(v[6]-v[19])*(md[19]-md[6])+(dchunk)(v[7]-v[18])*(md[18]-md[7])+(dchunk)(v[8]-v[17])*(md[17]-md[8])+(dchunk)(v[9]-v[16])*(md[16]-md[9])+(dchunk)(v[10]-v[15])*(md[15]-md[10])+(dchunk)(v[11]-v[14])*(md[14]-md[11])+(dchunk)(v[12]-v[13])*(md[13]-md[12]); v[25]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[25]*md[0];  dd[25]=(dchunk)v[25]*md[25]; s+=dd[25]; c=(t>>BASEBITS_B1024_28); 
	t=d[26]+c+s+(dchunk)v[0]*md[26]+(dchunk)(v[1]-v[25])*(md[25]-md[1])+(dchunk)(v[2]-v[24])*(md[24]-md[2])+(dchunk)(v[3]-v[23])*(md[23]-md[3])+(dchunk)(v[4]-v[22])*(md[22]-md[4])+(dchunk)(v[5]-v[21])*(md[21]-md[5])+(dchunk)(v[6]-v[20])*(md[20]-md[6])+(dchunk)(v[7]-v[19])*(md[19]-md[7])+(dchunk)(v[8]-v[18])*(md[18]-md[8])+(dchunk)(v[9]-v[17])*(md[17]-md[9])+(dchunk)(v[10]-v[16])*(md[16]-md[10])+(dchunk)(v[11]-v[15])*(md[15]-md[11])+(dchunk)(v[12]-v[14])*(md[14]-md[12]); v[26]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[26]*md[0];  dd[26]=(dchunk)v[26]*md[26]; s+=dd[26]; c=(t>>BASEBITS_B1024_28); 
	t=d[27]+c+s+(dchunk)v[0]*md[27]+(dchunk)(v[1]-v[26])*(md[26]-md[1])+(dchunk)(v[2]-v[25])*(md[25]-md[2])+(dchunk)(v[3]-v[24])*(md[24]-md[3])+(dchunk)(v[4]-v[23])*(md[23]-md[4])+(dchunk)(v[5]-v[22])*(md[22]-md[5])+(dchunk)(v[6]-v[21])*(md[21]-md[6])+(dchunk)(v[7]-v[20])*(md[20]-md[7])+(dchunk)(v[8]-v[19])*(md[19]-md[8])+(dchunk)(v[9]-v[18])*(md[18]-md[9])+(dchunk)(v[10]-v[17])*(md[17]-md[10])+(dchunk)(v[11]-v[16])*(md[16]-md[11])+(dchunk)(v[12]-v[15])*(md[15]-md[12])+(dchunk)(v[13]-v[14])*(md[14]-md[13]); v[27]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[27]*md[0];  dd[27]=(dchunk)v[27]*md[27]; s+=dd[27]; c=(t>>BASEBITS_B1024_28); 
	t=d[28]+c+s+(dchunk)v[0]*md[28]+(dchunk)(v[1]-v[27])*(md[27]-md[1])+(dchunk)(v[2]-v[26])*(md[26]-md[2])+(dchunk)(v[3]-v[25])*(md[25]-md[3])+(dchunk)(v[4]-v[24])*(md[24]-md[4])+(dchunk)(v[5]-v[23])*(md[23]-md[5])+(dchunk)(v[6]-v[22])*(md[22]-md[6])+(dchunk)(v[7]-v[21])*(md[21]-md[7])+(dchunk)(v[8]-v[20])*(md[20]-md[8])+(dchunk)(v[9]-v[19])*(md[19]-md[9])+(dchunk)(v[10]-v[18])*(md[18]-md[10])+(dchunk)(v[11]-v[17])*(md[17]-md[11])+(dchunk)(v[12]-v[16])*(md[16]-md[12])+(dchunk)(v[13]-v[15])*(md[15]-md[13]); v[28]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[28]*md[0];  dd[28]=(dchunk)v[28]*md[28]; s+=dd[28]; c=(t>>BASEBITS_B1024_28); 
	t=d[29]+c+s+(dchunk)v[0]*md[29]+(dchunk)(v[1]-v[28])*(md[28]-md[1])+(dchunk)(v[2]-v[27])*(md[27]-md[2])+(dchunk)(v[3]-v[26])*(md[26]-md[3])+(dchunk)(v[4]-v[25])*(md[25]-md[4])+(dchunk)(v[5]-v[24])*(md[24]-md[5])+(dchunk)(v[6]-v[23])*(md[23]-md[6])+(dchunk)(v[7]-v[22])*(md[22]-md[7])+(dchunk)(v[8]-v[21])*(md[21]-md[8])+(dchunk)(v[9]-v[20])*(md[20]-md[9])+(dchunk)(v[10]-v[19])*(md[19]-md[10])+(dchunk)(v[11]-v[18])*(md[18]-md[11])+(dchunk)(v[12]-v[17])*(md[17]-md[12])+(dchunk)(v[13]-v[16])*(md[16]-md[13])+(dchunk)(v[14]-v[15])*(md[15]-md[14]); v[29]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[29]*md[0];  dd[29]=(dchunk)v[29]*md[29]; s+=dd[29]; c=(t>>BASEBITS_B1024_28); 
	t=d[30]+c+s+(dchunk)v[0]*md[30]+(dchunk)(v[1]-v[29])*(md[29]-md[1])+(dchunk)(v[2]-v[28])*(md[28]-md[2])+(dchunk)(v[3]-v[27])*(md[27]-md[3])+(dchunk)(v[4]-v[26])*(md[26]-md[4])+(dchunk)(v[5]-v[25])*(md[25]-md[5])+(dchunk)(v[6]-v[24])*(md[24]-md[6])+(dchunk)(v[7]-v[23])*(md[23]-md[7])+(dchunk)(v[8]-v[22])*(md[22]-md[8])+(dchunk)(v[9]-v[21])*(md[21]-md[9])+(dchunk)(v[10]-v[20])*(md[20]-md[10])+(dchunk)(v[11]-v[19])*(md[19]-md[11])+(dchunk)(v[12]-v[18])*(md[18]-md[12])+(dchunk)(v[13]-v[17])*(md[17]-md[13])+(dchunk)(v[14]-v[16])*(md[16]-md[14]); v[30]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[30]*md[0];  dd[30]=(dchunk)v[30]*md[30]; s+=dd[30]; c=(t>>BASEBITS_B1024_28); 
	t=d[31]+c+s+(dchunk)v[0]*md[31]+(dchunk)(v[1]-v[30])*(md[30]-md[1])+(dchunk)(v[2]-v[29])*(md[29]-md[2])+(dchunk)(v[3]-v[28])*(md[28]-md[3])+(dchunk)(v[4]-v[27])*(md[27]-md[4])+(dchunk)(v[5]-v[26])*(md[26]-md[5])+(dchunk)(v[6]-v[25])*(md[25]-md[6])+(dchunk)(v[7]-v[24])*(md[24]-md[7])+(dchunk)(v[8]-v[23])*(md[23]-md[8])+(dchunk)(v[9]-v[22])*(md[22]-md[9])+(dchunk)(v[10]-v[21])*(md[21]-md[10])+(dchunk)(v[11]-v[20])*(md[20]-md[11])+(dchunk)(v[12]-v[19])*(md[19]-md[12])+(dchunk)(v[13]-v[18])*(md[18]-md[13])+(dchunk)(v[14]-v[17])*(md[17]-md[14])+(dchunk)(v[15]-v[16])*(md[16]-md[15]); v[31]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[31]*md[0];  dd[31]=(dchunk)v[31]*md[31]; s+=dd[31]; c=(t>>BASEBITS_B1024_28); 
	t=d[32]+c+s+(dchunk)v[0]*md[32]+(dchunk)(v[1]-v[31])*(md[31]-md[1])+(dchunk)(v[2]-v[30])*(md[30]-md[2])+(dchunk)(v[3]-v[29])*(md[29]-md[3])+(dchunk)(v[4]-v[28])*(md[28]-md[4])+(dchunk)(v[5]-v[27])*(md[27]-md[5])+(dchunk)(v[6]-v[26])*(md[26]-md[6])+(dchunk)(v[7]-v[25])*(md[25]-md[7])+(dchunk)(v[8]-v[24])*(md[24]-md[8])+(dchunk)(v[9]-v[23])*(md[23]-md[9])+(dchunk)(v[10]-v[22])*(md[22]-md[10])+(dchunk)(v[11]-v[21])*(md[21]-md[11])+(dchunk)(v[12]-v[20])*(md[20]-md[12])+(dchunk)(v[13]-v[19])*(md[19]-md[13])+(dchunk)(v[14]-v[18])*(md[18]-md[14])+(dchunk)(v[15]-v[17])*(md[17]-md[15]); v[32]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[32]*md[0];  dd[32]=(dchunk)v[32]*md[32]; s+=dd[32]; c=(t>>BASEBITS_B1024_28); 
	t=d[33]+c+s+(dchunk)v[0]*md[33]+(dchunk)(v[1]-v[32])*(md[32]-md[1])+(dchunk)(v[2]-v[31])*(md[31]-md[2])+(dchunk)(v[3]-v[30])*(md[30]-md[3])+(dchunk)(v[4]-v[29])*(md[29]-md[4])+(dchunk)(v[5]-v[28])*(md[28]-md[5])+(dchunk)(v[6]-v[27])*(md[27]-md[6])+(dchunk)(v[7]-v[26])*(md[26]-md[7])+(dchunk)(v[8]-v[25])*(md[25]-md[8])+(dchunk)(v[9]-v[24])*(md[24]-md[9])+(dchunk)(v[10]-v[23])*(md[23]-md[10])+(dchunk)(v[11]-v[22])*(md[22]-md[11])+(dchunk)(v[12]-v[21])*(md[21]-md[12])+(dchunk)(v[13]-v[20])*(md[20]-md[13])+(dchunk)(v[14]-v[19])*(md[19]-md[14])+(dchunk)(v[15]-v[18])*(md[18]-md[15])+(dchunk)(v[16]-v[17])*(md[17]-md[16]); v[33]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[33]*md[0];  dd[33]=(dchunk)v[33]*md[33]; s+=dd[33]; c=(t>>BASEBITS_B1024_28); 
	t=d[34]+c+s+(dchunk)v[0]*md[34]+(dchunk)(v[1]-v[33])*(md[33]-md[1])+(dchunk)(v[2]-v[32])*(md[32]-md[2])+(dchunk)(v[3]-v[31])*(md[31]-md[3])+(dchunk)(v[4]-v[30])*(md[30]-md[4])+(dchunk)(v[5]-v[29])*(md[29]-md[5])+(dchunk)(v[6]-v[28])*(md[28]-md[6])+(dchunk)(v[7]-v[27])*(md[27]-md[7])+(dchunk)(v[8]-v[26])*(md[26]-md[8])+(dchunk)(v[9]-v[25])*(md[25]-md[9])+(dchunk)(v[10]-v[24])*(md[24]-md[10])+(dchunk)(v[11]-v[23])*(md[23]-md[11])+(dchunk)(v[12]-v[22])*(md[22]-md[12])+(dchunk)(v[13]-v[21])*(md[21]-md[13])+(dchunk)(v[14]-v[20])*(md[20]-md[14])+(dchunk)(v[15]-v[19])*(md[19]-md[15])+(dchunk)(v[16]-v[18])*(md[18]-md[16]); v[34]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[34]*md[0];  dd[34]=(dchunk)v[34]*md[34]; s+=dd[34]; c=(t>>BASEBITS_B1024_28); 
	t=d[35]+c+s+(dchunk)v[0]*md[35]+(dchunk)(v[1]-v[34])*(md[34]-md[1])+(dchunk)(v[2]-v[33])*(md[33]-md[2])+(dchunk)(v[3]-v[32])*(md[32]-md[3])+(dchunk)(v[4]-v[31])*(md[31]-md[4])+(dchunk)(v[5]-v[30])*(md[30]-md[5])+(dchunk)(v[6]-v[29])*(md[29]-md[6])+(dchunk)(v[7]-v[28])*(md[28]-md[7])+(dchunk)(v[8]-v[27])*(md[27]-md[8])+(dchunk)(v[9]-v[26])*(md[26]-md[9])+(dchunk)(v[10]-v[25])*(md[25]-md[10])+(dchunk)(v[11]-v[24])*(md[24]-md[11])+(dchunk)(v[12]-v[23])*(md[23]-md[12])+(dchunk)(v[13]-v[22])*(md[22]-md[13])+(dchunk)(v[14]-v[21])*(md[21]-md[14])+(dchunk)(v[15]-v[20])*(md[20]-md[15])+(dchunk)(v[16]-v[19])*(md[19]-md[16])+(dchunk)(v[17]-v[18])*(md[18]-md[17]); v[35]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[35]*md[0];  dd[35]=(dchunk)v[35]*md[35]; s+=dd[35]; c=(t>>BASEBITS_B1024_28); 
	t=d[36]+c+s+(dchunk)v[0]*md[36]+(dchunk)(v[1]-v[35])*(md[35]-md[1])+(dchunk)(v[2]-v[34])*(md[34]-md[2])+(dchunk)(v[3]-v[33])*(md[33]-md[3])+(dchunk)(v[4]-v[32])*(md[32]-md[4])+(dchunk)(v[5]-v[31])*(md[31]-md[5])+(dchunk)(v[6]-v[30])*(md[30]-md[6])+(dchunk)(v[7]-v[29])*(md[29]-md[7])+(dchunk)(v[8]-v[28])*(md[28]-md[8])+(dchunk)(v[9]-v[27])*(md[27]-md[9])+(dchunk)(v[10]-v[26])*(md[26]-md[10])+(dchunk)(v[11]-v[25])*(md[25]-md[11])+(dchunk)(v[12]-v[24])*(md[24]-md[12])+(dchunk)(v[13]-v[23])*(md[23]-md[13])+(dchunk)(v[14]-v[22])*(md[22]-md[14])+(dchunk)(v[15]-v[21])*(md[21]-md[15])+(dchunk)(v[16]-v[20])*(md[20]-md[16])+(dchunk)(v[17]-v[19])*(md[19]-md[17]); v[36]=((chunk)t*MC)&BMASK_B1024_28; t+=(dchunk)v[36]*md[0];  dd[36]=(dchunk)v[36]*md[36]; s+=dd[36]; c=(t>>BASEBITS_B1024_28); 

	t=d[37]+c+s+(dchunk)(v[1]-v[36])*(md[36]-md[1])+(dchunk)(v[2]-v[35])*(md[35]-md[2])+(dchunk)(v[3]-v[34])*(md[34]-md[3])+(dchunk)(v[4]-v[33])*(md[33]-md[4])+(dchunk)(v[5]-v[32])*(md[32]-md[5])+(dchunk)(v[6]-v[31])*(md[31]-md[6])+(dchunk)(v[7]-v[30])*(md[30]-md[7])+(dchunk)(v[8]-v[29])*(md[29]-md[8])+(dchunk)(v[9]-v[28])*(md[28]-md[9])+(dchunk)(v[10]-v[27])*(md[27]-md[10])+(dchunk)(v[11]-v[26])*(md[26]-md[11])+(dchunk)(v[12]-v[25])*(md[25]-md[12])+(dchunk)(v[13]-v[24])*(md[24]-md[13])+(dchunk)(v[14]-v[23])*(md[23]-md[14])+(dchunk)(v[15]-v[22])*(md[22]-md[15])+(dchunk)(v[16]-v[21])*(md[21]-md[16])+(dchunk)(v[17]-v[20])*(md[20]-md[17])+(dchunk)(v[18]-v[19])*(md[19]-md[18]); a[0]=(chunk)t&BMASK_B1024_28;  s-=dd[1]; c=(t>>BASEBITS_B1024_28); 
	t=d[38]+c+s+(dchunk)(v[2]-v[36])*(md[36]-md[2])+(dchunk)(v[3]-v[35])*(md[35]-md[3])+(dchunk)(v[4]-v[34])*(md[34]-md[4])+(dchunk)(v[5]-v[33])*(md[33]-md[5])+(dchunk)(v[6]-v[32])*(md[32]-md[6])+(dchunk)(v[7]-v[31])*(md[31]-md[7])+(dchunk)(v[8]-v[30])*(md[30]-md[8])+(dchunk)(v[9]-v[29])*(md[29]-md[9])+(dchunk)(v[10]-v[28])*(md[28]-md[10])+(dchunk)(v[11]-v[27])*(md[27]-md[11])+(dchunk)(v[12]-v[26])*(md[26]-md[12])+(dchunk)(v[13]-v[25])*(md[25]-md[13])+(dchunk)(v[14]-v[24])*(md[24]-md[14])+(dchunk)(v[15]-v[23])*(md[23]-md[15])+(dchunk)(v[16]-v[22])*(md[22]-md[16])+(dchunk)(v[17]-v[21])*(md[21]-md[17])+(dchunk)(v[18]-v[20])*(md[20]-md[18]); a[1]=(chunk)t&BMASK_B1024_28;  s-=dd[2]; c=(t>>BASEBITS_B1024_28); 
	t=d[39]+c+s+(dchunk)(v[3]-v[36])*(md[36]-md[3])+(dchunk)(v[4]-v[35])*(md[35]-md[4])+(dchunk)(v[5]-v[34])*(md[34]-md[5])+(dchunk)(v[6]-v[33])*(md[33]-md[6])+(dchunk)(v[7]-v[32])*(md[32]-md[7])+(dchunk)(v[8]-v[31])*(md[31]-md[8])+(dchunk)(v[9]-v[30])*(md[30]-md[9])+(dchunk)(v[10]-v[29])*(md[29]-md[10])+(dchunk)(v[11]-v[28])*(md[28]-md[11])+(dchunk)(v[12]-v[27])*(md[27]-md[12])+(dchunk)(v[13]-v[26])*(md[26]-md[13])+(dchunk)(v[14]-v[25])*(md[25]-md[14])+(dchunk)(v[15]-v[24])*(md[24]-md[15])+(dchunk)(v[16]-v[23])*(md[23]-md[16])+(dchunk)(v[17]-v[22])*(md[22]-md[17])+(dchunk)(v[18]-v[21])*(md[21]-md[18])+(dchunk)(v[19]-v[20])*(md[20]-md[19]); a[2]=(chunk)t&BMASK_B1024_28;  s-=dd[3]; c=(t>>BASEBITS_B1024_28); 
	t=d[40]+c+s+(dchunk)(v[4]-v[36])*(md[36]-md[4])+(dchunk)(v[5]-v[35])*(md[35]-md[5])+(dchunk)(v[6]-v[34])*(md[34]-md[6])+(dchunk)(v[7]-v[33])*(md[33]-md[7])+(dchunk)(v[8]-v[32])*(md[32]-md[8])+(dchunk)(v[9]-v[31])*(md[31]-md[9])+(dchunk)(v[10]-v[30])*(md[30]-md[10])+(dchunk)(v[11]-v[29])*(md[29]-md[11])+(dchunk)(v[12]-v[28])*(md[28]-md[12])+(dchunk)(v[13]-v[27])*(md[27]-md[13])+(dchunk)(v[14]-v[26])*(md[26]-md[14])+(dchunk)(v[15]-v[25])*(md[25]-md[15])+(dchunk)(v[16]-v[24])*(md[24]-md[16])+(dchunk)(v[17]-v[23])*(md[23]-md[17])+(dchunk)(v[18]-v[22])*(md[22]-md[18])+(dchunk)(v[19]-v[21])*(md[21]-md[19]); a[3]=(chunk)t&BMASK_B1024_28;  s-=dd[4]; c=(t>>BASEBITS_B1024_28); 
	t=d[41]+c+s+(dchunk)(v[5]-v[36])*(md[36]-md[5])+(dchunk)(v[6]-v[35])*(md[35]-md[6])+(dchunk)(v[7]-v[34])*(md[34]-md[7])+(dchunk)(v[8]-v[33])*(md[33]-md[8])+(dchunk)(v[9]-v[32])*(md[32]-md[9])+(dchunk)(v[10]-v[31])*(md[31]-md[10])+(dchunk)(v[11]-v[30])*(md[30]-md[11])+(dchunk)(v[12]-v[29])*(md[29]-md[12])+(dchunk)(v[13]-v[28])*(md[28]-md[13])+(dchunk)(v[14]-v[27])*(md[27]-md[14])+(dchunk)(v[15]-v[26])*(md[26]-md[15])+(dchunk)(v[16]-v[25])*(md[25]-md[16])+(dchunk)(v[17]-v[24])*(md[24]-md[17])+(dchunk)(v[18]-v[23])*(md[23]-md[18])+(dchunk)(v[19]-v[22])*(md[22]-md[19])+(dchunk)(v[20]-v[21])*(md[21]-md[20]); a[4]=(chunk)t&BMASK_B1024_28;  s-=dd[5]; c=(t>>BASEBITS_B1024_28); 
	t=d[42]+c+s+(dchunk)(v[6]-v[36])*(md[36]-md[6])+(dchunk)(v[7]-v[35])*(md[35]-md[7])+(dchunk)(v[8]-v[34])*(md[34]-md[8])+(dchunk)(v[9]-v[33])*(md[33]-md[9])+(dchunk)(v[10]-v[32])*(md[32]-md[10])+(dchunk)(v[11]-v[31])*(md[31]-md[11])+(dchunk)(v[12]-v[30])*(md[30]-md[12])+(dchunk)(v[13]-v[29])*(md[29]-md[13])+(dchunk)(v[14]-v[28])*(md[28]-md[14])+(dchunk)(v[15]-v[27])*(md[27]-md[15])+(dchunk)(v[16]-v[26])*(md[26]-md[16])+(dchunk)(v[17]-v[25])*(md[25]-md[17])+(dchunk)(v[18]-v[24])*(md[24]-md[18])+(dchunk)(v[19]-v[23])*(md[23]-md[19])+(dchunk)(v[20]-v[22])*(md[22]-md[20]); a[5]=(chunk)t&BMASK_B1024_28;  s-=dd[6]; c=(t>>BASEBITS_B1024_28); 
	t=d[43]+c+s+(dchunk)(v[7]-v[36])*(md[36]-md[7])+(dchunk)(v[8]-v[35])*(md[35]-md[8])+(dchunk)(v[9]-v[34])*(md[34]-md[9])+(dchunk)(v[10]-v[33])*(md[33]-md[10])+(dchunk)(v[11]-v[32])*(md[32]-md[11])+(dchunk)(v[12]-v[31])*(md[31]-md[12])+(dchunk)(v[13]-v[30])*(md[30]-md[13])+(dchunk)(v[14]-v[29])*(md[29]-md[14])+(dchunk)(v[15]-v[28])*(md[28]-md[15])+(dchunk)(v[16]-v[27])*(md[27]-md[16])+(dchunk)(v[17]-v[26])*(md[26]-md[17])+(dchunk)(v[18]-v[25])*(md[25]-md[18])+(dchunk)(v[19]-v[24])*(md[24]-md[19])+(dchunk)(v[20]-v[23])*(md[23]-md[20])+(dchunk)(v[21]-v[22])*(md[22]-md[21]); a[6]=(chunk)t&BMASK_B1024_28;  s-=dd[7]; c=(t>>BASEBITS_B1024_28); 
	t=d[44]+c+s+(dchunk)(v[8]-v[36])*(md[36]-md[8])+(dchunk)(v[9]-v[35])*(md[35]-md[9])+(dchunk)(v[10]-v[34])*(md[34]-md[10])+(dchunk)(v[11]-v[33])*(md[33]-md[11])+(dchunk)(v[12]-v[32])*(md[32]-md[12])+(dchunk)(v[13]-v[31])*(md[31]-md[13])+(dchunk)(v[14]-v[30])*(md[30]-md[14])+(dchunk)(v[15]-v[29])*(md[29]-md[15])+(dchunk)(v[16]-v[28])*(md[28]-md[16])+(dchunk)(v[17]-v[27])*(md[27]-md[17])+(dchunk)(v[18]-v[26])*(md[26]-md[18])+(dchunk)(v[19]-v[25])*(md[25]-md[19])+(dchunk)(v[20]-v[24])*(md[24]-md[20])+(dchunk)(v[21]-v[23])*(md[23]-md[21]); a[7]=(chunk)t&BMASK_B1024_28;  s-=dd[8]; c=(t>>BASEBITS_B1024_28); 
	t=d[45]+c+s+(dchunk)(v[9]-v[36])*(md[36]-md[9])+(dchunk)(v[10]-v[35])*(md[35]-md[10])+(dchunk)(v[11]-v[34])*(md[34]-md[11])+(dchunk)(v[12]-v[33])*(md[33]-md[12])+(dchunk)(v[13]-v[32])*(md[32]-md[13])+(dchunk)(v[14]-v[31])*(md[31]-md[14])+(dchunk)(v[15]-v[30])*(md[30]-md[15])+(dchunk)(v[16]-v[29])*(md[29]-md[16])+(dchunk)(v[17]-v[28])*(md[28]-md[17])+(dchunk)(v[18]-v[27])*(md[27]-md[18])+(dchunk)(v[19]-v[26])*(md[26]-md[19])+(dchunk)(v[20]-v[25])*(md[25]-md[20])+(dchunk)(v[21]-v[24])*(md[24]-md[21])+(dchunk)(v[22]-v[23])*(md[23]-md[22]); a[8]=(chunk)t&BMASK_B1024_28;  s-=dd[9]; c=(t>>BASEBITS_B1024_28); 
	t=d[46]+c+s+(dchunk)(v[10]-v[36])*(md[36]-md[10])+(dchunk)(v[11]-v[35])*(md[35]-md[11])+(dchunk)(v[12]-v[34])*(md[34]-md[12])+(dchunk)(v[13]-v[33])*(md[33]-md[13])+(dchunk)(v[14]-v[32])*(md[32]-md[14])+(dchunk)(v[15]-v[31])*(md[31]-md[15])+(dchunk)(v[16]-v[30])*(md[30]-md[16])+(dchunk)(v[17]-v[29])*(md[29]-md[17])+(dchunk)(v[18]-v[28])*(md[28]-md[18])+(dchunk)(v[19]-v[27])*(md[27]-md[19])+(dchunk)(v[20]-v[26])*(md[26]-md[20])+(dchunk)(v[21]-v[25])*(md[25]-md[21])+(dchunk)(v[22]-v[24])*(md[24]-md[22]); a[9]=(chunk)t&BMASK_B1024_28;  s-=dd[10]; c=(t>>BASEBITS_B1024_28); 
	t=d[47]+c+s+(dchunk)(v[11]-v[36])*(md[36]-md[11])+(dchunk)(v[12]-v[35])*(md[35]-md[12])+(dchunk)(v[13]-v[34])*(md[34]-md[13])+(dchunk)(v[14]-v[33])*(md[33]-md[14])+(dchunk)(v[15]-v[32])*(md[32]-md[15])+(dchunk)(v[16]-v[31])*(md[31]-md[16])+(dchunk)(v[17]-v[30])*(md[30]-md[17])+(dchunk)(v[18]-v[29])*(md[29]-md[18])+(dchunk)(v[19]-v[28])*(md[28]-md[19])+(dchunk)(v[20]-v[27])*(md[27]-md[20])+(dchunk)(v[21]-v[26])*(md[26]-md[21])+(dchunk)(v[22]-v[25])*(md[25]-md[22])+(dchunk)(v[23]-v[24])*(md[24]-md[23]); a[10]=(chunk)t&BMASK_B1024_28;  s-=dd[11]; c=(t>>BASEBITS_B1024_28); 
	t=d[48]+c+s+(dchunk)(v[12]-v[36])*(md[36]-md[12])+(dchunk)(v[13]-v[35])*(md[35]-md[13])+(dchunk)(v[14]-v[34])*(md[34]-md[14])+(dchunk)(v[15]-v[33])*(md[33]-md[15])+(dchunk)(v[16]-v[32])*(md[32]-md[16])+(dchunk)(v[17]-v[31])*(md[31]-md[17])+(dchunk)(v[18]-v[30])*(md[30]-md[18])+(dchunk)(v[19]-v[29])*(md[29]-md[19])+(dchunk)(v[20]-v[28])*(md[28]-md[20])+(dchunk)(v[21]-v[27])*(md[27]-md[21])+(dchunk)(v[22]-v[26])*(md[26]-md[22])+(dchunk)(v[23]-v[25])*(md[25]-md[23]); a[11]=(chunk)t&BMASK_B1024_28;  s-=dd[12]; c=(t>>BASEBITS_B1024_28); 
	t=d[49]+c+s+(dchunk)(v[13]-v[36])*(md[36]-md[13])+(dchunk)(v[14]-v[35])*(md[35]-md[14])+(dchunk)(v[15]-v[34])*(md[34]-md[15])+(dchunk)(v[16]-v[33])*(md[33]-md[16])+(dchunk)(v[17]-v[32])*(md[32]-md[17])+(dchunk)(v[18]-v[31])*(md[31]-md[18])+(dchunk)(v[19]-v[30])*(md[30]-md[19])+(dchunk)(v[20]-v[29])*(md[29]-md[20])+(dchunk)(v[21]-v[28])*(md[28]-md[21])+(dchunk)(v[22]-v[27])*(md[27]-md[22])+(dchunk)(v[23]-v[26])*(md[26]-md[23])+(dchunk)(v[24]-v[25])*(md[25]-md[24]); a[12]=(chunk)t&BMASK_B1024_28;  s-=dd[13]; c=(t>>BASEBITS_B1024_28); 
	t=d[50]+c+s+(dchunk)(v[14]-v[36])*(md[36]-md[14])+(dchunk)(v[15]-v[35])*(md[35]-md[15])+(dchunk)(v[16]-v[34])*(md[34]-md[16])+(dchunk)(v[17]-v[33])*(md[33]-md[17])+(dchunk)(v[18]-v[32])*(md[32]-md[18])+(dchunk)(v[19]-v[31])*(md[31]-md[19])+(dchunk)(v[20]-v[30])*(md[30]-md[20])+(dchunk)(v[21]-v[29])*(md[29]-md[21])+(dchunk)(v[22]-v[28])*(md[28]-md[22])+(dchunk)(v[23]-v[27])*(md[27]-md[23])+(dchunk)(v[24]-v[26])*(md[26]-md[24]); a[13]=(chunk)t&BMASK_B1024_28;  s-=dd[14]; c=(t>>BASEBITS_B1024_28); 
	t=d[51]+c+s+(dchunk)(v[15]-v[36])*(md[36]-md[15])+(dchunk)(v[16]-v[35])*(md[35]-md[16])+(dchunk)(v[17]-v[34])*(md[34]-md[17])+(dchunk)(v[18]-v[33])*(md[33]-md[18])+(dchunk)(v[19]-v[32])*(md[32]-md[19])+(dchunk)(v[20]-v[31])*(md[31]-md[20])+(dchunk)(v[21]-v[30])*(md[30]-md[21])+(dchunk)(v[22]-v[29])*(md[29]-md[22])+(dchunk)(v[23]-v[28])*(md[28]-md[23])+(dchunk)(v[24]-v[27])*(md[27]-md[24])+(dchunk)(v[25]-v[26])*(md[26]-md[25]); a[14]=(chunk)t&BMASK_B1024_28;  s-=dd[15]; c=(t>>BASEBITS_B1024_28); 
	t=d[52]+c+s+(dchunk)(v[16]-v[36])*(md[36]-md[16])+(dchunk)(v[17]-v[35])*(md[35]-md[17])+(dchunk)(v[18]-v[34])*(md[34]-md[18])+(dchunk)(v[19]-v[33])*(md[33]-md[19])+(dchunk)(v[20]-v[32])*(md[32]-md[20])+(dchunk)(v[21]-v[31])*(md[31]-md[21])+(dchunk)(v[22]-v[30])*(md[30]-md[22])+(dchunk)(v[23]-v[29])*(md[29]-md[23])+(dchunk)(v[24]-v[28])*(md[28]-md[24])+(dchunk)(v[25]-v[27])*(md[27]-md[25]); a[15]=(chunk)t&BMASK_B1024_28;  s-=dd[16]; c=(t>>BASEBITS_B1024_28); 
	t=d[53]+c+s+(dchunk)(v[17]-v[36])*(md[36]-md[17])+(dchunk)(v[18]-v[35])*(md[35]-md[18])+(dchunk)(v[19]-v[34])*(md[34]-md[19])+(dchunk)(v[20]-v[33])*(md[33]-md[20])+(dchunk)(v[21]-v[32])*(md[32]-md[21])+(dchunk)(v[22]-v[31])*(md[31]-md[22])+(dchunk)(v[23]-v[30])*(md[30]-md[23])+(dchunk)(v[24]-v[29])*(md[29]-md[24])+(dchunk)(v[25]-v[28])*(md[28]-md[25])+(dchunk)(v[26]-v[27])*(md[27]-md[26]); a[16]=(chunk)t&BMASK_B1024_28;  s-=dd[17]; c=(t>>BASEBITS_B1024_28); 
	t=d[54]+c+s+(dchunk)(v[18]-v[36])*(md[36]-md[18])+(dchunk)(v[19]-v[35])*(md[35]-md[19])+(dchunk)(v[20]-v[34])*(md[34]-md[20])+(dchunk)(v[21]-v[33])*(md[33]-md[21])+(dchunk)(v[22]-v[32])*(md[32]-md[22])+(dchunk)(v[23]-v[31])*(md[31]-md[23])+(dchunk)(v[24]-v[30])*(md[30]-md[24])+(dchunk)(v[25]-v[29])*(md[29]-md[25])+(dchunk)(v[26]-v[28])*(md[28]-md[26]); a[17]=(chunk)t&BMASK_B1024_28;  s-=dd[18]; c=(t>>BASEBITS_B1024_28); 
	t=d[55]+c+s+(dchunk)(v[19]-v[36])*(md[36]-md[19])+(dchunk)(v[20]-v[35])*(md[35]-md[20])+(dchunk)(v[21]-v[34])*(md[34]-md[21])+(dchunk)(v[22]-v[33])*(md[33]-md[22])+(dchunk)(v[23]-v[32])*(md[32]-md[23])+(dchunk)(v[24]-v[31])*(md[31]-md[24])+(dchunk)(v[25]-v[30])*(md[30]-md[25])+(dchunk)(v[26]-v[29])*(md[29]-md[26])+(dchunk)(v[27]-v[28])*(md[28]-md[27]); a[18]=(chunk)t&BMASK_B1024_28;  s-=dd[19]; c=(t>>BASEBITS_B1024_28); 
	t=d[56]+c+s+(dchunk)(v[20]-v[36])*(md[36]-md[20])+(dchunk)(v[21]-v[35])*(md[35]-md[21])+(dchunk)(v[22]-v[34])*(md[34]-md[22])+(dchunk)(v[23]-v[33])*(md[33]-md[23])+(dchunk)(v[24]-v[32])*(md[32]-md[24])+(dchunk)(v[25]-v[31])*(md[31]-md[25])+(dchunk)(v[26]-v[30])*(md[30]-md[26])+(dchunk)(v[27]-v[29])*(md[29]-md[27]); a[19]=(chunk)t&BMASK_B1024_28;  s-=dd[20]; c=(t>>BASEBITS_B1024_28); 
	t=d[57]+c+s+(dchunk)(v[21]-v[36])*(md[36]-md[21])+(dchunk)(v[22]-v[35])*(md[35]-md[22])+(dchunk)(v[23]-v[34])*(md[34]-md[23])+(dchunk)(v[24]-v[33])*(md[33]-md[24])+(dchunk)(v[25]-v[32])*(md[32]-md[25])+(dchunk)(v[26]-v[31])*(md[31]-md[26])+(dchunk)(v[27]-v[30])*(md[30]-md[27])+(dchunk)(v[28]-v[29])*(md[29]-md[28]); a[20]=(chunk)t&BMASK_B1024_28;  s-=dd[21]; c=(t>>BASEBITS_B1024_28); 
	t=d[58]+c+s+(dchunk)(v[22]-v[36])*(md[36]-md[22])+(dchunk)(v[23]-v[35])*(md[35]-md[23])+(dchunk)(v[24]-v[34])*(md[34]-md[24])+(dchunk)(v[25]-v[33])*(md[33]-md[25])+(dchunk)(v[26]-v[32])*(md[32]-md[26])+(dchunk)(v[27]-v[31])*(md[31]-md[27])+(dchunk)(v[28]-v[30])*(md[30]-md[28]); a[21]=(chunk)t&BMASK_B1024_28;  s-=dd[22]; c=(t>>BASEBITS_B1024_28); 
	t=d[59]+c+s+(dchunk)(v[23]-v[36])*(md[36]-md[23])+(dchunk)(v[24]-v[35])*(md[35]-md[24])+(dchunk)(v[25]-v[34])*(md[34]-md[25])+(dchunk)(v[26]-v[33])*(md[33]-md[26])+(dchunk)(v[27]-v[32])*(md[32]-md[27])+(dchunk)(v[28]-v[31])*(md[31]-md[28])+(dchunk)(v[29]-v[30])*(md[30]-md[29]); a[22]=(chunk)t&BMASK_B1024_28;  s-=dd[23]; c=(t>>BASEBITS_B1024_28); 
	t=d[60]+c+s+(dchunk)(v[24]-v[36])*(md[36]-md[24])+(dchunk)(v[25]-v[35])*(md[35]-md[25])+(dchunk)(v[26]-v[34])*(md[34]-md[26])+(dchunk)(v[27]-v[33])*(md[33]-md[27])+(dchunk)(v[28]-v[32])*(md[32]-md[28])+(dchunk)(v[29]-v[31])*(md[31]-md[29]); a[23]=(chunk)t&BMASK_B1024_28;  s-=dd[24]; c=(t>>BASEBITS_B1024_28); 
	t=d[61]+c+s+(dchunk)(v[25]-v[36])*(md[36]-md[25])+(dchunk)(v[26]-v[35])*(md[35]-md[26])+(dchunk)(v[27]-v[34])*(md[34]-md[27])+(dchunk)(v[28]-v[33])*(md[33]-md[28])+(dchunk)(v[29]-v[32])*(md[32]-md[29])+(dchunk)(v[30]-v[31])*(md[31]-md[30]); a[24]=(chunk)t&BMASK_B1024_28;  s-=dd[25]; c=(t>>BASEBITS_B1024_28); 
	t=d[62]+c+s+(dchunk)(v[26]-v[36])*(md[36]-md[26])+(dchunk)(v[27]-v[35])*(md[35]-md[27])+(dchunk)(v[28]-v[34])*(md[34]-md[28])+(dchunk)(v[29]-v[33])*(md[33]-md[29])+(dchunk)(v[30]-v[32])*(md[32]-md[30]); a[25]=(chunk)t&BMASK_B1024_28;  s-=dd[26]; c=(t>>BASEBITS_B1024_28); 
	t=d[63]+c+s+(dchunk)(v[27]-v[36])*(md[36]-md[27])+(dchunk)(v[28]-v[35])*(md[35]-md[28])+(dchunk)(v[29]-v[34])*(md[34]-md[29])+(dchunk)(v[30]-v[33])*(md[33]-md[30])+(dchunk)(v[31]-v[32])*(md[32]-md[31]); a[26]=(chunk)t&BMASK_B1024_28;  s-=dd[27]; c=(t>>BASEBITS_B1024_28); 
	t=d[64]+c+s+(dchunk)(v[28]-v[36])*(md[36]-md[28])+(dchunk)(v[29]-v[35])*(md[35]-md[29])+(dchunk)(v[30]-v[34])*(md[34]-md[30])+(dchunk)(v[31]-v[33])*(md[33]-md[31]); a[27]=(chunk)t&BMASK_B1024_28;  s-=dd[28]; c=(t>>BASEBITS_B1024_28); 
	t=d[65]+c+s+(dchunk)(v[29]-v[36])*(md[36]-md[29])+(dchunk)(v[30]-v[35])*(md[35]-md[30])+(dchunk)(v[31]-v[34])*(md[34]-md[31])+(dchunk)(v[32]-v[33])*(md[33]-md[32]); a[28]=(chunk)t&BMASK_B1024_28;  s-=dd[29]; c=(t>>BASEBITS_B1024_28); 
	t=d[66]+c+s+(dchunk)(v[30]-v[36])*(md[36]-md[30])+(dchunk)(v[31]-v[35])*(md[35]-md[31])+(dchunk)(v[32]-v[34])*(md[34]-md[32]); a[29]=(chunk)t&BMASK_B1024_28;  s-=dd[30]; c=(t>>BASEBITS_B1024_28); 
	t=d[67]+c+s+(dchunk)(v[31]-v[36])*(md[36]-md[31])+(dchunk)(v[32]-v[35])*(md[35]-md[32])+(dchunk)(v[33]-v[34])*(md[34]-md[33]); a[30]=(chunk)t&BMASK_B1024_28;  s-=dd[31]; c=(t>>BASEBITS_B1024_28); 
	t=d[68]+c+s+(dchunk)(v[32]-v[36])*(md[36]-md[32])+(dchunk)(v[33]-v[35])*(md[35]-md[33]); a[31]=(chunk)t&BMASK_B1024_28;  s-=dd[32]; c=(t>>BASEBITS_B1024_28); 
	t=d[69]+c+s+(dchunk)(v[33]-v[36])*(md[36]-md[33])+(dchunk)(v[34]-v[35])*(md[35]-md[34]); a[32]=(chunk)t&BMASK_B1024_28;  s-=dd[33]; c=(t>>BASEBITS_B1024_28); 
	t=d[70]+c+s+(dchunk)(v[34]-v[36])*(md[36]-md[34]); a[33]=(chunk)t&BMASK_B1024_28;  s-=dd[34]; c=(t>>BASEBITS_B1024_28); 
	t=d[71]+c+s+(dchunk)(v[35]-v[36])*(md[36]-md[35]); a[34]=(chunk)t&BMASK_B1024_28;  s-=dd[35]; c=(t>>BASEBITS_B1024_28); 
	t=d[72]+c+s; a[35]=(chunk)t&BMASK_B1024_28;  s-=dd[36]; c=(t>>BASEBITS_B1024_28); 
	a[36]=d[73]+((chunk)c&BMASK_B1024_28);


#else

    	t = d[0];
	v[0] = ((chunk)t * MC)&BMASK_B1024_28;
	t += (dchunk)v[0] * md[0];
	t = (t >> BASEBITS_B1024_28) + d[1];
	t += (dchunk)v[0] * md[1] ; v[1] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[1] * md[0]; t = (t >> BASEBITS_B1024_28) + d[2];
	t += (dchunk)v[0] * md[2] + (dchunk)v[1]*md[1]; v[2] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[2] * md[0]; t = (t >> BASEBITS_B1024_28) + d[3];
	t += (dchunk)v[0] * md[3] + (dchunk)v[1]*md[2]+ (dchunk)v[2]*md[1]; v[3] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[3] * md[0]; t = (t >> BASEBITS_B1024_28) + d[4];
	t += (dchunk)v[0] * md[4] + (dchunk)v[1]*md[3]+ (dchunk)v[2]*md[2]+ (dchunk)v[3]*md[1]; v[4] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[4] * md[0]; t = (t >> BASEBITS_B1024_28) + d[5];
	t += (dchunk)v[0] * md[5] + (dchunk)v[1]*md[4]+ (dchunk)v[2]*md[3]+ (dchunk)v[3]*md[2]+ (dchunk)v[4]*md[1]; v[5] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[5] * md[0]; t = (t >> BASEBITS_B1024_28) + d[6];
	t += (dchunk)v[0] * md[6] + (dchunk)v[1]*md[5]+ (dchunk)v[2]*md[4]+ (dchunk)v[3]*md[3]+ (dchunk)v[4]*md[2]+ (dchunk)v[5]*md[1]; v[6] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[6] * md[0]; t = (t >> BASEBITS_B1024_28) + d[7];
	t += (dchunk)v[0] * md[7] + (dchunk)v[1]*md[6]+ (dchunk)v[2]*md[5]+ (dchunk)v[3]*md[4]+ (dchunk)v[4]*md[3]+ (dchunk)v[5]*md[2]+ (dchunk)v[6]*md[1]; v[7] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[7] * md[0]; t = (t >> BASEBITS_B1024_28) + d[8];
	t += (dchunk)v[0] * md[8] + (dchunk)v[1]*md[7]+ (dchunk)v[2]*md[6]+ (dchunk)v[3]*md[5]+ (dchunk)v[4]*md[4]+ (dchunk)v[5]*md[3]+ (dchunk)v[6]*md[2]+ (dchunk)v[7]*md[1]; v[8] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[8] * md[0]; t = (t >> BASEBITS_B1024_28) + d[9];
	t += (dchunk)v[0] * md[9] + (dchunk)v[1]*md[8]+ (dchunk)v[2]*md[7]+ (dchunk)v[3]*md[6]+ (dchunk)v[4]*md[5]+ (dchunk)v[5]*md[4]+ (dchunk)v[6]*md[3]+ (dchunk)v[7]*md[2]+ (dchunk)v[8]*md[1]; v[9] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[9] * md[0]; t = (t >> BASEBITS_B1024_28) + d[10];
	t += (dchunk)v[0] * md[10] + (dchunk)v[1]*md[9]+ (dchunk)v[2]*md[8]+ (dchunk)v[3]*md[7]+ (dchunk)v[4]*md[6]+ (dchunk)v[5]*md[5]+ (dchunk)v[6]*md[4]+ (dchunk)v[7]*md[3]+ (dchunk)v[8]*md[2]+ (dchunk)v[9]*md[1]; v[10] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[10] * md[0]; t = (t >> BASEBITS_B1024_28) + d[11];
	t += (dchunk)v[0] * md[11] + (dchunk)v[1]*md[10]+ (dchunk)v[2]*md[9]+ (dchunk)v[3]*md[8]+ (dchunk)v[4]*md[7]+ (dchunk)v[5]*md[6]+ (dchunk)v[6]*md[5]+ (dchunk)v[7]*md[4]+ (dchunk)v[8]*md[3]+ (dchunk)v[9]*md[2]+ (dchunk)v[10]*md[1]; v[11] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[11] * md[0]; t = (t >> BASEBITS_B1024_28) + d[12];
	t += (dchunk)v[0] * md[12] + (dchunk)v[1]*md[11]+ (dchunk)v[2]*md[10]+ (dchunk)v[3]*md[9]+ (dchunk)v[4]*md[8]+ (dchunk)v[5]*md[7]+ (dchunk)v[6]*md[6]+ (dchunk)v[7]*md[5]+ (dchunk)v[8]*md[4]+ (dchunk)v[9]*md[3]+ (dchunk)v[10]*md[2]+ (dchunk)v[11]*md[1]; v[12] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[12] * md[0]; t = (t >> BASEBITS_B1024_28) + d[13];
	t += (dchunk)v[0] * md[13] + (dchunk)v[1]*md[12]+ (dchunk)v[2]*md[11]+ (dchunk)v[3]*md[10]+ (dchunk)v[4]*md[9]+ (dchunk)v[5]*md[8]+ (dchunk)v[6]*md[7]+ (dchunk)v[7]*md[6]+ (dchunk)v[8]*md[5]+ (dchunk)v[9]*md[4]+ (dchunk)v[10]*md[3]+ (dchunk)v[11]*md[2]+ (dchunk)v[12]*md[1]; v[13] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[13] * md[0]; t = (t >> BASEBITS_B1024_28) + d[14];
	t += (dchunk)v[0] * md[14] + (dchunk)v[1]*md[13]+ (dchunk)v[2]*md[12]+ (dchunk)v[3]*md[11]+ (dchunk)v[4]*md[10]+ (dchunk)v[5]*md[9]+ (dchunk)v[6]*md[8]+ (dchunk)v[7]*md[7]+ (dchunk)v[8]*md[6]+ (dchunk)v[9]*md[5]+ (dchunk)v[10]*md[4]+ (dchunk)v[11]*md[3]+ (dchunk)v[12]*md[2]+ (dchunk)v[13]*md[1]; v[14] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[14] * md[0]; t = (t >> BASEBITS_B1024_28) + d[15];
	t += (dchunk)v[0] * md[15] + (dchunk)v[1]*md[14]+ (dchunk)v[2]*md[13]+ (dchunk)v[3]*md[12]+ (dchunk)v[4]*md[11]+ (dchunk)v[5]*md[10]+ (dchunk)v[6]*md[9]+ (dchunk)v[7]*md[8]+ (dchunk)v[8]*md[7]+ (dchunk)v[9]*md[6]+ (dchunk)v[10]*md[5]+ (dchunk)v[11]*md[4]+ (dchunk)v[12]*md[3]+ (dchunk)v[13]*md[2]+ (dchunk)v[14]*md[1]; v[15] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[15] * md[0]; t = (t >> BASEBITS_B1024_28) + d[16];
	t += (dchunk)v[0] * md[16] + (dchunk)v[1]*md[15]+ (dchunk)v[2]*md[14]+ (dchunk)v[3]*md[13]+ (dchunk)v[4]*md[12]+ (dchunk)v[5]*md[11]+ (dchunk)v[6]*md[10]+ (dchunk)v[7]*md[9]+ (dchunk)v[8]*md[8]+ (dchunk)v[9]*md[7]+ (dchunk)v[10]*md[6]+ (dchunk)v[11]*md[5]+ (dchunk)v[12]*md[4]+ (dchunk)v[13]*md[3]+ (dchunk)v[14]*md[2]+ (dchunk)v[15]*md[1]; v[16] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[16] * md[0]; t = (t >> BASEBITS_B1024_28) + d[17];
	t += (dchunk)v[0] * md[17] + (dchunk)v[1]*md[16]+ (dchunk)v[2]*md[15]+ (dchunk)v[3]*md[14]+ (dchunk)v[4]*md[13]+ (dchunk)v[5]*md[12]+ (dchunk)v[6]*md[11]+ (dchunk)v[7]*md[10]+ (dchunk)v[8]*md[9]+ (dchunk)v[9]*md[8]+ (dchunk)v[10]*md[7]+ (dchunk)v[11]*md[6]+ (dchunk)v[12]*md[5]+ (dchunk)v[13]*md[4]+ (dchunk)v[14]*md[3]+ (dchunk)v[15]*md[2]+ (dchunk)v[16]*md[1]; v[17] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[17] * md[0]; t = (t >> BASEBITS_B1024_28) + d[18];
	t += (dchunk)v[0] * md[18] + (dchunk)v[1]*md[17]+ (dchunk)v[2]*md[16]+ (dchunk)v[3]*md[15]+ (dchunk)v[4]*md[14]+ (dchunk)v[5]*md[13]+ (dchunk)v[6]*md[12]+ (dchunk)v[7]*md[11]+ (dchunk)v[8]*md[10]+ (dchunk)v[9]*md[9]+ (dchunk)v[10]*md[8]+ (dchunk)v[11]*md[7]+ (dchunk)v[12]*md[6]+ (dchunk)v[13]*md[5]+ (dchunk)v[14]*md[4]+ (dchunk)v[15]*md[3]+ (dchunk)v[16]*md[2]+ (dchunk)v[17]*md[1]; v[18] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[18] * md[0]; t = (t >> BASEBITS_B1024_28) + d[19];
	t += (dchunk)v[0] * md[19] + (dchunk)v[1]*md[18]+ (dchunk)v[2]*md[17]+ (dchunk)v[3]*md[16]+ (dchunk)v[4]*md[15]+ (dchunk)v[5]*md[14]+ (dchunk)v[6]*md[13]+ (dchunk)v[7]*md[12]+ (dchunk)v[8]*md[11]+ (dchunk)v[9]*md[10]+ (dchunk)v[10]*md[9]+ (dchunk)v[11]*md[8]+ (dchunk)v[12]*md[7]+ (dchunk)v[13]*md[6]+ (dchunk)v[14]*md[5]+ (dchunk)v[15]*md[4]+ (dchunk)v[16]*md[3]+ (dchunk)v[17]*md[2]+ (dchunk)v[18]*md[1]; v[19] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[19] * md[0]; t = (t >> BASEBITS_B1024_28) + d[20];
	t += (dchunk)v[0] * md[20] + (dchunk)v[1]*md[19]+ (dchunk)v[2]*md[18]+ (dchunk)v[3]*md[17]+ (dchunk)v[4]*md[16]+ (dchunk)v[5]*md[15]+ (dchunk)v[6]*md[14]+ (dchunk)v[7]*md[13]+ (dchunk)v[8]*md[12]+ (dchunk)v[9]*md[11]+ (dchunk)v[10]*md[10]+ (dchunk)v[11]*md[9]+ (dchunk)v[12]*md[8]+ (dchunk)v[13]*md[7]+ (dchunk)v[14]*md[6]+ (dchunk)v[15]*md[5]+ (dchunk)v[16]*md[4]+ (dchunk)v[17]*md[3]+ (dchunk)v[18]*md[2]+ (dchunk)v[19]*md[1]; v[20] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[20] * md[0]; t = (t >> BASEBITS_B1024_28) + d[21];
	t += (dchunk)v[0] * md[21] + (dchunk)v[1]*md[20]+ (dchunk)v[2]*md[19]+ (dchunk)v[3]*md[18]+ (dchunk)v[4]*md[17]+ (dchunk)v[5]*md[16]+ (dchunk)v[6]*md[15]+ (dchunk)v[7]*md[14]+ (dchunk)v[8]*md[13]+ (dchunk)v[9]*md[12]+ (dchunk)v[10]*md[11]+ (dchunk)v[11]*md[10]+ (dchunk)v[12]*md[9]+ (dchunk)v[13]*md[8]+ (dchunk)v[14]*md[7]+ (dchunk)v[15]*md[6]+ (dchunk)v[16]*md[5]+ (dchunk)v[17]*md[4]+ (dchunk)v[18]*md[3]+ (dchunk)v[19]*md[2]+ (dchunk)v[20]*md[1]; v[21] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[21] * md[0]; t = (t >> BASEBITS_B1024_28) + d[22];
	t += (dchunk)v[0] * md[22] + (dchunk)v[1]*md[21]+ (dchunk)v[2]*md[20]+ (dchunk)v[3]*md[19]+ (dchunk)v[4]*md[18]+ (dchunk)v[5]*md[17]+ (dchunk)v[6]*md[16]+ (dchunk)v[7]*md[15]+ (dchunk)v[8]*md[14]+ (dchunk)v[9]*md[13]+ (dchunk)v[10]*md[12]+ (dchunk)v[11]*md[11]+ (dchunk)v[12]*md[10]+ (dchunk)v[13]*md[9]+ (dchunk)v[14]*md[8]+ (dchunk)v[15]*md[7]+ (dchunk)v[16]*md[6]+ (dchunk)v[17]*md[5]+ (dchunk)v[18]*md[4]+ (dchunk)v[19]*md[3]+ (dchunk)v[20]*md[2]+ (dchunk)v[21]*md[1]; v[22] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[22] * md[0]; t = (t >> BASEBITS_B1024_28) + d[23];
	t += (dchunk)v[0] * md[23] + (dchunk)v[1]*md[22]+ (dchunk)v[2]*md[21]+ (dchunk)v[3]*md[20]+ (dchunk)v[4]*md[19]+ (dchunk)v[5]*md[18]+ (dchunk)v[6]*md[17]+ (dchunk)v[7]*md[16]+ (dchunk)v[8]*md[15]+ (dchunk)v[9]*md[14]+ (dchunk)v[10]*md[13]+ (dchunk)v[11]*md[12]+ (dchunk)v[12]*md[11]+ (dchunk)v[13]*md[10]+ (dchunk)v[14]*md[9]+ (dchunk)v[15]*md[8]+ (dchunk)v[16]*md[7]+ (dchunk)v[17]*md[6]+ (dchunk)v[18]*md[5]+ (dchunk)v[19]*md[4]+ (dchunk)v[20]*md[3]+ (dchunk)v[21]*md[2]+ (dchunk)v[22]*md[1]; v[23] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[23] * md[0]; t = (t >> BASEBITS_B1024_28) + d[24];
	t += (dchunk)v[0] * md[24] + (dchunk)v[1]*md[23]+ (dchunk)v[2]*md[22]+ (dchunk)v[3]*md[21]+ (dchunk)v[4]*md[20]+ (dchunk)v[5]*md[19]+ (dchunk)v[6]*md[18]+ (dchunk)v[7]*md[17]+ (dchunk)v[8]*md[16]+ (dchunk)v[9]*md[15]+ (dchunk)v[10]*md[14]+ (dchunk)v[11]*md[13]+ (dchunk)v[12]*md[12]+ (dchunk)v[13]*md[11]+ (dchunk)v[14]*md[10]+ (dchunk)v[15]*md[9]+ (dchunk)v[16]*md[8]+ (dchunk)v[17]*md[7]+ (dchunk)v[18]*md[6]+ (dchunk)v[19]*md[5]+ (dchunk)v[20]*md[4]+ (dchunk)v[21]*md[3]+ (dchunk)v[22]*md[2]+ (dchunk)v[23]*md[1]; v[24] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[24] * md[0]; t = (t >> BASEBITS_B1024_28) + d[25];
	t += (dchunk)v[0] * md[25] + (dchunk)v[1]*md[24]+ (dchunk)v[2]*md[23]+ (dchunk)v[3]*md[22]+ (dchunk)v[4]*md[21]+ (dchunk)v[5]*md[20]+ (dchunk)v[6]*md[19]+ (dchunk)v[7]*md[18]+ (dchunk)v[8]*md[17]+ (dchunk)v[9]*md[16]+ (dchunk)v[10]*md[15]+ (dchunk)v[11]*md[14]+ (dchunk)v[12]*md[13]+ (dchunk)v[13]*md[12]+ (dchunk)v[14]*md[11]+ (dchunk)v[15]*md[10]+ (dchunk)v[16]*md[9]+ (dchunk)v[17]*md[8]+ (dchunk)v[18]*md[7]+ (dchunk)v[19]*md[6]+ (dchunk)v[20]*md[5]+ (dchunk)v[21]*md[4]+ (dchunk)v[22]*md[3]+ (dchunk)v[23]*md[2]+ (dchunk)v[24]*md[1]; v[25] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[25] * md[0]; t = (t >> BASEBITS_B1024_28) + d[26];
	t += (dchunk)v[0] * md[26] + (dchunk)v[1]*md[25]+ (dchunk)v[2]*md[24]+ (dchunk)v[3]*md[23]+ (dchunk)v[4]*md[22]+ (dchunk)v[5]*md[21]+ (dchunk)v[6]*md[20]+ (dchunk)v[7]*md[19]+ (dchunk)v[8]*md[18]+ (dchunk)v[9]*md[17]+ (dchunk)v[10]*md[16]+ (dchunk)v[11]*md[15]+ (dchunk)v[12]*md[14]+ (dchunk)v[13]*md[13]+ (dchunk)v[14]*md[12]+ (dchunk)v[15]*md[11]+ (dchunk)v[16]*md[10]+ (dchunk)v[17]*md[9]+ (dchunk)v[18]*md[8]+ (dchunk)v[19]*md[7]+ (dchunk)v[20]*md[6]+ (dchunk)v[21]*md[5]+ (dchunk)v[22]*md[4]+ (dchunk)v[23]*md[3]+ (dchunk)v[24]*md[2]+ (dchunk)v[25]*md[1]; v[26] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[26] * md[0]; t = (t >> BASEBITS_B1024_28) + d[27];
	t += (dchunk)v[0] * md[27] + (dchunk)v[1]*md[26]+ (dchunk)v[2]*md[25]+ (dchunk)v[3]*md[24]+ (dchunk)v[4]*md[23]+ (dchunk)v[5]*md[22]+ (dchunk)v[6]*md[21]+ (dchunk)v[7]*md[20]+ (dchunk)v[8]*md[19]+ (dchunk)v[9]*md[18]+ (dchunk)v[10]*md[17]+ (dchunk)v[11]*md[16]+ (dchunk)v[12]*md[15]+ (dchunk)v[13]*md[14]+ (dchunk)v[14]*md[13]+ (dchunk)v[15]*md[12]+ (dchunk)v[16]*md[11]+ (dchunk)v[17]*md[10]+ (dchunk)v[18]*md[9]+ (dchunk)v[19]*md[8]+ (dchunk)v[20]*md[7]+ (dchunk)v[21]*md[6]+ (dchunk)v[22]*md[5]+ (dchunk)v[23]*md[4]+ (dchunk)v[24]*md[3]+ (dchunk)v[25]*md[2]+ (dchunk)v[26]*md[1]; v[27] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[27] * md[0]; t = (t >> BASEBITS_B1024_28) + d[28];
	t += (dchunk)v[0] * md[28] + (dchunk)v[1]*md[27]+ (dchunk)v[2]*md[26]+ (dchunk)v[3]*md[25]+ (dchunk)v[4]*md[24]+ (dchunk)v[5]*md[23]+ (dchunk)v[6]*md[22]+ (dchunk)v[7]*md[21]+ (dchunk)v[8]*md[20]+ (dchunk)v[9]*md[19]+ (dchunk)v[10]*md[18]+ (dchunk)v[11]*md[17]+ (dchunk)v[12]*md[16]+ (dchunk)v[13]*md[15]+ (dchunk)v[14]*md[14]+ (dchunk)v[15]*md[13]+ (dchunk)v[16]*md[12]+ (dchunk)v[17]*md[11]+ (dchunk)v[18]*md[10]+ (dchunk)v[19]*md[9]+ (dchunk)v[20]*md[8]+ (dchunk)v[21]*md[7]+ (dchunk)v[22]*md[6]+ (dchunk)v[23]*md[5]+ (dchunk)v[24]*md[4]+ (dchunk)v[25]*md[3]+ (dchunk)v[26]*md[2]+ (dchunk)v[27]*md[1]; v[28] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[28] * md[0]; t = (t >> BASEBITS_B1024_28) + d[29];
	t += (dchunk)v[0] * md[29] + (dchunk)v[1]*md[28]+ (dchunk)v[2]*md[27]+ (dchunk)v[3]*md[26]+ (dchunk)v[4]*md[25]+ (dchunk)v[5]*md[24]+ (dchunk)v[6]*md[23]+ (dchunk)v[7]*md[22]+ (dchunk)v[8]*md[21]+ (dchunk)v[9]*md[20]+ (dchunk)v[10]*md[19]+ (dchunk)v[11]*md[18]+ (dchunk)v[12]*md[17]+ (dchunk)v[13]*md[16]+ (dchunk)v[14]*md[15]+ (dchunk)v[15]*md[14]+ (dchunk)v[16]*md[13]+ (dchunk)v[17]*md[12]+ (dchunk)v[18]*md[11]+ (dchunk)v[19]*md[10]+ (dchunk)v[20]*md[9]+ (dchunk)v[21]*md[8]+ (dchunk)v[22]*md[7]+ (dchunk)v[23]*md[6]+ (dchunk)v[24]*md[5]+ (dchunk)v[25]*md[4]+ (dchunk)v[26]*md[3]+ (dchunk)v[27]*md[2]+ (dchunk)v[28]*md[1]; v[29] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[29] * md[0]; t = (t >> BASEBITS_B1024_28) + d[30];
	t += (dchunk)v[0] * md[30] + (dchunk)v[1]*md[29]+ (dchunk)v[2]*md[28]+ (dchunk)v[3]*md[27]+ (dchunk)v[4]*md[26]+ (dchunk)v[5]*md[25]+ (dchunk)v[6]*md[24]+ (dchunk)v[7]*md[23]+ (dchunk)v[8]*md[22]+ (dchunk)v[9]*md[21]+ (dchunk)v[10]*md[20]+ (dchunk)v[11]*md[19]+ (dchunk)v[12]*md[18]+ (dchunk)v[13]*md[17]+ (dchunk)v[14]*md[16]+ (dchunk)v[15]*md[15]+ (dchunk)v[16]*md[14]+ (dchunk)v[17]*md[13]+ (dchunk)v[18]*md[12]+ (dchunk)v[19]*md[11]+ (dchunk)v[20]*md[10]+ (dchunk)v[21]*md[9]+ (dchunk)v[22]*md[8]+ (dchunk)v[23]*md[7]+ (dchunk)v[24]*md[6]+ (dchunk)v[25]*md[5]+ (dchunk)v[26]*md[4]+ (dchunk)v[27]*md[3]+ (dchunk)v[28]*md[2]+ (dchunk)v[29]*md[1]; v[30] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[30] * md[0]; t = (t >> BASEBITS_B1024_28) + d[31];
	t += (dchunk)v[0] * md[31] + (dchunk)v[1]*md[30]+ (dchunk)v[2]*md[29]+ (dchunk)v[3]*md[28]+ (dchunk)v[4]*md[27]+ (dchunk)v[5]*md[26]+ (dchunk)v[6]*md[25]+ (dchunk)v[7]*md[24]+ (dchunk)v[8]*md[23]+ (dchunk)v[9]*md[22]+ (dchunk)v[10]*md[21]+ (dchunk)v[11]*md[20]+ (dchunk)v[12]*md[19]+ (dchunk)v[13]*md[18]+ (dchunk)v[14]*md[17]+ (dchunk)v[15]*md[16]+ (dchunk)v[16]*md[15]+ (dchunk)v[17]*md[14]+ (dchunk)v[18]*md[13]+ (dchunk)v[19]*md[12]+ (dchunk)v[20]*md[11]+ (dchunk)v[21]*md[10]+ (dchunk)v[22]*md[9]+ (dchunk)v[23]*md[8]+ (dchunk)v[24]*md[7]+ (dchunk)v[25]*md[6]+ (dchunk)v[26]*md[5]+ (dchunk)v[27]*md[4]+ (dchunk)v[28]*md[3]+ (dchunk)v[29]*md[2]+ (dchunk)v[30]*md[1]; v[31] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[31] * md[0]; t = (t >> BASEBITS_B1024_28) + d[32];
	t += (dchunk)v[0] * md[32] + (dchunk)v[1]*md[31]+ (dchunk)v[2]*md[30]+ (dchunk)v[3]*md[29]+ (dchunk)v[4]*md[28]+ (dchunk)v[5]*md[27]+ (dchunk)v[6]*md[26]+ (dchunk)v[7]*md[25]+ (dchunk)v[8]*md[24]+ (dchunk)v[9]*md[23]+ (dchunk)v[10]*md[22]+ (dchunk)v[11]*md[21]+ (dchunk)v[12]*md[20]+ (dchunk)v[13]*md[19]+ (dchunk)v[14]*md[18]+ (dchunk)v[15]*md[17]+ (dchunk)v[16]*md[16]+ (dchunk)v[17]*md[15]+ (dchunk)v[18]*md[14]+ (dchunk)v[19]*md[13]+ (dchunk)v[20]*md[12]+ (dchunk)v[21]*md[11]+ (dchunk)v[22]*md[10]+ (dchunk)v[23]*md[9]+ (dchunk)v[24]*md[8]+ (dchunk)v[25]*md[7]+ (dchunk)v[26]*md[6]+ (dchunk)v[27]*md[5]+ (dchunk)v[28]*md[4]+ (dchunk)v[29]*md[3]+ (dchunk)v[30]*md[2]+ (dchunk)v[31]*md[1]; v[32] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[32] * md[0]; t = (t >> BASEBITS_B1024_28) + d[33];
	t += (dchunk)v[0] * md[33] + (dchunk)v[1]*md[32]+ (dchunk)v[2]*md[31]+ (dchunk)v[3]*md[30]+ (dchunk)v[4]*md[29]+ (dchunk)v[5]*md[28]+ (dchunk)v[6]*md[27]+ (dchunk)v[7]*md[26]+ (dchunk)v[8]*md[25]+ (dchunk)v[9]*md[24]+ (dchunk)v[10]*md[23]+ (dchunk)v[11]*md[22]+ (dchunk)v[12]*md[21]+ (dchunk)v[13]*md[20]+ (dchunk)v[14]*md[19]+ (dchunk)v[15]*md[18]+ (dchunk)v[16]*md[17]+ (dchunk)v[17]*md[16]+ (dchunk)v[18]*md[15]+ (dchunk)v[19]*md[14]+ (dchunk)v[20]*md[13]+ (dchunk)v[21]*md[12]+ (dchunk)v[22]*md[11]+ (dchunk)v[23]*md[10]+ (dchunk)v[24]*md[9]+ (dchunk)v[25]*md[8]+ (dchunk)v[26]*md[7]+ (dchunk)v[27]*md[6]+ (dchunk)v[28]*md[5]+ (dchunk)v[29]*md[4]+ (dchunk)v[30]*md[3]+ (dchunk)v[31]*md[2]+ (dchunk)v[32]*md[1]; v[33] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[33] * md[0]; t = (t >> BASEBITS_B1024_28) + d[34];
	t += (dchunk)v[0] * md[34] + (dchunk)v[1]*md[33]+ (dchunk)v[2]*md[32]+ (dchunk)v[3]*md[31]+ (dchunk)v[4]*md[30]+ (dchunk)v[5]*md[29]+ (dchunk)v[6]*md[28]+ (dchunk)v[7]*md[27]+ (dchunk)v[8]*md[26]+ (dchunk)v[9]*md[25]+ (dchunk)v[10]*md[24]+ (dchunk)v[11]*md[23]+ (dchunk)v[12]*md[22]+ (dchunk)v[13]*md[21]+ (dchunk)v[14]*md[20]+ (dchunk)v[15]*md[19]+ (dchunk)v[16]*md[18]+ (dchunk)v[17]*md[17]+ (dchunk)v[18]*md[16]+ (dchunk)v[19]*md[15]+ (dchunk)v[20]*md[14]+ (dchunk)v[21]*md[13]+ (dchunk)v[22]*md[12]+ (dchunk)v[23]*md[11]+ (dchunk)v[24]*md[10]+ (dchunk)v[25]*md[9]+ (dchunk)v[26]*md[8]+ (dchunk)v[27]*md[7]+ (dchunk)v[28]*md[6]+ (dchunk)v[29]*md[5]+ (dchunk)v[30]*md[4]+ (dchunk)v[31]*md[3]+ (dchunk)v[32]*md[2]+ (dchunk)v[33]*md[1]; v[34] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[34] * md[0]; t = (t >> BASEBITS_B1024_28) + d[35];
	t += (dchunk)v[0] * md[35] + (dchunk)v[1]*md[34]+ (dchunk)v[2]*md[33]+ (dchunk)v[3]*md[32]+ (dchunk)v[4]*md[31]+ (dchunk)v[5]*md[30]+ (dchunk)v[6]*md[29]+ (dchunk)v[7]*md[28]+ (dchunk)v[8]*md[27]+ (dchunk)v[9]*md[26]+ (dchunk)v[10]*md[25]+ (dchunk)v[11]*md[24]+ (dchunk)v[12]*md[23]+ (dchunk)v[13]*md[22]+ (dchunk)v[14]*md[21]+ (dchunk)v[15]*md[20]+ (dchunk)v[16]*md[19]+ (dchunk)v[17]*md[18]+ (dchunk)v[18]*md[17]+ (dchunk)v[19]*md[16]+ (dchunk)v[20]*md[15]+ (dchunk)v[21]*md[14]+ (dchunk)v[22]*md[13]+ (dchunk)v[23]*md[12]+ (dchunk)v[24]*md[11]+ (dchunk)v[25]*md[10]+ (dchunk)v[26]*md[9]+ (dchunk)v[27]*md[8]+ (dchunk)v[28]*md[7]+ (dchunk)v[29]*md[6]+ (dchunk)v[30]*md[5]+ (dchunk)v[31]*md[4]+ (dchunk)v[32]*md[3]+ (dchunk)v[33]*md[2]+ (dchunk)v[34]*md[1]; v[35] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[35] * md[0]; t = (t >> BASEBITS_B1024_28) + d[36];
	t += (dchunk)v[0] * md[36] + (dchunk)v[1]*md[35]+ (dchunk)v[2]*md[34]+ (dchunk)v[3]*md[33]+ (dchunk)v[4]*md[32]+ (dchunk)v[5]*md[31]+ (dchunk)v[6]*md[30]+ (dchunk)v[7]*md[29]+ (dchunk)v[8]*md[28]+ (dchunk)v[9]*md[27]+ (dchunk)v[10]*md[26]+ (dchunk)v[11]*md[25]+ (dchunk)v[12]*md[24]+ (dchunk)v[13]*md[23]+ (dchunk)v[14]*md[22]+ (dchunk)v[15]*md[21]+ (dchunk)v[16]*md[20]+ (dchunk)v[17]*md[19]+ (dchunk)v[18]*md[18]+ (dchunk)v[19]*md[17]+ (dchunk)v[20]*md[16]+ (dchunk)v[21]*md[15]+ (dchunk)v[22]*md[14]+ (dchunk)v[23]*md[13]+ (dchunk)v[24]*md[12]+ (dchunk)v[25]*md[11]+ (dchunk)v[26]*md[10]+ (dchunk)v[27]*md[9]+ (dchunk)v[28]*md[8]+ (dchunk)v[29]*md[7]+ (dchunk)v[30]*md[6]+ (dchunk)v[31]*md[5]+ (dchunk)v[32]*md[4]+ (dchunk)v[33]*md[3]+ (dchunk)v[34]*md[2]+ (dchunk)v[35]*md[1]; v[36] = ((chunk)t * MC)&BMASK_B1024_28; t += (dchunk)v[36] * md[0]; t = (t >> BASEBITS_B1024_28) + d[37];
	t=t + (dchunk)v[1]*md[36] + (dchunk)v[2]*md[35] + (dchunk)v[3]*md[34] + (dchunk)v[4]*md[33] + (dchunk)v[5]*md[32] + (dchunk)v[6]*md[31] + (dchunk)v[7]*md[30] + (dchunk)v[8]*md[29] + (dchunk)v[9]*md[28] + (dchunk)v[10]*md[27] + (dchunk)v[11]*md[26] + (dchunk)v[12]*md[25] + (dchunk)v[13]*md[24] + (dchunk)v[14]*md[23] + (dchunk)v[15]*md[22] + (dchunk)v[16]*md[21] + (dchunk)v[17]*md[20] + (dchunk)v[18]*md[19] + (dchunk)v[19]*md[18] + (dchunk)v[20]*md[17] + (dchunk)v[21]*md[16] + (dchunk)v[22]*md[15] + (dchunk)v[23]*md[14] + (dchunk)v[24]*md[13] + (dchunk)v[25]*md[12] + (dchunk)v[26]*md[11] + (dchunk)v[27]*md[10] + (dchunk)v[28]*md[9] + (dchunk)v[29]*md[8] + (dchunk)v[30]*md[7] + (dchunk)v[31]*md[6] + (dchunk)v[32]*md[5] + (dchunk)v[33]*md[4] + (dchunk)v[34]*md[3] + (dchunk)v[35]*md[2] + (dchunk)v[36]*md[1] ; a[0] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[38];
	t=t + (dchunk)v[2]*md[36] + (dchunk)v[3]*md[35] + (dchunk)v[4]*md[34] + (dchunk)v[5]*md[33] + (dchunk)v[6]*md[32] + (dchunk)v[7]*md[31] + (dchunk)v[8]*md[30] + (dchunk)v[9]*md[29] + (dchunk)v[10]*md[28] + (dchunk)v[11]*md[27] + (dchunk)v[12]*md[26] + (dchunk)v[13]*md[25] + (dchunk)v[14]*md[24] + (dchunk)v[15]*md[23] + (dchunk)v[16]*md[22] + (dchunk)v[17]*md[21] + (dchunk)v[18]*md[20] + (dchunk)v[19]*md[19] + (dchunk)v[20]*md[18] + (dchunk)v[21]*md[17] + (dchunk)v[22]*md[16] + (dchunk)v[23]*md[15] + (dchunk)v[24]*md[14] + (dchunk)v[25]*md[13] + (dchunk)v[26]*md[12] + (dchunk)v[27]*md[11] + (dchunk)v[28]*md[10] + (dchunk)v[29]*md[9] + (dchunk)v[30]*md[8] + (dchunk)v[31]*md[7] + (dchunk)v[32]*md[6] + (dchunk)v[33]*md[5] + (dchunk)v[34]*md[4] + (dchunk)v[35]*md[3] + (dchunk)v[36]*md[2] ; a[1] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[39];
	t=t + (dchunk)v[3]*md[36] + (dchunk)v[4]*md[35] + (dchunk)v[5]*md[34] + (dchunk)v[6]*md[33] + (dchunk)v[7]*md[32] + (dchunk)v[8]*md[31] + (dchunk)v[9]*md[30] + (dchunk)v[10]*md[29] + (dchunk)v[11]*md[28] + (dchunk)v[12]*md[27] + (dchunk)v[13]*md[26] + (dchunk)v[14]*md[25] + (dchunk)v[15]*md[24] + (dchunk)v[16]*md[23] + (dchunk)v[17]*md[22] + (dchunk)v[18]*md[21] + (dchunk)v[19]*md[20] + (dchunk)v[20]*md[19] + (dchunk)v[21]*md[18] + (dchunk)v[22]*md[17] + (dchunk)v[23]*md[16] + (dchunk)v[24]*md[15] + (dchunk)v[25]*md[14] + (dchunk)v[26]*md[13] + (dchunk)v[27]*md[12] + (dchunk)v[28]*md[11] + (dchunk)v[29]*md[10] + (dchunk)v[30]*md[9] + (dchunk)v[31]*md[8] + (dchunk)v[32]*md[7] + (dchunk)v[33]*md[6] + (dchunk)v[34]*md[5] + (dchunk)v[35]*md[4] + (dchunk)v[36]*md[3] ; a[2] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[40];
	t=t + (dchunk)v[4]*md[36] + (dchunk)v[5]*md[35] + (dchunk)v[6]*md[34] + (dchunk)v[7]*md[33] + (dchunk)v[8]*md[32] + (dchunk)v[9]*md[31] + (dchunk)v[10]*md[30] + (dchunk)v[11]*md[29] + (dchunk)v[12]*md[28] + (dchunk)v[13]*md[27] + (dchunk)v[14]*md[26] + (dchunk)v[15]*md[25] + (dchunk)v[16]*md[24] + (dchunk)v[17]*md[23] + (dchunk)v[18]*md[22] + (dchunk)v[19]*md[21] + (dchunk)v[20]*md[20] + (dchunk)v[21]*md[19] + (dchunk)v[22]*md[18] + (dchunk)v[23]*md[17] + (dchunk)v[24]*md[16] + (dchunk)v[25]*md[15] + (dchunk)v[26]*md[14] + (dchunk)v[27]*md[13] + (dchunk)v[28]*md[12] + (dchunk)v[29]*md[11] + (dchunk)v[30]*md[10] + (dchunk)v[31]*md[9] + (dchunk)v[32]*md[8] + (dchunk)v[33]*md[7] + (dchunk)v[34]*md[6] + (dchunk)v[35]*md[5] + (dchunk)v[36]*md[4] ; a[3] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[41];
	t=t + (dchunk)v[5]*md[36] + (dchunk)v[6]*md[35] + (dchunk)v[7]*md[34] + (dchunk)v[8]*md[33] + (dchunk)v[9]*md[32] + (dchunk)v[10]*md[31] + (dchunk)v[11]*md[30] + (dchunk)v[12]*md[29] + (dchunk)v[13]*md[28] + (dchunk)v[14]*md[27] + (dchunk)v[15]*md[26] + (dchunk)v[16]*md[25] + (dchunk)v[17]*md[24] + (dchunk)v[18]*md[23] + (dchunk)v[19]*md[22] + (dchunk)v[20]*md[21] + (dchunk)v[21]*md[20] + (dchunk)v[22]*md[19] + (dchunk)v[23]*md[18] + (dchunk)v[24]*md[17] + (dchunk)v[25]*md[16] + (dchunk)v[26]*md[15] + (dchunk)v[27]*md[14] + (dchunk)v[28]*md[13] + (dchunk)v[29]*md[12] + (dchunk)v[30]*md[11] + (dchunk)v[31]*md[10] + (dchunk)v[32]*md[9] + (dchunk)v[33]*md[8] + (dchunk)v[34]*md[7] + (dchunk)v[35]*md[6] + (dchunk)v[36]*md[5] ; a[4] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[42];
	t=t + (dchunk)v[6]*md[36] + (dchunk)v[7]*md[35] + (dchunk)v[8]*md[34] + (dchunk)v[9]*md[33] + (dchunk)v[10]*md[32] + (dchunk)v[11]*md[31] + (dchunk)v[12]*md[30] + (dchunk)v[13]*md[29] + (dchunk)v[14]*md[28] + (dchunk)v[15]*md[27] + (dchunk)v[16]*md[26] + (dchunk)v[17]*md[25] + (dchunk)v[18]*md[24] + (dchunk)v[19]*md[23] + (dchunk)v[20]*md[22] + (dchunk)v[21]*md[21] + (dchunk)v[22]*md[20] + (dchunk)v[23]*md[19] + (dchunk)v[24]*md[18] + (dchunk)v[25]*md[17] + (dchunk)v[26]*md[16] + (dchunk)v[27]*md[15] + (dchunk)v[28]*md[14] + (dchunk)v[29]*md[13] + (dchunk)v[30]*md[12] + (dchunk)v[31]*md[11] + (dchunk)v[32]*md[10] + (dchunk)v[33]*md[9] + (dchunk)v[34]*md[8] + (dchunk)v[35]*md[7] + (dchunk)v[36]*md[6] ; a[5] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[43];
	t=t + (dchunk)v[7]*md[36] + (dchunk)v[8]*md[35] + (dchunk)v[9]*md[34] + (dchunk)v[10]*md[33] + (dchunk)v[11]*md[32] + (dchunk)v[12]*md[31] + (dchunk)v[13]*md[30] + (dchunk)v[14]*md[29] + (dchunk)v[15]*md[28] + (dchunk)v[16]*md[27] + (dchunk)v[17]*md[26] + (dchunk)v[18]*md[25] + (dchunk)v[19]*md[24] + (dchunk)v[20]*md[23] + (dchunk)v[21]*md[22] + (dchunk)v[22]*md[21] + (dchunk)v[23]*md[20] + (dchunk)v[24]*md[19] + (dchunk)v[25]*md[18] + (dchunk)v[26]*md[17] + (dchunk)v[27]*md[16] + (dchunk)v[28]*md[15] + (dchunk)v[29]*md[14] + (dchunk)v[30]*md[13] + (dchunk)v[31]*md[12] + (dchunk)v[32]*md[11] + (dchunk)v[33]*md[10] + (dchunk)v[34]*md[9] + (dchunk)v[35]*md[8] + (dchunk)v[36]*md[7] ; a[6] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[44];
	t=t + (dchunk)v[8]*md[36] + (dchunk)v[9]*md[35] + (dchunk)v[10]*md[34] + (dchunk)v[11]*md[33] + (dchunk)v[12]*md[32] + (dchunk)v[13]*md[31] + (dchunk)v[14]*md[30] + (dchunk)v[15]*md[29] + (dchunk)v[16]*md[28] + (dchunk)v[17]*md[27] + (dchunk)v[18]*md[26] + (dchunk)v[19]*md[25] + (dchunk)v[20]*md[24] + (dchunk)v[21]*md[23] + (dchunk)v[22]*md[22] + (dchunk)v[23]*md[21] + (dchunk)v[24]*md[20] + (dchunk)v[25]*md[19] + (dchunk)v[26]*md[18] + (dchunk)v[27]*md[17] + (dchunk)v[28]*md[16] + (dchunk)v[29]*md[15] + (dchunk)v[30]*md[14] + (dchunk)v[31]*md[13] + (dchunk)v[32]*md[12] + (dchunk)v[33]*md[11] + (dchunk)v[34]*md[10] + (dchunk)v[35]*md[9] + (dchunk)v[36]*md[8] ; a[7] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[45];
	t=t + (dchunk)v[9]*md[36] + (dchunk)v[10]*md[35] + (dchunk)v[11]*md[34] + (dchunk)v[12]*md[33] + (dchunk)v[13]*md[32] + (dchunk)v[14]*md[31] + (dchunk)v[15]*md[30] + (dchunk)v[16]*md[29] + (dchunk)v[17]*md[28] + (dchunk)v[18]*md[27] + (dchunk)v[19]*md[26] + (dchunk)v[20]*md[25] + (dchunk)v[21]*md[24] + (dchunk)v[22]*md[23] + (dchunk)v[23]*md[22] + (dchunk)v[24]*md[21] + (dchunk)v[25]*md[20] + (dchunk)v[26]*md[19] + (dchunk)v[27]*md[18] + (dchunk)v[28]*md[17] + (dchunk)v[29]*md[16] + (dchunk)v[30]*md[15] + (dchunk)v[31]*md[14] + (dchunk)v[32]*md[13] + (dchunk)v[33]*md[12] + (dchunk)v[34]*md[11] + (dchunk)v[35]*md[10] + (dchunk)v[36]*md[9] ; a[8] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[46];
	t=t + (dchunk)v[10]*md[36] + (dchunk)v[11]*md[35] + (dchunk)v[12]*md[34] + (dchunk)v[13]*md[33] + (dchunk)v[14]*md[32] + (dchunk)v[15]*md[31] + (dchunk)v[16]*md[30] + (dchunk)v[17]*md[29] + (dchunk)v[18]*md[28] + (dchunk)v[19]*md[27] + (dchunk)v[20]*md[26] + (dchunk)v[21]*md[25] + (dchunk)v[22]*md[24] + (dchunk)v[23]*md[23] + (dchunk)v[24]*md[22] + (dchunk)v[25]*md[21] + (dchunk)v[26]*md[20] + (dchunk)v[27]*md[19] + (dchunk)v[28]*md[18] + (dchunk)v[29]*md[17] + (dchunk)v[30]*md[16] + (dchunk)v[31]*md[15] + (dchunk)v[32]*md[14] + (dchunk)v[33]*md[13] + (dchunk)v[34]*md[12] + (dchunk)v[35]*md[11] + (dchunk)v[36]*md[10] ; a[9] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[47];
	t=t + (dchunk)v[11]*md[36] + (dchunk)v[12]*md[35] + (dchunk)v[13]*md[34] + (dchunk)v[14]*md[33] + (dchunk)v[15]*md[32] + (dchunk)v[16]*md[31] + (dchunk)v[17]*md[30] + (dchunk)v[18]*md[29] + (dchunk)v[19]*md[28] + (dchunk)v[20]*md[27] + (dchunk)v[21]*md[26] + (dchunk)v[22]*md[25] + (dchunk)v[23]*md[24] + (dchunk)v[24]*md[23] + (dchunk)v[25]*md[22] + (dchunk)v[26]*md[21] + (dchunk)v[27]*md[20] + (dchunk)v[28]*md[19] + (dchunk)v[29]*md[18] + (dchunk)v[30]*md[17] + (dchunk)v[31]*md[16] + (dchunk)v[32]*md[15] + (dchunk)v[33]*md[14] + (dchunk)v[34]*md[13] + (dchunk)v[35]*md[12] + (dchunk)v[36]*md[11] ; a[10] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[48];
	t=t + (dchunk)v[12]*md[36] + (dchunk)v[13]*md[35] + (dchunk)v[14]*md[34] + (dchunk)v[15]*md[33] + (dchunk)v[16]*md[32] + (dchunk)v[17]*md[31] + (dchunk)v[18]*md[30] + (dchunk)v[19]*md[29] + (dchunk)v[20]*md[28] + (dchunk)v[21]*md[27] + (dchunk)v[22]*md[26] + (dchunk)v[23]*md[25] + (dchunk)v[24]*md[24] + (dchunk)v[25]*md[23] + (dchunk)v[26]*md[22] + (dchunk)v[27]*md[21] + (dchunk)v[28]*md[20] + (dchunk)v[29]*md[19] + (dchunk)v[30]*md[18] + (dchunk)v[31]*md[17] + (dchunk)v[32]*md[16] + (dchunk)v[33]*md[15] + (dchunk)v[34]*md[14] + (dchunk)v[35]*md[13] + (dchunk)v[36]*md[12] ; a[11] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[49];
	t=t + (dchunk)v[13]*md[36] + (dchunk)v[14]*md[35] + (dchunk)v[15]*md[34] + (dchunk)v[16]*md[33] + (dchunk)v[17]*md[32] + (dchunk)v[18]*md[31] + (dchunk)v[19]*md[30] + (dchunk)v[20]*md[29] + (dchunk)v[21]*md[28] + (dchunk)v[22]*md[27] + (dchunk)v[23]*md[26] + (dchunk)v[24]*md[25] + (dchunk)v[25]*md[24] + (dchunk)v[26]*md[23] + (dchunk)v[27]*md[22] + (dchunk)v[28]*md[21] + (dchunk)v[29]*md[20] + (dchunk)v[30]*md[19] + (dchunk)v[31]*md[18] + (dchunk)v[32]*md[17] + (dchunk)v[33]*md[16] + (dchunk)v[34]*md[15] + (dchunk)v[35]*md[14] + (dchunk)v[36]*md[13] ; a[12] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[50];
	t=t + (dchunk)v[14]*md[36] + (dchunk)v[15]*md[35] + (dchunk)v[16]*md[34] + (dchunk)v[17]*md[33] + (dchunk)v[18]*md[32] + (dchunk)v[19]*md[31] + (dchunk)v[20]*md[30] + (dchunk)v[21]*md[29] + (dchunk)v[22]*md[28] + (dchunk)v[23]*md[27] + (dchunk)v[24]*md[26] + (dchunk)v[25]*md[25] + (dchunk)v[26]*md[24] + (dchunk)v[27]*md[23] + (dchunk)v[28]*md[22] + (dchunk)v[29]*md[21] + (dchunk)v[30]*md[20] + (dchunk)v[31]*md[19] + (dchunk)v[32]*md[18] + (dchunk)v[33]*md[17] + (dchunk)v[34]*md[16] + (dchunk)v[35]*md[15] + (dchunk)v[36]*md[14] ; a[13] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[51];
	t=t + (dchunk)v[15]*md[36] + (dchunk)v[16]*md[35] + (dchunk)v[17]*md[34] + (dchunk)v[18]*md[33] + (dchunk)v[19]*md[32] + (dchunk)v[20]*md[31] + (dchunk)v[21]*md[30] + (dchunk)v[22]*md[29] + (dchunk)v[23]*md[28] + (dchunk)v[24]*md[27] + (dchunk)v[25]*md[26] + (dchunk)v[26]*md[25] + (dchunk)v[27]*md[24] + (dchunk)v[28]*md[23] + (dchunk)v[29]*md[22] + (dchunk)v[30]*md[21] + (dchunk)v[31]*md[20] + (dchunk)v[32]*md[19] + (dchunk)v[33]*md[18] + (dchunk)v[34]*md[17] + (dchunk)v[35]*md[16] + (dchunk)v[36]*md[15] ; a[14] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[52];
	t=t + (dchunk)v[16]*md[36] + (dchunk)v[17]*md[35] + (dchunk)v[18]*md[34] + (dchunk)v[19]*md[33] + (dchunk)v[20]*md[32] + (dchunk)v[21]*md[31] + (dchunk)v[22]*md[30] + (dchunk)v[23]*md[29] + (dchunk)v[24]*md[28] + (dchunk)v[25]*md[27] + (dchunk)v[26]*md[26] + (dchunk)v[27]*md[25] + (dchunk)v[28]*md[24] + (dchunk)v[29]*md[23] + (dchunk)v[30]*md[22] + (dchunk)v[31]*md[21] + (dchunk)v[32]*md[20] + (dchunk)v[33]*md[19] + (dchunk)v[34]*md[18] + (dchunk)v[35]*md[17] + (dchunk)v[36]*md[16] ; a[15] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[53];
	t=t + (dchunk)v[17]*md[36] + (dchunk)v[18]*md[35] + (dchunk)v[19]*md[34] + (dchunk)v[20]*md[33] + (dchunk)v[21]*md[32] + (dchunk)v[22]*md[31] + (dchunk)v[23]*md[30] + (dchunk)v[24]*md[29] + (dchunk)v[25]*md[28] + (dchunk)v[26]*md[27] + (dchunk)v[27]*md[26] + (dchunk)v[28]*md[25] + (dchunk)v[29]*md[24] + (dchunk)v[30]*md[23] + (dchunk)v[31]*md[22] + (dchunk)v[32]*md[21] + (dchunk)v[33]*md[20] + (dchunk)v[34]*md[19] + (dchunk)v[35]*md[18] + (dchunk)v[36]*md[17] ; a[16] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[54];
	t=t + (dchunk)v[18]*md[36] + (dchunk)v[19]*md[35] + (dchunk)v[20]*md[34] + (dchunk)v[21]*md[33] + (dchunk)v[22]*md[32] + (dchunk)v[23]*md[31] + (dchunk)v[24]*md[30] + (dchunk)v[25]*md[29] + (dchunk)v[26]*md[28] + (dchunk)v[27]*md[27] + (dchunk)v[28]*md[26] + (dchunk)v[29]*md[25] + (dchunk)v[30]*md[24] + (dchunk)v[31]*md[23] + (dchunk)v[32]*md[22] + (dchunk)v[33]*md[21] + (dchunk)v[34]*md[20] + (dchunk)v[35]*md[19] + (dchunk)v[36]*md[18] ; a[17] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[55];
	t=t + (dchunk)v[19]*md[36] + (dchunk)v[20]*md[35] + (dchunk)v[21]*md[34] + (dchunk)v[22]*md[33] + (dchunk)v[23]*md[32] + (dchunk)v[24]*md[31] + (dchunk)v[25]*md[30] + (dchunk)v[26]*md[29] + (dchunk)v[27]*md[28] + (dchunk)v[28]*md[27] + (dchunk)v[29]*md[26] + (dchunk)v[30]*md[25] + (dchunk)v[31]*md[24] + (dchunk)v[32]*md[23] + (dchunk)v[33]*md[22] + (dchunk)v[34]*md[21] + (dchunk)v[35]*md[20] + (dchunk)v[36]*md[19] ; a[18] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[56];
	t=t + (dchunk)v[20]*md[36] + (dchunk)v[21]*md[35] + (dchunk)v[22]*md[34] + (dchunk)v[23]*md[33] + (dchunk)v[24]*md[32] + (dchunk)v[25]*md[31] + (dchunk)v[26]*md[30] + (dchunk)v[27]*md[29] + (dchunk)v[28]*md[28] + (dchunk)v[29]*md[27] + (dchunk)v[30]*md[26] + (dchunk)v[31]*md[25] + (dchunk)v[32]*md[24] + (dchunk)v[33]*md[23] + (dchunk)v[34]*md[22] + (dchunk)v[35]*md[21] + (dchunk)v[36]*md[20] ; a[19] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[57];
	t=t + (dchunk)v[21]*md[36] + (dchunk)v[22]*md[35] + (dchunk)v[23]*md[34] + (dchunk)v[24]*md[33] + (dchunk)v[25]*md[32] + (dchunk)v[26]*md[31] + (dchunk)v[27]*md[30] + (dchunk)v[28]*md[29] + (dchunk)v[29]*md[28] + (dchunk)v[30]*md[27] + (dchunk)v[31]*md[26] + (dchunk)v[32]*md[25] + (dchunk)v[33]*md[24] + (dchunk)v[34]*md[23] + (dchunk)v[35]*md[22] + (dchunk)v[36]*md[21] ; a[20] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[58];
	t=t + (dchunk)v[22]*md[36] + (dchunk)v[23]*md[35] + (dchunk)v[24]*md[34] + (dchunk)v[25]*md[33] + (dchunk)v[26]*md[32] + (dchunk)v[27]*md[31] + (dchunk)v[28]*md[30] + (dchunk)v[29]*md[29] + (dchunk)v[30]*md[28] + (dchunk)v[31]*md[27] + (dchunk)v[32]*md[26] + (dchunk)v[33]*md[25] + (dchunk)v[34]*md[24] + (dchunk)v[35]*md[23] + (dchunk)v[36]*md[22] ; a[21] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[59];
	t=t + (dchunk)v[23]*md[36] + (dchunk)v[24]*md[35] + (dchunk)v[25]*md[34] + (dchunk)v[26]*md[33] + (dchunk)v[27]*md[32] + (dchunk)v[28]*md[31] + (dchunk)v[29]*md[30] + (dchunk)v[30]*md[29] + (dchunk)v[31]*md[28] + (dchunk)v[32]*md[27] + (dchunk)v[33]*md[26] + (dchunk)v[34]*md[25] + (dchunk)v[35]*md[24] + (dchunk)v[36]*md[23] ; a[22] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[60];
	t=t + (dchunk)v[24]*md[36] + (dchunk)v[25]*md[35] + (dchunk)v[26]*md[34] + (dchunk)v[27]*md[33] + (dchunk)v[28]*md[32] + (dchunk)v[29]*md[31] + (dchunk)v[30]*md[30] + (dchunk)v[31]*md[29] + (dchunk)v[32]*md[28] + (dchunk)v[33]*md[27] + (dchunk)v[34]*md[26] + (dchunk)v[35]*md[25] + (dchunk)v[36]*md[24] ; a[23] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[61];
	t=t + (dchunk)v[25]*md[36] + (dchunk)v[26]*md[35] + (dchunk)v[27]*md[34] + (dchunk)v[28]*md[33] + (dchunk)v[29]*md[32] + (dchunk)v[30]*md[31] + (dchunk)v[31]*md[30] + (dchunk)v[32]*md[29] + (dchunk)v[33]*md[28] + (dchunk)v[34]*md[27] + (dchunk)v[35]*md[26] + (dchunk)v[36]*md[25] ; a[24] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[62];
	t=t + (dchunk)v[26]*md[36] + (dchunk)v[27]*md[35] + (dchunk)v[28]*md[34] + (dchunk)v[29]*md[33] + (dchunk)v[30]*md[32] + (dchunk)v[31]*md[31] + (dchunk)v[32]*md[30] + (dchunk)v[33]*md[29] + (dchunk)v[34]*md[28] + (dchunk)v[35]*md[27] + (dchunk)v[36]*md[26] ; a[25] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[63];
	t=t + (dchunk)v[27]*md[36] + (dchunk)v[28]*md[35] + (dchunk)v[29]*md[34] + (dchunk)v[30]*md[33] + (dchunk)v[31]*md[32] + (dchunk)v[32]*md[31] + (dchunk)v[33]*md[30] + (dchunk)v[34]*md[29] + (dchunk)v[35]*md[28] + (dchunk)v[36]*md[27] ; a[26] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[64];
	t=t + (dchunk)v[28]*md[36] + (dchunk)v[29]*md[35] + (dchunk)v[30]*md[34] + (dchunk)v[31]*md[33] + (dchunk)v[32]*md[32] + (dchunk)v[33]*md[31] + (dchunk)v[34]*md[30] + (dchunk)v[35]*md[29] + (dchunk)v[36]*md[28] ; a[27] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[65];
	t=t + (dchunk)v[29]*md[36] + (dchunk)v[30]*md[35] + (dchunk)v[31]*md[34] + (dchunk)v[32]*md[33] + (dchunk)v[33]*md[32] + (dchunk)v[34]*md[31] + (dchunk)v[35]*md[30] + (dchunk)v[36]*md[29] ; a[28] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[66];
	t=t + (dchunk)v[30]*md[36] + (dchunk)v[31]*md[35] + (dchunk)v[32]*md[34] + (dchunk)v[33]*md[33] + (dchunk)v[34]*md[32] + (dchunk)v[35]*md[31] + (dchunk)v[36]*md[30] ; a[29] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[67];
	t=t + (dchunk)v[31]*md[36] + (dchunk)v[32]*md[35] + (dchunk)v[33]*md[34] + (dchunk)v[34]*md[33] + (dchunk)v[35]*md[32] + (dchunk)v[36]*md[31] ; a[30] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[68];
	t=t + (dchunk)v[32]*md[36] + (dchunk)v[33]*md[35] + (dchunk)v[34]*md[34] + (dchunk)v[35]*md[33] + (dchunk)v[36]*md[32] ; a[31] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[69];
	t=t + (dchunk)v[33]*md[36] + (dchunk)v[34]*md[35] + (dchunk)v[35]*md[34] + (dchunk)v[36]*md[33] ; a[32] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[70];
	t=t + (dchunk)v[34]*md[36] + (dchunk)v[35]*md[35] + (dchunk)v[36]*md[34] ; a[33] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[71];
	t=t + (dchunk)v[35]*md[36] + (dchunk)v[36]*md[35] ; a[34] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[72];
	t=t + (dchunk)v[36]*md[36] ; a[35] = (chunk)t & BMASK_B1024_28; t = (t >> BASEBITS_B1024_28) + d[73];
	a[36] = (chunk)t & BMASK_B1024_28;


#endif

#else

#ifndef USE_KARATSUBA 
    t = d[0];
    v[0] = ((chunk)t * MC)&BMASK_B1024_28;
    t += (dchunk)v[0] * md[0];
    t = (t >> BASEBITS_B1024_28) + d[1];
   
    for (i = 1; i < NLEN_B1024_28; i++)
    {
        k=1;
        t += (dchunk)v[0] * md[i];
        while (k<i) {t += (dchunk)v[k]*md[i-k]; k++;}
        v[i] = ((chunk)t * MC)&BMASK_B1024_28;
        t += (dchunk)v[i] * md[0];
        t = (t >> BASEBITS_B1024_28) + d[i + 1];
    }
    for (i = NLEN_B1024_28; i < 2 * NLEN_B1024_28 - 1; i++)
    {
        k=i-(NLEN_B1024_28-1);
        while (k<=NLEN_B1024_28-1) {t += (dchunk)v[k]*md[i-k]; k++;}
        a[i - NLEN_B1024_28] = (chunk)t & BMASK_B1024_28;
        t = (t >> BASEBITS_B1024_28) + d[i + 1];
    }
    a[NLEN_B1024_28 - 1] = (chunk)t & BMASK_B1024_28;
#else

    t = d[0];
    v[0] = ((chunk)t * MC)&BMASK_B1024_28;
    t += (dchunk)v[0] * md[0];
    t = (t >> BASEBITS_B1024_28) + d[1];
    s = 0;

    for (k = 1; k < NLEN_B1024_28; k++)
    {
        t = t + s + (dchunk)v[0] * md[k];

        for (i=1+k/2;i<k;i++) t += (dchunk)(v[k - i] - v[i]) * (md[i] - md[k - i]);
        v[k] = ((chunk)t * MC)&BMASK_B1024_28;
        t += (dchunk)v[k] * md[0];
        t = (t >> BASEBITS_B1024_28) + d[k + 1];
        dd[k] = (dchunk)v[k] * md[k];
        s += dd[k];
    }
    for (k = NLEN_B1024_28; k < 2 * NLEN_B1024_28 - 1; k++)
    {
        t = t + s;
        for (i=1+k/2;i<NLEN_B1024_28;i++) t += (dchunk)(v[k - i] - v[i]) * (md[i] - md[k - i]);
        a[k - NLEN_B1024_28] = (chunk)t & BMASK_B1024_28;
        t = (t >> BASEBITS_B1024_28) + d[k + 1];
        s -= dd[k - NLEN_B1024_28 + 1];
    }
    a[NLEN_B1024_28 - 1] = (chunk)t & BMASK_B1024_28;
#endif

#endif



#else
    int j;
    chunk m, carry;
    for (i = 0; i < NLEN_B1024_28; i++)
    {
        if (MC == -1) m = (-d[i])&BMASK_B1024_28;
        else
        {
            if (MC == 1) m = d[i];
            else m = (MC * d[i])&BMASK_B1024_28;
        }
        carry = 0;
        for (j = 0; j < NLEN_B1024_28; j++)
            carry = muladd(m, md[j], carry, &d[i + j]);
        d[NLEN_B1024_28 + i] += carry;
    }
    BIG_sducopy(a, d);
    BIG_norm(a);

#endif

#ifdef DEBUG_NORM
    a[MPV_B1024_28] = 1;  a[MNV_B1024_28] = 0;
#endif
}

/* General shift left of a by n bits */
/* a MUST be normalised */
/* SU= 32 */
void B1024_28::BIG_shl(BIG a, int k)
{
    int i;
    int n = k % BASEBITS_B1024_28;
    int m = k / BASEBITS_B1024_28;

    a[NLEN_B1024_28 - 1] = ((a[NLEN_B1024_28 - 1 - m] << n));
    if (NLEN_B1024_28 >= m + 2) a[NLEN_B1024_28 - 1] |= (a[NLEN_B1024_28 - m - 2] >> (BASEBITS_B1024_28 - n));

    for (i = NLEN_B1024_28 - 2; i > m; i--)
        a[i] = ((a[i - m] << n)&BMASK_B1024_28) | (a[i - m - 1] >> (BASEBITS_B1024_28 - n));
    a[m] = (a[0] << n)&BMASK_B1024_28;
    for (i = 0; i < m; i++) a[i] = 0;

}

/* Fast shift left of a by n bits, where n less than a word, Return excess (but store it as well) */
/* a MUST be normalised */
/* SU= 16 */
int B1024_28::BIG_fshl(BIG a, int n)
{
    int i;

    a[NLEN_B1024_28 - 1] = ((a[NLEN_B1024_28 - 1] << n)) | (a[NLEN_B1024_28 - 2] >> (BASEBITS_B1024_28 - n)); /* top word not masked */
    for (i = NLEN_B1024_28 - 2; i > 0; i--)
        a[i] = ((a[i] << n)&BMASK_B1024_28) | (a[i - 1] >> (BASEBITS_B1024_28 - n));
    a[0] = (a[0] << n)&BMASK_B1024_28;

    return (int)(a[NLEN_B1024_28 - 1] >> ((8 * MODBYTES_B1024_28) % BASEBITS_B1024_28)); /* return excess - only used in ff.c */
}

/* double length left shift of a by k bits - k can be > BASEBITS_B1024_28 , a MUST be normalised */
/* SU= 32 */
void B1024_28::BIG_dshl(DBIG a, int k)
{
    int i;
    int n = k % BASEBITS_B1024_28;
    int m = k / BASEBITS_B1024_28;

    a[DNLEN_B1024_28 - 1] = ((a[DNLEN_B1024_28 - 1 - m] << n)) | (a[DNLEN_B1024_28 - m - 2] >> (BASEBITS_B1024_28 - n));

    for (i = DNLEN_B1024_28 - 2; i > m; i--)
        a[i] = ((a[i - m] << n)&BMASK_B1024_28) | (a[i - m - 1] >> (BASEBITS_B1024_28 - n));
    a[m] = (a[0] << n)&BMASK_B1024_28;
    for (i = 0; i < m; i++) a[i] = 0;

}

/* General shift rightof a by k bits */
/* a MUST be normalised */
/* SU= 32 */
void B1024_28::BIG_shr(BIG a, int k)
{
    int i;
    int n = k % BASEBITS_B1024_28;
    int m = k / BASEBITS_B1024_28;
    for (i = 0; i < NLEN_B1024_28 - m - 1; i++)
        a[i] = (a[m + i] >> n) | ((a[m + i + 1] << (BASEBITS_B1024_28 - n))&BMASK_B1024_28);
    if (NLEN_B1024_28 > m)  a[NLEN_B1024_28 - m - 1] = a[NLEN_B1024_28 - 1] >> n;
    for (i = NLEN_B1024_28 - m; i < NLEN_B1024_28; i++) a[i] = 0;

}

/* Fast combined shift, subtract and norm. Return sign of result */
int B1024_28::BIG_ssn(BIG r, BIG a, BIG m)
{
    int i, n = NLEN_B1024_28 - 1;
    chunk carry;
    m[0] = (m[0] >> 1) | ((m[1] << (BASEBITS_B1024_28 - 1))&BMASK_B1024_28);
    r[0] = a[0] - m[0];
    carry = r[0] >> BASEBITS_B1024_28;
    r[0] &= BMASK_B1024_28;

    for (i = 1; i < n; i++)
    {
        m[i] = (m[i] >> 1) | ((m[i + 1] << (BASEBITS_B1024_28 - 1))&BMASK_B1024_28);
        r[i] = a[i] - m[i] + carry;
        carry = r[i] >> BASEBITS_B1024_28;
        r[i] &= BMASK_B1024_28;
    }

    m[n] >>= 1;
    r[n] = a[n] - m[n] + carry;
#ifdef DEBUG_NORM
    r[MPV_B1024_28] = 1; r[MNV_B1024_28] = 0;
#endif
    return ((r[n] >> (CHUNK - 1)) & 1);
}

/* Faster shift right of a by k bits. Return shifted out part */
/* a MUST be normalised */
/* SU= 16 */
int B1024_28::BIG_fshr(BIG a, int k)
{
    int i;
    chunk r = a[0] & (((chunk)1 << k) - 1); /* shifted out part */
    for (i = 0; i < NLEN_B1024_28 - 1; i++)
        a[i] = (a[i] >> k) | ((a[i + 1] << (BASEBITS_B1024_28 - k))&BMASK_B1024_28);
    a[NLEN_B1024_28 - 1] = a[NLEN_B1024_28 - 1] >> k;
    return (int)r;
}

/* double length right shift of a by k bits - can be > BASEBITS_B1024_28 */
/* SU= 32 */
void B1024_28::BIG_dshr(DBIG a, int k)
{
    int i;
    int n = k % BASEBITS_B1024_28;
    int m = k / BASEBITS_B1024_28;
    for (i = 0; i < DNLEN_B1024_28 - m - 1; i++)
        a[i] = (a[m + i] >> n) | ((a[m + i + 1] << (BASEBITS_B1024_28 - n))&BMASK_B1024_28);
    a[DNLEN_B1024_28 - m - 1] = a[DNLEN_B1024_28 - 1] >> n;
    for (i = DNLEN_B1024_28 - m; i < DNLEN_B1024_28; i++ ) a[i] = 0;
}

/* Split DBIG d into two BIGs t|b. Split happens at n bits, where n falls into NLEN_B1024_28 word */
/* d MUST be normalised */
/* SU= 24 */
chunk B1024_28::BIG_split(BIG t, BIG b, DBIG d, int n)
{
    int i;
    chunk nw, carry = 0;
    int m = n % BASEBITS_B1024_28;

    if (m == 0)
    {
        for (i = 0; i < NLEN_B1024_28; i++) b[i] = d[i];
        if (t != b)
        {
            for (i = NLEN_B1024_28; i < 2 * NLEN_B1024_28; i++) t[i - NLEN_B1024_28] = d[i];
            carry = t[NLEN_B1024_28 - 1] >> BASEBITS_B1024_28;
            t[NLEN_B1024_28 - 1] = t[NLEN_B1024_28 - 1] & BMASK_B1024_28; /* top word normalized */
        }
        return carry;
    }

    for (i = 0; i < NLEN_B1024_28 - 1; i++) b[i] = d[i];

    b[NLEN_B1024_28 - 1] = d[NLEN_B1024_28 - 1] & (((chunk)1 << m) - 1);

    if (t != b)
    {
        carry = (d[DNLEN_B1024_28 - 1] << (BASEBITS_B1024_28 - m));
        for (i = DNLEN_B1024_28 - 2; i >= NLEN_B1024_28 - 1; i--)
        {
            nw = (d[i] >> m) | carry;
            carry = (d[i] << (BASEBITS_B1024_28 - m))&BMASK_B1024_28;
            t[i - NLEN_B1024_28 + 1] = nw;
        }
    }
#ifdef DEBUG_NORM
    t[MPV_B1024_28] = 1; t[MNV_B1024_28] = 0;
    b[MPV_B1024_28] = 1; b[MNV_B1024_28] = 0;
#endif
    return carry;
}

/* you gotta keep the sign of carry! Look - no branching! */
/* Note that sign bit is needed to disambiguate between +ve and -ve values */
/* normalise BIG - force all digits < 2^BASEBITS_B1024_28 */
chunk B1024_28::BIG_norm(BIG a)
{
    int i;
    chunk d, carry;

    carry=a[0]>>BASEBITS_B1024_28;
    a[0]&=BMASK_B1024_28;

    for (i = 1; i < NLEN_B1024_28 - 1; i++)
    {
        d = a[i] + carry;
        a[i] = d & BMASK_B1024_28;
        carry = d >> BASEBITS_B1024_28;
    }
    a[NLEN_B1024_28 - 1] = (a[NLEN_B1024_28 - 1] + carry);

#ifdef DEBUG_NORM
    a[MPV_B1024_28] = 1; a[MNV_B1024_28] = 0;
#endif
    return (a[NLEN_B1024_28 - 1] >> ((8 * MODBYTES_B1024_28) % BASEBITS_B1024_28)); /* only used in ff.c */
}

void B1024_28::BIG_dnorm(DBIG a)
{
    int i;
    chunk d, carry;

    carry=a[0]>>BASEBITS_B1024_28;
    a[0]&=BMASK_B1024_28;

    for (i = 1; i < DNLEN_B1024_28 - 1; i++)
    {
        d = a[i] + carry;
        a[i] = d & BMASK_B1024_28;
        carry = d >> BASEBITS_B1024_28;
    }
    a[DNLEN_B1024_28 - 1] = (a[DNLEN_B1024_28 - 1] + carry);
#ifdef DEBUG_NORM
    a[DMPV_B1024_28] = 1; a[DMNV_B1024_28] = 0;
#endif
}

/* Compare a and b. Return 1 for a>b, -1 for a<b, 0 for a==b */
/* a and b MUST be normalised before call */
/* sodium constant time implementation */

int B1024_28::BIG_comp(BIG a, BIG b)
{
    int i;
    chunk gt=0; chunk eq=1;
    for (i = NLEN_B1024_28-1; i>=0; i--)
    {
        gt |= ((b[i]-a[i]) >> BASEBITS_B1024_28) & eq;
        eq &= ((b[i]^a[i])-1) >> BASEBITS_B1024_28;
    }
    return (int)(gt+gt+eq-1);
}

int B1024_28::BIG_dcomp(DBIG a, DBIG b)
{
    int i;
    chunk gt=0; chunk eq=1;
    for (i = DNLEN_B1024_28-1; i>=0; i--)
    {
        gt |= ((b[i]-a[i]) >> BASEBITS_B1024_28) & eq;
        eq &= ((b[i]^a[i])-1) >> BASEBITS_B1024_28;
    }
    return (int)(gt+gt+eq-1);
}

/* return number of bits in a */
/* SU= 8 */
int B1024_28::BIG_nbits(BIG a)
{
    int bts, k = NLEN_B1024_28 - 1;
    BIG t;
    chunk c;
    BIG_copy(t, a);
    BIG_norm(t);
    while (k >= 0 && t[k] == 0) k--;
    if (k < 0) return 0;
    bts = BASEBITS_B1024_28 * k;
    c = t[k];
    while (c != 0)
    {
        c /= 2;
        bts++;
    }
    return bts;
}

/* SU= 8, Calculate number of bits in a DBIG - output normalised */
int B1024_28::BIG_dnbits(DBIG a)
{
    int bts, k = DNLEN_B1024_28 - 1;
    DBIG t;
    chunk c;
    BIG_dcopy(t, a);
    BIG_dnorm(t);
    while (k >= 0 && t[k] == 0) k--;
    if (k < 0) return 0;
    bts = BASEBITS_B1024_28 * k;
    c = t[k];
    while (c != 0)
    {
        c /= 2;
        bts++;
    }
    return bts;
}


/* Set b=b mod c */
/* SU= 16 */
void B1024_28::BIG_mod(BIG b, BIG c1)
{
    int k = 0;
    BIG r; /**/
    BIG c;
    BIG_copy(c, c1);

    BIG_norm(b);
    if (BIG_comp(b, c) < 0)
        return;
    do
    {
        BIG_fshl(c, 1);
        k++;
    }
    while (BIG_comp(b, c) >= 0);

    while (k > 0)
    {
        BIG_fshr(c, 1);

// constant time...
        BIG_sub(r, b, c);
        BIG_norm(r);
        BIG_cmove(b, r, 1 - ((r[NLEN_B1024_28 - 1] >> (CHUNK - 1)) & 1));
        k--;
    }
}

/* Set a=b mod c, b is destroyed. Slow but rarely used. */
/* SU= 96 */
void B1024_28::BIG_dmod(BIG a, DBIG b, BIG c)
{
    int k = 0;
    DBIG m, r;
    BIG_dnorm(b);
    BIG_dscopy(m, c);

    if (BIG_dcomp(b, m) < 0)
    {
        BIG_sdcopy(a, b);
        return;
    }

    do
    {
        BIG_dshl(m, 1);
        k++;
    }
    while (BIG_dcomp(b, m) >= 0);

    while (k > 0)
    {
        BIG_dshr(m, 1);
// constant time...
        BIG_dsub(r, b, m);
        BIG_dnorm(r);
        BIG_dcmove(b, r, 1 - ((r[DNLEN_B1024_28 - 1] >> (CHUNK - 1)) & 1));

        k--;
    }
    BIG_sdcopy(a, b);
}

/* Set a=b/c,  b is destroyed. Slow but rarely used. */
/* SU= 136 */

void B1024_28::BIG_ddiv(BIG a, DBIG b, BIG c)
{
    int d, k = 0;
    DBIG m, dr;
    BIG e, r;
    BIG_dnorm(b);
    BIG_dscopy(m, c);

    BIG_zero(a);
    BIG_zero(e);
    BIG_inc(e, 1);

    while (BIG_dcomp(b, m) >= 0)
    {
        BIG_fshl(e, 1);
        BIG_dshl(m, 1);
        k++;
    }

    while (k > 0)
    {
        BIG_dshr(m, 1);
        BIG_fshr(e, 1);

        BIG_dsub(dr, b, m);
        BIG_dnorm(dr);
        d = 1 - ((dr[DNLEN_B1024_28 - 1] >> (CHUNK - 1)) & 1);
        BIG_dcmove(b, dr, d);

        BIG_add(r, a, e);
        BIG_norm(r);
        BIG_cmove(a, r, d);

        k--;
    }
}

/* SU= 136 */

void B1024_28::BIG_sdiv(BIG a, BIG c)
{
    int d, k = 0;
    BIG m, e, b, r;
    BIG_norm(a);
    BIG_copy(b, a);
    BIG_copy(m, c);

    BIG_zero(a);
    BIG_zero(e);
    BIG_inc(e, 1);

    while (BIG_comp(b, m) >= 0)
    {
        BIG_fshl(e, 1);
        BIG_fshl(m, 1);
        k++;
    }

    while (k > 0)
    {
        BIG_fshr(m, 1);
        BIG_fshr(e, 1);

        BIG_sub(r, b, m);
        BIG_norm(r);
        d = 1 - ((r[NLEN_B1024_28 - 1] >> (CHUNK - 1)) & 1);
        BIG_cmove(b, r, d);

        BIG_add(r, a, e);
        BIG_norm(r);
        BIG_cmove(a, r, d);
        k--;
    }
}

/* return LSB of a */
int B1024_28::BIG_parity(BIG a)
{
    return a[0] % 2;
}

/* return n-th bit of a */
/* SU= 16 */
int B1024_28::BIG_bit(BIG a, int n)
{
    if (a[n / BASEBITS_B1024_28] & ((chunk)1 << (n % BASEBITS_B1024_28))) return 1;
    else return 0;
}

/* return last n bits of a, where n is small < BASEBITS_B1024_28 */
/* SU= 16 */
int B1024_28::BIG_lastbits(BIG a, int n)
{
    int msk = (1 << n) - 1;
    BIG_norm(a);
    return ((int)a[0])&msk;
}

/* get 8*MODBYTES_B1024_28 size random number */
void B1024_28::BIG_random(BIG m, csprng *rng)
{
    int i, b, j = 0, r = 0;
    int len = 8 * MODBYTES_B1024_28;

    BIG_zero(m);
    /* generate random BIG */
    for (i = 0; i < len; i++)
    {
        if (j == 0) r = RAND_byte(rng);
        else r >>= 1;
        b = r & 1;
        BIG_shl(m, 1);
        m[0] += b;
        j++;
        j &= 7;
    }

#ifdef DEBUG_NORM
    m[MPV_B1024_28] = 1; m[MNV_B1024_28] = 0;
#endif
}

/* get random BIG from rng, modulo q. Done one bit at a time, so its portable */

void B1024_28::BIG_randomnum(BIG m, BIG q, csprng *rng)
{
    int i, b, j = 0, r = 0;
    DBIG d;
    BIG_dzero(d);
    /* generate random DBIG */
    for (i = 0; i < 2 * BIG_nbits(q); i++)
    {
        if (j == 0) r = RAND_byte(rng);
        else r >>= 1;
        b = r & 1;
        BIG_dshl(d, 1);
        d[0] += b;
        j++;
        j &= 7;
    }
    /* reduce modulo a BIG. Removes bias */
    BIG_dmod(m, d, q);
#ifdef DEBUG_NORM
    m[MPV_B1024_28] = 1; m[MNV_B1024_28] = 0;
#endif
}

/* create randum BIG less than r and less than trunc bits */
void B1024_28::BIG_randtrunc(BIG s, BIG r, int trunc, csprng *rng)
{
    BIG_randomnum(s, r, rng);
    if (BIG_nbits(r) > trunc)
        BIG_mod2m(s, trunc);
}

/* Set r=a*b mod m */
/* SU= 96 */
void B1024_28::BIG_modmul(BIG r, BIG a1, BIG b1, BIG m)
{
    DBIG d;
    BIG a, b;
    BIG_copy(a, a1);
    BIG_copy(b, b1);
    BIG_mod(a, m);
    BIG_mod(b, m);

    BIG_mul(d, a, b);
    BIG_dmod(r, d, m);
}

/* Set a=a*a mod m */
/* SU= 88 */
void B1024_28::BIG_modsqr(BIG r, BIG a1, BIG m)
{
    DBIG d;
    BIG a;
    BIG_copy(a, a1);
    BIG_mod(a, m);
    BIG_sqr(d, a);
    BIG_dmod(r, d, m);
}

/* Set r=-a mod m */
/* SU= 16 */
void B1024_28::BIG_modneg(BIG r, BIG a1, BIG m)
{
    BIG a;
    BIG_copy(a, a1);
    BIG_mod(a, m);
    BIG_sub(r, m, a);
    BIG_mod(r, m);
}

/* Set r=a+b mod m */
void B1024_28::BIG_modadd(BIG r, BIG a1, BIG b1, BIG m)
{
    BIG a, b;
    BIG_copy(a, a1);
    BIG_copy(b, b1);
    BIG_mod(a, m);
    BIG_mod(b, m);
    BIG_add(r,a,b); BIG_norm(r);
    BIG_mod(r,m);
}

/* Set a=a/b mod m */
/* SU= 136 */
void B1024_28::BIG_moddiv(BIG r, BIG a1, BIG b1, BIG m)
{
    DBIG d;
    BIG z;
    BIG a, b;
    BIG_copy(a, a1);
    BIG_copy(b, b1);
    BIG_mod(a, m);
    BIG_invmodp(z, b, m);

    BIG_mul(d, a, z);
    BIG_dmod(r, d, m);
}

/* Get jacobi Symbol (a/p). Returns 0, 1 or -1 */
/* SU= 216 */
int B1024_28::BIG_jacobi(BIG a, BIG p)
{
    int n8, k, m = 0;
    BIG t, x, n, zilch, one;
    BIG_one(one);
    BIG_zero(zilch);
    if (BIG_parity(p) == 0 || BIG_comp(a, zilch) == 0 || BIG_comp(p, one) <= 0) return 0;
    BIG_norm(a);
    BIG_copy(x, a);
    BIG_copy(n, p);
    BIG_mod(x, p);

    while (BIG_comp(n, one) > 0)
    {
        if (BIG_comp(x, zilch) == 0) return 0;
        n8 = BIG_lastbits(n, 3);
        k = 0;
        while (BIG_parity(x) == 0)
        {
            k++;
            BIG_shr(x, 1);
        }
        if (k % 2 == 1) m += (n8 * n8 - 1) / 8;
        m += (n8 - 1) * (BIG_lastbits(x, 2) - 1) / 4;
        BIG_copy(t, n);

        BIG_mod(t, x);
        BIG_copy(n, x);
        BIG_copy(x, t);
        m %= 2;

    }
    if (m == 0) return 1;
    else return -1;
}

/*

int B1024_28::step1(BIG u,BIG x,BIG p)
{
    int k=0;
    BIG t;
    while (BIG_bit(u,k)==0)
    {
        BIG_add(t,x,p);
        BIG_cmove(x,t,BIG_parity(x));
        BIG_norm(x);
        BIG_fshr(x,1);
        k++;
    }
    return k;
}

void B1024_28::step2(BIG xf,BIG xs,BIG p)
{
    BIG t;
    BIG_add(t,xf,p);
    BIG_cmove(xf,t,(BIG_comp(xf,xs)>>1)&1); // move if x1<x2 
    BIG_sub(xf,xf,xs);
    BIG_norm(xf);
}

*/

/* Set r=1/a mod p. Binary method */
void B1024_28::BIG_invmodp(BIG r, BIG a, BIG p)
{
    BIG u, v, x1, x2, t, one;
    int par,s;

    BIG_mod(a, p);
    if (BIG_iszilch(a))
    {
        BIG_zero(r);
        return;
    }

    BIG_copy(u, a);
    BIG_copy(v, p);
    BIG_one(one);
    BIG_copy(x1, one);
    BIG_zero(x2);

    while (BIG_comp(u, one) != 0 && BIG_comp(v, one) != 0)
    {
        while (BIG_parity(u) == 0)
        {
            BIG_fshr(u, 1);
            BIG_add(t,x1,p);
            BIG_cmove(x1,t,BIG_parity(x1));
            BIG_norm(x1);
            BIG_fshr(x1,1);
        }
        while (BIG_parity(v) == 0)
        {
            BIG_fshr(v, 1);
            BIG_add(t,x2,p);
            BIG_cmove(x2,t,BIG_parity(x2));
            BIG_norm(x2);
            BIG_fshr(x2,1);
        } 
        if (BIG_comp(u, v) >= 0) 
        {
            BIG_sub(u, u, v);
            BIG_norm(u);
            BIG_add(t,x1,p);
            BIG_cmove(x1,t,(BIG_comp(x1,x2)>>1)&1); // move if x1<x2 
            BIG_sub(x1,x1,x2);
            BIG_norm(x1);
        }
        else
        {
            BIG_sub(v, v, u);
            BIG_norm(v);
            BIG_add(t,x2,p);
            BIG_cmove(x2,t,(BIG_comp(x2,x1)>>1)&1); // move if x2<x1 
            BIG_sub(x2,x2,x1);
            BIG_norm(x2);
        }
    }
    BIG_copy(r,x1);
    BIG_cmove(r,x2,BIG_comp(u,one)&1);
}


/* set x = x mod 2^m */
void B1024_28::BIG_mod2m(BIG x, int m)
{
    int i, wd, bt;
    chunk msk;
    BIG_norm(x);

    wd = m / BASEBITS_B1024_28;
    bt = m % BASEBITS_B1024_28;
    msk = ((chunk)1 << bt) - 1;
    x[wd] &= msk;
    for (i = wd + 1; i < NLEN_B1024_28; i++) x[i] = 0;
}

// new
/* Convert to DBIG number from byte array of given length */
void B1024_28::BIG_dfromBytesLen(DBIG a, char *b, int s)
{
    int i, len = s;
    BIG_dzero(a);

    for (i = 0; i < len; i++)
    {
        BIG_dshl(a, 8);
        a[0] += (int)(unsigned char)b[i];
    }
#ifdef DEBUG_NORM
    a[DMPV_B1024_28] = 1; a[DMNV_B1024_28] = 0;
#endif
}
