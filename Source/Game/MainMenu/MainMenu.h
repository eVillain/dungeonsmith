//
//  MainMenu.h
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__MainMenu__
#define __DungeonSmith__MainMenu__

#include "Scene.h"
#include "InputListener.h"
#include "ButtonLabeled.h"

class MainMenu : public Scene, public InputEventListener
{
public:
    MainMenu();
    ~MainMenu();
    // Overridden from Scene
    void Initialize( void );
    void ReInitialize( void );
    void Release( void );
    void Cleanup();
    
    void Pause( void );
    void Resume( void );
    void Update ( double deltaTime );
    void Draw( void );
    
private:
    bool OnEvent( const std::string& event, const float& amount );

    void CreateButtons();
    void RemoveButtons();
    GUI::ButtonLabeled* _buttonStartGame;
    GUI::ButtonLabeled* _buttonStartEditor;
    GUI::ButtonLabeled* _buttonPlanetoid;
    GUI::ButtonLabeled* _buttonExit;

};
#endif /* defined(__DungeonSmith__MainMenu__) */
