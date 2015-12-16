/* 
 * File:   Parser.cpp
 * Author: yoannjvr
 *
 * Created on 16 décembre 2015, 13:48
 */

#include <cstdlib>
#include <sstream>
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
            int tmpScenarCost;
            while ((i < _nbScenario) && (ssline)) {
                ssline >> tmpScenarCost;
                _costScenarTask[ite].push_back(tmpScenarCost);
                i++;
            }

            _probaVector.push_back(proba);

            ite++;
        }

        file.close();
    } else
        cerr << "Impossible to open the file\n";

}

Parser::printParser(const Parser& p) {
    cout << _nbCommands << " " << _nbScenario << " " << _capacity << "\n";
    for(int i= 0; i<_nbCommands; i++){
        cout << i <<" "<< _benefVector[i] << " " << _subTreatedCost[i] << "\n";
        for (int j=0; j<_nbScenario; j++){
            cout << costScenarTask[i][j] << " ";
        }
        cout<<"\n";
    }
    
}

Parser::nbCommands() {
    return _nbCommands;
}

Parser::nbScenario() {
    return _nbScenario;
}

Parser::capacity() {
    return _capacity;
}

Parser::benefVector() {
    return _benefVector;
}

Parser::subTreatedCost() {
    return _subTreatedCost;
}

Parser::costScenarTask() {
    return _costScenarTask;
}

Parser::probaVector() {
    return _probaVector();
}






