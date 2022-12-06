/*
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#define NOMINMAX
// adapt sleep function depending on operating system
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Simulator.h"
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

//Declare menu method
void displayMenu();

//Method to read machine code from a file and load into a vector
bool Simulator::readFromFile()
{
    ifstream f;

    // Iterate through ONLY .txt files in current directory 
    // ***FIX NEEDED FOR SHOWING TXT ONLY***
    for (const auto & entry : fs::directory_iterator("./"))
        cout << entry.path() << endl;
	
    cout << "\nWhich machine code file would you like to read in? (include .txt at end): ";
    string c;
    cin >> c;
    f.open(c);
    memory.clear();

    if(!f.is_open()) 
    {
        cout << "Could not open file of that name - incompatible or does not exist." << endl;
        // clear input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
	
    int count = 0;
    string line;
    while(getline(f, line)) 
    {
	//STILL TO PROPERLY IMPLEMENT: formatting of line to strip spaces
        // line.erase(remove(line.begin(),line.end(),' '),line.end());
        // line.erase(remove(line.begin(),line.end(),'\n'),line.end());
        //cout << line.length() << "AHHH" << endl;
	    
	// Ensure number of lines does not exceed size of memory
        if (count > sizeOfMemory)
            return false;

        //cout << count << ": LINE LEN: " << line.length()-1 << endl;
        //cout << "MEM LOCA: " << sizeOfMemLoca << endl;

        // Ensure line of length matches size of memory location
        if ((line.length()-1) != sizeOfMemLoca)
            return false;
	   
        // Insert each character of each line into a vector
        vector<int> v;
        for (int n = 0; n < (line.length()-1); n++)
        {
            char c = line[n];
            int temp = c - '0'; // Conversion to int

            if (temp != 0 && temp != 1)
            {
                f.close();
                v.clear();
                memory.clear();
                // clear input buffer
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                return false;
            }
            
            v.push_back(temp); 
        }
        memory.push_back(v); // Push vector into memory
	    count++;
    }

    f.close();

    // Fill rest of memory with 0's
    for (int i = 0; i <= sizeOfMemory - count; i++)
    {
        vector<int> vec(32, 0);
        memory.push_back(vec);
    }
	
    // for(int i = 0; i < memory.size(); i++)
    // {
    // 	cout << "MEMORY(i): " << memory.size() << endl;
    // 	for (int j = 0; j < memory[i].size(); j++)
    // 	{
    // 		if (j == 0)
    // 			cout << "MEMORY(j): " << memory[j].size() << endl;
    // 		cout << memory[i][j];
    // 	}
    // 	cout <<endl;
    // }

    return true;
}

//Adds one to control instruction
void Simulator::increment_CI(){
    // Convert CI to a decimal integer, add one and set this to var num
    int num;
    num = binaryToDec(getCI());
    num++;

    // If the new value is greater than sizeOfMemory, turn on the lamp
    setLamp(num >= sizeOfMemory);

    if (getLamp())
        return;

    
    // Convert the new value back to a binary and store it in CI
    CI = decToBinary(num);
    //cout << getCI() << endl;
}

// Fetchs next instruction from store
void Simulator::fetch(){
    PI = memory[binaryToDec(getCI())];
}

//Decodes 5 bit operand and 3 bit opcode
void Simulator::decode(){
    vector<int> opc;
    if (currentInstructionSet == 3) opc = {PI[13],PI[14],PI[15]};
    else if (currentInstructionSet == 4) opc = {PI[13],PI[14],PI[15],PI[16]};
    else opc = {PI[13],PI[14],PI[15],PI[16],PI[17]};

    int operand = getOperand();

    execute(opc, operand);
}

vector<int> Simulator::findLineInMemory(int linNum)
{
	//Intialises a vector with size of memorysize (32) and intialises all to be 0
    vector<int> ins(sizeOfMemory, 0);

    //Iterates through whole line to the size of the memory (32)
	for (int i=0; i<sizeOfMemory; i++)
	{
		ins[i] = ins[i] + memory[linNum][i];
	}
	return ins;
}

//Gets the opperand and returns it in decimal value
int Simulator::getOperand(){
    //Creates operand vector that has the values between 0 and 5 in the PI
    vector<int> operand{PI[0], PI[1], PI[2], PI[3], PI[4], PI[5], PI[6], PI[7], PI[8], PI[9], PI[10], PI[11], PI[12]};

    //Converts the operand from bin to dec and returns it
    return binaryToDec(operand);
}

//Set CI to content of store location
void Simulator::JMP(){
    //Gets opperand of PI
    int operand = getOperand();
    
    //Sets the current instruction to be the line of the store location defined by the operand
    CI = memory[operand];
}

// Add content of store location to CI
void Simulator::JRP() {
    //Get operand of PI
    int operand = getOperand();

    //Create an int decimal version of the current store 
    int store = binaryToDec(memory[operand]);

    //Add store to int value of CI, convert result to binary and set it as the new CI
    setCI(decToBinary(store + binaryToDec(getCI())));
}

//NOT SURE IF THIS WORKS (NOT SURE IF IT HAS TO BE NEGATED BEFORE PASSING INTO BIN CONV ETC)
//Load accumulator with negative form of store content 
void Simulator::LDN(){
    //Get operand of PI
    int operand = getOperand();

    //Create an int decimal version of the current store 
    int store = binaryToDec(memory[operand]);

    //Negates store
    int store = -store;

    //Sets accumulator as binary conversion of store
    setAccumulator(decToBinary(store));
}

//Copy accumulator to store location
void Simulator::STO(){
    //Get operand of PI
    int operand = getOperand();

    //Copys accmulator to store location
    memory[operand] = getAccumulator();
}

//Subtract content of store location from accumulator 
void Simulator::SUB(){
    //Get operand of PI
    int operand = getOperand();

    //Create an int decimal version of the current store 
    int store = binaryToDec(memory[operand]);

    //Create an int decimal version of the accumulator
    int acc = binaryToDec(getAccumulator());

    //Accumulator value minus store value
    acc = acc - store;

    //Sets accumulator as binary conversion of acc
    setAccumulator(decToBinary(acc));
}

//Increment CI if Accumulator value negative otherwise do nothing
void Simulator::CMP(){

}

//Halts machine
void Simulator::STP(){
    //Sets the lamp to true therefore machine is halted
    setLamp(true);
}

//Sets CI to the decimal form of the operand passed in the instruction
void Simulator::JMPA(){

}

//Add decimal form of the operand passed in the instruction to CI
void Simulator::JRPA(){

}

//Load accumulator with negative and decimal form of the operand passed in the instruction
void Simulator::LDNA(){

}

//Add decimal form of the operand passed in the instruction to accumulator
void Simulator::ADDA(){
    
}

// /Subtract decimal form of the operand passed in the instruction from accumulator
void Simulator::SUBA(){

}

//Add content of store location to accumulator
void Simulator::ADD(){

}

//Divide accumulator by the content of store location
void Simulator::DIV(){

}

//Multiply accumulator by the content of store location 
void Simulator::MUL(){

}

//Displays memory state
void Simulator::display(){
    //Iterates from 0 to the size of the memory
    cout << "MEMORY: " << endl;
    for(int i = 0; i < memory.size(); i++)
    {
        for (int j = 0; j < memory[i].size(); j++)
        {
            //Outputs memory at the location of [i][j]
            cout << memory[i][j];
        }
        cout << " Address " << i << endl;
    }
    cout << endl;

    cout << getCI() << " ; CI" << endl;
    cout << getPI() << " ; PI" << endl;
    cout << getAccumulator() << " ; Accumulator" << endl;
}

// OPCODE functions 
void Simulator::opcode(vector<int> opc){
    int num = binaryToDec(opc);

    // Switch statement to select the op code
    switch (num) {
        case 0: Simulator::JMP(); break;
        case 1: Simulator::JRP(); break;
        case 2: Simulator::LDN(); break;
        case 3: Simulator::STO(); break;
        case 4: case 5: Simulator::SUB(); break; // combine cases 4 and 5
        case 6: Simulator::CMP(); break;
        case 7: Simulator::STP(); break;
    }
}

vector<int> Simulator::decToBinary(int num)
{
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

int Simulator::binaryToDec(vector<int> num)
{
    bool negative = false;
    if (num.size() == sizeOfMemLoca)
    {
        if (num[sizeOfMemLoca-1] == 1)
        {
            negative = true;
            num[sizeOfMemLoca-1] = 0;
        }
    }

    // Store decimal and base
    int dec = 0, base = 1;

    // Reverse vector to account for big-endian
    reverse(num.begin(), num.end());

    // TEST: Print reversed vector
    // for (auto element : num)
    //     cout << element;
    // cout << endl;
    
    // Store each element of vector into string

    if (num.size() == sizeOfMemLoca-1)
    {
        while(num[num.size()-1] == 0)
        {
            num.pop_back();
            if (num.empty())
                return 0;
        }
    }


    stringstream ss;
    for(int i = 0; i < num.size(); ++i)
    {
        ss << num[i];
    }
    string s = ss.str();

    // Convert to binary
    long long temp = stoll(s);
    while (temp)
    {
        long long lastDigit = temp % 10;
        temp = temp / 10;
        dec += lastDigit * base;
        base = base * 2;
    }

    if (negative)
        dec = dec * (-1);
    
    return dec;
}

//Menu method
void displayMenu()
{
    Simulator sim;

    bool finished = false;
    int choice;

    while(!finished)
    {
        cout << "MENU" << endl;
        cout << "1) Load in machine code from file and execute it" << endl;
        cout << "2) Change instruction set" << endl;
        cout << "3) Change number of memory locations" << endl;
        cout << "4) Change the size of each memory location" << endl;
        cout << "5) Exit" << endl;

        cin >> choice;
        switch (choice)
        {
            case 1: 
            {
                cout << endl;
                if (sim.readFromFile())
                {
                    cout << "Read successful and memory updated. Beginning program..." << endl;
                    // cout << "Current state of memory: \n" << endl;
                    // sim.display();
                    cout << endl;
                    sleep(1);
                }
                else
                {
                    cout << "Read unsuccessful. Memory has been emptied.\n" << endl;
                    break;
                }
                int count = 0;
                // Should loop until STP function is recieved
                while (sim.getLamp() == false)
                {
                    system("clear"); // Clear screen after every cycle
                    cout << "\nTerminate program by CTRL+C, or automatically return to menu once stop lamp is set by the program.\n";
                    sim.increment_CI();
                    if(sim.getLamp())
                        continue;
                    sim.fetch();
                    sim.decode();
                    // sim.execute(); // execute is called within decode
                    sim.display();
                    sleep(3);
                }
                break;
            }
            case 2: 
	    {	    
		    int num = 0;
                    cout << "Enter the amount of bits dedicated for the instructions opcodes (3,4 or 5): ";
                    cin >> num;
                    while(num != 3 && num != 4 && num != 5)
                    {
                        cout << "Invalid entry";
                        cout << "Enter the amount of bits dedicated for the instructions opcodes (3,4 or 5): ";
                        cin >> num;
                    }

                    if(num == 3)
                        sim.setInstructionSet(num);
                    else if(num == 4)
                        sim.setInstructionSet(num);
                    else
                        sim.setInstructionSet(num);

                    break;
	    }
            case 3: 
	    {
		    int num = 33;
                    cout << "Enter a number for the new amount of memory locations (between 31 and 65): ";
                    cin >> num;
                    while(num < 32 && num > 64)
                    {
                        cout << "Invalid entry";
                        cout << "Enter a number for the new amount of memory locations (between 31 and 65): ";
                        cin >> num;
                    }

                    sim.setMemorySize(num);
                    break;
	    }
            case 4: 
	    {
		    int num = 0;
                    cout << "Enter a number for the new size of each memory location (between 31 and 101): ";
                    cin >> num;
                    while(num < 32 && num > 100)
                    {
                        cout << "Invalid entry";
                        cout << "Enter a number for the new size of each memory location (between 31 and 101): ";
                        cin >> num;
                    }

                    sim.setMemLocaSize(num);
                    break;
	    }
            case 5: 
	    {	
		    finished = true;
                    break;
	    }
            default: 
	    {
		    cout << "Invalid Input" << endl;
                    break;
	    }
        }
    }
}

//Main method to simply run the menu method
int main()
{
	displayMenu();
	return 0;
}
