/*
 * Log.h
 *
 *  Created on: 24-Nov-2015
 *      Author: sriee
 */

#include <string>
#ifndef LOG_H_
#define LOG_H_

class Log {
public:
	Log();
	void logger(int lineNumber, std::string comment);
	void logger(int lineNumber, std::string comment,int arg);
	void logger(int lineNumber, std::string comment,float arg);
	void logger(int lineNumber, std::string comment,std::string arg);
};
#endif /* LOG_H_ */
