//
//  CCMask.cpp
//
//  Created by Akihiro Matsuura on 1/22/13.
//
//


#include "CCMask.h"
USING_NS_CC;

CCMask::CCMask()
: m_pSprite(NULL)
, m_pMaskTexture(NULL)
, m_uTextureLocation(0)
, m_uMaskLocation(0)
, m_pMaskSprite(NULL)
{
    
}

CCMask* CCMask::create(Sprite *maskSprite, Sprite * sprite)
{
    CCMask* pRet = new CCMask();
    if (pRet && pRet->initWithMaskSprite(maskSprite, sprite))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCMask::initWithMaskSprite(Sprite *maskSprite, Sprite *sprite)
{
    if (CCSprite::initWithTexture(maskSprite->getTexture()))
    {
        this->setSprite(sprite);
        
        // Set up the mask with the Mask shader
        this->setMaskSprite(maskSprite);
        this->setMaskTexture(maskSprite->getTexture());
        GLProgram* pProgram = new GLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccExSwitchMask_frag);
        this->setShaderProgram(pProgram);
        pProgram->release();
        
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        this->getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        this->getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->link();
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        
        m_uTextureLocation    = glGetUniformLocation(this->getShaderProgram()->getProgram(), "u_texture");
        m_uMaskLocation       = glGetUniformLocation(this->getShaderProgram()->getProgram(), "u_mask");
        CHECK_GL_ERROR_DEBUG();
        
        this->setContentSize(m_pMaskTexture->getContentSize());
        
        needsLayout();
        return true;
    }
    return false;
}

void CCMask::draw()
{
    CC_NODE_DRAW_SETUP();
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    this->getShaderProgram()->setUniformForModelViewProjectionMatrix();
  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, this->getTexture()->getName());
    glUniform1i(m_uTextureLocation, 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture( GL_TEXTURE_2D, m_pMaskTexture->getName() );
    glUniform1i(m_uMaskLocation, 1);
    
#define kQuadSize sizeof(_quad.bl)
    long offset = (long)&_quad;
    
    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glActiveTexture(GL_TEXTURE0);
}

void CCMask::needsLayout()
{
    if (m_pSprite==NULL) return;
    
    m_pSprite->setFlippedY(true);
    RenderTexture *rt = RenderTexture::create((int)m_pMaskTexture->getContentSize().width, (int)m_pMaskTexture->getContentSize().height);
    
    rt->begin();
    m_pSprite->visit();
    rt->end();
    
    this->setTexture(rt->getSprite()->getTexture());
}

void CCMask::changeObject(Sprite* object_)
{
    this->setSprite(object_);
    this->needsLayout();
}

CCMask::~CCMask() {

}