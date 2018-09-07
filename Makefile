CC := clang++
STD :=c++11
CPPFLAGS= -std=$(STD) -O3 -pthread -Wall
TARGET=teste

# Change the line below
OBJECTS=PG.o Solucao.o Instancia.o Djasa.o SA.o Grasp.o dummy_PR.o BateriaDeTestes.o TimeSeries.o CommonDebug.o

all: ${TARGET}

%.o: %.cpp
	${CC} -c $< -std=c++11 -D_threading_now -O3

${TARGET}: ${OBJECTS}
	${CC} -o ${TARGET} ${OBJECTS} ${CPPFLAGS}

clean:
	rm -f ${TARGET} ${OBJECTS}
	rm *~ -rf -r