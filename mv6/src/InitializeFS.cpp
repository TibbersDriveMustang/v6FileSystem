/*
 * InitializeFS.cpp
 *
 *  Created on: 24-Nov-2015
 *      Author: Hongyi Guo
 */

#include "InitializeFS.h"

/**
 *Used to check whether the parameters entered for inifs command are valid
 *
 *@param argc Argument count
 *@param *argv[] Argument array from main function
 *@return true - If parameters are valid for initfs command
 *@return false - If parameters are invalid
 */
bool InitializeFS :: checkParameters(int argc, char *argv[]){
	bool valid=false;
	if (argc < 5){
		cout <<"Insufficient number of arguments" <<endl;
		valid = false;
	}else{
		this->setFileSystemPath(argv[2]);
		this->setNumOfBlocks(atoi(argv[3]));
		this->setNumOfInodes(atoi(argv[4]));
		valid = true;
	}
	return valid;
}

/**
 * Initialized the super block ,inode block and data blocks of the file system
 *
 *@param argc :Argument count
 *@param *argv[] :Argument array
 *@throws Exception when unkown block is reached, invalid block is traveresed
 *
 */

void InitializeFS :: createFileSystem(int argc, char *argv[]){
    
    superBlock SB = {};
    iNode node = {},rootNode = {};
    Directory rootDirectory = {};
    unsigned short freeHeadChain;
    
    try{
        if(checkParameters(argc,argv)){
            file.open("fsaccess",ios::binary | ios::app);
            if (file.is_open()){
                //Initializing the file system
                SB.isize = getInodesBlock();

                //1 Block is the directory for root node and another block is the head of the free chain list
                SB.fsize = getFreeBlocks();
                SB.ninode = getNumOfInodes();
                SB.nfree = 100;
                
                for(int i=0;i<100;i++){
                    SB.free[i] = (getFreeBlocksIndex() + i)*BLOCK_SIZE;
                }
                
                file.write((char *)&SB,BLOCK_SIZE);

                //Setting up root node
                rootNode.flags = (rootNode.flags | 0xC0);       //?????
                
                rootNode.addr[0] = (1+ getInodesBlock())*BLOCK_SIZE;
                file.write((char *)&rootNode,getSizeOfInode());

                //Writing all inodes block to the file system
                for(int i=2;i<=getNumOfInodes();i++){
                    file.write((char *)&node,getSizeOfInode());
                }

                //Padding empty characters to complete block
                //multiple inodes will be put into one block,thus the last block
                //might have empty space need to be remain empty
                //1 block = 512 bytes
                //1 char = 1 byte
                
                if(calculateInodePadding() !=0 ){
                    char *iNodeBuffer = new char[calculateInodePadding()];
                    file.write((char *)&iNodeBuffer,calculateInodePadding());
                }

                //Writing Root Directory
                //Setting './' character
                rootDirectory.inodeNumber=1;
                strcpy(rootDirectory.fileName,".");
                file.write((char *)&rootDirectory,sizeof(rootDirectory));

                //Setting '..' character
                rootDirectory.inodeNumber=1;
                strcpy(rootDirectory.fileName,"..");
                file.write((char *)&rootDirectory,sizeof(rootDirectory));

                //Setting the remaining directory entries
                rootDirectory = {};
                for(int j=3; j<=numDirectoryEntry; j++){
                    file.write((char *)&rootDirectory,sizeof(rootDirectory));
                }

                //Empty Char buffer
                char *buffer = new char[BLOCK_SIZE];
                char *headChainBuffer;

                //Writing free data blocks
                //If the number of free blocks are <100 no need to set free head chain values
                if((getFreeBlocks() - getFreeBlocksIndex()) < 100){
                    for(int i=getFreeBlocksIndex();i<=getFreeBlocks();i++){
                        file.write((char *)&buffer,BLOCK_SIZE);
                    }
                }
                else {
                    //Checks for every 100th block and assigns the head chain values
                    for(int i=getFreeBlocksIndex();i<=getFreeBlocks();i++){
                        //To assign the head chain value link to the next free list . two condition are checked here
                        //Wether the cursor position is traversed 100 blocks starting from the index and
                        //Making sure the head chain don't point to blocks > than our free data blocks
                        if((i % 100 == getFreeBlocksIndex()) && ((getFreeBlocks() - i) >= 100)){
							freeHeadChain = i+100;
							file.write((char *)&freeHeadChain,2);
							headChainBuffer = new char[BLOCK_SIZE - 2];
							file.write((char *)&headChainBuffer,BLOCK_SIZE - 2);
                        }
                        else
                        {
                            file.write((char *)&buffer,BLOCK_SIZE);
                        }
                    }
                }
            }
            file.close();
        }
        else{
            return;
        }

    }
    
    catch(exception& e){
		cout <<"Exception at createFileSystem method" <<endl;
	}

}
/**
 * Caluculates the size of empty spaces that needs to be added to inode block, if there are any
 * hanging inodes they will be padded with spaces to nearest block size
 *
 * @return paddingSize empty spaces to be added to hanging inode block
 */

int InitializeFS :: calculateInodePadding(void){
	int paddingSize;
	paddingSize =  (getInodesBlock() * BLOCK_SIZE) - (getNumOfInodes() * getSizeOfInode());
	return paddingSize;
}

/*
 * @return Total Number of blocks for all inodes
 *  The calculated value is rounded up
 */

int InitializeFS :: getInodesBlock(void){
	float size_Inode, numInodesBlock, num_Inodes,blockSize;
	int totalBlock;
	size_Inode = getSizeOfInode();   //fixed size
	num_Inodes = getNumOfInodes();
	blockSize = BLOCK_SIZE;
	numInodesBlock = (size_Inode * num_Inodes)/blockSize;
	totalBlock = ceil(numInodesBlock);
	return totalBlock;
}

/**
 * Calculates the number of free data blocks. It eliminates the super block and the block allocated
 * for root directory
 *
 * @return freeBlockCount Count of free data blocks
 */
int InitializeFS :: getFreeBlocks(void){
	int freeBlockCount;
	freeBlockCount = (getNumOfBlocks() - (getInodesBlock() + 2));
	return freeBlockCount;
}

/**
 * Calculates the starting index for free data blocks.'1' is added to point to the starting position
 * of data blocks
 *
 * @return freeBlockIndex Starting index where the free data blocks start
 */
int InitializeFS :: getFreeBlocksIndex(void){
	int freeBlockIndex;
	freeBlockIndex = getNumOfBlocks() - getFreeBlocks() + 1;
	return freeBlockIndex;
}

/**
 * Setter method to set the File system path to place the modified v6 file system
 *
 * @param path File system path given as command line argument
 */
void InitializeFS :: setFileSystemPath(string path){
	this->fsPath = path;
}

/**
 * Getter method for File system path to place the modified v6 file system
 *
 * @return fsPath File system path
 */
string InitializeFS :: getFileSystemPath(void){
	return this->fsPath;
}

/**
 * Setter method to set the Number of blocks allocated for the modified v6 file system
 *
 * @param n1 Number of blocks
 */
void InitializeFS :: setNumOfBlocks(int n1){
	this->numOfBlocks = n1;
}

/**
 * Getter method for getting the total number of blocks allocated to the modified v6 file system
 *
 * @return numOfBlocks Total number of blocks
 */
int InitializeFS :: getNumOfBlocks(void){
	return this->numOfBlocks;
}

/**
 * Setter method to set the Number of inodes used by the modified v6 file system
 *
 * @param n2 Number of iNodes
 */
void InitializeFS :: setNumOfInodes(int n2){
	this->numOfinodes = n2;
}


/*
  * @return Total number of inodes
 */
int InitializeFS :: getNumOfInodes(void){
	return this->numOfinodes;
}

/**
 * Getter method for getting size of inodes
 *
 * @return size of inode (fixed)
 */
int InitializeFS :: getSizeOfInode(void){
	return sizeof(iNode);
}

/*
 * Code used for checking (Remove)
 */
void InitializeFS :: readBlocks(){
	iNode node;
	ifstream infile;
	infile.open("fsaccess",ios::binary);
	if(infile.is_open()){
		infile.seekg(BLOCK_SIZE);
		for(int l=1;l<=300;l++){
		infile.read((char *)&node,getSizeOfInode());
		cout <<"Flag value = " <<node.flags <<endl;
		}
	}
	infile.close();
}
