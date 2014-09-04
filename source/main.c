//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <pimpa.h>
#include <clipboard.h>

PAMetadata*
PA_Metadata (void)
{
	static PAMetadata meta = {
		.name        = "Clipboard",
		.description = "Clipboard",
		.version     = "1.0.0.0",
		.mime        = "application/x-clipboard:clp:Clipboard",
	};

	return &meta;
}

bool
PA_Initialize (void)
{
	return true;
}

bool
PA_Shutdown (void)
{
	return true;
}

void*
PA_New (const char* mime, int argc, char* argn[], char* argv[])
{
	(void) mime;
	(void) argc;
	(void) argn;
	(void) argv;

	return (void*) 0xDEADCA7;
}

bool
PA_Destroy (void* pointer)
{
	(void) pointer;

	return true;
}

NPObject*
PA_Interface (NPP plugin)
{
	return NPN_CreateObject(plugin, &ClipboardClass);
}
