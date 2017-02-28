#include "c4d.h"
#include "c4d_symbols.h"

// forward declarations
Bool RegisterPipeObject();


Bool PluginStart()
{
	if (!RegisterPipeObject())
		return false;

	return true;
}

void PluginEnd()
{}

Bool PluginMessage(Int32 id, void *data)
{
	switch (id)
	{
		case C4DPL_INIT_SYS:
			return resource.Init();
	}

	return false;
}
