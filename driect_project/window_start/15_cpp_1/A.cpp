#include "A.h"
#include "B.h"
void A::fooA(B& b)
{
	b.fooB(*this);
}
