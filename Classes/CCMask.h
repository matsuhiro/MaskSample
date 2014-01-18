//
//  CCMask.h
//
//  Created by Akihiro Matsuura on 1/22/13.
//
//

#ifndef __Tailer__CCMask__
#define __Tailer__CCMask__

#include "cocos2d.h"
using namespace cocos2d;

class CCMask : public Sprite {
private:
    void needsLayout();
public:
    void draw();
    CC_SYNTHESIZE_RETAIN(Sprite*, m_pSprite, Sprite)
    CC_SYNTHESIZE_RETAIN(Texture2D*, m_pMaskTexture, MaskTexture)
    CC_SYNTHESIZE(GLuint, m_uTextureLocation, TextureLocation)
    CC_SYNTHESIZE(GLuint, m_uMaskLocation, MaskLocation)
    CC_SYNTHESIZE_RETAIN(Sprite*, m_pMaskSprite, MaskSprite);
    
    CCMask();
    virtual ~CCMask();

    // Initialize a masked object based on an object sprite and a mask sprite
    bool initWithMaskSprite(Sprite *maskSprite, Sprite *sprite);
    static CCMask* create(Sprite *maskSprite, Sprite * sprite);

    void changeObject(Sprite* object);
};

#endif /* defined(__Tailer__CCMask__) */
