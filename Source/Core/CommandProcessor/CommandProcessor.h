//
//  CommandProcessor.h
//  Ingenium
//
//  Created by The Drudgerist on 02/12/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//
//  Holds a list of commands tied to functions within the engine, as well as a
//  buffer of commands to execute.
//  Update(double delta) should be called within regular intervals
//
//  Note: The usage of AddCommand takes the TYPE Command / InstanceCommand as
//  an argument - ie. do not call it with 'new Command<>()' just 'Command<>()'
//
//  Usage for static functions (this function takes an std::string as an argument):
// CommandProcessor::AddCommand("commandName1", Command<std::string>(StaticFunction));
// CommandProcessor::Execute("commandName1", std::string("This string gets passed to the function"));
//
//  Usage for member functions (where std::string is the input for those functions):
// CommandProcessor::AddCommand("commandName2", Command<ClassName*,std::string>(&ClassName::MemberFunction));
// CommandProcessor::Execute("commandName2", instancePtr, std::string("This string gets passed to the function"));
//  Note: THIS SHIT IS USELESS, WE WONT HAVE THE INSTANCE POINTER IN OUR CONSOLE
//
//  Usage for lambda functions / calling a particular instance member function
// CommandProcessor::AddCommand("commandName3", Command<>([&](){ this->MemberFunction(); }));
// CommandProcessor::Execute("commandName3");
//
//  Usage for member functions in a specific instance with parameters
// CommandProcessor::AddCommand("commandName2", InstanceCommand<ClassName*,std::string>(&ClassName::MemberFunction, instancePtr));
// CommandProcessor::Execute("commandName2", std::string("This string gets passed to the function"));
//
// TODO: Make cool macros to simplify usage of this class
//#define ADD_COMMAND(name,class,type1,function) CommandProcessor::AddCommand(name, Command<type1>(function));
//#define ADD_INSTANCECOMMAND(name,type1,function,instance) CommandProcessor::AddCommand(name, InstanceCommand<class*,type1>(&class::function, instance));


#ifndef NGN_CMD_PROCESSOR_H
#define NGN_CMD_PROCESSOR_H

#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <future>
#include "Command.h"

// Base command class for common inheritance
class CommandBase
{
public:
    virtual ~CommandBase() {}
};

// Command class, takes a variable number of arguments
template <typename... ArgTypes>
class Command : public CommandBase
{
private:
    typedef std::function<void(ArgTypes...)> FunctionType;
    FunctionType functionPtr;
public:
    Command() {};
    Command(FunctionType f) : functionPtr(f) {};
    void operator()(ArgTypes... args) { if(functionPtr) functionPtr(args...); };
};

// InstanceCommand class, takes a variable number of arguments
// and an instance pointer on which to call the method
template <typename ClassType, typename... ArgTypes>
class InstanceCommand : public CommandBase
{
private:
    typedef std::function<void(ArgTypes...)> FunctionType;
    ClassType* instancePtr;
    FunctionType functionPtr;
public:
    InstanceCommand() {};
    InstanceCommand(FunctionType f, ClassType* instance) : instancePtr(instance), functionPtr(f) {};
    void operator()(ArgTypes... args) { if(functionPtr) instancePtr->functionPtr(args...); };
};

#define INCLUDE_CONSOLE 0

#if INCLUDE_CONSOLE
#include "Console.h"
#define CONSOLE(x,y) Console::Print(x,y);
#define CONSOLE_COLORED(x,y) Console::PrintString(x,y);
#else
#define CONSOLE(x,y)
#define CONSOLE_COLORED(x,y)
#endif



// Command processor class
class CommandProcessor
{
public:
    static void Initialize();
    static void Terminate();
    
    // Call this update with regular intervals
    // It will process and execute the next command in the buffer
    static void Update(double delta);
    
    // Adds a new command to the map
    template <class T>
    static void AddCommand(std::string cmdName, const T& cmd)
    {
        commandMap.insert(std::pair<std::string, CommandBasePtr>(cmdName, CommandBasePtr(new T(cmd))));
    }
    
    // Executes a command with arguments
    template <class... ArgTypes>
    static void ExecuteCommand( std::string cmdName, ArgTypes... args )
    {
        typedef Command<ArgTypes...> CommandType;
        CommandMap::const_iterator it = commandMap.find(cmdName);
        if ( it != commandMap.end())
        {
            if ( it->second.get() != NULL ) {
                CommandType* c = (CommandType*)(it->second.get());
                if (c) {
                    (*c)(args...);
                } else {
                    CONSOLE("[CmdProcessor] NULL command exec - %s", cmdName.c_str());
                    
                }
            } else {
                CONSOLE("[CmdProcessor] Bad command exec - %s", cmdName.c_str());
            }
        } else {
            CONSOLE_COLORED(std::string("Unknown command: ")+cmdName, COLOR_RED );
        }
    }
    // Removes a command from the map
    static void RemoveCommand(std::string cmdName);
    
    // Process and execute the given command
    static void Process( std::string command );
    // Buffer raw text into the buffer
    static void Buffer( std::string input );
    // Buffer command-line arguments into buffer
    static void Buffer( int argc, char * arg[] );
    
    static size_t GetBufferCount() { return commandBuffer.size(); };
    
    typedef std::shared_ptr<CommandBase> CommandBasePtr;
    typedef std::map<std::string, CommandBasePtr> CommandMap;
private:
    // The command map (string holds the name of the command)
    static CommandMap commandMap;
    // The command buffer, contains next command to execute at front
    static std::deque<std::string> commandBuffer;
};

#endif /* defined(NGN_CMD_PROCESSOR_H) */
