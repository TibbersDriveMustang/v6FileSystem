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
#include <fstream>
#include <cstring>
#include <vector>
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
	string fsPath;
	int numOfBlocks,numOfinodes;
public:
	bool checkParameters(int argc, char *argv[]);
	void setFileSystemPath(string path);
	string getFileSystemPath(void);
	void setNumOfBlocks(int n1);
	int getNumOfBlocks(void);
	void setNumOfInodes(int n2);
	int getNumOfInodes(void);
	void createFileSystem(int argc, char *argv[]);
};

bool InitializeFS :: checkParameters(int argc, char *argv[]){
	bool valid=false;
	if (argc < 5){
		cout <<"!!Insufficient number of arguments!!" <<endl;
		valid = false;
	}else{
		this->setFileSystemPath(argv[2]);
		this->setNumOfBlocks(atoi(argv[3]));
		this->setNumOfInodes(atoi(argv[4]));
		valid = true;
	}
	return valid;
}

void InitializeFS :: setFileSystemPath(string path){
	this->fsPath = path;
}

string InitializeFS :: getFileSystemPath(void){
	return this->fsPath;
}

void InitializeFS :: setNumOfBlocks(int n1){
	this->numOfBlocks = n1;
}

int InitializeFS :: getNumOfBlocks(void){
	return this->numOfBlocks;
}

void InitializeFS :: setNumOfInodes(int n2){
	this->numOfinodes = n2;
}

int InitializeFS :: getNumOfInodes(void){
	return this->numOfinodes;
}

void InitializeFS :: createFileSystem(int argc, char *argv[]){
	ofstream fs;
	try{
	 if(checkParameters(argc,argv)){
	 fs.open("fsaccess",ios::binary | ios::out | ios::app);
		if (fs.is_open()){
			vector<char> empty(BLOCK_SIZE,0);
		    for(int i = 0; i < getNumOfBlocks(); i++){
				fs.write(&empty[0], empty.size());
		    }
		}
	 }else{
		exit(0);
	 }
	 fs.close();
	}catch(exception e){
		cout <<"Exception at createFileSystem method" <<endl;
	}

}

int main(int argc,char *args[]) {

	string firstArgument=args[1];
	Entry entry(firstArgument);
	InitializeFS fs;
	if(entry.isValidCommand(firstArgument)){
		cout<<"!!Entered Valid Command!!" <<endl;
	}else{
		cout<<"!!Entered Invalid Command!!\nTry Again...." <<endl;
		exit(0);
	}
	switch(entry.getCommandType()){
		case initfs:
					 fs.createFileSystem(argc,args);
					 break;
		case cpin:  entry.printCommand(); break;
		case cpout: entry.printCommand(); break;
		case mkdir: entry.printCommand(); break;
		default: cout<<"!!Entered Invalid Command!!\nTry Again...." <<endl; break;
	}
	return 0;
}
