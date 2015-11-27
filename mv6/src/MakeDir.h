/*
 * MakeDir.h
 *
 *  Created on: 26-Nov-2015
 *      Author: sriee
 */

#ifndef MAKEDIR_H_
#define MAKEDIR_H_

#include "InitializeFS.h"

using namespace std;
class MakeDir {
	Log log1;
	string dirname;
	fstream fileSystem;
	iNode dirNode;
	Directory dir,emptyDirEntry;
	superBlock dirSB;
	int matchedInodeNum;
public:
	void setDirectoryName(string);
	string getDirectoryName(void);
	void setMatchedDirectoryInode(int);
	int getMatchedDirectoryInode(void);
	bool searchDirectoryEntries(void);
	bool searchFreeDirectoryEntry(void);
	int getNextFreeInode(void);
	int getNextFreeBlock(void);
	void createDirectory(int, char **);
	bool checkParameters(int, char **);
};

#endif /* MAKEDIR_H_ */
