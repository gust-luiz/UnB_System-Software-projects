#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


void findAndReplaceAll(string &data, string toSearch, string replaceStr);
string lineUpperCase(string line);
string lineCleaning(string line);
string lineCleanExtraBlanks(string line);
string lineCleanComments(string line);

#endif