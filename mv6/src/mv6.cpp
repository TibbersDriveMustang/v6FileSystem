//============================================================================
// Name        : mv6.cpp
// Author      : Sriee
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <cstdlib>
#include "Constants.h"
#include "Log.h"

using namespace std;

Log log;

class Entry{
	string command;
	int commandType=0;
public:
	Entry(string command){
			this->command = command;
	}
	bool isValidCommand(string command);
	void printCommand(void);
	void setCommandType(int value);
	int getCommandType();

};

void Entry::printCommand(){
	cout <<"The Entered command is = " <<this->command <<endl;
}

void Entry::setCommandType(int value){
	this->commandType = value;
}

int Entry::getCommandType(){
	return this->commandType;
}
bool Entry::isValidCommand(string command){
	bool valid = false;
	if (INITFS.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(initfs));
	}else if (CP_IN.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(cpin));
	}else if (CP_OUT.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(cpout));
	}else if (MK_DIR.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(mkdir));
	}else if (Q.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(q));
	}else{
		valid = false;
	}
	return valid;
}

class InitializeFS{
public:
	void checkParameters(int argc, char *argv[]);
};

void InitializeFS :: checkParameters(int argc, char *argv[]){
	string fsPath;
	int numOfBlocks,numOfinodes;
	if (argc < 5){
		cout <<"!!Insufficient number of arguments!!" <<endl;
	}else{
		fsPath = argv[2];
		numOfBlocks = atoi(argv[3]);
		numOfinodes = atoi(argv[4]);
		log.logger(82,"Argument Count",argc);
		log.logger(83,"Path",fsPath);
		log.logger(84,"First argument",numOfBlocks);
		log.logger(85,"Second argument",numOfinodes);
	}
}

int main(int argc,char *args[]) {

	string firstArgument=args[1];
	Entry entry(firstArgument);

	if(entry.isValidCommand(firstArgument)){
		cout<<"!!Entered Valid Command!!" <<endl;
	}else{
		cout<<"!!Entered Invalid Command!!\nTry Again...." <<endl;
		exit(0);
	}
	switch(entry.getCommandType()){
		case initfs: InitializeFS fs;
					 fs.checkParameters(argc,args);
					 break;
		case cpin:  entry.printCommand(); break;
		case cpout: entry.printCommand(); break;
		case mkdir: entry.printCommand(); break;
		default: cout<<"!!Entered Invalid Command!!\nTry Again...." <<endl; break;
	}
	return 0;
}
