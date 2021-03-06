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
    
    int capacity();
    
    vector<double> probaVector();
    
    vector<vector<int> > durationScenarTask();
    
    vector<int> benefVector();
    
    vector<int> subTreatedCost();
    
    void printParser();
    
private:
    int _nbCommands;
    int _nbScenario;
    int _capacity;
    
    vector<int> _benefVector;
    vector<int> _subTreatedCost;
    
    vector<double> _probaVector;
    vector<vector<int> > _durationScenarTask; 
};

#endif	/* PARSER_H */

