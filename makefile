CC=g++
CFLAGS=-g -Wall
OBJS=main.o LPPL.o
TARGET=yLPPL.out

VCPKG_LIB=/home/tahn/vcpkg/installed/x64-linux/lib

LDFLAGS = -L$(VCPKG_LIB) -ljsoncpp

all: $(TARGET)

clean:
	rm -f *.o
	rm -f */*.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -g -o  $@ $(OBJS) $(LDFLAGS)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) $(CFLAGS) -g -c -O0 -o $@ $<
