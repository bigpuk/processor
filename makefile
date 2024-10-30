
DED_FLAGS=-Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal 	\
-Wformat-security -Wformat=2 -Wignored-qualifiers -Winit-self -Winline -Wlogical-op -Wmain -Wmissing-declarations 		\
-Wno-missing-field-initializers -Wmissing-include-dirs -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith 			\
-Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wswitch-default	\
-Wswitch-enum -Wtype-limits -Wundef -Wunreachable-code -Wwrite-strings -fexceptions -g -pipe -D_DEBUG 					\
-D_EJUDGE_CLIENT_SIDE -D_EJC

FILES=main.cpp assembler.cpp processor.cpp disassembler.cpp proc_dump.cpp
FILE_OBJECTS=$(addprefix build/, $(patsubst %.cpp, %.o, $(FILES)))

EXE_NAME=PROCESSOR

# TODO read about
# addprefix
# patsubst
# wildcard * ->  
# -l -L, стат. библиотека

#TODO разбить на 3 проги

all: prepare $(EXE_NAME)

prepare: 
	cd ../stack && make lib

$(EXE_NAME): $(FILE_OBJECTS)
	g++ $^ $(DED_FLAGS) -L ../stack -lmystack -o $(EXE_NAME)

build/%.o: src/%.cpp
	g++ -c $^ -I./include -I../../stack/include/INCLUDE.h -o $@

clean:
	rm -rf build/* *.exe