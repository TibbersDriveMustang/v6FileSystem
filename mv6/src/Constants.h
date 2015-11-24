#include <string>

const std::string INITFS = "initfs";
const std::string CP_IN = "cpin";
const std::string CP_OUT = "cpout";
const std::string MK_DIR = "mkdir";
const std::string Q = "q";

typedef enum{
	initfs=1,
	cpin,
	cpout,
	mkdir,
	q
} CommandTypes;

