/* 
 * File:   Parser.cpp
 * Author: yoannjvr
 *
 * Created on 16 décembre 2015, 13:48
 */

#include <cstdlib>
#include "Parser.h"

using namespace std;

Parser::Parser() {
}

Parser::Parser(const string & filename) {
}

Parser::nbCommands() {
    return _nbCommands;
}

Parser::nbScenario() {
    return _nbScenario;
}

Parser::benefVector(){
    return _benefVector;
}

Parser::subTreatedCost(){
    return _subTreatedCost;
}

Parser::costScenarTask(){
    return _costScenarTask;
}

Parser::probaVector(){
    return _probaVector();
}






