/*
  ASSEMBLER HEADER FILE
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#include <iostream>
#include <vector>
using namespace std;

class Assembler
{
private:
    vector<vector<string>> output; // Holds finished output containing machine code
    vector<string> mnemonics = {"JMP", "JRP", "LDN", "STO", "SUB", "SUB", "CMP", "STP"};
    vector<int> opcodes = {0, 1, 2, 3, 4, 5, 6};
    int sizeOfMemLoca;
    bool extraInstructions; // Trigger for if user wants to use expanded instruction set

public:
    void setInstructions();
    void displayOutput();
    int mnemonicToOpcode(string mnemonic); // Grab corresponding opcode for mnemonic
    void formatInput(); // Clean input file to assemble
    void assemble(); // Process file for variables and instructions
    void writeToFile(); // Write output machine code into a text file

    friend std::ostream& operator<<(std::ostream &output, const vector<int> &v);
};

// Output overload for vector
ostream& operator<<(ostream &output, const vector<int> &v) 
{
    for (int i = 0; i < v.size(); i++)
    {
        output << v[i];
    }

    return output;
}
