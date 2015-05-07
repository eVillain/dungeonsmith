//
//  Console.h
//  Ingenium
//
//  Created by The Drudgerist on 17/11/12.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_CONSOLE_H
#define NGN_CONSOLE_H

#include <string>
#include <map>
#include <vector>
#include "ConsoleVar.h"
#include "Color.h"

#define CONSOLE_FONT_SIZE 18
#define CONSOLE_BG_DEPTH 9
#define CONSOLE_TEXT_DEPTH 10
#define CONSOLE_MAX_MESSAGES 10
#define CONSOLE_TEXT_HEIGHT 22

typedef struct {
    std::string text;
    int blobID;
    Color color;
    double timeStamp;
} ConsoleLine;

class Console {
public:
    static void Initialize();
    static void Terminate();
    
    static void Print(const char * format, ...);
    static void PrintString(std::string text, Color col = COLOR_WHITE);
    static void AddMessage(std::string text, Color col);
    static void ToggleVisibility( void );
    static bool isVisible( void );
    static void Draw( double delta );
    
    static void SaveLog( void );
    static void Process( std::string input );
    
    static void AddVar( ConsoleVar* newCVar, std::string varName );
    static void RemoveVar( std::string varName );
    
    template<typename T> static void AddVar( T& newVar, std::string varName );
private:

    static std::map<std::string, ConsoleVar*>  m_cvars;                // Holds cvars by name
    static std::deque<ConsoleLine>             m_textLines;            // Holds lines of text
    static bool                                visible;                 // Whether the console is shown on screen
//   static UITextInputSCB*                     textWidget = NULL;      // Widget which takes text input

    static void Refresh();
    static void Show();
    static void Hide();
};

#endif
