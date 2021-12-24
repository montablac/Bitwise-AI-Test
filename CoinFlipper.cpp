#include "CoinFlipper.h"
#include<ctime>

Rando * Rando::GetRando()
{
	if (rando == 0)
	{
		rando = new Rando;
	}

	return rando;
}

unsigned int Rando::GetRandomNumber()
{
	return RandEngine();
}

unsigned int Rando::GetMax()
{
	unsigned int Max = RandEngine.max();
	return Max;
}

Rando::~Rando()
{
	if (rando != 0)
	{
		delete rando;
	}
}

Rando::Rando()
{
	RandEngine.seed(time(NULL));
}

Rando* Rando::rando = 0;