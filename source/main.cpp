#include "c4d.h"
#include "main.h"
#include "c4d_symbols.h"

#define PLUGIN_VERSION String("PipeObject 0.3")

Bool PluginStart()
{
	if (!RegisterPipeObject())
		return false;

	GePrint(PLUGIN_VERSION);

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
