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
void Assembler::formatInput(string inputFile)
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
            else if(c != ' ' && c != ':'){         //if not a space add char to temp
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


vector<int> Assembler::decToBinary(string var)
{
    
    int num = stoi(var);
    bool negative = false;
    if (num < 0)
    {
        negative = true;
        num = num * (-1);
    }

    // Store binary as string to ensure initial 0's are not ignored
    string bin = "";
    while (num > 0)
    {
        bin += to_string(num % 2);
        num = num / 2;
    }

    vector<int> v;
    int count = 0;

    // For each character in string, convert into int and store in vector
    for (char ch : bin)
    {
        int n = stoi(&ch);
        v.push_back(n);
        count++;
    }

    for (int i = count; i < sizeOfMemLoca; i++)
        v.push_back(0);

    if (negative)
    {   
        v[v.size() - 1] = 1;
    }

    return v;
}


// Process file for variables and instructions
void Assembler::assemble()
{

}

// Write output machine code into a text file
void Assembler::writeToFile()
{
   ofstream f("BabyTest1-MC"); 
    f.open(f);
    for(int i = 0; i< output.size(); i++){
            for(int j = 0; j<output.at(i).size(); j++){
            f << output.at(i).at(j) << "\t";
        }
        f << endl;
    }
    if (!f.open()){
        cout << "Could not open file of that name - incompatible or does not exist." << endl;
    }

    f.close(); 
}
}

void displayMenu()
{
    Assembler assembler; 
    bool finished = false;
    int choice;
    string file;
    while(!finished)
    {
        cout << "MENU" << endl;
        cout << "1) assemble code to binary" << endl;
        cout << "2) exit" << endl;
        cin >> choice;
        switch(choice)
        {
            case 1:
            {
                cout << "enter source file name" << endl;
                cin >> file;
                assembler.formatInput(file);
                assembler.displayOutput();
            }
            case 2:
            {
                finished == true;
            }
        }
    }
}
}

int main()
{
    displayMenu();
    return 0;
}
