//
//  Console.cpp
//  NeverEngine
//
//  Created by Ville-Veikko Urrila on 17/11/12.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "Console.h"
#include "ConsoleVar.h"

#include <vector>       // vector for the text strings
#include <map>          // map for the cvars and their names

#include <cstdarg>      // for the macros va_list, va_start and va_end
#include <iostream>     // standard input/output stream
#include <string>       // string data type
#include <sstream>      // string stream
#include <fstream>      // file stream

#include "Locator.h"
#include "HyperVisor.h"
#include "IRenderer.h"
#include "DrawPrimitives.h"
#include "CommandProcessor.h"
#include "StringUtil.h"
#include "Timer.h"
#include "FileUtil.h"

inline void VarLoadGuard( void ) {
    bool b = true;
    int i = 1;
    float f = 1.5f;
    std::string s = "Yep";
    Console::AddVar(b, "");
    Console::AddVar(i, "");
    Console::AddVar(f, "");
    Console::AddVar(s, "");
};

std::map<std::string, ConsoleVar*>  Console::m_cvars;                // Holds cvars by name
std::deque<ConsoleLine>             Console::m_textLines;            // Holds lines of text
bool                                Console::visible = false;        // Whether the console is shown on screen

//========================================================================
// 'Public' console functions, available to outside
//========================================================================
void Console::Initialize()
{
    PrintString("Console initialized, good times ahead", COLOR_GREEN);
    
    CommandProcessor::AddCommand("toggleConsole", Command<>(ToggleVisibility));
    CommandProcessor::AddCommand("vars", Command<>([&](){
        Print("Printing (%l) vars:", m_cvars.size());
        std::map<std::string, ConsoleVar*>::const_iterator it;
        for ( it = m_cvars.begin(); it != m_cvars.end(); it++ ) {
            PrintString( it->first, COLOR_GREEN );
        }
    }));
}

void Console::Terminate()
{
    
}

bool Console::isVisible() { return visible; };

void Console::Process( std::string command ) {
    // Try to tokenize input text
    std::deque<std::string> tokens;
    StringUtil::Tokenize(command, tokens);
    bool wasCVar = false;
    if ( tokens.size() == 1 ) { // Check if we just want to display the value of a ConsoleVar
        std::map<std::string, ConsoleVar*>::iterator it = m_cvars.find( tokens[0] );
        if ( it != m_cvars.end() ) {
            Print("ConsoleVar: %s = %s", tokens[0].c_str(), (*it->second).GetString().c_str() );
            wasCVar = true;
        }
    } else {
        // Check for known commands
        if ( tokens[0] == "set" ) {
            bool setNewValue = false;
            if ( tokens.size() == 3 ) {
                std::map<std::string, ConsoleVar*>::iterator it = m_cvars.find( tokens[1] );
                if ( it != m_cvars.end() && !tokens[2].empty() ) {    // Found cVar to set new value
                    if ( it->second->IsType<bool>() ) {
                        bool & cv = it->second->as<bool>();
                        if ( !StringUtil::IsNumber(tokens[2]) ) {
                            if ( tokens[2] == "false" ) {
                                cv = false;
                            } else if ( tokens[2] == "true" ) {
                                cv = true;
                            }
                            setNewValue = true;
                        } else {
                            int val = stoi(tokens[2]);
                            cv = val;
                            setNewValue = true;
                        }
                    } else if ( it->second->IsType<int>() ) {
                        int val = stoi(tokens[2]);
                        int & cv = it->second->as<int>();
                        cv = val;
                        setNewValue = true;
                    } else if ( it->second->IsType<float>() ) {
                        float val = stof(tokens[2]);
                        float & cv = it->second->as<float>();
                        cv = val;
                        setNewValue = true;
                    } else if ( it->second->IsType<double>() ) {
                        double val = stod(tokens[2]);
                        double & cv = it->second->as<double>();
                        cv = val;
                        setNewValue = true;
                    } else if ( it->second->IsType<std::string>() ) {
                        std::string & cv = it->second->as<std::string>();
                        cv = tokens[2];
                        setNewValue = true;
                    }
                } else {
                    Print("Unknown cvar: %s", tokens[1].c_str());
                }
            }
            if ( setNewValue == false ) {
                Print("Usage: set variable value");
            } else {
                Print("%s set to %s", tokens[1].c_str(), tokens[2].c_str());
            }
            wasCVar = true;
        }
    }
    if ( !wasCVar ) {   // Pass command to processor
        CommandProcessor::Buffer(command);
    }
    //        textWidget->Clear();
}
// The mother lode! Parses arguments almost like a real printf()
// True monkey coding at its best - throw shit at it until something sticks...
void Console::Print( const char * str, ... ) {
    // Grab arguments
    va_list args;
    va_start(args, str);
    std::ostringstream output;
    // Parse arguments
    for (unsigned int i = 0; i < strlen(str); i++) {
        // If we need to format the next char
        if (str[i] == '%' && str[i+1] != '%' && i+1 < strlen(str)) {
            // Switch the next character
            switch (str[i+1]) {
                case 's': {
                    char *temp = va_arg (args, char*);
                    output << temp;
                }
                    break;
                case 'i':
                case 'd': {
                    int temp = va_arg (args, int);
                    output << temp;
                }
                    break;
                case 'u': {
                    unsigned int temp = va_arg (args, unsigned int);
                    output << temp;
                }
                    break;
                case 'f': {
                    double temp = va_arg (args, double);
                    output << temp;
                }
                    break;
                case 'l': {
                    long temp = va_arg (args, long);
                    output << temp;
                }
                    break;
                case 'b': {
                    int var = va_arg (args, int);
                    std::string temp = var ? "True" : "False";
                    output << temp;
                }
                    break;
                default: {
                    output << str[i];
                }
                    break;
            }
            // Skip over the next character
            i++;
        }
        else if (str[i] == '%' && str[i+1] == '%' && i+1 < strlen(str) ) {
            output << str[i];
            // Skip over one percentage character
            i ++;
        }
        else if (str[i] == '\\' && str[i+1] == 'n' && i+1 < strlen(str) ) {
            // Skip over newline character
            i ++;
        }
        else { output << str[i]; }
    }
    va_end(args);
    
    // Finally we have a string to dump into the console output
    PrintString( output.str() );
}
void Console::PrintString( std::string text, Color col ) {
    // No parsing necessary here, just add timestamp
    std::string timeS = Timer::TimeStamp();
    timeS.append(" - ");
    timeS.append( text );
    AddMessage( timeS , col );
}
void Console::AddMessage( std::string text, Color col ) {
    int newLabelID = -1;
    ConsoleLine newLine = { text, newLabelID, col, 0.0 };
    m_textLines.push_back(newLine);
    Refresh();
}
void Console::ToggleVisibility() {
    if ( visible )
    { Hide(); }
    else
    { Show(); }
}
void Console::Draw( double delta ) {
    if (!visible) return;
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    // Draw background box
    Color gradColTop = COLOR_UI_GRADIENT1; gradColTop.a = 0.5f;
    Color gradColBottom = COLOR_UI_GRADIENT2; gradColBottom.a = 0.5f;
    Locator::getRenderer().Primitives()->RectangleGradientY(glm::vec2(0, (windowSize.y/4.0f)+CONSOLE_TEXT_HEIGHT),
                                                            glm::vec2(windowSize.x, windowSize.y/2.0f-CONSOLE_TEXT_HEIGHT),
                                                            gradColBottom,
                                                            gradColTop,
                                                            CONSOLE_BG_DEPTH);
}
void Console::SaveLog( void ) {
    std::string logPath = FileUtil::GetPath().append("Console.log");
    std::ofstream file( logPath.c_str() );
    for(unsigned int i=0; i< m_textLines.size(); i++) {
        file << m_textLines[i].text << std::endl;
    }
    file.close();
}

void Console::AddVar( ConsoleVar* newCVar, std::string varName ) {
    m_cvars[varName] = newCVar;
}

void Console::RemoveVar( std::string varName ) {
    if ( m_cvars.find(varName) != m_cvars.end() ) {
        m_cvars.erase(varName);
    }
}


template<typename T> void Console::AddVar( T& newVar, std::string varName ) {
    ConsoleVar* newCVar = new ConsoleVar( newVar );
    AddVar(newCVar, varName);
};

//========================================================================
// 'Private' console vars and functions, unavailable to outside
//========================================================================


void Console::Refresh() {
    IRenderer* renderer = &Locator::getRenderer();
    if (renderer != NULL) {
        IText* tMan = &Locator::getText();
        if (tMan != NULL) {
            int msgCount = (int)m_textLines.size();
            if (visible) {
                int winWidth = 640; //Locator::getRenderer().GetSettings().viewWidth;
                double labelPosX = -winWidth / 2 + 8;    // left edge of screen
                double labelPosY = CONSOLE_TEXT_HEIGHT+(msgCount + 2)*CONSOLE_FONT_SIZE;
                // Move existing labels up
                for (int i = 0; i < msgCount; i++) {
                    if (m_textLines[i].blobID != -1) {  // Move text
                        tMan->UpdateTextPos( m_textLines[i].blobID,
                                             glm::vec3(labelPosX, labelPosY, CONSOLE_TEXT_DEPTH));
                    } else {                            // Add line
                        m_textLines[i].blobID = tMan->AddText( m_textLines[i].text,
                                                               glm::vec3(labelPosX, labelPosY, CONSOLE_TEXT_DEPTH),
                                                               true,
                                                               CONSOLE_FONT_SIZE,
                                                               FONT_PIXEL,
                                                               0.0,
                                                               m_textLines[i].color );
                    }
                    labelPosY -= CONSOLE_FONT_SIZE;
                }
            }
        }
    }
}

void Console::Show() {
    IText* tMan = &Locator::getText();
    if ( !tMan || visible ) { return; }
//        int winWidth = Locator::getRenderer().GetSettings().viewWidth;
//        std::string consoleInfo = "Console:  DungeonSmith v.";
//        consoleInfo.append(Locator::getHyperVisor().GetVersion());
//        textWidget = new UITextInputSCB(-winWidth/2, 1.0, winWidth-1, CONSOLE_TEXT_HEIGHT, CONSOLE_TEXT_DEPTH,consoleInfo, &Process );
//        textWidget->StartTextInput();
    visible = true;
    Refresh();
}
void Console::Hide() {
    IText* tMan = &Locator::getText();
    if ( !tMan ) { return; }
    for (size_t i = 0; i < m_textLines.size(); i++)
    {
        tMan->RemoveText(m_textLines[i].blobID);
        m_textLines[i].blobID = -1;
    }
    
    visible = false;
//        delete textWidget;
//        textWidget = NULL;
}
