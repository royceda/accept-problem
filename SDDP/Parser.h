/* 
 * File:   newfile.h
 * Author: yoannjvr
 *
 * Created on 16 décembre 2015, 13:49
 */

#ifndef PARSER_H
#define	PARSER_H

#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

class Parser{
public:
    Parser();
    
    Parser(const string & filename);
    
    int nbCommands();
    
    int nbScenario();
    
    vector<int> probaVector();
    
    vector<vector<int> > costScenarTask();
    
    vector<int> benefVector();
    
    vector<int> subTreatedCost();
    
private:
    int _nbCommands;
    int _nbScenario;
    
    vector<int> _benefVector;
    vector<int> _subTreatedCost;
    
    vector<int> _probaVector;
    vector<vector<int> > _costScenarTask; 
};

#endif	/* PARSER_H */

