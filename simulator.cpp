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

enum COLORS {
    NC=-1,
    BLACK,
    Red,
    Green,
    YELLOW,
    Gray, //ACTUALLY NOT GRAY
    MAGENTA,
    CYAN,
    White,
};

/**
* Colorize terminal colors ANSI escape sequences.
*
* @param font font color (-1 to 7), see COLORS enum
**/
const char* setColour(int font) {
    static char code[20];
    if (font >= 0){
        font += 30;
    }else{
        font = 0;
    }
    sprintf(code, "\033[%dm", font);
    return code;
}

//Method to read machine code from a file and load into a vector
bool Simulator::readFromFile()
{
    //Sets text colour as white
    cout << setColour(White);

    ifstream f;
    cout << "\nCURRENT TEXT FILES IN DIRECTORY:\n";
    // Iterate through ONLY .txt files in current directory 
    for (const auto & entry : fs::directory_iterator("./"))
    {
        string name = entry.path();

        string str = name.substr(2); // Trim the "./" at start of string
        int pos = str.find(".");
        string sub = str.substr(pos + 1);

        if (sub == "txt") // Check if it is a txt file
            cout << str << endl;;
    }
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
	 // Ensure number of lines does not exceed size of memory
        if (count > sizeOfMemory)
        {
            cout << "File contents exceeds size of memory." << endl;
            return false;
        }

        // Ensure line of length matches size of memory location
        if ((line.length()-1) != sizeOfMemLoca)
        {
            cout << "File contents does not match bit size." << endl;
            return false;
        }
	   
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
		cout << "File includes non-binary characters." << endl;
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
}

// Fetchs next instruction from store
void Simulator::fetch(){
    PI = memory[binaryToDec(getCI())];
}

//Decodes 5 bit operand and 3 bit opcode
void Simulator::decode(){
    //Sets text colour as white
    cout << setColour(White);

    vector<int> opc;
    if (currentInstructionSet == 3) 
    {
        opc = {PI[13],PI[14],PI[15]};
        if (PI[16] != 0)
        {
            cout << "\n**INVALID OPCODE DETECTED. PROGRAM TERMINATING...**" << endl;
            exit(1);
        }
    }
    else if (currentInstructionSet == 4) opc = {PI[13],PI[14],PI[15],PI[16]};

    execute(opc);
}

//Gets the operand and returns it in decimal value
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

//Load accumulator with negative form of store content 
void Simulator::LDN(){
    //Get operand of PI
    int operand = getOperand();

    //Create an int decimal version of the current store 
    int store = binaryToDec(memory[operand]);

    //Negates store
    store = -store;

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
    //Convert accumulator value to dec if its less than 0 its negative 
    if(binaryToDec(getAccumulator()) < 0){
        //therefore increment CI
        increment_CI();
    }
}

//Halts machine
void Simulator::STP(){
    //Sets the lamp to true therefore machine is halted
    setLamp(true);
}

// Sets CI to the decimal form of the operand passed in the instruction
void Simulator::DJP()
{
    int operand = getOperand();

    vector<int> num = decToBinary(operand);

    CI = num;
}

//Add decimal form of the operand passed in the instruction to CI
void Simulator::DJR()
{
    int operand = getOperand();

    int num = binaryToDec(CI);

    num = operand + num;

    CI = decToBinary(num);
}

//Load accumulator with negative and decimal form of the operand passed in the instruction
void Simulator::DLD()
{
    int operand = getOperand();

    operand = operand * (-1);

    accumulator = decToBinary(operand);
}

// Add decimal form of the operand passed in the instruction to accumulator
void Simulator::DAD()
{
    int operand = getOperand();

    int num = binaryToDec(accumulator);

    operand = operand + num;

    accumulator = decToBinary(operand);
}

//Subtract decimal form of the operand passed in the instruction from accumulator
void Simulator::DSB()
{
    int operand = getOperand();

    int num = binaryToDec(accumulator);

    operand = num - operand;

    accumulator = decToBinary(operand);
}

//Add content of store location to accumulator
void Simulator::ADD()
{
    int operand = getOperand();

    operand = binaryToDec(memory[operand]);

    int num = binaryToDec(accumulator);

    operand = operand + num;

    accumulator = decToBinary(operand);
}

// Divide accumulator by the content of store location
void Simulator::DIV()
{
    int operand = getOperand();

    operand = binaryToDec(memory[operand]);

    int num = binaryToDec(accumulator);

    operand = num/operand;

    accumulator = decToBinary(operand);
}

// Multiply accumulator by the content of store location
void Simulator::MUL()
{
    int operand = getOperand();

    operand = binaryToDec(memory[operand]);

    int num = binaryToDec(accumulator);

    operand = num * operand;

    accumulator = decToBinary(operand);
}

//Displays memory state
void Simulator::display(){
    //Sets text colour as gray
    cout << setColour(Gray);
    cout << "MEMORY: " << endl;
    //Sets text colour as white
    cout << setColour(White);

    //Iterates from 0 to the size of the memory
    for(int i = 0; i < memory.size(); i++)
    {
        for (int j = 0; j < memory[i].size(); j++)
        {
            //Outputs memory at the location of [i][j]
            if (memory[i][j] == 0)
                cout << ".";
            else
                cout << "1";
        }
        cout << " Address " << i << endl;
    }
    cout << endl;

    //Sets text colour as gray
    cout << setColour(Gray);
    cout << getCI() << " ; CI (in decimal: " << binaryToDec(getCI()) << ")" << endl;
    cout << getPI() << " ; PI" << endl;
    cout << getAccumulator() << " ; Accumulator (in decimal: " << binaryToDec(getAccumulator()) << ")" << endl;
    //Sets text colour as white
    cout << setColour(White);
}

// OPCODE functions 
void Simulator::execute(vector<int> opc){
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
	case 8: Simulator::DJP(); break;
	case 9: Simulator::DJR(); break;
	case 10: Simulator::DLD(); break;
	case 11: Simulator::DAD(); break;
	case 12: Simulator::DSB(); break;
	case 13: Simulator::ADD(); break;
	case 14: Simulator::DIV(); break;
	case 15: Simulator::MUL(); break;
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
    //Sets text colour as white
    cout << setColour(White);

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
    try
    {
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
    catch (out_of_range& e)
    {
        
        //Sets text colour red
        cout << setColour(Red);
        cout << "ERROR: VARIABLE TOO LARGE AND OUT OF RANGE. PROGRAM TERMINATING..." << endl;

        //Sets text colour as white
        cout << setColour(White);
        exit(1);
    }
}

//Menu method
void displayMenu()
{
    //Create object of class Simulator
    Simulator sim;

    //Sets text colour as white
    cout << setColour(White);

    //Boolean for whether menu loop has finished
    bool finished = false;
    //Stores user menu choice
    int choice;
    //Used for printing out OFF/ON refering to additional instructions
    string s;

    while(!finished)
    {
	// Check if extra instructions to be used
        if (sim.getInstructionSet() == 3)
            s = "OFF";
        else
            s = "ON";
	    
        cout << "\nMENU" << endl;
	cout << "Current size of memory: " << sim.getMemorySize() << " each with " << sim.getMemLocaSize() << " bits." << endl;    
        cout << "\n1) Load in machine code from file and execute it" << endl;
        cout << "2) Turn on/off extra instructions (currently " << s << ")" << endl;
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
                    //Sets text colour as green
                    cout << setColour(Green);
                    cout << "Read successful and memory updated. Beginning program..." << endl;
                    //Sets text colour as white
                    cout << setColour(White);

                    sleep(3);
                }
                else
                {
                    //Sets text colour red
                    cout << setColour(Red);
                    cout << "ERROR: Read unsuccessful. Memory has been emptied.\n" << endl;
                    //Sets text colour as white
                    cout << setColour(White);
                    break;
                }
                //int count = 0;
                // Should loop until STP function is recieved
                while (sim.getLamp() == false)
                {
                    //Sets text colour as white
                    cout << setColour(White);
                    
                    // Clear screen after every cycle
                    system("clear"); 
                    cout << "\nTerminate program by CTRL+C, or automatically return to menu once stop lamp is set by the program.\n";

                    sim.increment_CI();
                    if(sim.getLamp()){
                        continue;  
                    }
                    
                    sim.fetch();

                    //Sets text colour as gray
                    cout << setColour(Gray);
                    cout << "\n**FETCHING...**\n" << endl;

                    //Sets text colour as white
                    cout << setColour(White);
                    sim.display();

                    sleep(2);
                    system("clear");

                    sim.decode();
                    cout << "\nTerminate program by CTRL+C, or automatically return to menu once stop lamp is set by the program.\n";

                    //Sets text colour as gray
                    cout << setColour(Gray);
                    cout << "\n**DECODING AND EXECUTING...**\n" << endl;
			
		    if (sim.getLamp())
                    cout << "**STOP CODE FOUND: Halting program...**\n\n";
			
                    //Sets text colour as white
                    cout << setColour(White);

                    sim.display();
                    sleep(2);
                }
                //Sets text colour as gray
                cout << setColour(Gray);
                cout <<"\nEND ACCUMULATOR IN DECIMAL: " << sim.binaryToDec(sim.getAccumulator()) << endl;
                //Sets text colour as white
                cout << setColour(White);

                // Reset
                sim.setLamp(false);
                vector <int> v(sim.getMemLocaSize(), 0);
                sim.setAccumulator(v);
                sim.setCI(v);
                break;
            }
            case 2: 
	    {	    
		if (sim.getInstructionSet() == 3)
                {
                    //Sets text colour as green
                    cout << setColour(Green);

                    sim.setInstructionSet(4);
                    cout << "\n**Extra instructions turned on.**" << endl;

                    //Sets text colour as white
                    cout << setColour(White);
                }
                else
                {
                    //Sets text colour as red
                    cout << setColour(Red);

                    sim.setInstructionSet(3);
                    cout << "\n**Extra instructions turned off.**" << endl;

                    //Sets text colour as white
                    cout << setColour(White);
                }
                cout << "\nExtra instructions: " << endl;
                cout << "DJP (0001): Sets CI to the decimal form of the operand passed in the instruction" << endl;
                cout << "DJR (1001): Add decimal form of the operand passed in the instruction to CI" << endl;
                cout << "DLD (0101): Load accumulator with negative and decimal form of the operand passed in the instruction" << endl;
                cout << "DAD (1101): Add decimal form of the operand passed in the instruction to accumulator" << endl;
                cout << "DSB (0011): Subtract decimal form of the operand passed in the instruction from accumulator" << endl;
                cout << "ADD (1011): Add content of store location to accumulator" << endl;
                cout << "DIV (0111): Divide accumulator by the content of store location" << endl;
                cout << "MUL (1111): Multiply accumulator by the content of store location" << endl;
                
                break;
	    }
            case 3: 
	    {
		    int num = 33;
                    cout << "Enter a number for the new amount of memory locations (between 32 and 64 inclusive):: ";
                    cin >> num;
                    while(num < 32 && num > 64)
                    {
                        //Sets text colour as red
                        cout << setColour(Red);
                        cout << "ERROR: Invalid entry";
                        //Sets text colour as white
                        cout << setColour(White);

                        cout << "Enter a number for the new amount of memory locations (between 32 and 64 inclusive): ";
                        // clear input buffer
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> num;
                    }
                    sim.setMemorySize(num);
                    break;
	    }
            case 4: 
	    {
		    int num = 0;
                    cout << "Enter a number for the new size of each memory location (between 32 and 64 inclusive): ";
                    cin >> num;
                    while(num < 32 && num > 64)
                    {
                        //Sets text colour as red
                        cout << setColour(Red);
                        cout << "ERROR: Invalid entry";
                        //Sets text colour as white
                        cout << setColour(White);

                        cout << "Enter a number for the new size of each memory location (between 32 and 64 inclusive): ";
                        // clear input buffer
		        cin.clear();
		        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            //Sets text colour as red
            cout << setColour(Red);
		    cout << "ERROR: Invalid Input" << endl;
            //Sets text colour as white
            cout << setColour(White);

		    // clear input buffer
		    cin.clear();
		    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
