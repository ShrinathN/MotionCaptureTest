CC = g++
LDLIBS = -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CFLAGS = -ggdb -I. -I/home/shinu/Desktop/OpenGL_Test/glad/include/

SRC = renderer.cpp \
network.cpp \
stb_image.cpp \
glad/src/glad.c

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDLIBS) -o renderer

clean:
	rm renderer
