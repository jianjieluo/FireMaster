#ifndef YELLOWTANK_H_
#define YELLOWTANK_H_
#include "BaseTank.h"

class YellowTank : public BaseTank
{
public:
    YellowTank();
    ~YellowTank();
    static YellowTank* create();

    // ��ʼ������������������Ϣ
    void initOptions() override;
    virtual void setDefaultProperty() override final;

    virtual int getBulletHurt() const override final;
    virtual Vec2 getBulletVelocity() const override final;
    virtual Animate getAttackAnimate() const override final;
    virtual Animate getAfterAttackAnimate() const override final;

private:
    // 不同的坦克可以使用不同的子弹图片
    const std::string default_bullet_name = "bullet1.png";
};

#endif