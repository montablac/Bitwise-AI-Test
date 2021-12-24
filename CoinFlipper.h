#pragma once
#include<random>

class Rando
{
public:
	static Rando* GetRando();
	unsigned int GetRandomNumber();
	unsigned int GetMax();

	~Rando();
private:
	Rando();
	static Rando* rando;
	std::default_random_engine RandEngine;
};