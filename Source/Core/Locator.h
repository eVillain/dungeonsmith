//
//  Locator.h
//  Ingenium
//
//  Created by The Drudgerist on 26/01/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//
//  Service locator class as a brute method for accessing engine singletons
//

#ifndef NGN_LOCATOR_H
#define NGN_LOCATOR_H

#include "IRenderer.h"
#include "HyperVisor.h"
#include "IText.h"

class Locator {
public:
    static HyperVisor& getHyperVisor(){ return *_hv; };
    // Getters to current services
    static IRenderer& getRenderer() { return *_renderService; }
    static IText& getText() { return *_textService; };

    // Setters for configuring services
    static void provideRenderer(IRenderer* service)
    {
        if (service == NULL) {
            _renderService = &_nullRenderService;
        } else {
            _renderService = service;
        }
    }
    static void provideText(IText* service)
    {
        if (service == NULL) {
            _textService = &_nullTextService;
        } else {
            _textService = service;
        }
    }
    static void provideHyperVisor(HyperVisor* service)
    {
        _hv = service;
    }
    
private:
    // Renderer
    static IRenderer* _renderService;
    static NullRenderer _nullRenderService;
    // Text manager
    static IText* _textService;
    static NullText _nullTextService;
    
    // Hypervisor, should be removed for release build
    static HyperVisor* _hv;
};

#endif /* defined(NGN_LOCATOR_H) */
