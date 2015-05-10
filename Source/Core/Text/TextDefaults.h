//
//  TextDefaults.h
//  Ingenium
//
//  Created by The Drudgerist on 08/02/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#ifndef Ingenium_TextDefaults_h
#define Ingenium_TextDefaults_h

/**********************************************************************
 * Default shader programs
 *********************************************************************/
static const GLchar *text_vertex_shader21[] = {
    "#version 120\n"
    "attribute vec4 coord;"
    "varying vec2 texcoord;"
    "void main(void) {"
    "gl_Position = vec4(coord.xy, 0, 1);"
    "texcoord = coord.zw;"
    "}"
};
static const GLchar *text_frag_shader21[] = {
    "#version 120\n"
    "varying vec2 texcoord;"
    "uniform sampler2D tex;"
    "uniform vec4 color;"
    "void main(void) {"
    "float intensity = texture2D(tex, texcoord).r;"
    "if (intensity <= 0.001) discard;"
    "gl_FragColor = vec4(1, 1, 1, intensity) * color;"
    "}"
};
static const GLchar *text_vertex_shader32[] = {
    "#version 400\n"
    "layout(location = 0) in vec4 vCoord;"
    "layout(location = 1) in vec2 tCoord;"
    "uniform mat4 MVP;"
    "out vec2 texCoord;"
    "void main(void) {"
    "gl_Position = MVP * vec4(vCoord.xyz,1);"
    "texCoord = tCoord;"
    "}"
};
static const GLchar *text_frag_shader32[] = {
    "#version 400\n"
    "in vec2 texCoord;"
    "out vec4 fragColor;"
    "uniform sampler2D tex;"
    "uniform vec4 color;"
    "void main(void) {"
    "float intensity = texture(tex, texCoord).r;"
    "if (intensity <= 0.001) discard;"
    "fragColor = vec4(1, 1, 1, intensity) * color;"
    "}"
};

#endif
