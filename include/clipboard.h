//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <pimpa.h>

bool Clipboard_HasMethod (NPObject* object, NPIdentifier name);

bool Clipboard_Invoke (NPObject* object, NPIdentifier name, const NPVariant* argv, uint32_t argc, NPVariant* result);

bool Clipboard_HasProperty (NPObject* object, NPIdentifier name);

bool Clipboard_GetProperty (NPObject* object, NPIdentifier name, NPVariant* result);

bool Clipboard_Enumerate (NPObject* object, NPIdentifier** names, uint32_t* length);

static NPClass ClipboardClass = {
	.hasMethod   = Clipboard_HasMethod,
	.invoke      = Clipboard_Invoke,
	.hasProperty = Clipboard_HasProperty,
	.getProperty = Clipboard_GetProperty,
	.enumerate   = Clipboard_Enumerate,
};

#endif
