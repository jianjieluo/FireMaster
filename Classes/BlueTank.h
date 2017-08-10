#ifndef BLUETANK_H_
#define BLUETANK_H_

#include "BaseTank.h"

class BlueTank : public BaseTank
{
public:
    BlueTank();
    ~BlueTank();

    static BlueTank* create();

    // ��ʼ������������������Ϣ
    void initOptions() override final;
    virtual void setDefaultProperty() override final;

    virtual int getBulletHurt() const override final;
    virtual Vec2 getBulletVelocity() const override final;
    virtual Animate getAttackAnimate() const override final;
    virtual Animate getAfterAttackAnimate() const override final;

private:
    // 不同的坦克可以使用不同的子弹图片
    const std::string default_bullet_name = "bullet2.png";
};

#endif