
#include <algorithm>
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Locator.h"
#include "IRenderer.h"
#include "Texture.h"
#include "TextureManager.h"


Sprite::Sprite(const glm::vec2 position,
               const glm::vec2 size,
               Texture * tex,
               const Rect2D texRect,
               const Color& col ) :
texture(tex),
position(position),
size(size),
z(0),
scale(1.0, 1.0),
rotation(0.0, 0.0),
color(col),
texRect(texRect),
batched(false),
batchID(-1)
{
    //texture->refCount++;
}
Sprite::Sprite(const glm::vec2 position,
               SpriteBatch* batch,
               const std::string frame,
               const Color& col ) :
position(position)
{
    if ( batch == NULL ) {
        printf( "Failing to init sprite, no batch for frame: %s\n", frame.c_str() );
        return;
    }
    spriteBatch = batch;
    texture = spriteBatch->GetTexture();
    Rect2D rect = spriteBatch->GetRectForFrame( frame );
    texRect = spriteBatch->GetTexRectForFrame( frame );
    z=0;
    scale = glm::vec2(1.0f,1.0f);
    rotation = glm::vec2(0.0f,0.0f);
    color = col;
    batched = true;
    batchID = batch->GetIDForFrame( frame );    
}

Sprite* Sprite::SpriteFromFile( const std::string filePath, const std::string fileName, glm::vec2 position )
{
    Texture* tex = Locator::getRenderer().Textures()->LoadTexture( filePath, fileName );
    if (!tex) {
        return NULL;
    }    
    Sprite* spr = new Sprite(position,
                             glm::vec2(tex->GetWidth(), tex->GetHeight()),
                             tex,
                             Rect2D(0.0f,0.0f,1.0f,1.0f));
    return spr;
}

Sprite::~Sprite() {
    if (!batched) {
        texture->Release();
        Locator::getRenderer().Textures()->UnloadTexture( texture );
    }
}

void Sprite::SetFrame(const glm::vec2 size, const Rect2D texRect, const int batchID)
{
    this->size.x = size.x;
    this->size.y = size.x;
    this->texRect = texRect;
    this->batchID = batchID;
}

// Resize sprite to fit a rectangle
void Sprite::FitToRect( const Rect2D &r ) {
    if ( size.x > r.w ||
        size.y > r.h ) {
        float scaleX = r.w / size.x;
        float scaleY = r.h / size.y;
        float minScale = (std::min)(scaleX, scaleY);
        size.x = size.x * minScale;
        size.y = size.y * minScale;
    }
    position.x = r.x;
    position.y = r.y;
}
