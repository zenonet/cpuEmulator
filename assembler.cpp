#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

int asRegister(std::string arg){
    if(arg[0] != 'r'){
        std::cout << "Error: Expected register reference but got '" << arg << "'\n";
        exit(1);
    }
    return std::stoi(arg.substr(1));
}

int main(){
    std::ifstream sourceStream("first.zasm");
    std::ofstream binStream("first.bin");
    while(!sourceStream.eof()){
        std::string line;
        std::getline(sourceStream, line);
        

        int firstSpace = line.find(' ');
        int secondSpace = line.find(' ', firstSpace+1);
        std::string keyword = line.substr(0, firstSpace);
        std::string arg1 = line.substr(firstSpace+1, secondSpace-firstSpace);
        std::string arg2 = line.substr(secondSpace+1);

        std::cout << "Keyword: '" << keyword << "'\n";

        std::cout << "Arg1: " << arg1 << '\n';
        std::cout << "Arg2: " << arg2 << '\n';

        if(keyword == "init"){
            
            int reg = asRegister(arg1);
            int value = std::stoi(arg2);
            std::cout << "Initializing register " << reg << " with value " << value <<  '\n';

        }
        else if(keyword == "mov"){
            char destReg = (char) asRegister(arg1);
            char srcReg = (char) asRegister(arg2);

            char firstByte;
            char secondByte;

            firstByte = (0b0010 << 4) | destReg;
            secondByte = srcReg << 4;

            binStream << firstByte;
            binStream << secondByte;
            std::cout << "Instruction bytes: " << (int)firstByte << ", " << (int) secondByte << '\n';
            std::cout << "Moving value from register " << srcReg << " to register " << destReg << '\n';
        }
        else{
            std::cout << "Error: Keyword '" << keyword << "' is not defined\n";
            exit(1);
        }
    }
}
