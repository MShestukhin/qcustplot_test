#include "parser.h"
#include <QDebug>
parser::parser()
{
}

vector<vector<std::string> > parser::pars_file(std::string fileName){
    vector<vector<std::string> > rows;
    vector<std::string> row;
    FILE* file;
    char* nameFile=(char*)fileName.c_str();
    file=fopen(nameFile,"r");
    if(file == NULL)
    {
        return rows;
    }
    int i=0;
    while(!feof(file))
    {
        char buf[1000];
        fgets(buf,1000,file);
        if(strlen(buf)==0)
        {
            break;
        }
        std::string s1(buf);
        memset(buf,0,1000);
        char delimiter;
        int n = s1.find(",")-1;
        delimiter=',';
        int j=0;
        int i=0;
        int end=0;
        while(i<s1.size()){
            if(s1.at(i)==delimiter)
            {
                std::string str=s1.substr(end,i-end);
                row.push_back(str);
                end=i+1;
                j++;
            }
            if(s1.at(i)=='\x0D') {};
            if(s1.at(i)!='\x0A') i=i+1;
            else break;
        }
        s1=s1.substr(end,i-end);
        row.push_back(s1);
           rows.push_back(row);
           row.clear();
    }
    fclose(file);
    return rows;
}
