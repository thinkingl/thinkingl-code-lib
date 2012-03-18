// DES Encyption Library
// Scott Dial
// Copyright 2004

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <math.h>
#include <set>
#include <sstream>
#include <algorithm>
#include <memory.h>
#include <complex>
#include <queue>
#include <stack>
#include <bitset>

using namespace std;

enum DesAction
{
	DES_ENCRYPT,
	DES_DECRYPT,
	
};

enum DESMod
{
	DES_ECB,
	DES_CBC,
	DES_CFB,
	DES_OFB,
	DES_CTR
};

// Standard DES Lookup Tables
static char des_LS[] =
{	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1};

static char des_LS1[] =
{	 2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25,
	26, 27, 28, 1,  0,  0,  0,  0};

static char des_LS2[] =
{	 3,  4,  5,  6,  7,  8,  9, 10,
	11, 12, 13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24, 25, 26,
	27, 28,  1,  2,  0,  0,  0,  0};

static char des_PC1[] =
{	57, 49, 41, 33, 25, 17,  9,  1,
	58, 50, 42, 34, 26, 18, 10,  2,
	59, 51, 43, 35, 27, 19, 11,  3,
	60, 52, 44, 36,  0,  0,  0,  0,
	63, 55, 47, 39, 31, 23, 15,  7,
	62, 54, 46, 38, 30, 22, 14,  6,
	61, 53, 45, 37, 29, 21, 13,  5,
	28, 20, 12,  4,  0,  0,  0,  0};

static char des_PC2[] =
{	14, 17, 11, 24,  1,  5,  3, 28,
	15,  6, 21, 10, 23, 19, 12,  4,
	26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40,
	51, 45, 33, 48, 44, 49, 39, 56,
	34, 53, 46, 42, 50, 36, 29, 32,	// 下面的和书上的不一样。
	45, 56, 35, 41, 51, 59, 34, 44,
	55, 49, 37, 52, 48, 53, 43, 60,
	38, 57, 50, 46, 54, 40, 33, 36};

static char des_IP[] =
{	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7};

static char des_IPi[] =
{	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25};

static char des_EP[] =
{	32,  1,  2,  3,  4,  5, 4,  5,
6,  7,  8,  9,  8,  9, 10, 11,
12, 13, 12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21, 20, 21,
22, 23, 24, 25, 24, 25, 26, 27,
28, 29,	28, 29, 30, 31, 32,  1};

static char des_S1[] =
{	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

static char des_S2[] =
{	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

static char des_S3[] =
{	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

static char des_S4[] =
{	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

static char des_S5[] =
{	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

static char des_S6[] =
{	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

static char des_S7[] =
{	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

static char des_S8[] =
{	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};

static char *des_S[] = {des_S1, des_S2, des_S3, des_S4, des_S5, des_S6, des_S7, des_S8};

static char des_sbox_P1[] = {0, 0,  1,  6,  2,  3,  4,  5};
static char des_sbox_P2[] = {0, 0,  7, 12,  8,  9, 10, 11};
static char des_sbox_P3[] = {0, 0, 13, 18, 14, 15, 16, 17};
static char des_sbox_P4[] = {0, 0, 19, 24, 20, 21, 22, 23};
static char des_sbox_P5[] = {0, 0, 25, 30, 26, 27, 28, 29};
static char des_sbox_P6[] = {0, 0, 31, 36, 32, 33, 34, 35};
static char des_sbox_P7[] = {0, 0, 37, 42, 38, 39, 40, 41};
static char des_sbox_P8[] = {0, 0, 43, 48, 44, 45, 46, 47};

static char *des_sbox_P[8] = {des_sbox_P1, des_sbox_P2, des_sbox_P3, des_sbox_P4, des_sbox_P5, des_sbox_P6, des_sbox_P7, des_sbox_P8};

static char des_P[] =
{	16,  7, 20, 21, 29, 12, 28, 17,
	 1, 15, 23, 26,  5, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25};

// Generalized Lookup Table Permuter (LSB = 1)
static void des_permute(char *out, const char *in, const char *table, const int width)
{
	int b, o, p;
	char mask;
	char v;

	int bytewidth = (width < 8 ? 1 : (width/8));

	if(bytewidth * 8 < width)
		bytewidth++;

	for(o = 0; o < bytewidth; o++)
		out[o] = 0;
	
	for(b = 0; b < width; b++)
	{
		if(table[b] == 0)
			continue;

		o = (table[b] - 1) / 8;
		
		mask = 1 << (7 - ((table[b] - 1) % 8));
		
		if(mask & in[o])
			v = 1;
		else
			v = 0;

		p = b / 8;

		out[p] = out[p] | (v << (7 - (b % 8)));
	}
}

// Scheduled Key Shift
static void des_key_shift(char *key, int round)
{
	char temp[4];

	if(des_LS[round] == 1)
	{
		des_permute(temp, key, des_LS1, 28);
		key[0] = temp[0];
		key[1] = temp[1];
		key[2] = temp[2];
		key[3] = temp[3];
		
		des_permute(temp, key+4, des_LS1, 28);
		key[4] = temp[0];
		key[5] = temp[1];
		key[6] = temp[2];
		key[7] = temp[3];
	} else {
		des_permute(temp, key, des_LS2, 28);
		key[0] = temp[0];
		key[1] = temp[1];
		key[2] = temp[2];
		key[3] = temp[3];
		
		des_permute(temp, key+4, des_LS2, 28);
		key[4] = temp[0];
		key[5] = temp[1];
		key[6] = temp[2];
		key[7] = temp[3];
	}
}

// S-Box Choice
static void des_sbox(char *out, const char *in)
{
	char s;
	char o[8];
	int n;

	for(n = 0; n < 8; n++)
	{
		des_permute(&s, in, des_sbox_P[n], 8);

		o[n] = des_S[n][s];
	}

	for(n = 0; n < 4; n++)
		out[n] = (o[2*n] << 4) | o[2*n+1];
}

// DES encrypt/decrypt a single block
void des_block(char *oblock, const char *block, const char *key, int action)
{
	char cur[8];
	char exp[6];
	char rkey[8];
	char ckey[6];
	char ckeys[16*6];
	char sbox[4];
	char per[4];
	char swap[8];
	int round;
	int n;

	// Generate all the round keys ahead of time
	des_permute(rkey, key, des_PC1, 64);
	for(round = 0; round < 16; round++)
	{
		des_key_shift(rkey, round);
		des_permute(ckey, rkey, des_PC2, 48);

		memcpy(ckeys+(6*round), ckey, 6);

		unsigned __int64 tmp = 0;
		memcpy( &tmp, ckey, 6 );
		bitset<48> bsKey( tmp );
		cout << "Key_" << round << ": " << bsKey << endl;
	}

	if(action == DES_DECRYPT)
	{
		// Rearrange the key order
		for(round = 0; round < 8; round++)
		{
			memcpy(ckey, ckeys+(6*round), 6);
			memcpy(ckeys+(6*round), ckeys+(6*(15 - round)), 6);
			memcpy(ckeys+(6*(15 - round)), ckey, 6);
		}
	}

	// IP
	des_permute(cur, block, des_IP, 64);

// 	std::bitset<32> bsLeft( *(int*)cur );
// 	cout << "ip left: " << bsLeft << endl;
// 	std::bitset<32> bsRight( *(int*)( cur+4 ) );
// 	cout << "ip right: " << bsRight << endl;


	// Rounds
	for(round = 0; round < 16; round++)
	{
		// E/P
		des_permute(exp, (cur+4), des_EP, 48);

// 		std::bitset<32> bsLeft( *(int*)cur );
// 		cout << "ip left: " << round << ": " << bsLeft << endl;
// 		std::bitset<48> bsRight( *(int*)( cur+4 ) );
// 		cout << "ip right: " << round << ": " << bsRight  << endl;

		// XOR
		for(n = 0; n < 6; n++)
			exp[n] = exp[n] ^ *(ckeys+(6*round)+n);

		//S-box choice
		des_sbox(sbox, exp);

		// P
		des_permute(per, sbox, des_P, 32);

		// XOR
		for(n = 0; n < 4; n++)
			per[n] = per[n] ^ cur[n];

		// Swap (Writeback)
		cur[0] = cur[4];
		cur[1] = cur[5];
		cur[2] = cur[6];
		cur[3] = cur[7];
		cur[4] = per[0];
		cur[5] = per[1];
		cur[6] = per[2];
		cur[7] = per[3];
	}

	// Final swap
	swap[0] = cur[4];
	swap[1] = cur[5];
	swap[2] = cur[6];
	swap[3] = cur[7];
	swap[4] = cur[0];
	swap[5] = cur[1];
	swap[6] = cur[2];
	swap[7] = cur[3];

	// IP^(-1)
	des_permute(oblock, swap, des_IPi, 64);
}

// DES encrypt/decrypt a stream (Padded Input)
// To pass IV, append to the key
void des(char *out, const char *in, size_t len, const char *key, int action, int mode)
{
	char block[8];
	char oblock[8];
	char feedback[8];
	size_t i,j;

	if(mode == DES_ECB)
	{
		// Traverse forwards by 8-byte blocks
		for(i = 0; i < len; i += 8)
		{
			for(j = 0; j < 8; j++)
				block[j] = in[i+j];

			des_block(oblock, block, key, action);

			for(j = 0; j < 8; j++)
				out[i+j] = oblock[j];
		}
	} else if(mode == DES_CBC)
	{
		memcpy(feedback, key+8, 8);

		// Traverse forwards by 8-byte blocks
		for(i = 0; i < len; i += 8)
		{
			if(action == DES_DECRYPT)
			{
				for(j = 0; j < 8; j++)
					block[j] = in[i+j];
			} else
			{
				for(j = 0; j < 8; j++)
					block[j] = in[i+j] ^ feedback[j];
			}

			des_block(oblock, block, key, action);

			if(action == DES_DECRYPT)
			{
				for(j = 0; j < 8; j++)
					out[i+j] = oblock[j] ^ feedback[j];

				memcpy(feedback, block, 8);
			} else
			{
				memcpy(feedback, oblock, 8);

				for(j = 0; j < 8; j++)
					out[i+j] = oblock[j];
			}
		}
	} else if(mode == DES_CFB)
	{
		memcpy(block, key+8, 8);

		// Traverse forwards byte by byte
		for(i = 0; i < len; i++)
		{
			// Encrypt shift register
			des_block(oblock, block, key, DES_ENCRYPT);

			// XOR with plaintext/ciphertext
			out[i] = in[i] ^ oblock[0];

			// Shift shift register
			memcpy(block, block+1, 7); 

			// Append ciphertext
			if(action == DES_DECRYPT)
				block[7] = in[i];
			else
				block[7] = out[i];
		}
	} else if(mode == DES_OFB)
	{
		memcpy(block, key+8, 8);

		// Traverse forwards byte by byte
		for(i = 0; i < len; i++)
		{
			// Encrypt shift register
			des_block(oblock, block, key, DES_ENCRYPT);

			// XOR with plaintext/ciphertext
			out[i] = in[i] ^ oblock[0];

			// Shift shift register
			memcpy(block, block+1, 7); 

			// Append Output
			block[7] = oblock[0];
		}
	} else if(mode == DES_CTR)
	{
		memcpy(block, key+8, 8);

		// Traverse forwards by 8-byte blocks
		for(i = 0; i < len; i += 8)
		{
			// Encrypt the counter
			des_block(oblock, block, key, DES_ENCRYPT);

			// XOR with plaintext/ciphertext
			for(j = 0; j < 8; j++)
				out[i+j] = oblock[j] ^ in[i+j];

			// Increment Counter (Rippled Adder)
			block[7]++;
			for(j = 6; j >= 0; j--)
				if(block[j+1] == 0)
					block[j]++;
				else
					break;
		}
	}	

}
