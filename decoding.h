/*
this header file implements the algorithm for 128-bit decryption 
*/
#include<iostream>
#include "lookup_table_decoding.h"
#include "key_expand.h"
using namespace std;


void decryptionleft(unsigned char *templeft, unsigned char *extendedkeys1);
void decryptionright(unsigned char *tempright, unsigned char *extendedkeys2);

void decryption(unsigned char *temp, unsigned char *extendedkeys1, unsigned char *extendedkeys2)
{
	unsigned char templeft[16];
	unsigned char tempright[16];

	for (int i = 0; i < 16; i++)
	{
		templeft[i] = temp[i];
		tempright[i] = temp[i + 16];
	}

	decryptionleft(templeft, extendedkeys1);
	decryptionright(tempright, extendedkeys2);

	for (int i = 0; i < 16; i++)
	{
		temp[i] = templeft[i];
		temp[i + 16] = tempright[i];
	}
}

void decryptionleft(unsigned char templeft[16], unsigned char *extendedkeys1)

{
	// unsigned char * temp = new unsigned char[16];

	// for (int i = 0; i < 16; i++)
	// {
	// 	temp[i] = templeft[i];
	// }  made temprigt to add keyextended1

	int kp=10;
	while(kp>0)
	{
		//subtract round key
		for(int i=0;i<16;i++)
		{
			templeft[i]^=extendedkeys1[(kp*16)+i];
		}

		//inverse mix column step
        if(kp<10){
	     unsigned char temp2[16];
	     	for (int i = 0; i < 16; i++)
			 {
		     temp2[i] = templeft[i];
			 }

	    templeft[0] = (unsigned char)lookup14[temp2[0]] ^ lookup11[temp2[1]] ^ lookup13[temp2[2]] ^ lookup9[temp2[3]];
	    templeft[1] = (unsigned char)lookup9[temp2[0]] ^ lookup14[temp2[1]] ^ lookup11[temp2[2]] ^ lookup13[temp2[3]];
	    templeft[2] = (unsigned char)lookup13[temp2[0]] ^ lookup9[temp2[1]] ^ lookup14[temp2[2]] ^ lookup11[temp2[3]];
	    templeft[3] = (unsigned char)lookup11[temp2[0]] ^ lookup13[temp2[1]] ^ lookup9[temp2[2]] ^ lookup14[temp2[3]];

	    templeft[4] = (unsigned char)lookup14[temp2[4]] ^ lookup11[temp2[5]] ^ lookup13[temp2[6]] ^ lookup9[temp2[7]];
	    templeft[5] = (unsigned char)lookup9[temp2[4]] ^ lookup14[temp2[5]] ^ lookup11[temp2[6]] ^ lookup13[temp2[7]];
	    templeft[6] = (unsigned char)lookup13[temp2[4]] ^ lookup9[temp2[5]] ^ lookup14[temp2[6]] ^ lookup11[temp2[7]];
	    templeft[7] = (unsigned char)lookup11[temp2[4]] ^ lookup13[temp2[5]] ^ lookup9[temp2[6]] ^ lookup14[temp2[7]];

	    templeft[8] = (unsigned char)lookup14[temp2[8]] ^ lookup11[temp2[9]] ^ lookup13[temp2[10]] ^ lookup9[temp2[11]];
	    templeft[9] = (unsigned char)lookup9[temp2[8]] ^ lookup14[temp2[9]] ^ lookup11[temp2[10]] ^ lookup13[temp2[11]];
	    templeft[10] = (unsigned char)lookup13[temp2[8]] ^ lookup9[temp2[9]] ^ lookup14[temp2[10]] ^ lookup11[temp2[11]];
	    templeft[11] = (unsigned char)lookup11[temp2[8]] ^ lookup13[temp2[9]] ^ lookup9[temp2[10]] ^ lookup14[temp2[11]];

	    templeft[12] = (unsigned char)lookup14[temp2[12]] ^ lookup11[temp2[13]] ^ lookup13[temp2[14]] ^ lookup9[temp2[15]];
	    templeft[13] = (unsigned char)lookup9[temp2[12]] ^ lookup14[temp2[13]] ^lookup11[temp2[14]] ^ lookup13[temp2[15]];
	    templeft[14] = (unsigned char)lookup13[temp2[12]] ^ lookup9[temp2[13]] ^ lookup14[temp2[14]] ^ lookup11[temp2[15]];
	    templeft[15] = (unsigned char)lookup11[temp2[12]] ^ lookup13[temp2[13]] ^ lookup9[temp2[14]] ^ lookup14[temp2[15]];
        }
		// Shifts rows right
     	unsigned char temp21[16];
     	for (int i = 0; i < 16; i++)
		 {
		temp21[i] = templeft[i];
	     }
	     //column one
	    templeft[0] = temp21[0];
	    templeft[4] = temp21[4];
	    templeft[8] = temp21[8];
	    templeft[12] = temp21[12];
         //column two
        templeft[1] = temp21[13];
	    templeft[5] = temp21[1];
	    templeft[9] = temp21[5];
	    templeft[13] = temp21[9];
         //column three
	    templeft[2] = temp21[10];
	    templeft[6] = temp21[14];
	    templeft[10] = temp21[2];
	    templeft[14] = temp21[6];
	    //column four
	    templeft[3] = temp21[7];
	    templeft[7] = temp21[11];
	    templeft[11] = temp21[15];
	    templeft[15] = temp21[3];

	    //substitution bits
	    for(int i=0;i<16;i++)
	     {
	        templeft[i]=in_sbox[templeft[i]];
	     }
	     kp--;
    }

	  	//subtract round key
		for(int i=0;i<16;i++)
		{
			templeft[i]^=extendedkeys1[i];
		}
		// for (int i = 0; i < 16; i++)
		// {
		// 	templeft[i] = temp[i];
		// }

}

void decryptionright(unsigned char tempright[16], unsigned char * extendedkeys2)
{	
	// unsigned char * temp = new unsigned char[16];

	// for (int i = 0; i < 16; i++)
	// {
	// 	temp[i] = tempright[i];
	// } made temprigt to add keyextended2

	int kp=10;
	while(kp>0)
	{
		//subtract round key
		for(int i=0;i<16;i++)
		{
			tempright[i]^=extendedkeys2[(kp*16)+i];
		}

		//inverse mix column step
        if(kp<10){
	     unsigned char temp2[16];
	     	for (int i = 0; i < 16; i++)
			 {
		     temp2[i] = tempright[i];
			 }

	    tempright[0] = (unsigned char)lookup14[temp2[0]] ^ lookup11[temp2[1]] ^ lookup13[temp2[2]] ^ lookup9[temp2[3]];
	    tempright[1] = (unsigned char)lookup9[temp2[0]] ^ lookup14[temp2[1]] ^ lookup11[temp2[2]] ^ lookup13[temp2[3]];
	    tempright[2] = (unsigned char)lookup13[temp2[0]] ^ lookup9[temp2[1]] ^ lookup14[temp2[2]] ^ lookup11[temp2[3]];
	    tempright[3] = (unsigned char)lookup11[temp2[0]] ^ lookup13[temp2[1]] ^ lookup9[temp2[2]] ^ lookup14[temp2[3]];

	    tempright[4] = (unsigned char)lookup14[temp2[4]] ^ lookup11[temp2[5]] ^ lookup13[temp2[6]] ^ lookup9[temp2[7]];
	    tempright[5] = (unsigned char)lookup9[temp2[4]] ^ lookup14[temp2[5]] ^ lookup11[temp2[6]] ^ lookup13[temp2[7]];
	    tempright[6] = (unsigned char)lookup13[temp2[4]] ^ lookup9[temp2[5]] ^ lookup14[temp2[6]] ^ lookup11[temp2[7]];
	    tempright[7] = (unsigned char)lookup11[temp2[4]] ^ lookup13[temp2[5]] ^ lookup9[temp2[6]] ^ lookup14[temp2[7]];

	    tempright[8] = (unsigned char)lookup14[temp2[8]] ^ lookup11[temp2[9]] ^ lookup13[temp2[10]] ^ lookup9[temp2[11]];
	    tempright[9] = (unsigned char)lookup9[temp2[8]] ^ lookup14[temp2[9]] ^ lookup11[temp2[10]] ^ lookup13[temp2[11]];
	    tempright[10] = (unsigned char)lookup13[temp2[8]] ^ lookup9[temp2[9]] ^ lookup14[temp2[10]] ^ lookup11[temp2[11]];
	    tempright[11] = (unsigned char)lookup11[temp2[8]] ^ lookup13[temp2[9]] ^ lookup9[temp2[10]] ^ lookup14[temp2[11]];

	    tempright[12] = (unsigned char)lookup14[temp2[12]] ^ lookup11[temp2[13]] ^ lookup13[temp2[14]] ^ lookup9[temp2[15]];
	    tempright[13] = (unsigned char)lookup9[temp2[12]] ^ lookup14[temp2[13]] ^lookup11[temp2[14]] ^ lookup13[temp2[15]];
	    tempright[14] = (unsigned char)lookup13[temp2[12]] ^ lookup9[temp2[13]] ^ lookup14[temp2[14]] ^ lookup11[temp2[15]];
	    tempright[15] = (unsigned char)lookup11[temp2[12]] ^ lookup13[temp2[13]] ^ lookup9[temp2[14]] ^ lookup14[temp2[15]];
        }
		// Shifts rows right
     	unsigned char temp21[16];
     	for (int i = 0; i < 16; i++)
		 {
		temp21[i] = tempright[i];
	     }
	     //column one
	    tempright[0] = temp21[0];
	    tempright[4] = temp21[4];
	    tempright[8] = temp21[8];
	    tempright[12] = temp21[12];
         //column two
        tempright[1] = temp21[13];
	    tempright[5] = temp21[1];
	    tempright[9] = temp21[5];
	    tempright[13] = temp21[9];
         //column three
	    tempright[2] = temp21[10];
	    tempright[6] = temp21[14];
	    tempright[10] = temp21[2];
	    tempright[14] = temp21[6];
	    //column four
	    tempright[3] = temp21[7];
	    tempright[7] = temp21[11];
	    tempright[11] = temp21[15];
	    tempright[15] = temp21[3];

	    //substitution bits
	    for(int i=0;i<16;i++)
	     {
	        tempright[i]=in_sbox[tempright[i]];
	     }
	     kp--;
       }

	  	//subtract round key
		for(int i=0;i<16;i++)
		{
			tempright[i]^=extendedkeys2[i];
		}

		// for (int i = 0; i < 16; i++)
		// {
		// 	tempright[i] = temp[i];
		// }

}

