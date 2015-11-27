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
	unsigned short free[100];
	unsigned short ninode;		//Not used
	unsigned short inode[100];	//Not used
	char flock;					//Not used
	char ilock;					//Not used
	char fmod;					//Not used
	unsigned short time[2];		//Not used
} superBlock;

/*
 * I-Nodes structure
 */
typedef struct {
	unsigned short flags;
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
typedef struct{
	unsigned short inodeNumber;
	char fileName[14];
}Directory;

/**
 * Constant number of directory entries in a data block
 */
const int numDirectoryEntry = (BLOCK_SIZE/sizeof(Directory));
