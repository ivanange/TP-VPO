CFLAGS= -g3 -c -Wall
CC= gcc
SRC = src
LIB = lib
OBJ = obj
BIN = bin
OBJECTS=$(OBJ)/utils.o
HEADERS= $(SRC)/utils.h

all : app test

$(OBJ)/utils.o: $(SRC)/utils.c $(HEADERS)
	$(CC) $(CFLAGS) $(SRC)/utils.c  -o $(OBJ)/utils.o

$(OBJ)/app.o: $(SRC)/app.c $(HEADERS)
	$(CC) $(CFLAGS) $(SRC)/app.c  -o $(OBJ)/app.o

$(OBJ)/test.o: $(SRC)/test.c $(HEADERS)
	$(CC) $(CFLAGS) $(SRC)/test.c -o $(OBJ)/test.o

app: ${OBJECTS} $(OBJ)/app.o $(HEADERS)
	$(CC) -o $(BIN)/app.exe ${OBJECTS} $(OBJ)/app.o

test: ${OBJECTS} $(OBJ)/test.o $(HEADERS)
	$(CC) -o $(BIN)/test.exe ${OBJECTS} $(OBJ)/test.o

clean: 
	rm -rf $(OBJ)/*.o
	rm -rf $(BIN)/*.exe

empty: 
	del /F /Q $(OBJ)\*.o
	del /F /Q $(BIN)\*.exe

run-test: test
	./$(BIN)/test.exe

run-app: app
	./$(BIN)/app.exe