/*
 * MakeDir.h
 *
 *  Created on: 26-Nov-2015
 *      Author: Hongyi Guo
 */

#ifndef MAKEDIR_H_
#define MAKEDIR_H_
#include "InitializeFS.h"

class MakeDir {
	string dirname;
	fstream fileSystem;
public:
	void setDirectoryName(string);
	string getDirectoryName(void);
	bool searchDirectoryEntries(void);
	bool allocateFreeDirectoryEntry(void);
	int getNextFreeInode(int);
	int getNextFreeBlock(void);
	void createDirectory(int argc, char *argv[]);
	bool checkParameters(int argc, char *argv[]);
};

#endif /* MAKEDIR_H_ */
