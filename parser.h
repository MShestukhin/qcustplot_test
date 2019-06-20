#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
using namespace std;
class parser
{
private :
public:
    parser();
    vector<vector<string> > pars_file(std::string fileName);
};

#endif // PARSER_H
