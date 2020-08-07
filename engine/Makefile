CC = g++
LDLIBS = -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CFLAGS = -ggdb -I. -I/home/shinu/Desktop/OpenGL_Test/glad/include/

SRC = test.cpp \
engine.cpp \
object.cpp \
shader.cpp \
shader_program.cpp \
stb_image.cpp \
glad/src/glad.c

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDLIBS) -o test

clean:
	rm test
