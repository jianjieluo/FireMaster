# Fire Master 打炮专家
A funny cocos2dx game ! Learn From the idea of a game called Fleabag Vs. Mutt.

## Demo
![](https://raw.githubusercontent.com/mgsweet/FireMaster/master/project_images/Demonew.png)

## 游戏介绍
打炮专家是一个Q版的回合制射击类双人对战游戏。夕阳下，玩家将在父子局对战中击败对手证明自己无人能敌的打炮能力。

### 1. 玩法说明
玩家通过在自己回合中通过改变按压坦克时间长短来控制坦克发射导弹的力度，导弹在飞行过程中会受到风力影响，导弹击中敌方坦克将会爆炸并对敌方坦克造成伤害，伤害量的大小与爆炸中心到坦克距离有关，最大伤害量为初始伤害量的2倍，期间玩家可通过使用技能调整战略，每个技能可使用一次，同时使用每个技能消耗一个回合数，血量最先到达0的玩家将会被判为战败。
### 2. 技能说明
![](https://raw.githubusercontent.com/mgsweet/FireMaster/master/project_images/bullet.png)
普通炮弹 普通伤害10%-20% 可用次数无限

![](https://raw.githubusercontent.com/mgsweet/FireMaster/master/project_images/powerBullet.png)
强爆导弹 普通伤害20%-40%  可用一次

![](https://raw.githubusercontent.com/mgsweet/FireMaster/master/project_images/defenceUI.png)
矩阵防御 可防御一次敌方攻击

![](https://raw.githubusercontent.com/mgsweet/FireMaster/master/project_images/fixUI.png)
临时修理 恢复30%总生命值

![](https://raw.githubusercontent.com/mgsweet/FireMaster/master/project_images/tribbleAttackUI.png)
三炮齐发 蓄力一次，按照该蓄力力度发射三发普通炮弹
### 3. 未来扩展计划
增加坦克样式，与金币购买制度
增加单人模式与多种难度的AI对战
增加技能——可控制导弹
增加功能——对战时地图随机生成的可靠攻击获取的技能包

## 注意
Win32的背景音乐音量调整函数官方没有实现，如果需要达到预期效果，需要使用非预编译的项目并将项目中特定文件替换，替换文件可在本项目中的musicDependence文件夹中获取，代码适用于cocos2dx-3.15版本。
