all: Stego StegoExtract

image.o: image.h
	gcc -c -g image.c

Stego.o: image.o 
	gcc -c -g Stego.c

StegoExtract.o: image.o
	gcc -c -g StegoExtract.c -lm

Stego: Stego.o 
	gcc -g -o Stego Stego.o image.o 

StegoExtract: StegoExtract.o
	gcc -g -o StegoExtract StegoExtract.o image.o -lm

clean:
	rm -f *.o

cleanall:
	rm -f *.o
	rm -f Stego
	rm -f StegoExtract
	rm -f sampleoutput*.txt

