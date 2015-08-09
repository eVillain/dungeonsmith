#ifndef NGN_GUI_TEXTINPUT_H
#define NGN_GUI_TEXTINPUT_H
//
//  GUITextInput.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/01/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "GUIWidget.h"
#include "InputFunctors.h"
#include "TextLabel.h"
#include <SDL2/SDL_events.h>

namespace GUI
{
    class GUITextInput: public GUIWidget
    {
    public:
        GUITextInput(const int posX, const int posY,
                     const int width, const int height,
                     const int depth,
                     const std::string defaultVal="<text input>");
        ~GUITextInput();
        
        // Overriden from UIWidget
        virtual void Draw();
        virtual void Update();
        // When clicked/pressed
        virtual void OnInteract( const bool interact );

        // Text input operations
        void StartTextInput();
        void StopTextInput();
        
        //    virtual void UpdatePos( int posX, int posY );
        //    void CursorRelease( int x, int y );
        //    void CursorPress( int x, int y  );
        //    void CursorHover( bool highlight, int x, int y  );
        //    void Draw( RendererBase* renderer );
        //
        //    // Keyboard key and character callbacks
        //    void ReceiveChar(int key, int action);
        //    void ReceiveKey(int key, int action);

        //    void CancelTextInput( void );
        //    void UpdateTextInput( void );
        //    void Update( void ) {};
        //    void Clear( void );
        //    
        //    std::string GetText( void );
        //    void SetText( std::string newText );
        //    virtual void DoCallBack() {};   // Override this in subclasses to actually perform callback
    private:
        // Text input parameters
        TextInputFunctor<GUITextInput> _textInputFunctor;
        void OnTextInputEvent( const SDL_Event& event );
        bool _textInputActive;

        // Text input attributes
        TextLabel _label;
        std::string _inputText;
        std::string _defaultText;
        
        //    InputFunctor<UITextInputBase> charReceiverFunc;
        //    InputFunctor<UITextInputBase> keyReceiverFunc;
        //    bool grabKeyboardInput;
        //    int keyboardIndex;
        //    char keyboardBuffer[64];
        //    int inputLabelID;
        
        double _lastCursorBlink;
        bool _cursorBlink;
//        double _lastBackSpace;
        

    };
}   /* namespace GUI */

// Text input widget with member callback function
//template <class UnknownClass>
//class UITextInput : public UITextInputBase {
//private:
//    // Text input callback attributes
//    void ( UnknownClass::*function )( std::string );    // Pointer to a member function
//    UnknownClass* object;                               // Pointer to an object instance
//public:
//    UITextInput( int posX, int posY,
//                int width,int height,
//                int depth,
//                std::string description,
//                std::string defaultVal="<enter text>",
//                UnknownClass* objectPtr = NULL,
//                void( UnknownClass::*func )( std::string ) = NULL ) :
//    UITextInputBase( posX, posY, width, height, depth, description, defaultVal ),
//    function(func),
//    object(objectPtr)
//    { };
//    
//    // Execute member callback function, sending text
//    void DoCallBack() {
//        if ( object && function ) {
//            (*object.*function)( GetText() );
//        }
//    };
//};
//
//// Text input widget with static callback
//class UITextInputSCB : public UITextInputBase {
//private:
//    // Pointer to text input callback function
//    void ( *function )( std::string );
//public:
//    UITextInputSCB( int posX, int posY,
//                int width,int height,
//                   int depth,
//                std::string description,
//                void( *func )( std::string ) = NULL ) :
//    UITextInputBase( posX, posY, width, height, depth, description ),
//    function(func)
//    { };
//    
//    // Execute static callback function, sending text
//    void DoCallBack() {
//        if ( function ) {
//            (*function)( GetText() );
//        }
//    };
//};
#endif
