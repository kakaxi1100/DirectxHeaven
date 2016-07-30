#pragma once

class B;
class A 
{
public:
	A() = default;
	void fooA(B& b);//error
	B& b;
};
