#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "SaveToFILE.h"
#pragma warning(disable:4996)

t_SnapShot* LoadFromFile()
{
	FILE* in = fopen("SnapShot.bin", "rb");
	if (!in)
	{
		LogError(strerror(GetLastError()));
		return;
	}

}