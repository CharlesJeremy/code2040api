//
//  main.cpp
//  code2040api
//
//  Created by Charles Jeremy Mulemi on 12/4/14.
//  Copyright (c) 2014 Redbell. All rights reserved.
//

#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <string.h>
#include <vector>
#include <stdio.h>

using namespace std;


std::string datingGame(std::string date, std::string interval){
     struct tm t_time;
     t_time.tm_year = atoi(date.substr(0, 4).c_str());
     t_time.tm_mon = atoi(date.substr(5, 2).c_str());
     t_time.tm_mday = atoi(date.substr(8, 2).c_str());
     t_time.tm_hour = atoi(date.substr(11, 2).c_str());
     t_time.tm_min = atoi(date.substr(14, 2).c_str());
     string rest = date.substr(17);
     t_time.tm_sec =  atoi((rest.substr(0, 2)).c_str()) + atoi(interval.c_str());
     string timezone = (rest.length() > 2) ?  (rest.substr(2)) : ("");
     time_t t = mktime(&t_time);
     t_time = *localtime(&t);
     string result = to_string(t_time.tm_year) + "-" + to_string( t_time.tm_mon) + "-" + to_string(t_time.tm_mday) + "T" + to_string(t_time.tm_hour) +  ":" +  to_string(t_time.tm_min) + ":" +to_string(t_time.tm_sec);
     return result + timezone;
}


void hasPrefix(std::vector<std::string>& toPopulate, std::string arr[], int len, std::string prefix){
    for(int i = 0; i < len; i ++){
        if(strncmp((arr[i]).c_str(), prefix.c_str(), prefix.length()) == 0){
            toPopulate.push_back(arr[i]);
        }
    }
}


string reverseString(string toReverse){
    std::reverse_iterator<std::string::iterator> r = toReverse.rbegin();
    std::string rev(r, toReverse.rend());
    return rev;
}

int needleInStack(string search, std::string arr[], int len){
    return (int)std::distance(arr, std::find(arr,arr + len, search));
}


int main(int argc, const char * argv[])
{
    std::cout << reverseString("Hello, World!\n") << endl;
    string myints[] = { "abboo", "cd", "abpuss", "boob", "abyaay", "dolittle", "shtanford"};
    std::cout << needleInStack("boob", myints,  7) << endl;
    std::vector<std::string> found;
    hasPrefix(found, myints, 7, "ab");
    for(string t: found)cout <<  t << endl;
    cout << "Date 1986-06-31T23:59:45+00:00  after interval 30 is " << datingGame("1986-06-31T23:59:45+00:00", "30") << endl;
    return 0;
}

