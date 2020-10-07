#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "include/preprocessing.hpp"


using namespace std;


int main(int argc, char **argv){

    string flagCommand = argv[1];
    string filename = argv[2];

    PreProcessing *preprocessing = new PreProcessing();

    preprocessing->runPreProcessing(filename);

    return 0;

}