#include "OpenDataServerCommand.h"

OpenDataServerCommand::OpenDataServerCommand() {

	this->lastKey = 0;

    this->listOfPathes.push_back("\"/instrumentation/airspeed-indicator/indicated-speed-kt\"");
    this->listOfPathes.push_back("\"/instrumentation/altimeter/indicated-altitude-ft\"");
    this->listOfPathes.push_back("\"/instrumentation/altimeter/pressure-alt-ft\"");
    this->listOfPathes.push_back("\"/instrumentation/attitude-indicator/indicated-pitch-deg\"");
    this->listOfPathes.push_back("\"/instrumentation/attitude-indicator/indicated-roll-deg\"");
    this->listOfPathes.push_back("\"/instrumentation/attitude-indicator/internal-pitch-deg\"");
    this->listOfPathes.push_back("\"/instrumentation/attitude-indicator/internal-roll-deg\"");
    this->listOfPathes.push_back("\"/instrumentation/encoder/indicated-altitude-ft\"");
    this->listOfPathes.push_back("\"/instrumentation/encoder/pressure-alt-ft\"");
    this->listOfPathes.push_back("\"/instrumentation/gps/indicated-altitude-ft\"");
    this->listOfPathes.push_back("\"/instrumentation/gps/indicated-ground-speed-kt\"");
    this->listOfPathes.push_back("\"/instrumentation/gps/indicated-vertical-speed\"");
    this->listOfPathes.push_back("\"/instrumentation/heading-indicator/indicated-heading-deg\"");
    this->listOfPathes.push_back("\"/instrumentation/magnetic-compass/indicated-heading-deg\"");
    this->listOfPathes.push_back("\"/instrumentation/slip-skid-ball/indicated-slip-skid\"");
    this->listOfPathes.push_back("\"/instrumentation/turn-indicator/indicated-turn-rate\"");
    this->listOfPathes.push_back("\"/instrumentation/vertical-speed-indicator/indicated-speed-fpm\"");
    this->listOfPathes.push_back("\"/controls/flight/aileron\"");
    this->listOfPathes.push_back("\"/controls/flight/elevator\"");
    this->listOfPathes.push_back("\"/controls/flight/rudder\"");
    this->listOfPathes.push_back("\"/controls/flight/flaps\"");
    this->listOfPathes.push_back("\"/controls/engines/current-engine/throttle\"");
 //   this->listOfPathes.push_back("\"/controls/engines/engine/throttle\"");
    this->listOfPathes.push_back("\"/engines/engine/rpm\"");
}

bool OpenDataServerCommand::isStringDouble(std::string str) {

    if (str.length() == 0) {
        return false;
    }

    int numOfDots = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] != '.'&& str[i] != '-' && !isdigit(str[i])) {

            return false;
        }

        if (str[i] == '-' && i != 0) {

            return false;
        }

        if (str[i] == '.') {
            numOfDots++;
            if (i == 0) {
                return false;
            }
            if (i == str.length() - 1) {
                return false;
            }
            if (numOfDots > 1) {
                return false;
            }
        }
    }

    return true;
}

std::map<std::string, std::string> OpenDataServerCommand
                                ::createSemulatorValuesMape(std::string buffer) {

    std::map<std::string, std::string> simulatorValuesMap;

    int numOfCurKey = this->lastKey;
    std::string curValue = this->restOfValue;
    this->restOfValue.clear();
    int i = 0;

    for (i = 0; i < buffer.length(); i++) {
        if (buffer[i] != '-' && buffer[i] != ',' && buffer[i] != '.' && !isdigit(buffer[i])) {
            break;
        }
        if (buffer[i] == ',' || buffer[i] == '\n') {

            if (this->isStringDouble(curValue)) {
                simulatorValuesMap[this->listOfPathes[numOfCurKey]] = curValue;
            }

            else {
                simulatorValuesMap[this->listOfPathes[numOfCurKey]] = "$";
            }
            curValue.clear();

            if (numOfCurKey == this->listOfPathes.size() - 1) {
                numOfCurKey = -1;
            }
            numOfCurKey++;
            continue;
        }

        else {
            curValue += buffer[i];
        }
    }

    this->lastKey = numOfCurKey;
    
    if (this->isStringDouble(curValue) && this->lastKey == this->listOfPathes.size() - 1) {
        simulatorValuesMap[this->listOfPathes[numOfCurKey]] = curValue;
        this->restOfValue.clear();
        this->lastKey = 0;
    }
    else {
        this->restOfValue = curValue;
    }

    return simulatorValuesMap;
}

int OpenDataServerCommand::execute() {

    return 3;
}

std::map<std::string, std::string> OpenDataServerCommand::updateValues() {
    int n;
    char buffer[256];

    /* If connection is established then start communicating */
   bzero(buffer,256);
   n = read( this->newsockfdGeneral, buffer, 255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   std::string bufferStr = buffer;
    std::map<std::string, std::string> simulatorValuesMap 
            = this->createSemulatorValuesMape(bufferStr);

    for (int i = 0; i < this->bindedVariables.size(); i++) {
        std::string nameOfVar = bindedVariables[i];
        std::string linkOfVar = this->bindMap[nameOfVar];
        std::string updatedValueofVar = simulatorValuesMap[linkOfVar];
        if (updatedValueofVar == "$" || !this->isStringDouble(updatedValueofVar)) {
               continue;
        }
        varMap[nameOfVar] = updatedValueofVar;
    }
    
    return varMap;
}

void OpenDataServerCommand::getConnection() {

    std::cout<<"Waiting for connection of FlightGear...\n";
   int sockfd, newsockfd, portno, clilen;
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = std::stod(this->port);
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);
	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   this->newsockfdGeneral = newsockfd;

    std::cout<<"Enter any char if FlightGear has finished the loading:\n";
    std::getchar();

}