#include "stdafx.h"
#include "main.h"

long Cleanup()
{
	Infinity::camera().disconnect();
	return 0;
}

HOST_IMPORT void main(IORecHandle ioRecHandle)
{	
	XOPInit(ioRecHandle);
	SetXOPEntry(XOPEntry);
	SetXOPResult(RegisterOperations());
}

long RegisterOperations()
{
	long error = 0, index = 0;
	while (!(error = Operations::Register(index++)));
	return error > 0 ? error : 0;
}

void XOPEntry()
{	
	long error = 0;

	switch (GetXOPMessage())
	{
	case FUNCADDRS:
		error = Functions::Register(GetXOPItem(0));
		break;

	case CLEANUP:
		error = Cleanup();
		break;
	}
	SetXOPResult(error);
}




