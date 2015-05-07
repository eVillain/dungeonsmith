//
//  StringUtil.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include <sstream>

#include "StringUtil.h"

// Split up an input string into separate lines
void StringUtil::SplitIntoLines( const std::string& input, std::deque <std::string>& result )
{
    std::istringstream ss( input );
    std::string line;
    while (std::getline( ss, line )) {
        result.push_back( line );
    }
}

// Split up an input string into separate tokens
void StringUtil::Tokenize( const std::string& input, std::deque<std::string>& tokens )
{
    std::string::size_type last_pos = 0;
    std::string::size_type pos = 0;
    
    while(true)
    {
        pos = input.find_first_of(' ', last_pos);
        if( pos == std::string::npos )
        {
            tokens.push_back(input.substr(last_pos));
            break;
        } else {
            tokens.push_back(input.substr(last_pos, pos - last_pos));
            last_pos = pos + 1;
        }
    }
}

bool StringUtil::IsNumber( const std::string& string)
{
    std::string::const_iterator it = string.begin();
    while ( it != string.end() &&
           (std::isdigit(*it) || *it == '.') )
    {
        ++it;
    }
    return !string.empty() && it == string.end();
}

std::string StringUtil::BoolToString( bool number ) {
    return number == true ? "TRUE" : "FALSE";   //return a string with the contents of the stream
}

std::string StringUtil::IntToString( int number ) {
    std::stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

std::string StringUtil::FloatToString( float number, int precision ) {
    std::ostringstream buff;
    buff.precision(precision);
    buff<<number;
    return buff.str();
}

std::string StringUtil::DoubleToString( double number, int precision ) {
    std::ostringstream buff;
    buff.precision(precision);
    buff<<number;
    return buff.str();
}

// --- WINDOWS SPECIFIC SHIZZLES --- //
#if defined(_WIN32)
// Convert a wide Unicode string to an UTF8 string
std::string StringUtil::utf8_encode(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}
// Convert an UTF8 string to a wide Unicode String
std::wstring StringUtil::utf8_decode(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
// Convert widechars to string
std::string StringUtil::wchar_t2string(const wchar_t *wchar)
{
    std::string str = "";
    int index = 0;
    while(wchar[index] != 0)
    {
        str += (char)wchar[index];
        ++index;
    }
    return str;
}
// Convert string to widechars
void StringUtil::string2wchar_t(const std::string &str,  wchar_t* wchar)
{
    unsigned int index = 0;
    while(index < str.size())
    {
        wchar[index] = (wchar_t)str[index];
        ++index;
    }
    wchar[index] = 0;
}
#endif