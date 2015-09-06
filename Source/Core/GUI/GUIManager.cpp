//
//  GUIManager.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 12/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "GUIManager.h"
#include "GUIWidget.h"
namespace GUI
{
    GUIManager::GUIManager()
    {
        Input::RegisterEventObserver(this);
        Input::RegisterMouseObserver(this);
    }
    
    GUIManager::~GUIManager()
    {
        Input::UnRegisterEventObserver(this);
        Input::UnRegisterMouseObserver(this);
    }
    
    void GUIManager::Add(GUIWidget *widget)
    {
        _widgets.push_back(widget);
    }
    
    void GUIManager::Remove(GUIWidget *widget)
    {
        std::vector<GUIWidget*>::iterator it = std::find(_widgets.begin(), _widgets.end(), widget);
        if ( it != _widgets.end() )
        {
            _widgets.erase(it);
        }
    }
    
    void GUIManager::Update(double delta)
    {
        for (GUIWidget* widget : _widgets)
        {
            widget->Update();
        }
    }
    
    const void GUIManager::Draw() const
    {
        for (GUIWidget* widget : _widgets)
        {
            widget->Draw();
        }
    }
    
    bool GUIManager::OnCursorPress( const glm::ivec2& coord ) {
        for (GUIWidget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->OnInteract(true);
                return true;
            }
        }
        return false;
    }
    
    bool GUIManager::OnCursorRelease( const glm::ivec2& coord ) {
        for (GUIWidget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->OnInteract(false);
                return true;
            }
        }
        return false;
    }
    
    bool GUIManager::OnCursorHover( const glm::ivec2& coord ) {
        bool overWidget = false;
        for (GUIWidget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->SetFocus(true);
                overWidget = true;
            } else {
                widget->SetFocus(false);
            }
        }
        return overWidget;
    }
    
    bool GUIManager::OnEvent( const std::string& event, const float& amount )
    {
        if ( event == "shoot")
        {
            if ( amount == -1 )
            {
                return OnCursorRelease(_currentMouseCoord);
            }
            else if ( amount == 1 )
            {
                return OnCursorPress(_currentMouseCoord);
            }
        }
        return false;
    }
    
    bool GUIManager::OnMouse( const glm::ivec2& coord )
    {
        _currentMouseCoord.x = coord.x;
        _currentMouseCoord.y = coord.y;
        return OnCursorHover(_currentMouseCoord);
    }
}   /* namespace GUI */
