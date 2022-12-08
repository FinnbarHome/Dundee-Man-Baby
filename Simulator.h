/*
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#include <iostream>
#include <vector>
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
    bool lamp;                      //Stop lamp, if true, lamp is on therefore program ends
    int sizeOfMemory;
    int sizeOfMemLoca;

public:
    //Fetch-Execute cycle
    void increment_CI();
    bool readFromFile();
    void fetch();
    void decode();
    void execute(vector<int> opc);

    //Binary-Decimal conversions
    vector<int> decToBinary(int num);
    int binaryToDec(vector<int> num);

    //Opcode and operand methods
    int getOperand();

    //Opcodes
    void JMP();
    void JRP();
    void LDN();
    void SUB();
    void STO();
    void CMP();
    void STP();
    void DJP();
    void DJR();
    void DLD();
    void DAD();
    void DSB();
    void ADD();
    void DIV();
    void MUL();

    //Display methods
    void display();
    void setColourWhite();
    void setColourRed();
    void setColourGray();
    void setColourGreen();

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

    int getInstructionSet()
    {
        return currentInstructionSet;
    }

    vector<int> getAccumulator()
    {
        return accumulator;
    }
	
    int getMemorySize()
    {
        return sizeOfMemory;
    }

    int getMemLocaSize()
    {
        return sizeOfMemLoca;
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
	
    friend std::ostream& operator<<(std::ostream &output, const vector<int> &v);
};

//Constructor
Simulator::Simulator(/* args */){

    accumulator.assign(32,0);
    CI.assign(32,0);
    PI.assign(32,0);
    memory.assign( 32 , vector<int> (32, 0));
    currentInstructionSet = 3;
    lamp = false;
    sizeOfMemory = 32;
    sizeOfMemLoca = 32;

}

//Destructor
Simulator::~Simulator(){

}

// Output overload
ostream& operator<<(ostream &output, const vector<int> &v) 
{
    for (int i = 0; i < v.size(); i++)
    {
        output << v[i];
    }

    return output;
}
