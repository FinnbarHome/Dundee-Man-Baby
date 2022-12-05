/*
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
// #include <dirent.h>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

// Class "Simulator" holding accumulator, CI, PI, memory store and
// // relevant functions

class Simulator{
private:
    vector<int> accumulator;
    vector<int> CI;                 // Control Instruction
    vector<int> PI;                 // Present Instruction/Instruction Decode
    vector<vector<int>> memory;     // 32x32bit memory
    int currentInstructionSet;
    bool lamp = false;              //Stop lamp, if true, lamp is on therefore program ends
    int sizeOfMemory = 32;
    int sizeOfMemLoca = 32;

public:
    //Fetch-Execute cycle
    void increment_CI();
    bool readFromFile();
    void fetch();
    void decode();
    bool execute();
    void opcode(vector<int> opc);

    vector<int> decToBinary(int num);
    int binaryToDec(vector<int> num);

    //Opcodes
    void JMP();
    void JRP();
    void LDN();
    void SUB();
    void STO();
    void CMP();
    void STP();


    //Display methods
    void display();


    //Getters 
    bool getLamp(){
        return lamp;
    }

    vector<int> getCI(){
        return CI;
    }

    vector<int> getPI()
    {
        return PI;
    }

    vector<vector<int>> getMemory()
    {
        return memory;
    }

    int getInstructionSet()
    {
        return currentInstructionSet;
    }

    vector<int> getAccumulator()
    {
        return accumulator;
    }


    //Setters
    void setLamp(bool l){
        lamp = l;
    }

    void setCI(vector<int> ci){
        CI = ci;
    }

    void setPI(vector<int> pi)
    {
        PI = pi;
    }

    void setMemorySize(int num)
    {
        sizeOfMemory = num;
        memory.clear();
    }

    void setMemLocaSize(int num)
    {
        sizeOfMemLoca = num;
        memory.clear();
    }

    void setInstructionSet(int insSet)
    {
        currentInstructionSet = insSet;
    }

    void setAccumulator(vector<int> acc)
    {
        accumulator = acc;
    }


    //Constructor
    Simulator(/* args */);


    //Destructor
    ~Simulator();
};

//Constructor
Simulator::Simulator(/* args */){

}

//Destructor
Simulator::~Simulator(){

}