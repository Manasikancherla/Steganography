/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * StegoExtract.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include <math.h>

byte getlsbs(byte *p)
{
	byte b0 = 0x0,b;
	
	for(int i=0;i<8;i++)
	{
		b = p[i]&0x1; //extracting lsb of p[i] for 8 bytes
		b0 = b0 | (b<<i); 
	}
	return b0; //returns lsbs of 8 consecutive bytes
}

int main(int argc, char *argv[])
{  
  int i, j, k, temp, cover_bits, bits, posPayload = 0,GNumber,posCover=0;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  byte b0,*tempByte;
   
  tempByte = (byte *)malloc(sizeof(byte)*10);
  if (argc != 3) 
    {
      printf("\n%s <stego_file> <file_to_extract> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros

  // hidden information 
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    

  b.size = 0;
  // extract four size bytes for the Buffer's size field
  // Set this to b.size
  for(i=0; i<12; i++) //Extracting b.size and GNumber at once
  {
	tempByte[i] = getlsbs(&GetGray(posCover));
	posCover = posCover + 8;
  }

  temp = pow(16,3); //using temp to convert hexadecimal to decimal
  for(i=0;i<4;i++)
  {
	b.size += temp*tempByte[i];
	temp = temp/16;
  }

  printf("\nb.size = %d\n",b.size);
  
  b.data = malloc(b.size); // Allocates room for the output data file
 	
 // extract the eight digits of your G# using 4 bits per digit
   temp = pow(16,7);
   GNumber = 0;
   for(i=4;i<12;i++)
   { 
	GNumber += temp*tempByte[i];
	temp /= 16;
    }

    printf("\nGNumber is: 0%d\n",GNumber);
    
    for (posPayload=0; posPayload < b.size; posPayload++)
    {
      // here you extract information from the image one byte at the time
         SetByte(posPayload,getlsbs(&GetGray(posCover)));
	 posCover = posCover + 8;
      // note that you should extract only the least significant bits of the image
    }

   WriteBinaryFile(argv[2],b);  // output payload file

   free(tempByte);
   free(b.data);
 
   tempByte = b.data = NULL;
}
