TARGET = outfile

SRC = sort_fptr.c

$(TARGET):sort_fptr.o
	cc sort_fptr.o -o $(TARGET)

sort_fptr.o:$(SRC)
	cc -c $(SRC)

clean:
	rm $(TARGET)
