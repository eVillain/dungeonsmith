#ifndef NGN_SPRITE_H
#define NGN_SPRITE_H

#include <string>
#include "RenderDefines.h"
#include "Texture.h"
#include "Color.h"
#include "Rect2D.h"

class SpriteBatch;

class Sprite {
public:
    static Sprite* SpriteFromFile(const std::string filePath,
                                  const std::string fileName,
                                  const glm::vec2 position = glm::vec2());
    
    Sprite(const glm::vec2 position,
           const glm::vec2 size,
           Texture * tex,
           const Rect2D texRect,
           const Color& col = COLOR_WHITE );
    Sprite(const glm::vec2 position,
           SpriteBatch* batch,
           const std::string frame,
           const Color& col = COLOR_WHITE );
    virtual ~Sprite();
    
    // Utility for extending this class into 'smarter' sprites which need update
    virtual void Update( double dt ) {};
    
    void SetFrame(const glm::vec2 size, const Rect2D texRect, const int batchID);
    
    void FitToRect(const Rect2D& rect );
    
    void Draw();

    void Visit();
    
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 scale;
    glm::vec2 rotation;
    Color color;
    
private:
    const SpriteBatch* spriteBatch;
    Texture* texture;

    float z;

    Rect2D texRect;
    
    bool batched;               // Is part of spritebatch
    int batchID;                // ID for frame in batch
};

#endif
