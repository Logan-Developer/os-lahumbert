SRCPATH=src
BUILDPATH=build
EXE=game.exe
HEADERS=$(addprefix ${SRCPATH}/, Game.hpp Sprite.hpp GameBridge.hpp Physics.hpp Collision.hpp Trigger.hpp)
OBJECTS=$(addprefix ${SRCPATH}/, Collision.hpp Key.hpp Player.hpp Ground.hpp)
SRC=${addprefix ${SRCPATH}/, Main.cpp}

ifeq ($(OS), Windows_NT)
	CFLAGS=-g -IC:\msys64_2\mingw64\include\SDL2 -DSDL_MAIN_HANDLED -IC:\msys64_2\mingw64\include\SDL2_mixer -IC:\msys64_2\mingw64\include\SDL2_image -D_REENTRANT -DNODEBUG
	LIBS=-LC:\msys64_2\mingw64\lib  -lSDL2.dll -lSDL2_mixer -lSDL2_image
else
	CFLAGS=-g -I/usr/include/SDL2 -D_REENTRANT -DNODEBUG
	LIBS=-lSDL2 -lSDL2_image -lSDL2_mixer
endif

${BUILDPATH}/${EXE}: ${SRC} ${HEADERS} ${OBJECTS}
	g++ ${CFLAGS} ${SRC} -o $@ ${LIBS}

clean:
	rm ${BUILDPATH}/${EXE}

run: ${BUILDPATH}/${EXE}
	./${BUILDPATH}/${EXE}

deploy.tgz: ${BUILDPATH}/${EXE}
	tar -czf deploy.tgz ${BUILDPATH}/${EXE} gfx/*

deploy.AppImage: deploy.tgz pkzip.sfx config.txt
	cp pkzip.sfx + config.txt + deploy.tgz deploy.AppImage

valgrind:
	valgrind --error-limit=no ./${BUILDPATH}/${EXE}
