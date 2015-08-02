//
//  StencilUtils.h
//  DungeonSmith
//
//  Created by eVillain on 31/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_StencilUtils_h
#define DungeonSmith_StencilUtils_h

class Stencil {
public:
    typedef enum : GLint {
        Layer_Clear = 0,
        Layer_Sky = 1,
        Layer_Solid = 2,
        Layer_Light = 3,
        Layer_Transparent = 4,
    } Layer;
    
    static void Enable()
    {
        glEnable(GL_STENCIL_TEST);
    }
    
    static void Disable()
    {
        glDisable(GL_STENCIL_TEST);
    }
    
    static void SetReplaceEqual( const Layer layer )
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_EQUAL, layer, 0xFF);
    }
    
    static void SetReplaceLower( const Layer layer )
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_GEQUAL, layer, 0xFF);
    }
    
    static void SetReplaceGreater( const Layer layer )
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_LEQUAL, layer, 0xFF);
    }
    
    static void SetKeepEqual( const Layer layer )
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_EQUAL, layer, 0xFF);
    }
    
    static void SetKeepLower( const Layer layer )
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_GEQUAL, layer, 0xFF);
    }
    
    static void SetKeepGreater( const Layer layer )
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_LEQUAL, layer, 0xFF);
    }
    
    static void Copy( const GLuint fromFBO, const GLuint toFBO, const GLint width, const GLint height )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, toFBO);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fromFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, toFBO);
        glBlitFramebuffer(0, 0, width, height,
                          0, 0, width, height,
                          GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
};

#endif
