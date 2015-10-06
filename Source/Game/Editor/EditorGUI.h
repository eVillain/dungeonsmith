//
//  EditorGUI.h
//  DungeonSmith
//
//  Created by eVillain on 05/10/15.
//  Copyright © 2015 The Drudgerist. All rights reserved.
//

#ifndef EditorGUI_h
#define EditorGUI_h

#include <memory>
#include "Menu.h"

class Editor;

class EditorGUI
{
public:
    EditorGUI(Editor& owner);
    
private:
    Editor& _owner;
    void Initialize();
    
    std::unique_ptr<GUI::Menu> _fileMenu;
    std::unique_ptr<GUI::Menu> _editMenu;
    std::unique_ptr<GUI::Menu> _cameraMenu;
    
    void SetupFileMenu(glm::ivec2& position);
    void SetupEditMenu(glm::ivec2& position);
};

#endif /* EditorGUI_h */
