/*
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#include "Simulator.h"
#include <iostream>
using namespace std;

// Function declarations
void displayMenu();

int main()
{
	displayMenu();

    return 0;
}

void displayMenu()
{
    Simulator sim;

    bool finished = false;
    int choice;

    while(finished == false)
    {
        cout << "MENU" << endl;
        cout << "1) Load in machine code from file" << endl;
        cout << "2) Change instruction set" << endl;
        cout << "3) Change number of memory locations" << endl;
        cout << "4) Change the size of each memory location" << endl;
        cout << "5) Exit" << endl;

        cin >> choice;
        switch (choice)
        {
            case 1: {sim.readFromFile();
                    break;}

            case 2: {int num = 0;
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

                    break;}

            case 3: {int num = 0;
                    cout << "Enter a number for the new amount of memory locations (between 31 and 65): ";
                    cin >> num;
                    while(num < 32 && num > 64)
                    {
                        cout << "Invalid entry";
                        cout << "Enter a number for the new amount of memory locations (between 31 and 65): ";
                        cin >> num;
                    }

                    sim.setMemorySize(num);
                    break;}

            case 4: {int num = 0;
                    cout << "Enter a number for the new size of each memory location (between 31 and 101): ";
                    cin >> num;
                    while(num < 32 && num > 100)
                    {
                        cout << "Invalid entry";
                        cout << "Enter a number for the new size of each memory location (between 31 and 101): ";
                        cin >> num;
                    }

                    sim.setMemLocaSize(num);
                    break;}

            case 5: {finished = true;
                    break;}

            default: {cout << "Invalid Input" << endl;
                    break;}
        }
    }
}
