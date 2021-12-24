#pragma once
#include<vector>

//bitwise rule deffinitions
#define RULE_AND 0x01
#define RULE_OR  0x02
#define RULE_XOR 0x04
#define RULE_NOT 0x08

#define RULE_VALID (RULE_AND + RULE_OR + RULE_XOR)
#define RULE_CAP (RULE_VALID + RULE_NOT)

//bit manipulation masks
#define BIT_ONE   0x01
#define BIT_TWO   0x02
#define BIT_THREE 0x04
#define BIT_FOUR  0x08
#define BIT_FIVE  0x10
#define BIT_SIX   0x20
#define BIT_SEVEN 0x40
#define BIT_EIGHT 0x80

struct Rule
{
	char cComparison;
	unsigned int uiBitOne;
	unsigned int uiBitTwo;
};

class AI
{
public:
	AI();
	~AI();

	bool Setup(double = 1.0, double = 0.5, AI* = 0, AI* = 0);

	bool SetInputSize(unsigned short);
	bool SetBufferSize(unsigned short);
	bool SetOutputSize(unsigned short);

	bool SetInput(void*, unsigned short);
	bool GetOutput(void*, unsigned short);

	bool GetRule(unsigned int, Rule*);

	bool Process();
private:
	bool GenerateRule(unsigned int, double = 1.0, const Rule* = 0);

	bool bReady = false;

	char* p_cInput = 0;
	unsigned short usInputLen = 0;

	char* p_cBuffer = 0;
	unsigned short usBufferLen = 0;

	char* p_cOutput = 0;
	unsigned short usOutputLen = 0;

	std::vector<Rule> Rules;
};