//
//  Texture.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 1/12/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <iostream>
#include "Texture.h"
#include "PNG/png.h"
#include "PNG/pngstruct.h"
#include "Console.h"

Texture::Texture( std::string fileName,
                 GLint wrap,
                 GLint minF,
                 GLint magF,
                 GLint level) :
loaded(false),
textureID(0),
wrapMethod(wrap),
minFilter(minF),
magFilter(magF),
mipLevel(level),
refCount(0)
{
    LoadFromFile( fileName );
}
Texture::~Texture()
{
    if ( loaded ) {
        printf("tried to destroy texture without unloading first, tsk tsk");
        Unload();
    }
}
void Texture::LoadFromFile( const std::string fileName ) {
    // Check filename extension
    if ( fileName.length() >= 4 )
    {
        if ( fileName.compare (fileName.length() - 4, 4, ".png") == 0 )
        {
            LoadFromPNG( fileName );
        }
        else
        {
            // File extension not .png, unsupported format?
            Console::Print( "Can't load texture %s, unsupported format", fileName.c_str() );
        }
    }
    else
    {
        // Filename was too short, can't be .png file
        Console::Print( "Can't load texture %s, filename too short", fileName.c_str() );
    }
    // Save file path if it exists
    size_t lastSlash = fileName.find_last_of('/');
    if ( lastSlash != std::string::npos ) {
        filePath = fileName.substr(0,lastSlash+1);
    }
}

void Texture::LoadFromPNG(const std::string fileName)
{
	if (loaded) {
		Console::Print("Error: texture already loaded\n");
		return;
	}
	png_byte header[8];

#ifdef _WIN32
    FILE *fp = NULL;
	fopen_s(&fp, fileName.c_str(), "rb");
#else
	FILE *fp = fopen(fileName.c_str(), "rb");
#endif
    if (fp == NULL) {
        perror(fileName.c_str());
        return;
    }
    
    // Read and check the header
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        Console::Print( "[Texture] error: %s is not a PNG.\n", fileName.c_str());
        fclose(fp);
        return;
    }
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        Console::Print( "[Texture] error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return;
    }
    
    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        Console::Print( "[Texture] error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return;
    }
    
    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        Console::Print( "[Texture] error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return;
    }
    
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        Console::Print( "[Texture] error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return;
    }
    
    // init png reading
    png_init_io(png_ptr, fp);
    
    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
    
    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);
    
    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;
    
    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
                 NULL, NULL, NULL);
    
    width = temp_width;
    height = temp_height;
    bpp = bit_depth;
    if ( color_type == PNG_COLOR_TYPE_RGB ) {
        format = GL_RGB;
    } else if ( color_type == PNG_COLOR_TYPE_RGB_ALPHA ) {
        format = GL_RGBA;
    }
    
    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);
    
    // Row size in bytes.
    unsigned long rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);
    
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (unsigned char *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        Console::Print( "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return;
    }
    
    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (unsigned char **)malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        Console::Print( "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return;
    }
    
    // set the individual row_pointers to point at the correct offsets of image_data
    for ( unsigned int i = 0; i < temp_height; i++ ) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }
    
    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
    
    // Generate the OpenGL texture object
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    if (mipLevel) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);

    loaded = true;
}

void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
                             png_size_t byteCountToRead)
{
    if(png_ptr->io_ptr == NULL)
        return;   // add custom error handling here
    
    // -> replace with your own data source interface
    const char* inputData = (const char*)png_ptr->io_ptr;

    memcpy(outBytes, inputData, (size_t)byteCountToRead);
    
}  // end ReadDataFromInputStream()

// Utility function to read custom png data chunks
int ReadCustomChunk(png_structp ptr, png_unknown_chunkp chunk) {
	static int unknown_numb = 0;
	printf("[Texture] Reading custom chunk [%s] @%p (%zu octets)\n", chunk->name, chunk->data, chunk->size);
	unknown_numb++;
	return 1;
}

void Texture::LoadFromPNGData( const char* data )
{
    if ( loaded ) {
        Console::Print( "[Texture] Error: texture already loaded" );
        return;
    }
    png_byte header[8];
        
    // Read and check the header
    memcpy(header, data, 8);
    
    if (png_sig_cmp(header, 0, 8)) {
        Console::Print( "[Texture] error: data is not a PNG.\n");
        return;
    }
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        Console::Print( "[Texture] error: png_create_read_struct returned 0.");
        return;
    }


//    // IGNORE UNKNOWN CHUNKS
//    static /* const */ png_byte chunks_to_ignore[] = {
//        01, 00, 00, 00, '\0',
//        99,  72,  82,  77, '\0',  /* cHRM */
//        104,  73,  83,  84, '\0',  /* hIST */
//        105,  67,  67,  80, '\0',  /* iCCP */
//        105,  84,  88, 116, '\0',  /* iTXt */
//        111,  70,  70, 115, '\0',  /* oFFs */
//        112,  67,  65,  76, '\0',  /* pCAL */
//        112,  72,  89, 115, '\0',  /* pHYs */
//        115,  66,  73,  84, '\0',  /* sBIT */
//        115,  67,  65,  76, '\0',  /* sCAL */
//        115,  80,  76,  84, '\0',  /* sPLT */
//        115,  84,  69,  82, '\0',  /* sTER */
//        116,  69,  88, 116, '\0',  /* tEXt */
//        116,  73,  77,  69, '\0',  /* tIME */
//        122,  84,  88, 116, '\0'   /* zTXt */
//    };
//    png_set_keep_unknown_chunks(png_ptr, 1 /* PNG_HANDLE_CHUNK_NEVER */,
//                                chunks_to_ignore, sizeof(chunks_to_ignore)/5);

//    png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_NEVER, NULL, 0);

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        Console::Print( "[Texture] error: png_create_info_struct returned 0.");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return;
    }
    
    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        Console::Print( "[Texture] error: png_create_info_struct returned 0.");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        return;
    }
    
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        Console::Print( "[Texture] error from libpng");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return;
    }
    // set data loading func
    png_set_read_fn(png_ptr, &data, ReadDataFromInputStream);

    // init png reading - apparently this is not necessary :/
//    png_init_io(png_ptr, data);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
    
#if defined(PNG_HANDLE_AS_UNKNOWN_SUPPORTED)
	// Setting unknown-chunk callback
	png_set_read_user_chunk_fn(png_ptr, NULL, (png_user_chunk_ptr)ReadCustomChunk);
	
    /* Ignore unused chunks */
    png_set_keep_unknown_chunks(png_ptr, 3, NULL, 0);
#endif
#if defined(PNG_BENIGN_ERRORS_SUPPORTED)
    printf("[Texture] PNG Errors benign\n");
#endif
#if defined(PNG_BENIGN_READ_ERRORS_SUPPORTED)
    printf("[Texture] PNG Errors read benign\n");
#endif
    png_set_benign_errors(png_ptr, 1);
    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;
    
    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
                 NULL, NULL, NULL);

    width = temp_width;
    height = temp_height;
    bpp = bit_depth;
    if ( color_type == PNG_COLOR_TYPE_RGB ) {
        format = GL_RGB;
    } else if ( color_type == PNG_COLOR_TYPE_RGB_ALPHA ) {
        format = GL_RGBA;
    }
    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    unsigned long rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);
    
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (unsigned char *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        Console::Print( "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return;
    }
    
    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (unsigned char **)malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        Console::Print( "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        return;
    }
    
    // set the individual row_pointers to point at the correct offsets of image_data
    for ( unsigned int i = 0; i < temp_height; i++ ) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }
    
    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
    
    // Generate the OpenGL texture object
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    if (mipLevel) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    
    loaded = true;
}

void Texture::Bind() const
{
    if ( !loaded ) {
        Console::Print( "Error: can't bind texture %i, it wasn't loaded\n", textureID );
        return;
    }
    glBindTexture(GL_TEXTURE_2D, textureID );
}
void Texture::Unload()
{
    if ( !loaded ) {
        Console::Print( "Error: can't unload texture %i, it wasn't loaded\n", textureID );
        return;
    }
    glDeleteTextures(1, &textureID);
    loaded = false;
}

void Texture::Retain()
{
    refCount++;
}
void Texture::Release()
{
    if ( refCount == 0 )
    {
        Console::Print( "Error: releasing texture %i, with 0 refCount\n", textureID );
        return;
    }
    refCount--;
}

void Texture::DrawAt(int posX, int posY)
{
    const GLint verts[] = {
        posX, posY,
        posX + width, posY,
        posX + width, posY + height,
        posX, posY + height
    };
    const GLfloat texVerts[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    
    // Bind the texture, enable the proper arrays
    Bind();
    // Enable plain 2D texturing
    glEnable( GL_TEXTURE_2D );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    glVertexPointer(2, GL_INT, 0, verts);
    glTexCoordPointer(2, GL_FLOAT, 0, texVerts);
    glDrawArrays(GL_QUADS, 0, 4);
}

