//
//  EditorGUI.cpp
//  DungeonSmith
//
//  Created by eVillain on 05/10/15.
//  Copyright © 2015 The Drudgerist. All rights reserved.
//

#include "EditorGUI.h"
#include "Editor.h"

#include "ButtonLabeled.h"
#include "Slider.h"
#include "Locator.h"
#include "SceneManager.h"

EditorGUI::EditorGUI(Editor& owner) :
_owner(owner)
{
    const glm::ivec2 windowSize = Locator::getRenderer().GetWindowSize();
    
    glm::ivec2 menuSize = glm::ivec2(200,40);

    glm::ivec2 guiPos = glm::ivec2(-windowSize.x/2 + menuSize.x/2,
                                   windowSize.y/2 - menuSize.y/2);
    
//    guiPos += glm::ivec2(2, -2); // Padding
    
    _menu = std::unique_ptr<GUI::Menu>(new GUI::Menu(guiPos,
                                                     menuSize,
                                                     0,
                                                     "Editor Menu"));
    
    GUI::ButtonLabeled* openFileButton = new GUI::ButtonLabeled("Open File",
                                                                guiPos,
                                                                menuSize,
                                                                0);
    openFileButton->SetBehavior(new GUI::ButtonBehaviorLambda( [](){
        // Open file menu
    } ));
    _menu->AddWidget(openFileButton);
    
    GUI::ButtonLabeled* saveFileButton = new GUI::ButtonLabeled("Save File",
                                                                guiPos,
                                                                menuSize,
                                                                0);
    saveFileButton->SetBehavior(new GUI::ButtonBehaviorLambda( [](){
        // Save file menu
    } ));
    _menu->AddWidget(saveFileButton);
    
    GUI::ButtonLabeled* exitButton = new GUI::ButtonLabeled("Exit",
                                                            guiPos,
                                                            menuSize,
                                                            0);
    exitButton->SetBehavior(new GUI::ButtonBehaviorLambda( [](){
        // Quit editor
        Locator::getSceneManager().RemoveActiveScene();
        Locator::getSceneManager().Cleanup();
    } ));
    _menu->AddWidget(exitButton);
    
    guiPos.x += menuSize.x;
    
    _cameraMenu = std::unique_ptr<GUI::Menu>(new GUI::Menu(guiPos,
                                                           menuSize,
                                                           0,
                                                           "Camera Menu"));
    GUI::Slider* slider = new GUI::Slider(glm::ivec2(-200,240),glm::ivec2(200,40),0, "Near Depth");
    slider->SetBehavior(new GUI::SliderBehavior<float>(_owner._camera.nearDepth, 0.001, 1.0));
    _cameraMenu->AddWidget(slider);

    slider = new GUI::Slider(glm::ivec2(-200,240),glm::ivec2(200,40),0, "Far Depth");
    slider->SetBehavior(new GUI::SliderBehavior<float>(_owner._camera.farDepth, 1.0, 1000.0));
    _cameraMenu->AddWidget(slider);
    
    _cameraMenu->minimizeable = true;
    _cameraMenu->Minimize();
}