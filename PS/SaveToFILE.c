#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "Log.h"
#include "Struct.h"
#include "SnapShot.h"
#pragma warning(disable:4996)

t_HeaderFile HeaderFile;

void WriteToBinaryFile(t_SnapShot* Tail)
{
	FILE* out = fopen("SnapShot.bin", "wb");
	if (!out)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	else
	{
		HeaderFile.version = 1;
		

		int written = fwrite(&HeaderFile, sizeof(t_HeaderFile), 1, out);
		if (!written)
		{
			LogError("Problem writing to a binary file");
		}

		fclose(out);
	}
}