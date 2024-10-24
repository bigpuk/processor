
DED_FLAGS=-Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations -Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC
FILE_OBJECTS=main.o assembler.o processor.o
EXE_NAME=processor

# TODO read about
# addprefix
# patsubst
# wildcard * ->  

all: processor

processor: $(FILE_OBJECTS)
	@echo "[LD] Linking"
	@g++ $(DED_FLAGS) $(FILE_OBJECTS) -o $(EXE_NAME)

main.o: main.cpp
	@echo "[CXX] main"
	@g++ $(DED_FLAGS) -c main.cpp -o main.o

assembler.o: assembler.cpp
	@echo "[CXX] assembler"
	@g++ $(DED_FLAGS) -c assembler.cpp -o assembler.o

processor.o: processor.cpp
	@echo "[CXX] processor"
	@g++ $(DED_FLAGS) -c processor.cpp -o processor.o

clean:
	rm -rf *.o assembler $(EXE_NAME).exe