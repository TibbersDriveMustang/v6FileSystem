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
	unsigned short isize;
	unsigned short fsize;
	unsigned short nfree;
	unsigned short free[100];
	unsigned short ninode;
	unsigned short inode[100];
	char flock;
	char ilock;
	char fmod;
	unsigned short time[2];
} superBlock;

/*
 * I-Nodes structure
 */
typedef struct {
	unsigned short flags;
	char nlinks;
	char uid;
	char gid;
	char size0;
	unsigned short size1;
	unsigned short addr[8];
	unsigned short actime[2];
	unsigned short modtime[2];
} iNode;
