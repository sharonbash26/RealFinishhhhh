#ifndef OPEN_DATA_SERVER_COMMAND_H
#define OPEN_DATA_SERVER_COMMAND_H

#include "Command.h"
#include "admin.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <map>
#include <sys/socket.h>

class OpenDataServerCommand : public Command
{
	std::string port;
	std::string frequency;
	int newsockfdGeneral;
	std::vector<std::string> listOfPathes;
	std::map <std::string, std::string> varMap;
    std::map <std::string, std::string> bindMap;
	std::vector<std::string> bindedVariables;
	std::string restOfValue;
	int lastKey;
public:
	OpenDataServerCommand();
	int execute();
	std::map<std::string, std::string> updateValues();
	void getFirstArgument(std::string arg){this->port = arg;}
	void getSecondArgument(std::string arg){this->frequency = arg;}
	void getThirdArgument(std::string arg){}
	void getConnection();
	bool isStringDouble(std::string str);
	std::map<std::string, std::string> createSemulatorValuesMape(std::string buffer);
		void setVarsAndBinds(std::map <std::string, std::string> _varMap,
		std::map <std::string, std::string> _bindMap,
		std::vector<std::string> _bindedVariables){
			varMap = _varMap;
			bindMap = _bindMap;
			bindedVariables = _bindedVariables;
	}
};

#endif