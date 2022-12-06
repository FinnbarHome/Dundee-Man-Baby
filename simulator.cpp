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

void displayMenu();

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

    int num = binaryToDec(CI);
    num += 1;
    if(num > sizeOfMemory)
    {
        setLamp(true);
    }
    vector<int> memloca = decToBinary(num);
    CI = memloca;

}

//Fetchs next instruction from store
void Simulator::fetch(){

    int dec = binaryToDec(CI);
    PI = memory[dec];

}

//Decodes 5 bit operand and 3 bit opcode
void Simulator::decode(){

    CI.assign(32,0);

    vector<int> opc;
    if(currentInstructionSet == 3)
    {
        opc = {PI[13],PI[14],PI[15]};
        opcode(opc);
    }
    else if(currentInstructionSet == 4)
    {
        opc = {PI[13],PI[14],PI[15],PI[16]};
        opcode(opc);
    }
    else
    {
        opc = {PI[13],PI[14],PI[15],PI[16],PI[17]};
        opcode(opc);
    }

}

//Executes instruction
bool Simulator::execute(){
    
    if (memory.empty())
        return false;

    return true;

}


//Set CI to content of store location
void Simulator::JMP(){

}

//Add content of store location to CI
void Simulator::JRP(){

}

//Load accumulator with negative form of store content 
void Simulator::LDN(){

}

//Copy accumulator to store location
void Simulator::STO(){

}

//Subtract content of store location from accumulator 
void Simulator::SUB(){

}

//Increment CI if Accumulator value negative otherwise do nothing
void Simulator::CMP(){

}

//Halts machine
void Simulator::STP(){
    bool stopped = true;

}

//Displays memory state
void Simulator::display(){
    //Iterates from 0 to the size of the memory
    for(int i = 0; i < memory.size(); i++)
    {
        for (int j = 0; j < memory[i].size(); j++)
        {
            //Outputs memory at the location of [i][j]
            cout << memory[i][j];
        }
        cout << endl;
    }

}

// OPCODE functions 
void Simulator::opcode(vector<int> opc){
    int num = binaryToDec(opc);
    
    //Switch statement to select the op code
    switch (num){
        case 0: Simulator::JMP();
            break;
        case 1: Simulator::JRP();
            break;
        case 2: Simulator::LDN();
            break;
        case 3: Simulator::STO();
            break;
        case 4: Simulator::SUB();
            break;
        case 5: Simulator::SUB();
            break;
        case 6: Simulator::CMP();
            break;
        case 7: Simulator::STP();
            break;
    default: break;
    }
}

vector<int> Simulator::decToBinary(int num)
{
    // Store binary as string to ensure initial 0's are not ignored
    string bin = "";
    while (num > 0)
    {
        bin += to_string(num % 2);
        num = num / 2;
    }

    vector<int> v;

    // For each character in string, convert into int and store in vector
    for (char ch : bin)
    {
        int n = stoi(&ch);
        v.push_back(n);
    }

    return v;
}

int Simulator::binaryToDec(vector<int> num)
{
    // Store decimal and base
    int dec = 0, base = 1;

    // Reverse vector to account for big-endian
    reverse(num.begin(), num.end());

    // TEST: Print reversed vector
    // for (auto element : num)
    // 	cout << element;
    // cout << endl;
    
    // Store each element of vector into string
    stringstream ss;
    for(int i = 0; i < num.size(); ++i)
    {
    ss << num[i];
    }
    string s = ss.str();

    // Convert to binary
    int temp = stoi(s);
    while (temp)
    {
        int lastDigit = temp % 10;
        temp = temp / 10;
        dec += lastDigit * base;
        base = base * 2;
    }

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
                    sim.execute();
                    sim.display();
                    sleep(1);
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