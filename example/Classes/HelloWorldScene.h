#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "../../oreore/oreore/oreore.h"

class HelloWorld final : public oreore::SceneBase<HelloWorld>
{
private:
protected:
    bool init() override;
public:
    void menuCloseCallback(cocos2d::Ref* sender);
    CREATE_FUNC(HelloWorld);
};

#endif
