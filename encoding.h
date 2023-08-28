/*
this header file implements the algorithm for 128-bit encryption 
*/
#include<iostream>
#include "lookup_table_encoding.h"
#include "key_expand.h"
using namespace std;

void encryptionLeft(unsigned char * templeft, unsigned char * extendedkeys1);
void encryptionLight(unsigned char * tempright, unsigned char * extendedkeys2);

void encryption(unsigned char *temp, unsigned char *extendedkeys1, unsigned char *extendedkeys2)
{
	unsigned char templeft[16];
	unsigned char tempright[16];

	for (int i = 0; i < 16; i++)
	{
		templeft[i] = temp[i];
		tempright[i] = temp[i + 16];
	}

	encryptionLeft(templeft, extendedkeys1);
	encryptionRight(tempright, extendedkeys2);

	for (int i = 0; i < 16; i++)
	{
		temp[i] = templeft[i];
		temp[i + 16] = tempright[i];
	}
}

void encryptionleft(unsigned char templeft[16], unsigned char * extendedkeys1 )
{	
	// unsigned char * temp = new unsigned char[16];
	// for(int i=0;i<16;i++)
	// {
	// 	temp[i]=templeft[i];
	// }

	int kp=0;
	for(int i=0;i<16;i++)
	{
		templeft[i]^=extendedkeys1[i];
	}
		
	kp++;
	while(kp<11)
	{
	      //substitution bits
	      for(int i=0;i<16;i++)
	      {
	          templeft[i]=sbox[templeft[i]];
	      }
	      //shift row
	      unsigned char * temp2 = new unsigned char[16];
		  for(int i=0;i<16;i++){
			temp2[i]=templeft[i];
		  }
	      //1st column
	       templeft[0]=temp2[0];
	       templeft[4]=temp2[4];
	       templeft[8]=temp2[8];
	       templeft[12]=temp2[12];
	       //2nd column
	       templeft[1]=temp2[5];
	       templeft[5]=temp2[9];
	       templeft[9]=temp2[13];
	       templeft[13]=temp2[1];
	       //3rd column
	       templeft[2]=temp2[10];
	       templeft[6]=temp2[14];
	       templeft[10]=temp2[2];
	       templeft[14]=temp2[6];
	       //4th column
	       templeft[3]=temp2[15];
	       templeft[7]=temp2[3];
	       templeft[11]=temp2[7];
	       templeft[15]=temp2[11];

	       //MIX column
	       if(kp<10)
	       {
	       	    for (int i = 0; i < 16; i++) {
		             temp2[i] = templeft[i];
	              }
	       	//1st row
	            templeft[0] = (unsigned char) lookup2[temp2[0]] ^ lookup3[temp2[1]] ^ temp2[2] ^ temp2[3];
	            templeft[1] = (unsigned char) temp2[0] ^ lookup2[temp2[1]] ^ lookup3[temp2[2]] ^ temp2[3];
	            templeft[2] = (unsigned char) temp2[0] ^ temp2[1] ^ lookup2[temp2[2]] ^ lookup3[temp2[3]];
	            templeft[3] = (unsigned char) lookup3[temp2[0]] ^ temp2[1] ^ temp2[2] ^ lookup2[temp2[3]];
            //2nd row
	            templeft[4] = (unsigned char)lookup2[temp2[4]] ^ lookup3[temp2[5]] ^ temp2[6] ^ temp2[7];
	            templeft[5] = (unsigned char)temp2[4] ^ lookup2[temp2[5]] ^ lookup3[temp2[6]] ^ temp2[7];
	            templeft[6] = (unsigned char)temp2[4] ^ temp2[5] ^ lookup2[temp2[6]] ^ lookup3[temp2[7]];
	            templeft[7] = (unsigned char)lookup3[temp2[4]] ^ temp2[5] ^ temp2[6] ^ lookup2[temp2[7]];
            //3rd row
	            templeft[8] = (unsigned char)lookup2[temp2[8]] ^ lookup3[temp2[9]] ^ temp2[10] ^ temp2[11];
	            templeft[9] = (unsigned char)temp2[8] ^ lookup2[temp2[9]] ^ lookup3[temp2[10]] ^ temp2[11];
	            templeft[10] = (unsigned char)temp2[8] ^ temp2[9] ^ lookup2[temp2[10]] ^ lookup3[temp2[11]];
	            templeft[11] = (unsigned char)lookup3[temp2[8]] ^ temp2[9] ^ temp2[10] ^ lookup2[temp2[11]];
            //4th row
	            templeft[12] = (unsigned char)lookup2[temp2[12]] ^ lookup3[temp2[13]] ^ temp2[14] ^ temp2[15];
	            templeft[13] = (unsigned char)temp2[12] ^ lookup2[temp2[13]] ^ lookup3[temp2[14]] ^ temp2[15];
	            templeft[14] = (unsigned char)temp2[12] ^ temp2[13] ^ lookup2[temp2[14]] ^ lookup3[temp2[15]];
	            templeft[15] = (unsigned char)lookup3[temp2[12]] ^ temp2[13] ^ temp2[14] ^ lookup2[temp2[15]];
		   }

		   //Add Round Key
		    for(int i=0;i<16;i++)
	        {
	          templeft[i]^=extendedkeys1[kp*16+i];
	        }
			delete[] temp2;
	        kp++;
			// for(int i=0;i<16;i++)
			// {
			// 	templeft[i]=temp[i];
			// 	}
	}
}

void encryptionright(unsigned char tempright[16], unsigned char * extendedkeys2 )
{	
	// unsigned char * temp = new unsigned char[16];
	// for(int i=0;i<16;i++)
	// {
	// 	temp[i]=tempright[i];
	// }
	int kp=0;
	for(int i=0;i<16;i++)
	{
		tempright[i]^=extendedkeys2[i];
	}
	kp++;
	while(kp<11)
	{
	      //substitution bits
	      for(int i=0;i<16;i++)
	      {
	          tempright[i]=sbox[tempright[i]];
	      }
	      //shift row
	      unsigned char * temp2 = new unsigned char[16];
	       for(int i=0;i<16;i++){
	       temp2[i]=tempright[i];
		   }
	      //1st column
	       tempright[0]=temp2[0];
	       tempright[4]=temp2[4];
	       tempright[8]=temp2[8];
	       tempright[12]=temp2[12];
	       //2nd column
	       tempright[1]=temp2[5];
	       tempright[5]=temp2[9];
	       tempright[9]=temp2[13];
	       tempright[13]=temp2[1];
	       //3rd column
	       tempright[2]=temp2[10];
	       tempright[6]=temp2[14];
	       tempright[10]=temp2[2];
	       tempright[14]=temp2[6];
	       //4th column
	       tempright[3]=temp2[15];
	       tempright[7]=temp2[3];
	       tempright[11]=temp2[7];
	       tempright[15]=temp2[11];

	       //MIX column
	       if(kp<10)
	       {
	       	    for (int i = 0; i < 16; i++) {
		             temp2[i] = tempright[i];
	              }
	       	//1st row
	            tempright[0] = (unsigned char) lookup2[temp2[0]] ^ lookup3[temp2[1]] ^ temp2[2] ^ temp2[3];
	            tempright[1] = (unsigned char) temp2[0] ^ lookup2[temp2[1]] ^ lookup3[temp2[2]] ^ temp2[3];
	            tempright[2] = (unsigned char) temp2[0] ^ temp2[1] ^ lookup2[temp2[2]] ^ lookup3[temp2[3]];
	            tempright[3] = (unsigned char) lookup3[temp2[0]] ^ temp2[1] ^ temp2[2] ^ lookup2[temp2[3]];
            //2nd row
	            tempright[4] = (unsigned char)lookup2[temp2[4]] ^ lookup3[temp2[5]] ^ temp2[6] ^ temp2[7];
	            tempright[5] = (unsigned char)temp2[4] ^ lookup2[temp2[5]] ^ lookup3[temp2[6]] ^ temp2[7];
	            tempright[6] = (unsigned char)temp2[4] ^ temp2[5] ^ lookup2[temp2[6]] ^ lookup3[temp2[7]];
	            tempright[7] = (unsigned char)lookup3[temp2[4]] ^ temp2[5] ^ temp2[6] ^ lookup2[temp2[7]];
            //3rd row
	            tempright[8] = (unsigned char)lookup2[temp2[8]] ^ lookup3[temp2[9]] ^ temp2[10] ^ temp2[11];
	            tempright[9] = (unsigned char)temp2[8] ^ lookup2[temp2[9]] ^ lookup3[temp2[10]] ^ temp2[11];
	            tempright[10] = (unsigned char)temp2[8] ^ temp2[9] ^ lookup2[temp2[10]] ^ lookup3[temp2[11]];
	            tempright[11] = (unsigned char)lookup3[temp2[8]] ^ temp2[9] ^ temp2[10] ^ lookup2[temp2[11]];
            //4th row
	            tempright[12] = (unsigned char)lookup2[temp2[12]] ^ lookup3[temp2[13]] ^ temp2[14] ^ temp2[15];
	            tempright[13] = (unsigned char)temp2[12] ^ lookup2[temp2[13]] ^ lookup3[temp2[14]] ^ temp2[15];
	            tempright[14] = (unsigned char)temp2[12] ^ temp2[13] ^ lookup2[temp2[14]] ^ lookup3[temp2[15]];
	            tempright[15] = (unsigned char)lookup3[temp2[12]] ^ temp2[13] ^ temp2[14] ^ lookup2[temp2[15]];
		   }

		   //Add Round Key
		    for(int i=0;i<16;i++)
	        {
	          tempright[i]^=extendedkeys2[kp*16+i];
	        }
			delete[] temp2;
	        kp++;
			// for(int i=0;i<16;i++)
			// {
			// 	tempright[i]=temp[i];
			// }
	}
}