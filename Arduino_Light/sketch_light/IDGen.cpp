#include "CIDGen.h"


int CIDGen::GetID()
{
    int z = vID;
    vID++;
    return z;
}

int CIDGen::vID = 1;