SRC_DIR		 	:= ./sources
OBJ_DIR         := ./bin/obj
TARGET          := ./bin/main.exe
CPP_FILES 		:= $(wildcard $(SRC_DIR)/*.cpp)
O_FILES         := $(CPP_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
INC_DIR  		:= -Iinclude -DSFML_STATIC
LIB_DIR     	:= -Llib -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -mwindows -lsfml-main -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -DSFML_STATIC

.PHONY: all run clean up
all: $(TARGET) run

$(TARGET): $(O_FILES)
	g++ $(^:%="%") -o "$@" $(LIB_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -c "$^" -o "$@" $(INC_DIR) 

run:
	$(TARGET)

clean:
	del "bin\obj\*.o"
	del "bin\main.exe"

up: clean all