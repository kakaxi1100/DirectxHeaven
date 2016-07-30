#include "B.h"
#include "A.h"
void B::fooB(A& a)
{
	a.fooA(*this);
}