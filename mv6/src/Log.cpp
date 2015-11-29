/*
 * Log.cpp
 *
 *  Created on: 24-Nov-2015
 *      Author: sriee
 */

#include "Log.h"
#include <iostream>

Log::Log() {
	// TODO Auto-generated constructor stub
	std::cout << "Logger is present in the code!!!" <<std::endl;
}

void Log::logger(int lineNumber, std::string comment){
	std::cout <<"***" <<lineNumber <<" " <<comment << std::endl;
}

void Log::logger(int lineNumber, std::string comment,int arg){
	std::cout <<"***" <<lineNumber <<" " <<comment <<" = " <<arg << std::endl;
}

void Log::logger(int lineNumber, std::string comment,std::string arg){
	std::cout <<"***" <<lineNumber <<" " <<comment <<" = " <<arg << std::endl;
}

void Log::logger(int lineNumber, std::string comment,float arg){
	std::cout.precision(3);
	std::cout <<"***" <<lineNumber <<" " <<comment <<" = " <<arg << std::endl;
}
