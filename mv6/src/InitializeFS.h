/*
 * InitializeFS.h
 *
 *  Created on: 24-Nov-2015
 *      Author: sriee
 */

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "Constants.h"
#include "Log.h"

#ifndef INITIALIZEFS_H_
#define INITIALIZEFS_H_
using namespace std;

class InitializeFS {
	Log log;
	ofstream file;
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
	int getSizeOfInode(void);
	void createFileSystem(int argc, char *argv[]);
	int getInodesBlock(void);
	int getFreeBlocks(void);
	void readBlocks(void);	//Used to test the written data
};

#endif /* INITIALIZEFS_H_ */
