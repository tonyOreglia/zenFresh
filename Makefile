OBJS = game.o move.o position.o testing-classes.o bitboard-utilities.o generate-bitboard-lookup-tables.o
TEST = catch2-unit-testing.o game.o move.o position.o bitboard-utilities.o generate-bitboard-lookup-tables.o
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)
ISO = -std=c++11

all: testing-classes catch2-unit-testing

testing-classes : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(ISO) -o zen

catch2-unit-testing : $(TEST)
	$(CC) $(LFLAGS) $(TEST) $(ISO) -o test

catch2-unit-testing.o : catch2-unit-testing.cpp position.h game.h move.h bitboard_utilities/bitboard-utilities.h bitboard_utilities/generate-bitboard-lookup-tables.h
	$(CC) $(CFLAGS) $(ISO) catch2-unit-testing.cpp

position.o : position.h position.cpp move.h ./bitboard_utilities/generate-bitboard-lookup-tables.h
	$(CC) $(CFLAGS) $(ISO) position.cpp

move.o : move.h move.cpp ./bitboard_utilities/bitboard-utilities.h
	$(CC) $(CFLAGS) $(ISO) move.cpp

game.o : game.h game.cpp move.h position.h ./bitboard_utilities/bitboard-utilities.h ./bitboard_utilities/generate-bitboard-lookup-tables.h
	$(CC) $(CFLAGS) $(ISO) game.cpp

testing-classes.o : testing-classes.cpp position.h move.h ./bitboard_utilities/bitboard-utilities.h
	$(CC) $(CFLAGS) $(ISO) testing-classes.cpp

bitboard-utilities.o :
	$(CC) $(CFLAGS) $(ISO) ./bitboard_utilities/bitboard-utilities.cpp

generate-bitboard-lookup-tables.o :
	$(CC) $(CFLAGS) $(ISO) ./bitboard_utilities/generate-bitboard-lookup-tables.cpp

clean:
	\rm *.o *~ zen a.out

tar:
	tar cfv testing-classes.tar position.h position.cpp move.h testing-classes.cpp bitboard_utilities/generate-bitboard-lookup-tables.h bitboard_utilities/generate-bitboard-lookup-tables.cpp game.h game.cpp bitboard_utilities/bitboard-utilities.cpp
