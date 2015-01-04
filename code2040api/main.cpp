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
#include <boost/array.hpp>
//#include <boost/asio.hpp>
//#include <network/protocol/http/client.hpp>
#include "strlib.h"
#include "stringutils.h"
#include "client-socket.h"

//using namespace std;

/**
 *Function:dating game
 *--------------------
 *Takes a string that represents a date formatted to the ISO 8601 specifaicatio
 *and returns a string similarly formatted but representing the date after interval
 *seconds have been added to the original date.
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
    std::string result = std::to_string(t_time.tm_year) + "-" + std::to_string( t_time.tm_mon) + "-" + std::to_string(t_time.tm_mday) + "T" + std::to_string(t_time.tm_hour) +  ":" +  std::to_string(t_time.tm_min) + ":" +std::to_string(t_time.tm_sec);
     return result + timezone;
}

/**
 *Function:hasPrefix
 *-------------------
 *
 */
void hasPrefix(std::vector<std::string>& toPopulate, std::vector<std::pair<std::string, std::string>>& solve , std::string prefix){
    size_t len = solve.size();
    for(size_t i = 0; i < len; i ++){
        if(strncmp(((solve[i]).second).c_str(), prefix.c_str(), prefix.length()) == 0){
            toPopulate.push_back((solve[i]).second);
        }
    }
}

/**
 *Function:reverseString
 *----------------------
 *
 */
std::string reverseString(std::string toReverse){
    std::reverse_iterator<std::string::iterator> r = toReverse.rbegin();
    std::string rev(r, toReverse.rend());
    return rev;
}

/**
 *Function:parseJSON
 *------------------
 *Takes a well formatted JSON object and populates a vector of pairs of strings with all
 *the key-value pairs that where in the JSON object.
 */
void parseJSON(std::string payload, std::vector<std::pair<std::string, std::string>>& array){
    payload = trim(payload);
    payload = payload.substr(1, payload.length() - 2);//remove the leading '[' and trailing ']'
    payload = trim(payload);
    size_t pos , prevPos = 0;
    while(true){
        payload = payload.substr(prevPos);//update the payload
        pos = payload.find(",");
        pos = (pos == std::string::npos) ? (payload.length()) : (pos);
        std::string token;
        token = payload.substr(0, pos);
        token = trim(token);
        size_t split = token.find(":");
        std::string key = token.substr(0, split);
        std::string value = token.substr(split + 1);
        key = trim(key); value = trim(value);
        key = key.substr(1, key.length() - 2);
        value = value.substr(1, value.length() - 2);
        const std::pair<std::string, std::string> thisPair (key, value);
        array.push_back(thisPair);
        if(pos == payload.length())break;
        prevPos = pos + 1;
    }
}

/**
 *Function:addtoJSON
 *------------------
 *Receives a  well formatted JSON object(empty or otherwise) and appends 
 *the key and value as a JSON string to the  end of the object.
 */
void addtoJSON(std::string& JSON, std::string key, std::string value){
    JSON = trim(JSON);
    JSON = JSON.substr(0, JSON.length() - 1) + ((JSON.length() == 2) ? ("") : (",")) + "\"" + key + "\"" + ":" + "\"" + value + "\"" + "}";
}


/**
 *Function:needleInStack
 *----------------------
 *
 */
int needleInStack(std::string search, std::string arr[], int len){
    return (int)std::distance(arr, std::find(arr, arr + len, search));
}


/**
 *Method:talkToServer
 *-------------------
 *
 */
//void talkToServer(boost::asio::ip::tcp::socket& socket, std::string serverURL, std::string path, std::string requestType, std::string payload)
//{
//try{
//    boost::asio::streambuf request;
//    std::ostream request_stream(&request);
//    request_stream << requestType << " " << path << " HTTP/1.1\r\n";
//    request_stream << "Host:" << serverURL << "\r\n";
//    request_stream << "Content-Type: application/json; charset=utf8 \r\n";
//    request_stream << payload << "\r\n";
//    request_stream << "Accept: */*\r\n";
//    request_stream << "Connection: close\r\n\r\n";
//    
//    boost::asio::write(socket, request);
//    
//    boost::asio::streambuf response;
//    boost::asio::read_until(socket, response, "\r\n");
//    
//    std::istream response_stream(&response);
//    std::string http_version;
//    response_stream >> http_version;
//    std::cout << http_version << std::endl;
//    unsigned int status_code;
//    response_stream >> status_code;
//    std::string status_message;
//    std::getline(response_stream, status_message);
//    if(!response_stream || http_version.substr(0, 5) != "HTTP/")
//    {
//        std:: cout << "Invalid response\n" << "\n";
//        return;
//    }
//    if(status_code != 200){
//        std::cout << "Response returned with status code " << status_code << "\n";
//        return;
//    }
//    
//    std::string header;
//    while(std::getline(response_stream, header) && header != "\r")
//        std::cout << header << "\n";
//    std::cout << "\n";
//
//    if(response.size() > 0)
//        std::cout << "TOKEN IS " << &response;
//    
//    //Read untiol EOF, writing data to output as we go
//    boost::system::error_code error;
//    while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
//        std::cout << &response;
//    if(error != boost::asio::error::eof)
//        throw boost::system::system_error(error);
// }
//    catch(std::exception & e)
//    {
//        std::cout << "Exception: " << e.what() << "\n";
//    
//    }
//    return;
//}



int main(int argc, const char * argv[])
{
//    boost::asio::io_service io_service;
//    boost::asio::ip::tcp::resolver resolver(io_service);
//    boost::asio::ip::tcp::resolver::query query("54.200.161.139", "80");
//    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
//    
//    boost::asio::ip::tcp::socket socket(io_service);
//    boost::asio::connect(socket, endpoint_iterator);
    
//    network::http::client client;
//    network::http::client::request request("challenge.code2040.org/api/register/");
    //request.add_header(make_pair("X-AUTH-TOKEN", token_));
//    request <<"Accept: application/json \r\n";
//    request << "Content-Type: application/json \r\n";
//    request <<  "Connection: close\r\n\r\n";
//    request <<  "r\n";
    std::string payload = "{}";
    addtoJSON(payload, "email", "cmulemi@stanford.edu");
    addtoJSON(payload, "gitHub", "https://github.com/CharlesJeremy/code2040api");
    std::cout << payload << std::endl;
    request << payload;
    network::http::client::response response = client.post(request);
    std::cout <<  &response;


    
    
    
    
    
    
    
    
    
//    string my_token = register;
//    std::cout << reverseString("Hello, World!\n") << endl;
//    string myints[] = { "abboo", "cd", "abpuss", "boob", "abyaay", "dolittle", "shtanford"};
//    std::cout << needleInStack("boob", myints,  7) << endl;
//    std::vector<std::string> found;
//    //hasPrefix(found, myints, 7, "ab");
//    for(string t: found)cout <<  t << endl;
//    string JSON = "[\"man\": \"feet\", \"woman\":  \"hands\"  , \"child\" : \"head\"]";
//    vector<pair<std::string, std::string>> result;
//    cout << "Date 1986-06-31T23:59:45+00:00  after interval 30 is " << datingGame("1986-06-31T23:59:45+00:00", "30") << endl;
//    parseJSON(JSON, result);
//    for(pair<std::string, std::string> t: result){
//        cout << t.first << " : " << t.second << endl;
//    
//    }
//    std::string payload = "{}";
//    addtoJSON(payload, "email", "cmulemi@stanford.edu");
//    addtoJSON(payload, "gitHub", "https://github.com/CharlesJeremy/code2040api");
//    std::cout << payload << std::endl;
    //talkToServer(socket, "challenge.code2040.org", "/api/register/", "POST", payload);
    
    return 0;
}

