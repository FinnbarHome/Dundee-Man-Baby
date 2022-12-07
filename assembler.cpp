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
	for(int i = 0; i< output.size(); i++)
    {

        for(int j = 0; j<output.at(i).size(); j++)
        {

            cout << output.at(i).at(j) << "\t";
        }

        cout << endl;
    }
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
    ifstream read(inputFile);
    string line;
    vector<string> temp;
    string s;
    while(getline(read, line)){    //iterate through lines in input file
        for(char& c : line){       //iterate through chars in line
            if(c == ' ' && !s.empty()){
                temp.push_back(s);
                s = "";
            }
            else if(c == ';'){          //move to next line if comment
                break;             
            }
            else if(c != ' ' ){         //if not a space add char to temp
                s += c;
            }
        }
        if(!s.empty()){
            temp.push_back(s);
        }
        if(!temp.empty()){
            cleanInput.push_back(temp);
        }
    }
    read.close();
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
