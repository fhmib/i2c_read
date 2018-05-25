CC = arm-xilinx-linux-gnueabi-gcc

i2c_1761: main.o
	$(CC) -o i2c_1761 main.o

main.o: main.c
	$(CC) -c main.c

clean:
	rm -rf *.o i2c_1761
