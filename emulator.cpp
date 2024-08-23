#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "symbols.h"


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

        if(instruction == INIT){
            char reg = program[instructionPtr*2] & 0b00001111;
            char value = program[instructionPtr*2+1];
            printf("Initializing register %d with value %d\n", reg, value);
            registers[reg] = value;
        }
        else if(instruction == MOV){
            char src = program[instructionPtr*2+1] >> 4;
            char dest = program[instructionPtr*2] & 0b00001111;
            printf("Copying value from register %d to register %d\n", src, dest);
            registers[dest] = registers[src];
        }
        else if(instruction == OUT){
            char reg = program[instructionPtr*2] & 0b00001111;
            printf("Outputting value of register %d\n", reg);
            std::cout << (int)registers[reg] << '\n';
        }
        else if(instruction == ADD){
            char baseReg = program[instructionPtr*2] & 0b00001111;
            char addentReg = program[instructionPtr*2+1] >> 4;

            registers[baseReg] += registers[addentReg];
        }
        else if(instructionPtr == JMP){
            char destination = program[instructionPtr*2+1];
            instructionPtr = destination;
            continue;
        }

        instructionPtr++;
    }


    free(program);
}
