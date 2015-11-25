/*
 * InitializeFS.cpp
 *
 *  Created on: 24-Nov-2015
 *      Author: sriee
 */

#include "InitializeFS.h"

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

void InitializeFS :: createFileSystem(int argc, char *argv[]){
superBlock sb;
iNode node;
try{
	 if(checkParameters(argc,argv)){
	 file.open("fsaccess",ios::binary | ios::app);
		if (file.is_open()){
			//Initializing the file system
			sb.isize = getInodesBlock();
			sb.nfree = 100;
			file.write((char *)&sb,BLOCK_SIZE);
			log.logger(35,"Cursor Position",(int)file.tellp());
			//Writing inodes block to the file system
			for(int i=1;i<=getInodesBlock();i++){
				file.write((char *)&node,getSizeOfInode());
			}
			log.logger(41,"Cursor Position",(int)file.tellp());
			//Empty Char buffer
			char *buffer = new char[BLOCK_SIZE];
			//Writing free data blocks
			for(int i=1;i<=getFreeBlocks();i++){
			   file.write((char *)&buffer,BLOCK_SIZE);
			}
			log.logger(47,"Cursor Position",(int)file.tellp());
		}
	 file.close();
	 }else{
		exit(0);
	 }

	}catch(exception& e){
		cout <<"Exception at createFileSystem method" <<endl;
	}

}

int InitializeFS :: getInodesBlock(void){
	float sizeOfInode, numInodesBlock, numOfInodes,blockSize;
	int inPB;
	sizeOfInode = getSizeOfInode();
	numOfInodes = getNumOfInodes();
	blockSize = BLOCK_SIZE;
	numInodesBlock = (sizeOfInode * numOfInodes)/blockSize;
	inPB = ceil(numInodesBlock);
	return inPB;
}

int InitializeFS :: getFreeBlocks(void){
	int freeBlockCount;
	freeBlockCount = (getNumOfBlocks() - (getInodesBlock() + 1));
	return freeBlockCount;
}
void InitializeFS :: readBlocks(){
	superBlock testsb;
	ifstream infile;
	infile.open("fsaccess",ios::binary);
	if(infile.is_open()){
		infile.read((char *)&testsb,BLOCK_SIZE);
		cout <<"\nRead File sucessfully\n" <<endl;
		cout <<"isize struct value = " <<testsb.isize <<endl;
	}
	infile.close();
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

int InitializeFS :: getSizeOfInode(void){
	return sizeof(iNode);
}
