#include <string>

/*
 * Constant assigned to the command names supported by modified v6 file system
 */
const std::string INITFS = "initfs";
const std::string CP_IN = "cpin";
const std::string CP_OUT = "cpout";
const std::string MK_DIR = "mkdir";
const std::string Q = "q";

/*
 * Constant assigned for block size
 */
const int BLOCK_SIZE=512;

/**
 * Constant Boolean values
 */
const bool TRUE = true;
const bool FALSE = false;

/*
 * Commands that are supported in the modified v6 file system
 */
typedef enum{
	initfs=1,
	cpin,
	cpout,
	mkdir,
	q
} CommandTypes;

/*
 * Super Block structure
 */
typedef struct {
	unsigned short isize;		//Number of blocks denoted to inodes
	unsigned short fsize;		//Total number of blocks
	unsigned short nfree;
	unsigned short free[100];   //free[0] is block number of the head of another free-chain block
	unsigned short ninode;		//Not used
	unsigned short inode[100];	//Not used
	char flock;					//Not used
	char ilock;					//Not used
	char fmod;					//Not used
	unsigned short time[2];		//Not used
} superBlock;

//I-Nodes structure
//32 bytes long
typedef struct {
	unsigned short flags;       //indicate the type of i-node
	char nlinks;			 	//Not used
	char uid;					//Not used
	char gid;					//Not used
	char size0;
	unsigned short size1;
	unsigned short addr[8];
	unsigned short actime[2];	//Not used
	unsigned short modtime[2];	//Not used
} iNode;

//Directory Structure
//16 bytes long
//first 2 bytes -> i-number(the number of i-node)
typedef struct{
	unsigned short inodeNumber;  //first 2 bytes
	char fileName[14]; //bytes 2 - 15  null padded on the right
}Directory;

/**
 * Constant number of directory entries in a data block
 */
const int numDirectoryEntry = (BLOCK_SIZE/sizeof(Directory));

