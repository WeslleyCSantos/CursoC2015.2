SRC=src
INC=inc
OBJ=obj
DIST=dist
EXE=questao4.exe
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
GCCARGS=-Wall
