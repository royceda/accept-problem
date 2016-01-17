#include <cstdlib>
#include <iostream>

#include "ilcplex/ilocplex.h"
#include "Parser.h"
#include "Determinist.h"
#include "MasterProb.h"

ILOSTLBEGIN
        
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    IloEnv env;
    Parser* p = new Parser("./../Instances/data_O5/OA_O5_S3_L0.5_B10_R50_4.txt");
    cout << "Parsing done\n";
    p->printParser();
    Determinist * det = new Determinist();
    det->solve(*p);

    MasterProb* MP = new MasterProb();
    MP->solve(*p);

    env.end();
    
    return 0;
}

