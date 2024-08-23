#include <cctype>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include "symbols.h"

char asRegister(std::string arg){
    if(arg[0] != 'r'){
        std::cout << "Error: Expected register reference but got '" << arg << "'\n";
        exit(1);
    }
    int v = std::stoi(arg.substr(1));
    if(v > 15){
        std::cout << "Error: Referenced register " << v << "but only 0 to 15 are available\n";
    }
    return (char) v;
}

int main(){
    std::ifstream sourceStream("first.zasm");
    std::ofstream binStream("first.bin");

    std::map<std::string, int> labels;
    int instructionIndex = 0;
    while(!sourceStream.eof()){

        if(instructionIndex > 255){
            std::cout << "Error: Exceeded maximum instruction index based on max instruction pointer value (255)\n";
        }
        std::string line;
        std::getline(sourceStream, line);
       
        if(line[0] == ';' || line.empty()) continue; // skip comments

        // Check for label definition
        int colonIndex = line.find(':');
        if(colonIndex != std::string::npos){
            for(int i=0;i<colonIndex;i++){
                if(line[i] == ';') goto nvm;
                if(!isalnum(line[i])){
                    std::cout << "Error: Invalid label name\n";
                    exit(1);
                }
            }

            labels[line.substr(0, colonIndex)] = instructionIndex;
            continue;
        }
nvm:

        int firstSpace = line.find(' ');
        int secondSpace = line.find(' ', firstSpace+1);
        std::string keyword = line.substr(0, firstSpace);
        std::string arg1 = line.substr(firstSpace+1, secondSpace-firstSpace);
        std::string arg2 = line.substr(secondSpace+1);

        std::cout << "Keyword: '" << keyword << "'\n";

        std::cout << "Arg1: " << arg1 << '\n';
        std::cout << "Arg2: " << arg2 << '\n';

        if(keyword == "init"){
            
            char reg = (char)asRegister(arg1);
            char value = (char)std::stoi(arg2);
            std::cout << "Initializing register " << reg << " with value " << value <<  '\n';

            binStream << (char)((INIT << 4) | reg);
            binStream << value;

        }
        else if(keyword == "mov"){
            char destReg = (char) asRegister(arg1);
            char srcReg = (char) asRegister(arg2);

            binStream << (char)((MOV << 4) | destReg);
            binStream << (char)(srcReg << 4);

            std::cout << "Moving value from register " << srcReg << " to register " << destReg << '\n';
        }
        else if(keyword == "out"){
            char srcReg = asRegister(arg1);

            binStream << (char)((OUT << 4) | srcReg);
            binStream << (char)0;
        }
        else if(keyword == "add"){
            char baseReg = asRegister(arg1);
            char addentReg = asRegister(arg2);

            binStream << (char)((ADD << 4) | baseReg);
            binStream << (char)(addentReg << 4);
        }
        else if(keyword == "jmp"){
            if(labels.find(arg1) == labels.end()){ // TODO: Allow jumping to labels after the jump instruction
                std::cout << "Error: Trying to jump to non existing label " << arg1 << "\n";
                exit(1);
            }
            binStream << (char)(JMP << 4);
            binStream << (char) labels[arg1];
        }
        else{
            std::cout << "Error: Keyword '" << keyword << "' is not defined\n";
            exit(1);
        }
        instructionIndex++;
    }
    binStream.close();
    sourceStream.close();
}
