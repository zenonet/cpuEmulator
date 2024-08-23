

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>


char* program;
size_t size;
char registers[16];
#define instructionPtr (registers[0])

int main(){
    std::ifstream binStream("first.bin");

    binStream.seekg(0, std::ios::end);
    size = binStream.tellg();
    program = (char*)malloc(size);
    binStream.seekg(0, std::ios::beg);
    binStream.read(program, size);

    std::cout << "Loaded program of size " << size << '\n';


    while(instructionPtr < size/2){
        char instruction = (program[instructionPtr*2]) >> 4;
        std::cout << "Executing instruction " << (int)instruction << '\n';

        instructionPtr++;
    }


    free(program);
}
