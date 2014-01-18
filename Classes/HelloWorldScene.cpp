#include "HelloWorldScene.h"
#include "CCMask.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

CCMask* createMasked(Sprite* mask, Color3B color) {
    auto size = mask->getContentSize();
    RenderTexture* pRenderTexture = RenderTexture::create(size.width, size.height);
    auto r = color.r / 255.0f;
    auto g = color.g / 255.0f;
    auto b = color.b / 255.0f;
    pRenderTexture->beginWithClear(r, g, b, 1);
    pRenderTexture->end();
    
    auto sprite = Sprite::createWithTexture(pRenderTexture->getSprite()->getTexture());
    sprite->setPosition(Point(mask->getContentSize().width/2, mask->getContentSize().height/2));
    
    CCMask* masked = CCMask::create(mask , sprite);
    return masked;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    auto center = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height /2);
    
    auto sprite = Sprite::create("mask.png");
    sprite->setAnchorPoint(Point(0.5, 0.5));
    sprite->setPosition(center + Point(-200, 0));
    this->addChild(sprite, 0);
    
    auto masked = createMasked(sprite, Color3B(100, 255, 255));
    masked->setAnchorPoint(Point(0.5, 0.5));
    masked->setPosition(center + Point(200, 0));
    this->addChild(masked, 1);
  
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
