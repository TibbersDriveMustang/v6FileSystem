/*
 * MakeDir.cpp
 *
 *  Created on: 26-Nov-2015
 *      Author: sriee
 */

#include "MakeDir.h"

void MakeDir :: createDirectory(int argc, char *argv[]){
	if(checkParameters(argc, argv)){
		if(searchDirectoryEntries()){ //Searched Directory has been found
			cout <<"!!Directory present in mv6 File System!!" <<endl;
			return;
		}else{
			if(searchFreeDirectoryEntry())
			cout <<"!!New Directory Entry allocated!!" <<endl;
			return;
		}
	}
}

bool MakeDir :: checkParameters(int argc, char *argv[]){
	bool valid = FALSE;
	string name = argv[2];
	if(argc >3){
		cout <<"!!Entered Invalid number of arguments!!!" <<endl;
		valid = FALSE;
	}else if((name.size()!=0) && (name.size()>14)){
		cout <<"!!Maximum number of character supported for Directory name is 14!!" <<endl;
		cout <<"!!Resizing " <<name.size() <<" characters ";
		name.resize(14);
		cout <<"to 14 characters:" <<name <<endl;
		setDirectoryName(name);
		valid = TRUE;
	}else{
		setDirectoryName(name);
		valid = TRUE;
	}
	return valid;
}

bool MakeDir :: searchDirectoryEntries(){
	bool valid = FALSE;
	int dataBlock;

	try{
	fileSystem.open("fsacess",ios::binary | ios::in | ios::out);

	//Moving to inodes block
	fileSystem.seekg(BLOCK_SIZE);

	//Read the content of root node which is the first iNode in iNodes block
	fileSystem.read((char *)&dirNode,sizeof(iNode));

	//Traversing through the data block of root node to search for free directory entry
	for(int i=0;i<8;i++){
	dataBlock = dirNode.addr[i];
		//Accessing the root directory
		if(dataBlock != 0){
			fileSystem.seekg(dataBlock);

			//Checking for each directory entry
			for(int j=0;j<numDirectoryEntry;j++){
				fileSystem.read((char *)&dir,sizeof(dir));
				if(dir.inodeNumber !=0){
					if(getDirectoryName().compare(dir.fileName) == 0){
						setMatchedDirectoryInode(dir.inodeNumber);
						valid = TRUE;
						break;
					}
				}
			}
		}else{
			valid = FALSE;
			break;
		}
	}
	}catch(exception& e){
		cout<<"Exception at searchDirectoryEntries " <<endl;
	}
return valid;
}

bool MakeDir :: searchFreeDirectoryEntry(){
	bool valid = FALSE;
	int dataBlock,currentCursorPos,freeInode,freeBlock;

	try{
	//Moving the cursor to the start of the file
	fileSystem.seekg(0);

	//Moving to inodes block
	fileSystem.seekg(BLOCK_SIZE);

	//Read the content of root node which is the first iNode in iNodes block
	fileSystem.read((char *)&dirNode,sizeof(iNode));

	//Traversing through the data block of root node to search for free directory entry
	for(int i=0;i<8;i++){
		dataBlock = dirNode.addr[i];

		//Accessing the root directory
		if(dataBlock != 0){
			fileSystem.seekg(dataBlock);

			//Checking for each directory entry
			for(int j=0;j<numDirectoryEntry;j++){
				fileSystem.read((char *)&dir,sizeof(dir));
				if(dir.inodeNumber !=0){
					if(getDirectoryName().compare(dir.fileName) == 0){
						setMatchedDirectoryInode(dir.inodeNumber);
						valid = TRUE;
						break;
					}
				}else{
					currentCursorPos = ((int)fileSystem.tellg()-(int)sizeof(dir));
					freeInode = getNextFreeInode();
					freeBlock = getNextFreeBlock();

					//Allocate the inode
					fileSystem.seekg(0);
					fileSystem.seekg(BLOCK_SIZE);
					fileSystem.seekg(freeInode * sizeof(iNode));
					fileSystem.read((char *)&dirNode,sizeof(iNode));
					dirNode.flags = dirNode.flags | 0x1000000 ;
					dirNode.addr[0] = freeBlock;

					//Assigning directory name to directory entry
					fileSystem.seekg(0);
					fileSystem.seekg(currentCursorPos);
					dir.inodeNumber=freeInode;
					strcpy(dir.fileName,getDirectoryName().c_str());
					fileSystem.write((char *)&dir,sizeof(dir));

					//Format the free block  to directory type
					fileSystem.seekg(0);
					fileSystem.seekg(freeBlock * BLOCK_SIZE);

					//Setting './' character
					dir.inodeNumber=freeInode;
					strcpy(dir.fileName,".");
					fileSystem.write((char *)&dir,sizeof(dir));

					//Setting '..' character
					dir.inodeNumber=freeInode;
					strcpy(dir.fileName,"..");
					fileSystem.write((char *)&dir,sizeof(dir));

					for(unsigned int j=3; j<=numDirectoryEntry; j++){
						fileSystem.write((char *)&emptyDirEntry,sizeof(emptyDirEntry));
					}
				}
			}
		}else{
			//TODO add free block content here
		}
	}
	}catch(exception& e){
		cout<<"Exception at searchFreeDirectoryEntry " <<endl;
	}
	return valid;
}

int MakeDir :: getNextFreeInode(){
	int freeNode;
	InitializeFS iFS;
	try{
	//Moving the cursor to the start of the file
	fileSystem.seekg(0);

	//Moving to inodes block
	fileSystem.seekg(BLOCK_SIZE);

	for(int i=0;i<iFS.getFreeBlocksIndex();i++){
		fileSystem.read((char *)&dirNode,sizeof(iNode));
		//Checking whether inode is allocated or not
		if((dirNode.flags & 0x1000000) == 1){
			//inode is allocated
			continue;
		}else{
			freeNode = i;
			break;
		}
	}
	}catch(exception& e){
		cout<<"Exception at getNextFreeInode " <<endl;
	}
	return freeNode;
}

int MakeDir :: getNextFreeBlock(void){
	int freeBlock,freeChainBlock;
	unsigned short freeHeadChain;
	if(dirSB.nfree == 1){
		freeChainBlock = dirSB.free[--dirSB.nfree];
		//Head of the chain points to next head of chain free list
		fileSystem.seekg(freeChainBlock);
		fileSystem.read((char *)&freeHeadChain,2);

		//reset nfree to 100
		dirSB.nfree = 100;

		//reset the free array to new free list
		for(int k=0;k<100;k++){
			dirSB.free[k] = freeHeadChain + k;
		}
		freeBlock = dirSB.free[--dirSB.nfree];
	}else{
		freeBlock = dirSB.free[--dirSB.nfree];
	}
	return freeBlock;
}

void MakeDir :: setMatchedDirectoryInode(int num){
	this->matchedInodeNum = num;
}

int MakeDir :: getMatchedDirectoryInode(){
	return this->matchedInodeNum;
}
void MakeDir :: setDirectoryName(string name){
	this->dirname = name;
}

string MakeDir :: getDirectoryName(void){
	return this->dirname;
}
