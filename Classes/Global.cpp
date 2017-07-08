#include "Global.h"

using namespace std;

int Global::turn = 0;
bool Global::winSide = false;
bool Global::isGameover = false;
std::list<Bullet*> Global::bullets;

Global::Global()
{
}

Global::~Global()
{
}


