//
//  Manager.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 12/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "Manager.h"
#include "Widget.h"
#include "Locator.h"

namespace GUI
{
    Manager::Manager()
    {
        printf("GUI Manager constructor...\n");
        Locator::getInput().RegisterEventObserver(this);
        Locator::getInput().RegisterMouseObserver(this);
        _mouseDrag = false;
    }
    
    Manager::~Manager()
    {
        Locator::getInput().UnRegisterEventObserver(this);
        Locator::getInput().UnRegisterMouseObserver(this);
    }
    
    void Manager::Add(Widget *widget)
    {
        _widgets.push_back(widget);
    }
    
    void Manager::Remove(Widget *widget)
    {
        std::vector<Widget*>::iterator it = std::find(_widgets.begin(), _widgets.end(), widget);
        if ( it != _widgets.end() )
        {
            _widgets.erase(it);
        }
    }
    
    void Manager::Update(double delta)
    {
        for (Widget* widget : _widgets)
        {
            widget->Update();
        }
    }
    
    const void Manager::Draw() const
    {
        for (Widget* widget : _widgets)
        {
            widget->Draw();
        }
    }
    
    bool Manager::OnCursorPress( const glm::ivec2& coord ) {
        for (Widget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->OnInteract(true, coord);
                return true;
            }
        }
        return false;
    }
    
    bool Manager::OnCursorDrag( const glm::ivec2& coord ) {
        for (Widget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->OnDrag(coord);
                return true;
            }
        }
        return false;
    }
    
    bool Manager::OnCursorRelease( const glm::ivec2& coord ) {
        for (Widget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->OnInteract(false, coord);
                return true;
            }
        }
        return false;
    }
    
    bool Manager::OnCursorHover( const glm::ivec2& coord ) {
        bool overWidget = false;
        for (Widget* widget : _widgets)
        {
            if( widget->Contains(coord) )
            {
                widget->SetFocus(true);
                overWidget = true;
            } else {
                widget->SetFocus(false);
            }
        }
        if ( !overWidget ) { _mouseDrag = false; }
        return overWidget;
    }
    
    bool Manager::OnEvent( const std::string& event, const float& amount )
    {
        if ( event == "shoot")
        {
            if ( amount == -1 )
            {
                _mouseDrag = false;
                return OnCursorRelease(_currentMouseCoord);
            }
            else if ( amount == 1 )
            {
                bool clickedWidget = OnCursorPress(_currentMouseCoord);
                if ( clickedWidget ) { _mouseDrag = true; }
                return clickedWidget;
            }
        }
        return false;
    }
    
    bool Manager::OnMouse( const glm::ivec2& coord )
    {
        _currentMouseCoord.x = coord.x;
        _currentMouseCoord.y = coord.y;
        if ( _mouseDrag) {
            return OnCursorDrag(coord);
        }
        return OnCursorHover(_currentMouseCoord);
    }
}   /* namespace GUI */
