// FCAI _ Object-Oriented Programming 2 _ 2023 _ Assignment 2
// Program Name: Vole Machine Language Simulator
// Last Modification Date: 14/11/2023
// Author1 and ID and Group: Seif Gamal Abdelmonem | 20220162 | S9
// Author2 and ID and Group: Samuel Moamen Samy | 20220168 | S9
// Author3 and ID and Group: Zeyad hussain Adel | 20220439 | S9

/*
Purpose: In this problem we developed a Machine Language Simulator using C++ OOP concept.
that can make the machine cycle as Fetch, Decode, and execute instructions entered in hexadecimal from file.
Those instructions are:
(1) Load from memory to register.
(2) Store bit pattern to register.
(3) Store from register to memory or show register content on display screen.
(4) Move content from register to another.
(5) Add two numbers from two register and store result in third register.
(6) Jump to change the program counter.
(7) Halt/terminate the program.
*/

#include <iostream>
#include <fstream>
#include<vector>
#include <string>
#include <iomanip>
#include <bitset>
#include <sstream>

using namespace std;

class mainMemory{
private:
    int size;
    vector<string>mem;
public:
    mainMemory (){
        size = 256;
        // Initialize main memory with 256 place all of them stores "00"
        mem.resize(size, "00");
    }
    void store(int address, string &value) {
        mem[address] = value;
    }
    string read(int address) {
        return mem[address];
    }
};

class Register{
private:
    int size;
    vector<string>reg;
public:
    Register(){
        size = 16;
        // Initialize main memory with 16 place all of them stores "00"
        reg.resize(size, "00");
    }
    void store(int address, string &value) {
        reg[address] = value;
    }
    string read(int address) {
        return reg[address];
    }
};

class Machine{
private:
    mainMemory memory;
    Register registers;
    string programCounter;
    string instructionRegister;
    vector<char>display_vector;
public:
    Machine(string &programCounter) {
        this->programCounter = programCounter;
    }

    void loadFile(){
        cout << "Store value in memory address before load from file? (y/n)" << endl;
        char store;
        cin >> store;
        if(store == 'y'){
            cout << "Enter value in hex (e.g. 03) :" << endl;
            string pre_value;
            cin >> pre_value;
            cout << "Enter address in hex (e.g. 03) :" << endl;
            string pre_address;
            cin >> pre_address;
            int int_pre_address = stoi(pre_address, nullptr, 16);
            memory.store(int_pre_address, pre_value);
        }

        int int_pc = stoi(programCounter, nullptr, 16);
        string filename;
        cout << "Enter file name (without .txt)" << endl;
        cin >> filename;
        filename = filename + ".txt";
        ifstream file(filename);
        if(!file){
            cout << "Error file: "<< filename << endl;
        }
        else{
            while(file.good()){
                string line;
                getline(file, line);
                string part1 = line.substr(2, 1);
                part1 += line.substr(6, 1);
                string part2 = line.substr(10, 2);
                memory.store(int_pc, part1);
                int_pc++;
                memory.store(int_pc, part2);
                int_pc++;
            }
        }
    }

    bool Fetch(){
        int pc = stoi(programCounter, nullptr, 16);
        string part1 = memory.read(pc);
        pc++;
        string part2 =memory.read(pc);
        pc++;

        if (!isValidStep(part1) || !isValidStep(part2)) {
            cout << "Invalid hexadecimal values in memory." << endl;
            return false;
        }

        instructionRegister = part1 + part2;
        programCounter = toHex(pc);

        return true;
    }

    bool DecodeAndExecute() {
        char opcode = instructionRegister[0];
        if (opcode == '1') {
            loadFromMemory();
        }
        else if (opcode == '2') {
            storeBitPattern();
        }
        else if (opcode == '3') {
            storeToMemory();
        }
        else if (opcode == '4') {
            moveRegisters();
        }
        else if (opcode == '5') {
            add();
        }
        else if (opcode == 'B') {
            jump();
        }
        else if (opcode == 'C') { // HALT
            return false;
        }
        return true;

    }

    void loadFromMemory(){
        string memory_address = instructionRegister.substr(2,2);
        int int_memory_address = stoi(memory_address, nullptr, 16);

        string loaded_value = memory.read(int_memory_address);

        string register_address = "0";
        register_address += instructionRegister[1];

        int int_register_address = stoi(register_address, nullptr, 16);

        registers.store(int_register_address, loaded_value);
    }

    void storeBitPattern(){
        string bit_pattern = instructionRegister.substr(2,2);

        string register_address = "0";
        register_address += instructionRegister[1];

        int int_register_address = stoi(register_address, nullptr, 16);

        registers.store(int_register_address, bit_pattern);
    }

    void storeToMemory(){
        if(instructionRegister.substr(2,2) == "00"){ // Display to screen
            string register_address = "0";
            register_address += instructionRegister[1];

            int int_register_address = stoi(register_address, nullptr, 16);

            string display = registers.read(int_register_address);

            int int_display = stoi(display, nullptr, 16);

            if(int_display > 31 && int_display < 127){
                display_vector.push_back(char(int_display));
            }
            else{
                cout << endl << "=======Display screen======="<<endl;
                cout << "Hex: " << display;
                cout << endl << "============================" << endl;
            }
        }
        else{ // store to memory
            string register_address = "0";
            register_address += instructionRegister[1];

            int int_register_address = stoi(register_address, nullptr, 16);

            string loaded_value = registers.read(int_register_address);

            string memory_address = instructionRegister.substr(2,2);
            int int_memory_address = stoi(memory_address, nullptr, 16);

            memory.store(int_memory_address, loaded_value);
        }
    }

    void moveRegisters(){
        string register_address_1 = "0";
        register_address_1 += instructionRegister[2];

        int int_register_address_1 = stoi(register_address_1, nullptr, 16);


        string loaded_value = registers.read(int_register_address_1);

        string register_address_2 = "0";
        register_address_2 += instructionRegister[3];

        int int_register_address_2 = stoi(register_address_2, nullptr, 16);

        registers.store(int_register_address_2, loaded_value);
    }

    void add(){
        string address1 = "0", address2 = "0", address_result = "0";
        address1 += instructionRegister[2];
        address2 += instructionRegister[3];
        address_result += instructionRegister[1];

        string add1 = registers.read(stoi(address1, nullptr, 16));
        string add2 = registers.read(stoi(address2, nullptr, 16));

        // Convert hexadecimal string to binary string using bitset and string libraries
        stringstream ss1;
        ss1 << hex << add1;
        int n1;
        ss1 >> n1;
        string bin1 = bitset<8> (n1).to_string();

        stringstream ss2;
        ss2 << hex << add2;
        int n2;
        ss2 >> n2;
        string bin2 = bitset<8> (n2).to_string();

        // add two binary number in 2's complement method
        string bin_result;
        int carry = 0;
        for (int i = 7; i >= 0; --i) {
            int digit = carry + ((int)(bin1[i]-48) + (int)(bin2[i]-48));
            if(digit == 0){
                digit = 0;
                carry = 0;
            }
            else if(digit == 1){
                digit = 1;
                carry = 0;
            }
            else if(digit == 2){
                digit = 0;
                carry = 1;
            }
            else {
                digit = 1;
                carry = 1;
            }
            bin_result = char(digit+48) + bin_result;
        }

        // Convert binary string to decimal integer
        bitset<8>bits(bin_result);
        int dec_result = bits.to_ulong();
        // Convert decimal integer to hex string
        string result = toHex(dec_result);

        registers.store(stoi(address_result, nullptr, 16), result);
    }

    void jump(){
        string compare = "0";
        compare += instructionRegister[1];
        string jumpTo;
        jumpTo = instructionRegister.substr(2,2);
        if(registers.read(stoi(compare, nullptr, 16)) == registers.read(0)){
            programCounter = jumpTo;
        }
    }

    void outputState(){
        cout << "==========Register Status==========" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << uppercase << hex << setw(2) << setfill('0') << i << ' ';
            cout << registers.read(i) << endl;
        }
        cout << "==========mainMemory Status==========" << endl;
        for (int j = 0; j < 256; ++j) {
            cout << uppercase << hex << setw(2) << setfill('0') << j << ' ';
            cout << memory.read(j) << endl;
        }
        cout << "=====================================" << endl;
        if(!display_vector.empty()){
            cout << endl << "=======Display screen=======" << endl;
            for (int i = 0; i < display_vector.size(); ++i) {
                cout << display_vector[i];
            }
            cout << endl << "============================" << endl;
        }
        cout << endl;
        cout << "Status in Program Counter: " << programCounter << endl;
        cout << "with Instruction Register: " << instructionRegister << endl;
    }

private:
    // convert integer value to string hex
    static string toHex(int value) {
        string hex;
        while (value > 0) {
            int r = value % 16;
            //remainder converted to hexadecimal digit
            char c = (r < 10) ? ('0' + r) : ('A' + r - 10);
            //hexadecimal digit added to start of the string
            hex = c + hex;
            value /= 16;
        }
        if(hex.size() == 1){
            hex = '0' + hex;
        }
        return hex;
    }

    // check valid step or not
    static bool isValidStep(const string& str) {
        for (char c : str) {
            // isxdigit checks if the character is hexadecimal number or not
            if (!isxdigit(c)) {
                return false;
            }
        }
        return true;
    }
};


int main() {
    // Start the program with choosing an address in main memory to start with then execute the instruction
    // and allow user to choose if start new program or not
    char c = 'y';
    while (c != 'n') {
        // Two booleans to control while loop
        bool isValidate;
        bool keepGoing = true;

        cout << "Enter start address (e.g. 2A)" << endl;
        string programCounter;
        cin >> programCounter;

        cout << "(1)Show status step by step\n(2)Show status after HALT" << endl;
        int choose;
        cin >> choose;

        if (choose == 1) {
            Machine machine(programCounter);
            machine.loadFile();

            while (keepGoing) {
                // if DecodeAndExecute or Fetch returns false after execution it, it will break this while loop
                isValidate = machine.Fetch();
                if (!isValidate){
                    break;
                }
                machine.outputState();
                keepGoing = machine.DecodeAndExecute();
            }
        }

        else if (choose == 2) {
            Machine machine(programCounter);
            machine.loadFile();

            while (keepGoing) {
                // if DecodeAndExecute or Fetch returns false after execution it, it will break this while loop
                isValidate = machine.Fetch();
                if (!isValidate){
                    break;
                }
                keepGoing = machine.DecodeAndExecute();
            }
            if(isValidate){
                machine.outputState();
            }
        }

        cout << "Load new program? (y/n)" << endl;
        cin >> c;
    }
    return 0;
}

