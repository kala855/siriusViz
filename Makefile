LDLIBS=-lm -lglut -lGLEW -lGL
all: graph
clean:
	rm -f *.o graph
graph: ./common/shader_utils.o ./utilities/utilidades.o
.PHONY: all clean
