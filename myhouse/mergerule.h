#pragma once

struct MergeRules
{
	int inputnum;
	int *input;
	int outputnum;
	int output;

	MergeRules():inputnum(0), outputnum(0), output(EMPTY)	{}
	MergeRules(const int pram[21]):inputnum(pram[0]), outputnum(pram[19]), output(pram[20])
	{
		input = new int[inputnum*2];
		int j = 0;
		for (int i = 2; i < 19; i+=2)
		{
			if (pram[i] != EMPTY)
			{
				input[j++] = pram[i-1];
				input[j++] = pram[i];
			}
		}
	}

	~MergeRules()
	{
		if (input != NULL)
		{
			delete []input;
		}
	}
};
