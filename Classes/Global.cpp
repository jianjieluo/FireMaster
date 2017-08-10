#include "Global.h"

using namespace std;

int Global::turn = 0;
bool Global::winSide = false;
bool Global::isGameover = false;
std::list<Bullet*> Global::bullets;

//ȫ�ֲ�������
int Global::smallBullet = 15;
int Global::bigBullet = 30;
// ���µ�ʱ����������������
float Global::pressv = 1;
int Global::windPower = 3;

Global::Global()
{
}

Global::~Global()
{
}


