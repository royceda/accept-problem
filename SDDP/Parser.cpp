#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>

#include <cstdio>

#include "Parser.h"

using namespace std;

Parser::Parser() {
}

Parser::Parser(const string & filename) {
    ifstream file(filename.c_str(), ios::in);
    if (file) {
        string line;
        istringstream ssline(line);

        ssline.clear();
        getline(file, line);
        ssline.str(line);

        ssline >> _nbCommands >> _nbScenario >> _capacity;

        double proba;
        proba = 1.0 / double(_nbScenario);
        
        int ite = 0;
        while (ite < _nbCommands) {
            ssline.clear();
            getline(file, line);
            ssline.str(line);

            int tmp;
            int tmpBenef;
            int tmpCost;
            ssline >> tmp >> tmpBenef >> tmpCost;

            _benefVector.push_back(tmpBenef);
            _subTreatedCost.push_back(tmpCost);

            ssline.clear();
            getline(file, line);
            ssline.str(line);

            int i = 0;
            vector<int> tmpVector;
            int tmpScenarCost;
                    
            while ((i < _nbScenario) && (ssline)) {
                ssline >> tmpScenarCost;
                tmpVector.push_back(tmpScenarCost);
                i++;
            }
            
            _durationScenarTask.push_back(tmpVector);
            tmpVector.clear();
            
            _probaVector.push_back(proba);

            ite++;
        }

        file.close();
    } else
        cerr << "Impossible to open the file\n";

}

void Parser::printParser() {
    cout << _nbCommands << " " << _nbScenario << " " << _capacity << "\n";
    for(int i= 0; i<_nbCommands; i++){
        cout << i <<" "<< _benefVector[i] << " " << _subTreatedCost[i] << "\n";
        for (int j=0; j<_nbScenario; j++){
            cout << _durationScenarTask[i][j] << " ";
        }
        cout<<"\n";
    }
    
}

int Parser::nbCommands() {
    return _nbCommands;
}

int Parser::nbScenario() {
    return _nbScenario;
}

int Parser::capacity() {
    return _capacity;
}

vector<int> Parser::benefVector() {
    return _benefVector;
}

vector<int> Parser::subTreatedCost() {
    return _subTreatedCost;
}

vector<vector<int> > Parser::durationScenarTask() {
    return _durationScenarTask;
}

vector<double> Parser::probaVector() {
    return _probaVector;
}






