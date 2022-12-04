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
// relevant functions
class Simulator
{
	private:
		vector<int> accumulator;
		vector<int> CI; // Control Instruction
		vector<int> PI; // Present Instruction/Instruction Decode
		vector<vector<int>> memory; // 32x32bit memory
        int currentInstructionSet;
        bool lamp = false;
        int sizeOfMemory = 32;
        int sizeOfMemLoca = 32;

	public:

        vector<int> getAccumulator()
        {
            return accumulator;
        }

        vector<int> getCI()
        {
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

        void setAccumulator(vector<int> accumulator)
        {
            this->accumulator = accumulator;
        }

        void setCI(vector<int> ci)
        {
            this->CI = ci;
        }

        void setPI(vector<int> pi)
        {
            this->PI = pi;
        }

        void setMemorySize(int num)
        {
            this->sizeOfMemory = num;
            memory.clear();
        }

        void setMemLocaSize(int num)
        {
            this->sizeOfMemLoca = num;
            memory.clear();
        }

        void setInstructionSet(int set)
        {
            this->currentInstructionSet = set;
        }

		// Read from file
		bool readFromFile()
		{
		    ifstream f;

		    // Iterate through ONLY .txt files in current directory
            DIR *di;
            char *ptr1,*ptr2;
            int retn;
            struct dirent *dir;
            di = opendir("."); // Open current directory
            if (di)
            {
                while ((dir = readdir(di)) != NULL)
                {
                    ptr1 = strtok(dir->d_name, ".");
                    ptr2 = strtok(NULL, ".");
                    if (ptr2!=NULL)
                    {
                        retn = strcmp(ptr2, "txt");
                        if(retn == 0)
                        {
                            printf("%s.txt\n", ptr1);
                        }
                    }
                }
                closedir(di);
            }
        	cout << "\nWhich machine code file would you like to read in? (include .txt at end): ";
        	string c;
        	cin >> c;
        	cout << c << endl;
		    f.open(c);

		    if(!f.is_open()) 
		    {
		    	cout << "Could not open file of that name, or does not exist." << endl;
		    	// clear input buffer
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return false;
		    }

			string line;
		    while(getline(f, line)) 
		    {
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

		// Decode instructions
		void decode()
		{
            CI.assign(32,0);

            while(lamp == false)
            {
                int dec = binaryToDec(CI);
                PI = memory[dec];
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

                int num = binaryToDec(CI);
                num += 1;
                vector<int> memloca = decToBinary(num);
                CI = memloca;
            }

            lamp = false;
		}

		// OPCODE functions (use case statements)
		void opcode(vector<int> opc)
		{
            int num = binaryToDec(opc);

            switch (num)
            {
            case 0: JMP();
                break;

            case 1: JRP();
                break;

            case 2: LDN();
                break;

            case 3: STO();
                break;

            case 4: SUB();
                break;

            case 5: SUB();
                break;

            case 6: CMP();
                break;

            case 7: STP();
                break;
            
            default: break;
            }
		}

        void JMP()
        {

        }

        void JRP()
        {

        }

        void LDN()
        {

        }

        void STO()
        {

        }

        void SUB()
        {

        }

        void CMP()
        {

        }

        void STP()
        {

        }

        void displayMemory()
        {
            for(int i = 0; i < memory.size(); i++)
            {
                for (int j = 0; j < memory[i].size(); j++)
                {
                    cout << memory[i][j];
                }
                cout << endl;
            }
        }

        bool execute()
        {
            if (memory.empty())
                return false;

            bool done = false;
            while (!done)
            {
                system("clear"); // Clear screen after every cycle
                displayMemory();
                cout << "\nTerminate program by CTRL+C.\n";
                sleep(1);
            }
            return true;
        }

        // Convert decimal to 32-bit binary equivalent
        vector<int> decToBinary(int num)
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

        // Convert binary to decimal equivalent
        int binaryToDec(vector<int> num)
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

};