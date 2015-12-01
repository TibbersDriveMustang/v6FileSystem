//============================================================================
// Name        : mv6.cpp
// Author      : Hongyi Guo
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
	}
    else if (CP_IN.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(cpin));
	}
    else if (CP_OUT.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(cpout));
	}
    else if (MK_DIR.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(mkdir));
	}
    else if (Q.compare(command)==0){
		valid = true;
		this->setCommandType(static_cast<int>(q));
	}
    else{
		valid = false;
	}
	
    return valid;
}


int main(int argc,char *args[]) {

	string firstArgument=args[1];
	//Creating class instances
	Entry entry(firstArgument);
	
    InitializeFS FS;
	
    MakeDir MKD;
	
    if(entry.isValidCommand(firstArgument)){
		cout<<"Valid Command" <<endl;
	}
    else{
		cout<<"Invalid Command" <<endl;
		exit(0);
	}
    
	switch(entry.getCommandType()){
		case initfs:
            FS.createFileSystem(argc,args);
            cout <<"Created File System: "<<endl;
            cout <<"Name: "<<args[1]<<endl;
            cout <<"Total Number of Blocks: "<<args[2]<<endl;
            cout <<"Total NUmber of i-nodes: "<<args[3]<<endl;
            FS.readBlocks();
            break;
		case cpin:  entry.printCommand(); break;
		case cpout: entry.printCommand(); break;
		case mkdir: MKD.createDirectory(argc,args);
					break;
		default: cout<<"Invalid Command" <<endl; break;
	}

	return 0;
}
