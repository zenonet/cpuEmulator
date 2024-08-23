#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

int main(){
    std::ifstream sourceStream("first.zasm");
    std::ofstream binStream("first.bin");
    while(!sourceStream.eof()){
        std::string line;
        std::getline(sourceStream, line);
        
        std::cout << "Working on instruction: " << line << '\n';

        binStream << "Was ist denn bei Karsten los?\n";
    }
}
