STD :=c++11
CPPFLAGS= -std=$(STD) -O3 -pthread -Wall -g -D_DEBUG
TARGET=teste

OBJECTS=PG.o Solucao.o Instancia.o Djasa.o SA.o Grasp.o dummy_PR.o BateriaDeTestes.o TimeSeries.o CommonDebug.o

all: ${TARGET}

%.o: %.cpp
	g++ -c $< -std=c++11 -D_threading_now -g -D_DEBUG

${TARGET}: ${OBJECTS}
	g++ -o ${TARGET} ${OBJECTS} ${CPPFLAGS}

clean:
	rm -f ${TARGET} ${OBJECTS}
	rm *~ -rf -r
