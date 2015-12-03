/*
 * MakeDir.cpp
 *
 *  Created on: 26-Nov-2015
 *      Author: Hongyi Guo
 */

#include "MakeDir.h"

/**
 *Checks whether the directory is already present in the file system. If not creates a new directory
 *entry
 *
 *@param argc Argument count
 *@param *argv[] Argument array from main function
 */

void MakeDir :: createDirectory(int argc, char *argv[]){
	if(checkParameters(argc, argv)){
		fileSystem.open("fsaccess",ios::binary | ios::in | ios::out);
		if(searchDirectoryEntries()){ //Searched Directory has been found
			cout <<"Directory exist in File System" <<endl;
			return;
		}else{
			if(allocateFreeDirectoryEntry())
                cout <<"New Directory Created" <<endl;
		}
		//fileSystem.close();
	}
}

/**
 * Used to check whether the parameter entered for mkdir command is valid
 *
 *@return true - If the parameter is valid for mkdir command
 *@return false - If the parameter is invalid
 */
bool MakeDir :: checkParameters(int argc, char *argv[]){
	bool validCP = FALSE;
	string name = argv[2];
	if(argc >3){
		cout <<"Invalid number of arguments for 'mkdir' " <<endl;
		validCP = FALSE;
	}
    else if((name.size()!=0) && (name.size()>13)){
		cout <<"!!Maximum number of character supported for Directory name is 13!!" <<endl;
		cout <<"!!Resizing " <<name.size() <<" characters ";
		name.resize(13);
		cout <<"to 13 characters:" <<name <<endl;
		setDirectoryName(name);
		validCP = TRUE;
	}else{
		setDirectoryName(name);
		validCP = TRUE;
	}
	return validCP;
}

/**
 * Searches whether the directory already exists in the file system
 *
 * @throws Exception when unkown block is reached, invalid block is traveresed
 * @return true - If the directory entry is present in the file system
 * @return false - If the directory entry is not present
 */
bool MakeDir :: searchDirectoryEntries(){
	bool valid = FALSE;
	int dataBlock;
	iNode searchDirectoryInode = {};
    try{

        if(fileSystem.is_open()){

            //Moving to inodes block
            fileSystem.seekg(BLOCK_SIZE);

            //Read the content of root node which is the first iNode in iNodes block
            fileSystem.read((char *)&searchDirectoryInode,sizeof(iNode));

            //Traversing through the data block of root node to search for free directory entry
            for(int i=0;i<8;i++){
                dataBlock = searchDirectoryInode.addr[i];
                //Accessing the root directory
                if(dataBlock != 0){
                    fileSystem.seekg(dataBlock);

                    //Checking for each directory entry
                    for(int j=0;j<numDirectoryEntry;j++){
                        Directory searchDir={};
                        fileSystem.read((char *)&searchDir,sizeof(Directory));

                        if(searchDir.inodeNumber !=0){
                            if(getDirectoryName().compare(searchDir.fileName) == 0){
                                valid = TRUE;
                                break;
                            }
                        }
                    }
                    if(valid) break;
                    dataBlock =0;
                }
                else
                {
                    valid = FALSE;
                    break;
                }
            }
        }
        else
        {
            cout<<"Could not find File System file" <<endl;
            valid = FALSE;
        }
    }
    catch(exception& e)
    {
		cout<<"Exception at searchDirectoryEntries " <<endl;
	}
    return valid;
}

/**
 * Allocates new directory entry, assigns inode & free data block for the new directory entry
 *
 * @throws Exception when unkown block is reached, invalid block is traveresed
 * @return true - If new directory entry is allocated in the file system
 * @return false - If new directory entry is not allocated
 */
bool MakeDir :: allocateFreeDirectoryEntry(){
    cout<<"Allocating FreeDirectoryEntry()"<<endl;
	
    bool valid = FALSE,breakJ=TRUE,breakI=TRUE;
	
    int dataBlock,currentCursorPos,freeInode,freeBlock,numInode;
	
    superBlock allocateSB = {};
	
    iNode allocateInode = {};
	
    Directory emptyDirEntry = {};
	
    try{

        if(fileSystem.is_open()){
            fileSystem.seekg(0);
            //Reading super block to know the number of inodes
            fileSystem.read((char *)&allocateSB,sizeof(superBlock));
            numInode= allocateSB.ninode;
            cout<<"Number of Inode in the system: "<<numInode<<endl;

            //Moving to inodes block
            fileSystem.seekg(BLOCK_SIZE);

            //Read the content of root node which is the first iNode in iNodes block
            cout<<"Reading root node"<<endl;                        //=======================
            fileSystem.read((char *)&allocateInode,sizeof(iNode));

            //Traversing through the data block of root node to search for free directory entry
            for(int i=0;i<8 && breakI;i++){
                dataBlock = allocateInode.addr[i];

                //Accessing the root directory
                if(dataBlock != 0){
                    cout<<"Accessing root directory"<<endl;
                    fileSystem.seekg(dataBlock);

                    //Checking for each directory entry
                    for(int j=0;j<numDirectoryEntry && breakJ;j++){
                        Directory dir = {};		//Reseting directory structure
                        fileSystem.read((char *)&dir,sizeof(Directory));
                        if(dir.inodeNumber ==0){

                            currentCursorPos = ((int)fileSystem.tellg()-(int)sizeof(Directory));
                            freeInode = getNextFreeInode(numInode);   //numInode : total #inode for the system
                                                                    //freeInode:the next available freeInode number
                            cout<<"Get next free inode number <= " <<freeInode << endl;
                            //freeBlock = getNextFreeBlock();
                            freeBlock = 61952;            //????????????????????????????

                            //Allocate the inode
                            iNode newInodeToAllocate = {};
                            
                            fileSystem.seekg(0);  //move file index to initial point
                            cout<<"initial position <= "<<fileSystem.tellg()<<endl;
                            fileSystem.seekg(BLOCK_SIZE * 2 + ((freeInode - 1) * (int)sizeof(iNode)));
                            
                            cout <<"currentCursorPos (the next available inode`s start position) <="<<fileSystem.tellg() <<endl;
                            
                            fileSystem.read((char *)&newInodeToAllocate,sizeof(iNode));
                            newInodeToAllocate.flags = (newInodeToAllocate.flags | 0xC0);   //????
                            
                            newInodeToAllocate.addr[0] = freeBlock;
                            
                            fileSystem.write((char *)&newInodeToAllocate,sizeof(iNode));

                            //Assigning directory name to directory entry
                            fileSystem.seekg(0);
                            fileSystem.seekg(currentCursorPos);
                            
                            cout <<"currentCursorPos =>"<<fileSystem.tellg() <<endl;
                            
                            dir.inodeNumber=freeInode;
                            strcpy(dir.fileName,getDirectoryName().c_str());
                            fileSystem.write((char *)&dir,sizeof(Directory));

                            //Format the free block to directory type
                            fileSystem.seekg(0);
                            fileSystem.seekg(freeBlock);
                            cout <<"currentCursorPos =>"<<fileSystem.tellg() <<endl;

                            //Setting './' character
                            cout<<"Setting './' character"<<endl;
                            Directory newEntry = {};
                            newEntry.inodeNumber=freeInode;
                            strcpy(newEntry.fileName,".");
                            fileSystem.write((char *)&newEntry,sizeof(Directory));

                            for(unsigned int j=2; j<=numDirectoryEntry; j++){
                                fileSystem.write((char *)&emptyDirEntry,sizeof(Directory));
                            }
                            valid = TRUE;
                            breakJ = FALSE;
                        }
                    }
                }
                else{
			//TODO add free block content here
                }
                if(valid) breakI = FALSE;
                dataBlock = 0;
            }
        }
    }
    catch(exception& e){
		cout<<"Exception at searchFreeDirectoryEntry " <<endl;
	}
    cout<<"here"<<endl;
	return valid;
}

/**
 * Finds an unallocated free iNode
 *
 * @throws Exception when unkown block is reached, invalid block is traveresed
 * @param numInode number of iNodes allocated to the file system
 * @return free inode number
 */
int MakeDir :: getNextFreeInode(int numInode){
	bool breakFromLoop = TRUE;	//To come out of loop when a free inode is found
	int freeNode;

	try{
		//Moving the cursor to the start of the file
		fileSystem.seekg(0);

		//Moving to inodes block
		fileSystem.seekg(BLOCK_SIZE);

		for(int i=1;i<=numInode && breakFromLoop;i++){
			iNode freeNodeSearch = {};
			fileSystem.read((char *)&freeNodeSearch,sizeof(iNode));
			//Checking whether inode is allocated or not
			if((freeNodeSearch.flags & 0x80) !=  0x80){
				freeNode = i;
				breakFromLoop = FALSE;
			}
		}
	}catch(exception& e){
		cout<<"Exception at getNextFreeInode " <<endl;
	}
	return freeNode;
}

/**
 * Finds a free block to be allocated for the new directory entry
 *
 * @throws Exception when unkown block is reached, invalid block is traveresed
 * @return free block
 */
int MakeDir :: getNextFreeBlock(void){
	int freeBlk,freeChainBlock;
	unsigned short freeHeadChain;
	superBlock freeBlockSuperBlock = {};
	
    try{
		//Moving the cursor to the start of the file
		fileSystem.seekg(0);

		//Reading the contents of super block
		fileSystem.read((char *)&freeBlockSuperBlock,BLOCK_SIZE);

		if(freeBlockSuperBlock.nfree == 1){
			freeChainBlock = freeBlockSuperBlock.free[--freeBlockSuperBlock.nfree]; //The header block number of the freeChainBlock

			fileSystem.seekg(freeChainBlock); //locate the header block of next freeChainBlock list
            
			fileSystem.read((char *)&freeHeadChain,2);  //read first 1 word(2 bytes) of the freeChainBlock which will contain the size of free array

			freeBlockSuperBlock.nfree = 100;    			//reset nfree to 100

			//reset the free array to new free list of new Super Block
			for(int k=0;k<100;k++){
				freeBlockSuperBlock.free[k] = freeHeadChain + k;
			}

			freeBlk = (int)freeBlockSuperBlock.free[--freeBlockSuperBlock.nfree];
		}
        
        else{
			freeBlk = (int)freeBlockSuperBlock.free[--freeBlockSuperBlock.nfree];
		}
	}
    catch(exception& e){
		cout<<"Exception at getNextFreeBlock " <<endl;
	}
    
    return freeBlk;
}

/**
 *Setter method to save the directory name input from the user
 *
 *@param name directory name
 */
void MakeDir :: setDirectoryName(string name){
	this->dirname = name;
}

/**
 * Getter method to get the directory name given by the user
 *
 * @return dirname Directory name
 */
string MakeDir :: getDirectoryName(void){
	return this->dirname;
}
