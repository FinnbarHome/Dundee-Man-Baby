/*
    ASSEMBLER PROGRAM
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "assembler.h"
using namespace std;

// Set instruction set to either include the additional instructions
void Assembler::setInstructions()
{

}

void Assembler::displayOutput()
{

}

// Grab corresponding opcode for mnemonic
int Assembler::mnemonicToOpcode(string mnemonic)
{
    for(int i=0; i<mnemonics.size(); ++i){       //iterate through set mnemonics
        if(mnemonics.at(i) == mnemonic){
            return opcodes.at(i);                //if found returns corresponding opcode
        }
    }
    return -1; //mnemonic not found in set
}

// Clean input file to assemble
void Assembler::formatInput()
{

}

// Process file for variables and instructions
void Assembler::assemble()
{

}

// Write output machine code into a text file
void Assembler::writeToFile()
{

}

void displayMenu()
{

}

int main()
{
    //displayMenu();
    return 0;
}
