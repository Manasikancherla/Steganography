/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *  * Stego.c: A program for manipulating images                           *
 *   *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image.h"

void setlsbs(byte *p,byte b0)
{
  for(int i=0; i<8 ;i++) //writing 8 bits at a time
  {
	byte b = b0&0x1; //temporary byte holidng lsb of b0
	p[i] = (p[i]&0xFE)|b; //changing lsb of p[i]
	b0 >>=1;// shifting lsb of b0
  }

}

int main(int argc, char *argv[])
{  
  int i, j, k, cover_bits, bits, posCover, temp, posPayload, GNumber;
  struct Buffer b = {NULL, 0, 0};
  char c;
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  byte b0,*tempByte;
 
  if (argc != 4) 
    {
      printf("\n%s <cover_file> <stego_file> <file_to_hide> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
                               
  ReadBinaryFile(argv[3],&b);    // Read binary data
 
  tempByte = (byte *)malloc(b.size*sizeof(byte));

  // hidden information 
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    
  bits = (8 + b.size)*8;
  if (bits > cover_bits)
    {
      printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
      exit(1);
    }

  // embed four size bytes for the Buffer's size field
  //b.size needs to be inserted into cover
  posCover = 0; //position in Cover file
  temp = b.size; 

  for(i=0; i<4; i++)
  {
	tempByte[3-i] = temp % 16; //tempByte stores b.size in 4 parts each of 8 bits length
	temp = temp/16; //equivalent to shifting by 4 bits
  }

  for(i=0; i<4; i++)
  {
	setlsbs(&GetGray(posCover),tempByte[i]);   //setting the least significant bit values of the image file
	posCover = posCover + 8; //writing 8 bits at a time in setlsbs, hence moving 8 bytes forward
  } 
  
  // embed the eight digits of your G# using 4 bits per digit
  //G# needs to be inserted into cover       
  GNumber = 1127190; 
  //Similar to b.size procedure  
  for(i=0; i<8 ; i++)
  {         	
	tempByte[7-i] = GNumber % 16;
	GNumber = GNumber/16; 
  }         

  for(i=0; i<8 ; i++)
  {
	setlsbs(&GetGray(posCover),tempByte[i]);
	posCover = posCover + 8;
  }
  
  for(posPayload=0; posPayload < b.size; posPayload++)
  {
      // here you embed information into the image one byte at the time
   	 setlsbs(&GetGray(posCover),b.data[posPayload]);      	
	 posCover = posCover + 8;
	// note that you should change only the least significant bits of the image
  }
  

   WriteImage(argv[2],img);  // output stego file (cover_file + file_to_hide)

   free(tempByte);
   tempByte = NULL;

   free(b.data);
   b.data = NULL;
}
    
