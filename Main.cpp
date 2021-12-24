#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include"AI_Core.h"
#include"CoinFlipper.h"

using namespace std;

#define MAX_MINDS 30
#define MIND_RUNS 50

struct test
{
	AI mind;
	unsigned char cInput[MIND_RUNS];
	unsigned char cOutput[MIND_RUNS];
	unsigned int cScore;
};

bool Compare(const test* One, const test* Two)
{
	return One->cScore < Two->cScore;
}

unsigned int ScoreMind(const test* Mind)
{
	unsigned int iReturn = 0;
	unsigned int iStreak = 1;

	for (int i = 0; i < MIND_RUNS; i++)
	{
		for (int y = 0; y < 8; y++)
		{
			if ((Mind->cInput[i] ^ Mind->cOutput[i]) & (0x01 << y))
			{
				iReturn += iStreak;
				iStreak++;
			}
			else
			{
				iStreak = 1;
			}
		}
	}

	return iReturn;
}

int main()
{
	test** Minds = new test*[MAX_MINDS];

	bool bRecord = false;
	fstream Record;

	Record.open("AiRecord.csv", fstream::out | fstream::trunc);

	if (Record.is_open())
	{
		bRecord = true;

		Record << ",";

		for (int i = 0; i < MAX_MINDS; i++)
		{
			Record << "Mind " << i + 1 << ",";
		}

		Record << "\n";
	}

	unsigned short usBuffer = 10;

	for (int i = 0; i < MAX_MINDS; i++)
	{
		Minds[i] = new test;

		Minds[i]->mind.SetInputSize(sizeof(unsigned char));
		Minds[i]->mind.SetBufferSize(usBuffer);
		Minds[i]->mind.SetOutputSize(sizeof(unsigned char));
	}

	bool bKeepRunning = true;
	unsigned int uiPerfectScore = 0;
	unsigned int uiStreak = 1;

	for (int i = 0; i < MIND_RUNS; i++)
	{
		for (int y = 0; y < 8; y++)
		{
			uiPerfectScore += uiStreak;
			uiStreak++;
		}
	}

	unsigned int uiLastScore = 0;
	unsigned int uiLastBest = 0;
	unsigned int uiHighest = 0;

	unsigned short usTotalRuns = 0;
	unsigned short usStagnant = 0;

	unsigned short usMaxBuffer = 100;
	unsigned short usMaxStagnant = 15;
	unsigned short usMaxRuns = 5000;

	unsigned int uiRunAverage = 0;
	unsigned int uiTotalAverage = 0;

	Rando* NumberGen = Rando::GetRando();

	unsigned char cTemp;

	bool bSwap;
	bool bMix = true;

	do 
	{
		usTotalRuns++;
		//running the mind
		for (int i = 0; i < MAX_MINDS; i++)
		{
			for (int y = 0; y < MIND_RUNS; y++)
			{
				cTemp = NumberGen->GetRandomNumber() % 0xff;
				Minds[i]->cInput[y] = cTemp;
				Minds[i]->mind.SetInput(&cTemp, sizeof(unsigned char));
				Minds[i]->mind.Process();
				Minds[i]->mind.GetOutput(&cTemp, sizeof(unsigned char));
				Minds[i]->cOutput[y] = cTemp;
			}

			Minds[i]->cScore = ScoreMind(Minds[i]);

			uiRunAverage += Minds[i]->cScore;
		}

		uiRunAverage = uiRunAverage / MAX_MINDS;
		uiTotalAverage += uiRunAverage;

		//sorting
		cout << "Run number " << usTotalRuns <<", sorting..." << endl;
		for (int i = MAX_MINDS - 1; i > 0; i--)
		{
			for (int x = 0; x < i; x++)
			{
				bSwap = Compare(Minds[x], Minds[x + 1]);

				if (bSwap)
				{
					test* temp = Minds[x];
					Minds[x] = Minds[x + 1];
					Minds[x + 1] = temp;
				}
			}
		}

		uiLastScore = Minds[0]->cScore;

		//cout << "Best score for generation " << usTotalRuns << " is " << uiLastScore << endl;
		//cout << "Worst score for generation " << usTotalRuns << " is " << Minds[MAX_MINDS - 1]->cScore << endl;
		//cout << "Best score overall is " << uiHighest << endl;
		//cout << "Average score is " << uiRunAverage << endl;

		if (bRecord)
		{
			Record << usTotalRuns << ",";
			for (int i = 0; i < MAX_MINDS; i++)
			{
				Record << Minds[i]->cScore << ",";
			}
			Record << "\n";
		}

		if (uiLastScore > uiLastBest)
		{
			uiLastBest = uiLastScore;

			if (uiLastBest > uiHighest)
			{
				uiHighest = uiLastBest;
			}

			if (usStagnant != 0)
			{
				usStagnant--;
			}
		}
		else
		{
			usStagnant++;

			if (usStagnant > (usMaxStagnant * (usBuffer + 1)) && usBuffer != usMaxBuffer)
			{
				usBuffer++;

				cout << "stagnation found, increasing buffer size to " << usBuffer << " bytes" << endl;

				usStagnant = 0;

				for (int i = 0; i < MAX_MINDS; i++)
				{
					Minds[i]->mind.SetBufferSize(usBuffer);
					Minds[i]->mind.Setup();

					uiLastBest = 0;
					bMix = false;
				}
			}
		}

		if (bMix)
		{
			double Mutation;
			double Favor;

			unsigned char cFirst;
			unsigned char cSecond;

			AI* First;
			AI* Second;

			for (int i = MAX_MINDS / 4; i < MAX_MINDS; i++)
			{
				do
				{
					cFirst = 0;
					cSecond = NumberGen->GetRandomNumber() % i;
				} while (cFirst == cSecond);

				Mutation = (uiPerfectScore - ((double)(Minds[cSecond]->cScore) + (double)(Minds[cFirst]->cScore))) / uiPerfectScore;

				First = &(Minds[cFirst]->mind);
				Second = &(Minds[cSecond]->mind);

				Favor = (double)Minds[cFirst]->cScore / (double)(Minds[cFirst]->cScore + Minds[cSecond]->cScore);

				Minds[i]->mind.Setup(Mutation, Favor, First, Second);
			}
		}
		else
		{
			bMix = true;
		}

		if (uiLastScore == uiPerfectScore || usTotalRuns > usMaxRuns)
		{
			bKeepRunning = false;
		}

	} while (bKeepRunning);

	if (bRecord)
	{
		Record.close();
	}

	uiTotalAverage = uiTotalAverage / usTotalRuns;

	cout << endl;
	cout << "Highest score:" << uiHighest << endl;
	cout << "Average score:" << uiTotalAverage << endl;
	cout << "Perfect score:" << uiPerfectScore << endl;

	for (int i = 0; i < MAX_MINDS; i++)
	{
		delete Minds[i];
	}
	delete Minds;

	return 0;
}