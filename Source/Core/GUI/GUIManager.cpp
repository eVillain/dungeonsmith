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
    GUIManager::GUIManager() :
    eventFunctor(this, &GUIManager::OnEvent),
    mouseFunctor(this, &GUIManager::OnMouse)
    {
        Input::RegisterEventObserver(&eventFunctor);
        Input::RegisterMouseObserver(&mouseFunctor);
    }
    
    GUIManager::~GUIManager()
    {
        Input::UnRegisterEventObserver(&eventFunctor);
        Input::UnRegisterMouseObserver(&mouseFunctor);
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
    
    void GUIManager::Draw()
    {
        for (GUIWidget* widget : _widgets)
        {
            widget->Draw();
        }
    }
    
    bool GUIManager::OnCursorPress( int x, int y ) {
        for (GUIWidget* widget : _widgets)
        {
            if( widget->Contains(x, y) )
            {
                widget->OnInteract(true);
                return true;
            }
        }
        return false;
    }
    
    bool GUIManager::OnCursorRelease( int x, int y ) {
        for (GUIWidget* widget : _widgets)
        {
            if( widget->Contains(x, y) )
            {
                widget->OnInteract(false);
                return true;
            }
        }
        return false;
    }
    
    bool GUIManager::OnCursorHover( int x, int y ) {
        bool overWidget = false;
        for (GUIWidget* widget : _widgets)
        {
            if( widget->Contains(x, y) )
            {
                widget->SetFocus(true);
                overWidget = true;
            } else {
                widget->SetFocus(false);
            }
        }
        return overWidget;
    }
    
    bool GUIManager::OnEvent( const typeInputEvent& theEvent, const float& amount )
    {
        if ( theEvent == "shoot")
        {
            if ( amount == -1 )
            {
                return OnCursorRelease(currentMouseCoord.x, currentMouseCoord.y);
            }
            else if ( amount == 1 )
            {
                return OnCursorPress(currentMouseCoord.x, currentMouseCoord.y);
            }
        }
        return false;
    }
    
    bool GUIManager::OnMouse( const int& x, const int& y )
    {
        currentMouseCoord.x = x;
        currentMouseCoord.y = y;
        return OnCursorHover(x, y);;
    }
}   /* namespace GUI */
