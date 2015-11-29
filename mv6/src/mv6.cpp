//============================================================================
// Name        : mv6.cpp
// Author      : Sriee
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "MakeDir.h"

using namespace std;

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


int main(int argc,char *args[]) {

	string firstArgument=args[1];
	//Creating class instances
	Entry entry(firstArgument);
	InitializeFS fs;
	MakeDir mkd;
	if(entry.isValidCommand(firstArgument)){
		cout<<"!!Entered Valid Command!!" <<endl;
	}else{
		cout<<"!!Entered Invalid Command!!\nTry Again...." <<endl;
		exit(0);
	}
	switch(entry.getCommandType()){
		case initfs: fs.createFileSystem(argc,args);
					 cout <<"!!!Created File System Successfully!!!";
					 fs.readBlocks();
					 break;
		case cpin:  entry.printCommand(); break;
		case cpout: entry.printCommand(); break;
		case mkdir: mkd.createDirectory(argc,args);
					break;
		default: cout<<"!!Entered Invalid Command!!\nTry Again...." <<endl; break;
	}

	return 0;
}
