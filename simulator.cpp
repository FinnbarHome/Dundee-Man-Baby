/*
	GROUP 1 MANCHESTER BABY SIMULATOR
	Names: Finnbar Home, Abz Mohamed, Samuel Tweedie, Oliver Shearer, John Nicol, Kayee Liu
	Module Code: AC21009
*/

#include<iostream>
using namespace std;

//SHOULD BE TRANSFERED TO A HEADER FILE!!!!!
class Simulator{
	public:
		void increment_CI();
		void fetch();
		void decode();
		void execute();
		void JMP();
		void JRP();
		void LDN();
		void SUB();
		void STO();
		void CMP();
		void STP();
		void display_everything();
};

//Adds one to control instruction
void Simulator::increment_CI(){

}

//Fetchs next instruction from store
void Simulator::fetch(){

}

//Decodes 5 bit operand and 3 bit opcode
void Simulator::decode(){

}

//Executes instruction
void Simulator::execute(){

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

}

//Displays memory state
void Simulator::display_everything(){

}





void run(){
	//Create new instance of simulator
	Simulator sim;

	//Should loop until STP function is recieved
	// while (halted != true)
	// {
	// 	sim.increment_CI();
	// 	sim.fetch();
	// 	sim.decode();
	// 	sim.execute();
	// 	sim.display_everything();
	// }
}

int main()
{
	run();
	return 0;
}




