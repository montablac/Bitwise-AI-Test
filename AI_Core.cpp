#include"AI_Core.h"
#include"CoinFlipper.h"
#include<time.h>

AI::AI()
{
	p_cInput = 0;
	p_cBuffer = 0;
	p_cOutput = 0;
}

AI::~AI()
{
	if (p_cInput != 0)
	{
		delete p_cInput;
		p_cInput = 0;
	}

	if (p_cBuffer != 0)
	{
		delete p_cBuffer;
		p_cBuffer = 0;
	}

	if (p_cOutput != 0)
	{
		delete p_cOutput;
		p_cOutput = 0;
	}

	Rules.clear();

	bReady = false;
}

bool AI::Setup(double dMutation, double dFavor, AI* p_FirstMind, AI* p_SecondMind)
{
	bool bReturn = false;

	if (Rules.size() != 0)
	{
		Rules.clear();
	}

	Rando* Mutation = Rando::GetRando();

	Rule OldRule;

	Rule FirstCompare;
	Rule SecondCompare;
	bool bMatch;

	unsigned int uiFavorCap = Mutation->GetMax() * dFavor;
	unsigned int uiFavor;

	unsigned int uiRulesToMake = (usBufferLen + usOutputLen) * 8;

	if (usInputLen != 0 && usOutputLen != 0)
	{
		for (int i = 0; i < uiRulesToMake; i++)
		{
			bMatch = false;

			if (p_FirstMind != 0 && p_SecondMind != 0)
			{
				p_FirstMind->GetRule(i, &FirstCompare);
				p_SecondMind->GetRule(i, &SecondCompare);

				if (FirstCompare.cComparison == SecondCompare.cComparison)
				{
					dMutation = dMutation * 0.25;
				}
				if (FirstCompare.uiBitOne == SecondCompare.uiBitOne)
				{
					dMutation = dMutation * 0.25;
				}
				if (FirstCompare.uiBitTwo == SecondCompare.uiBitTwo)
				{
					dMutation = dMutation * 0.25;
				}
			}

			uiFavor = Mutation->GetRandomNumber();

			if (uiFavor < uiFavorCap)
			{
				if (p_FirstMind != 0)
				{
					p_FirstMind->GetRule(i, &OldRule);

					GenerateRule(i, dMutation, &OldRule);
				}
				else
				{
					GenerateRule(i, 1.0, 0);
				}
			}
			else
			{
				if (p_SecondMind != 0)
				{
					p_SecondMind->GetRule(i, &OldRule);

					GenerateRule(i, dMutation, &OldRule);
				}
				else
				{
					GenerateRule(i, 1.0, 0);
				}
			}
		}

		if (Rules.size() == uiRulesToMake)
		{
			bReady = true;
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::GetRule(unsigned int uiRuleToGet, Rule* p_Rule)
{
	bool bReturn = false;

	unsigned int uiTotalRules = Rules.size();

	if (p_Rule != 0)
	{
		if (uiRuleToGet <= uiTotalRules)
		{
			*p_Rule = Rules[uiRuleToGet];
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::SetInputSize(unsigned short usDataLen)
{
	bool bReturn = false;

	if (usDataLen != 0)
	{
		usInputLen = usDataLen;

		if (p_cInput != 0)
		{
			delete p_cInput;
			p_cInput = 0;
		}

		p_cInput = new char[usInputLen];

		if (p_cInput != 0)
		{
			bReady = false;
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::SetBufferSize(unsigned short usDataLen)
{
	bool bReturn = false;

	if (usDataLen != 0)
	{
		usBufferLen = usDataLen;

		if (p_cBuffer != 0)
		{
			delete p_cBuffer;
			p_cBuffer = 0;
		}

		p_cBuffer = new char[usBufferLen];

		if (p_cBuffer != 0)
		{
			bReady = false;
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::SetOutputSize(unsigned short usDataLen)
{
	bool bReturn = false;

	if (usDataLen != 0)
	{
		usOutputLen = usDataLen;

		if (p_cOutput != 0)
		{
			delete p_cOutput;
			p_cOutput = 0;
		}

		p_cOutput = new char[usOutputLen];

		if (p_cOutput != 0)
		{
			bReady = false;
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::SetInput(void* vp_Input, unsigned short usSize)
{
	bool bReturn = false;

	if (vp_Input != 0)
	{
		if (usSize > usInputLen)
		{
			SetInputSize(usSize);
		}

		if (memcpy_s(p_cInput, usInputLen, vp_Input, usSize) == 0)
		{
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::GetOutput(void* vp_Output, unsigned short usSize)
{
	bool bReturn = false;

	if (vp_Output != 0)
	{
		if (usSize > usOutputLen)
		{
			SetOutputSize(usSize);
		}

		if (memcpy_s(vp_Output, usSize, p_cOutput, usOutputLen) == 0)
		{
			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::Process()
{
	bool bReturn = false;

	if (!bReady)
	{
		Setup();
	}

	if (bReady)
	{
		unsigned int uiRules = Rules.size();

		for (int i = 0; i < uiRules; i++)
		{
			char cComparison = Rules[i].cComparison & RULE_VALID;

			bool bFirst;
			bool bSecond;

			char cFirst = 0xff;
			char cSecond = 0xff;

			unsigned short usBitBlock;
			unsigned short usBitMask;

			usBitBlock = Rules[i].uiBitOne / 8;
			usBitMask = Rules[i].uiBitOne % 8;

			switch (usBitMask)
			{
			case 0:
				cFirst = cFirst & BIT_ONE;
				break;
			case 1:
				cFirst = cFirst & BIT_TWO;
				break;
			case 2:
				cFirst = cFirst & BIT_THREE;
				break;
			case 3:
				cFirst = cFirst & BIT_FOUR;
				break;
			case 4:
				cFirst = cFirst & BIT_FIVE;
				break;
			case 5:
				cFirst = cFirst & BIT_SIX;
				break;
			case 6:
				cFirst = cFirst & BIT_SEVEN;
				break;
			case 7:
				cFirst = cFirst & BIT_EIGHT;
				break;
			}

			if (usInputLen > usBitBlock)
			{
				bFirst = p_cInput[usBitBlock] & cFirst;
			}
			else if ((usBufferLen + usInputLen) > usBitBlock)
			{
				bFirst = p_cBuffer[(usBitBlock - usInputLen)] & cFirst;
			}
			else
			{
				bFirst = p_cOutput[(usBitBlock - (usInputLen + usBufferLen))] & cFirst;
			}

			usBitBlock = Rules[i].uiBitTwo / 8;
			usBitMask = Rules[i].uiBitTwo % 8;

			switch (usBitMask)
			{
			case 0:
				cSecond = cSecond & BIT_ONE;
				break;
			case 1:
				cSecond = cSecond & BIT_TWO;
				break;
			case 2:
				cSecond = cSecond & BIT_THREE;
				break;
			case 3:
				cSecond = cSecond & BIT_FOUR;
				break;
			case 4:
				cSecond = cSecond & BIT_FIVE;
				break;
			case 5:
				cSecond = cSecond & BIT_SIX;
				break;
			case 6:
				cSecond = cSecond & BIT_SEVEN;
				break;
			case 7:
				cSecond = cSecond & BIT_EIGHT;
				break;
			}

			if (usInputLen > usBitBlock)
			{
				bSecond = p_cInput[usBitBlock] & cSecond;
			}
			else if ((usBufferLen + usInputLen) > usBitBlock)
			{
				bSecond = p_cBuffer[(usBitBlock - usInputLen)] & cSecond;
			}
			else
			{
				bSecond = p_cOutput[(usBitBlock - (usInputLen + usBufferLen))] & cSecond;
			}

			bool bResult = false;

			switch (cComparison)
			{
			case RULE_AND:
				bResult = bFirst && bSecond;
				break;
			case RULE_OR:
				bResult = bFirst || bSecond;
				break;
			case RULE_XOR:
				bResult = (bFirst || bSecond) && !(bFirst && bSecond);
				break;
			default:
				bResult = bFirst;
				break;
			}

			if (Rules[i].cComparison & RULE_NOT)
			{
				bResult = !bResult;
			}

			char cReturn = 0xff;

			usBitBlock = i / 8;
			usBitMask = i % 8;

			switch (usBitMask)
			{
			case 0:
				cReturn = cReturn & BIT_ONE;
				break;
			case 1:
				cReturn = cReturn & BIT_TWO;
				break;
			case 2:
				cReturn = cReturn & BIT_THREE;
				break;
			case 3:
				cReturn = cReturn & BIT_FOUR;
				break;
			case 4:
				cReturn = cReturn & BIT_FIVE;
				break;
			case 5:
				cReturn = cReturn & BIT_SIX;
				break;
			case 6:
				cReturn = cReturn & BIT_SEVEN;
				break;
			case 7:
				cReturn = cReturn & BIT_EIGHT;
				break;
			}

			if (bResult)
			{
				if (usBitBlock < usBufferLen)
				{
					p_cBuffer[usBitBlock] = p_cBuffer[usBitBlock] | cReturn;
				}
				else
				{
					usBitBlock = usBitBlock - usBufferLen;

					p_cOutput[usBitBlock] = p_cOutput[usBitBlock] | cReturn;
				}
			}
			else
			{
				cReturn = !cReturn;

				if (usBitBlock < usBufferLen)
				{
					p_cBuffer[usBitBlock] = p_cBuffer[usBitBlock] & cReturn;
				}
				else
				{
					usBitBlock = usBitBlock - usBufferLen;

					p_cOutput[usBitBlock] = p_cOutput[usBitBlock] & cReturn;
				}

			}

			bReturn = true;
		}
	}

	return bReturn;
}

bool AI::GenerateRule(unsigned int uiRuleNumber, double dMutationChance, const Rule* p_PriorRule)
{
	bool bReturn = false;

	Rando* Mutation = Rando::GetRando();

	Rule NewRule;

	unsigned int uiInputBits = usInputLen * 8;
	unsigned int uiBufferBits = usBufferLen * 8;
	unsigned int uiOutputBits = usOutputLen * 8;

	if (p_PriorRule == 0)
	{
		dMutationChance = 1.0;
	}

	unsigned long ulMutationCap = Mutation->GetMax() * dMutationChance;
	unsigned long ulMutation;

	ulMutation = Mutation->GetRandomNumber();

	bool bValid = false;

	if (ulMutation < ulMutationCap)
	{
		bool bHasNot;
		bool bHasAnd;
		bool bHasXor;
		bool bHasOr;

		bool bValidNot;
		bool bValidNotNot;

		do {
			ulMutation = (Mutation->GetRandomNumber()) % RULE_CAP;

			bHasNot = ulMutation & RULE_NOT;
			bHasAnd = ulMutation & RULE_AND;
			bHasXor = ulMutation & RULE_XOR;
			bHasOr = ulMutation & RULE_OR;

			bValidNotNot = (bHasAnd ^ bHasOr ^ bHasXor) && !(bHasOr && bHasXor && bHasAnd);
			bValidNot = (!bHasAnd && !bHasOr && !bHasXor) && bHasNot;

			if (bValidNot || bValidNotNot)
			{
				bValid = true;
			}
		} while (!bValid);

		NewRule.cComparison = ulMutation;
	}
	else
	{
		NewRule.cComparison = p_PriorRule->cComparison;
	}

	bValid = false;

	do {
		ulMutation = Mutation->GetRandomNumber();

		if (ulMutation <= ulMutationCap)
		{
			ulMutation = (Mutation->GetRandomNumber() % uiInputBits) + uiRuleNumber;

			NewRule.uiBitOne = ulMutation;
		}
		else
		{
			NewRule.uiBitOne = p_PriorRule->uiBitOne;
		}

		ulMutation = Mutation->GetRandomNumber();

		if (ulMutation <= ulMutationCap)
		{
			ulMutation = (Mutation->GetRandomNumber() % uiInputBits ) + uiRuleNumber;

			NewRule.uiBitTwo = ulMutation;
		}
		else
		{
			NewRule.uiBitTwo = p_PriorRule->uiBitTwo;
		}

		if (NewRule.uiBitOne != NewRule.uiBitTwo)
		{
			bValid = true;
		}
	} while (!bValid);

	Rules.push_back(NewRule);

	return bReturn;
}