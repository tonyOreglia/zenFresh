OBJS = game.o move.o position.o testing-classes.o bitboard-utilities.o generate-bitboard-lookup-tables.o
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

testing-classes : $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) -o zen

position.o : position.h position.cpp move.h ./bitboard_utilities/generate-bitboard-lookup-tables.h
	$(CC) $(CFLAGS) position.cpp

move.o : move.h move.cpp ./bitboard_utilities/bitboard-utilities.h
	$(CC) $(CFLAGS) move.cpp

game.o : game.h game.cpp move.h position.h ./bitboard_utilities/bitboard-utilities.h ./bitboard_utilities/generate-bitboard-lookup-tables.h
	$(CC) $(CFLAGS) game.cpp

testing-classes.o : position.h move.h ./bitboard_utilities/bitboard-utilities.h
	$(CC) $(CFLAGS) testing-classes.cpp

bitboard-utilities.o :
	$(CC) $(CFLAGS) ./bitboard_utilities/bitboard-utilities.cpp

generate-bitboard-lookup-tables.o :
	$(CC) $(CFLAGS) ./bitboard_utilities/generate-bitboard-lookup-tables.cpp

clean:
	\rm *.o *~ zen

tar:
	tar cfv testing-classes.tar position.h position.cpp move.h testing-classes.cpp bitboard_utilities/generate-bitboard-lookup-tables.h bitboard_utilities/generate-bitboard-lookup-tables.cpp game.h game.cpp bitboard_utilities/bitboard-utilities.cpp
