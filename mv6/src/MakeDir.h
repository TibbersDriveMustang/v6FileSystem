/*
 * MakeDir.h
 *
 *  Created on: 26-Nov-2015
 *      Author: sriee
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
	void createDirectory(int, char **);
	bool checkParameters(int, char **);
};

#endif /* MAKEDIR_H_ */
