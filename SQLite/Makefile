CC=g++
CPPFLAGS=-I.
DEPS=*.h
LDFLAGS=-L../sqlite
LIBS=-lsqlite3 -lpthread -ldl
OBJ = main.o SQLiteStatement.o 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LDFLAGS) $(LIBS)