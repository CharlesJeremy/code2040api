//
//  main.cpp
//  code2040api
//
//  Created by Charles Jeremy Mulemi on 12/4/14.
//  Copyright (c) 2014 Redbell. All rights reserved.
//  ---------------------------------------------------------------------------------
//  This is the first complete project I have done outside of my classwork.It was a fun
//  learning experience and I look forward to doing more projects on my own.
//

#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>
#include "strlib.h"

/**
 *Function:reverseString
 *----------------------
 *This function is straightforward.It  reverses a string.
 *That is, if the string toReverse says “cupcake,” it is going to send back “ekacpuc.”
 *-----------------------------------------------------------------------------------
 *This is the first time I have used a reverse iterator.I think it is 
 *really neat and saved me from writing a lot of code
 */
std::string reverseString(std::string toReverse){
    toReverse = trim(toReverse);
    std::reverse_iterator<std::string::iterator> r = toReverse.rbegin();
    std::string rev(r, toReverse.rend());
    return rev;
}


/**
 *Function:needleInStack
 *----------------------
 *This function receives two arguments. The first value, needle, is a string. The second 
 *value,haystack, is an array of strings. You’re going to tell the API where the needle is
 *in the array.
 *----------------------------------------------------------------------------------------
 *I bumped into the distance function on stack overflow.I really like
 *how it has abstracted away all the searching that goes into finding a
 *particular element in an array.
 */
int needleInStack(const char* haystack[], std::string needle,  size_t len){
    return (int)std::distance(haystack, std::find(haystack, haystack + len, needle));
}


/**
 *Function:hasPrefix
 *------------------
 *This function recieves  arguments. The first parameter, prefix, is a string. The second
 *value, array, is an array of strings.It returns an vactor of strings containing only the
 *strings that do not start with that prefix.
 *----------------------------------------------------------------------------------------
 *I like my solution for this problem but I cant  help but think that their is a more
 *elegant solution out there(I get icky when I see iterations).I hope one of the other applicants
 *has a better solution that I can check out.
 */
void hasPrefix(std::string prefix, std::vector<std::pair<std::string, std::string>>& solve, std::vector<std::string>& toPopulate){
    size_t len = solve.size();
    for(size_t i = 0; i < len; i ++){
        if(strncmp(((solve[i]).second).c_str(), prefix.c_str(), prefix.length()) != 0){
            toPopulate.push_back((solve[i]).second);
        }
    }
}


/**
 *Function:dating game
 *--------------------
 *Takes a string that represents a date formatted to the ISO 8601 specifaication
 *and returns a string similarly formatted but representing the date after interval
 *seconds have been added to the original date.
 *--------------------------------------------------------------------------------------------------
 *This is my first encounter the <ctime> class.I fell in love with the functionality 
 *it provides and this was one of the functions I enjoyed writing best.I wish there was a
 *neater way to concatenate the result string though.
 */
std::string datingGame(std::string date, std::string interval){
     struct tm t_time;
     t_time.tm_year = atoi(date.substr(0, 4).c_str());
     t_time.tm_mon = atoi(date.substr(5, 2).c_str());
     t_time.tm_mday = atoi(date.substr(8, 2).c_str());
     t_time.tm_hour = atoi(date.substr(11, 2).c_str());
     t_time.tm_min = atoi(date.substr(14, 2).c_str());
     std::string rest = date.substr(17);
     t_time.tm_sec =  atoi((rest.substr(0, 2)).c_str()) + atoi(interval.c_str());
     std::string timezone = (rest.length() > 2) ?  (rest.substr(2)) : ("");
     time_t t = mktime(&t_time);
     t_time = *localtime(&t);
     std::string month = ((t_time.tm_mon > 9) ? (std::to_string(t_time.tm_mon)) : ("0" + std::to_string(t_time.tm_mon)));
     std::string day = ((t_time.tm_mday > 9) ? (std::to_string(t_time.tm_mday)) : ("0" + std::to_string(t_time.tm_mday > 9)));
     std::string hour = ((t_time.tm_hour > 9) ? (std::to_string(t_time.tm_hour)) : ("0" + std::to_string(t_time.tm_hour)));
     std::string minute = ((t_time.tm_min > 9) ? (std::to_string(t_time.tm_min)) : ("0" + std::to_string(t_time.tm_min)));
     std::string second = ((t_time.tm_sec > 9) ? (std::to_string(t_time.tm_sec)) : ("0" + std::to_string(t_time.tm_sec)));
     std::string result = std::to_string(t_time.tm_year) + "-" + month + "-" + day  + "T" + hour +  ":" + minute  + ":" + second;
     return result + timezone;
}


/**
 *Function:parseTree
 *------------------
 *This function parses a boost property_tree that is passed as the first argument.It
 *populates a vector of string pairs with key-value pairs if they are terminal pairs on the tree
 *If the value attribute of the pair is not terminal, then the function recursively parses the
 *value as a new boost property_tree
 *-----------------------------------------------------------------------------------------------
 *I learnt a lot about how xcode(and generally comiler work) by having to manually 
 *download and set up the boost libraries on my comuter.I had to fiddle around my 
 *usr/include folder and to do a lot of googling and 2:00am messages to friends.
 *I really made slow progress on this part since I had no prior experience in setting 
 *up my own user includes.I had initially implemented my own JSON parser but it would not 
 *work correctly if JSON inputs where nested(as where some of the responses on the challenge).
 *I therefore had no choice but to learn how to use the JSON property_tree(It is really neat 
 *and I would like to implement it someday).I felt very accomplished when I could finally
 *parse any JSON files accurately.
 */
void parseTree(boost::property_tree::ptree const& pt, std::vector<std::pair<std::string, std::string>>& array)
{
    using boost::property_tree::ptree;
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it)
    {
        ptree childTree = it->second;
        if(childTree.empty() && !childTree.data().empty()){
            std::pair<std::string, std::string> thisPair (it->first, it->second.get_value<std::string>());
            array.push_back(thisPair);
        }else{
            parseTree(childTree, array);
        }
    }
}


/**
 *Function:parseJSON
 *------------------
 *Takes a well formatted JSON object and populates a vector of pairs of strings with all
 *the key-value pairs that where in the JSON object.It is wrapped around the parseTree
 *function which performs most of the parsing.
 *---------------------------------------------------------------------------------------
 *Originally I had my own implementation of a JSON parser, but -as I mentioned earlier
 *it couldn't work on Nested JSON arrays therefreo I had to learn how to use the boost
 *property_tree class.
 */
int parseJSON(std::string payload, std::vector<std::pair<std::string, std::string>>& array){
   
    try{
        std::stringstream ss;
        ss << payload;
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);
        parseTree(pt, array);
        return EXIT_SUCCESS;
    }
    catch (std::exception const& e)
    {
    std::cerr << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}


/**
 *Function:addtoJSON
 *------------------
 *Receives a  well formatted JSON object(empty or otherwise) and appends 
 *the key and value as a JSON string to the  end of the object.
 */
void addtoJSON(std::string& JSON, std::string key, std::string value){
    JSON = trim(JSON);
    JSON = JSON.substr(0, JSON.length() - 1) + ((JSON.length() == 2) ? ("") : (",")) + "\"" + key + "\"" + ":" + ((value[0] == '[') ? (value) :("\"" + value + "\"")) + "}";
}



/*Function: write_to_string
 *-------------------------
 *The callback function that is passedas an option so that it will receive the data
 *that is in the http response received.We let CURL know about your function using 
 *CURLOPT_WRITEFUNCTION.
 *---------------------------------------------------------------------------------
 *I got this code right off of stack overflow.It took me a while to understand what 
 *exactly  it was doing but eventually I was able to.It was really handy since curl 
 *responses are printed directly to stdout.I had to fiddle around in order to figure 
 *out how to store the http response for later manipulation
 */
size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);
    return realsize;
}


/**
 *Method:talkToServer
 *-------------------
 *This method receives two arguments. a url and a payload.It communicates with 
 *the server adressed ny URL.This is done through a http POST request(abstracted
 *by the libcurl library.
 *-------------------------------------------------------------------------------
 *I spent the bulk of my time on this challenge trying to figure out a way to 
 *perform the http randezvous to post and receive JSON formatted strings from the code2040
 *webserver.After three unsuccessful attempts with the boost library, cpp-netlib library and
 *a custom Stanford University library, I bumped into the curl library on a forum on stack
 *overflow.By this point I was conversant with setting up the include files on my computer
 *and configure my project on xcode.
 *The library is very helpful and I like how it abstracts  the  entire HTTP request and 
 *internet sockets modules that are needed to communicate with a server. I had some level
 *of experience with HTTP and internet sockets but the libcurl library wrapped it all 
 *up really well to provide very neat functionality.
 *
 */
std::string talkToSever(std::string url, std::string payload){
    std::string response;
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
         just as well be a https:// URL if that is what should receive the
         data. */
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
    /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return response;
}


/**Function:registration
  *---------------------
  *This function obtains the unique identification token from the server.
  */
void registration(std::string& token){
    std::string url, payload, response;
    std::vector<std::pair<std::string, std::string>> parsedJSON;
    url = "http://challenge.code2040.org/api/register";
    payload = "{}";
    addtoJSON(payload, "email", "cmulemi@stanford.edu");
    addtoJSON(payload, "github", "https://github.com/CharlesJeremy/code2040api");
    token = talkToSever(url, payload);
    parseJSON(token, parsedJSON);
    token = (parsedJSON[parsedJSON.size() - 1]).second;
}


/**
 *Function:stageOne
 *-----------------
 *Obtains the JSON dictionary for the Stage I task, performs the task and posts
 *the result back to the website.
 */
void stageOne(std::string& token){
    std::string url, payload, response;
    std::vector<std::pair<std::string, std::string>> parsedJSON;
    url = "http://challenge.code2040.org/api/getstring";
    payload = "{}";
    addtoJSON(payload, "token", token);
    response = talkToSever(url, payload);
    parseJSON(response, parsedJSON);
    url = "http://challenge.code2040.org/api/validatestring";
    addtoJSON(payload, "string", reverseString(parsedJSON[0].second));
    response = talkToSever(url, payload);
    std::cout << "Response for Reverse a string is " << response << std::endl;
}


/**
 *Function:stageTwo
 *-----------------
 *Obtains the JSON dictionary for the Stage II task, performs the task and posts
 *the result back to the server.
 */
void stageTwo (std::string& token){
    std::string url, payload, response;
    std::vector<std::pair<std::string, std::string>> parsedJSON;
    url = "http://challenge.code2040.org/api/haystack";
    payload = "{}";
    addtoJSON(payload, "token", token);
    response = talkToSever(url, payload);
    parseJSON(response, parsedJSON);
    size_t numNeedles = parsedJSON.size() - 1;
    const char* needles[numNeedles];
    for(size_t i = 0; i < numNeedles; i ++){
        needles[i] = ((parsedJSON[i]).second).c_str();
    }
    url = "http://challenge.code2040.org/api/validateneedle";
    int result = needleInStack(needles,(parsedJSON[numNeedles]).second, numNeedles);
    addtoJSON(payload, "needle", std::to_string(result));
    response = talkToSever(url, payload);
    std::cout << "Response for Needle in a haystack is " << response << std::endl;
}


/**
 *Function:stageThree
 *--------------------
 *Obtains the JSON dictionary for the Stage III task, performs the task and posts
 *the result back to the server.
 */
void stageThree(std::string& token){
    std::string url, payload, response;
    std::vector<std::pair<std::string, std::string>> parsedJSON;
    url = "http://challenge.code2040.org/api/prefix";
    payload = "{}";
    addtoJSON(payload, "token", token);
    response = talkToSever(url, payload);
    parseJSON(response, parsedJSON);
    std::string prefix = (parsedJSON[parsedJSON.size() - 1]).second;
    parsedJSON.erase(parsedJSON.begin() + parsedJSON.size() - 1);
    std::vector <std::string> withPrefix;
    hasPrefix(prefix, parsedJSON, withPrefix);
    std::string res = "[";
    for(std::string s: withPrefix){
        res +=  ("\"" + s + "\",");
    }
    res[res.length() - 1] = ']';
    addtoJSON(payload,"array", res);
    url = "http://challenge.code2040.org/api/validateprefix";
    response = talkToSever(url, payload);
    std::cout << "Response for Has Prefix is " << response << std::endl;

}

/**
 *Function:stageFour
 *------------------
 *Obtains the JSON dictionary for the Stage III task, performs the task and posts
 *the result back to the server.
 */
void stageFour(std::string& token){
    std::string url, payload, response;
    std::vector<std::pair<std::string, std::string>> parsedJSON;
    url = "http://challenge.code2040.org/api/time";
    payload = "{}";
    addtoJSON(payload, "token", token);
    response = talkToSever(url, payload);
    parseJSON(response, parsedJSON);
    datingGame(parsedJSON[0].second, parsedJSON[1].second);
    addtoJSON(payload, "datestamp", datingGame(parsedJSON[0].second, parsedJSON[1].second));
    url = "http://challenge.code2040.org/api/validatetime";
    response = talkToSever(url, payload);
    std::cout << "Response for The dating game is " << response << std::endl;
}

/**
 *Function:stageFour
 *------------------
 *Obtains from the code2040server the JSON dictionary that shows the progressI have made on the 
 *challenge.
 */
 void showWork(std::string& token){
    std::string url, payload, response;
    std::vector<std::pair<std::string, std::string>> parsedJSON;
    url = "http://challenge.code2040.org/api/status";
    payload = "{}";
    addtoJSON(payload, "token", token);
    response = talkToSever(url, payload);
    std::cout << "Response for show Your Work is " << response << std::endl;
}


int main(int argc, const char * argv[])
{
    std::string token;
    registration(token);
    stageOne(token);
    stageTwo(token);
    stageThree(token);
    stageFour(token);
    showWork(token);
    return 0;
}
    
    
    
    
