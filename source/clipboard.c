//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <clipboard.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

static char*
_format_to_name (unsigned format)
{
	char* name = NPN_MemAlloc(256);

	if (GetClipboardFormatName(format, name, 255) == 0) {
		switch (format) {
			case CF_BITMAP:
				strcpy(name, "CF_BITMAP");
				break;

			case CF_DIB:
				strcpy(name, "CF_DIB");
				break;

			case CF_DIBV5:
				strcpy(name, "CF_DIBV5");
				break;

			case CF_DIF:
				strcpy(name, "CF_DIF");
				break;

			case CF_DSPBITMAP:
				strcpy(name, "CF_DSPBITMAP");
				break;

			case CF_DSPENHMETAFILE:
				strcpy(name, "CF_DSPENHMETAFILE");
				break;

			case CF_DSPMETAFILEPICT:
				strcpy(name, "CF_DSPMETAFILEPICT");
				break;

			case CF_DSPTEXT:
				strcpy(name, "CF_DSPTEXT");
				break;

			case CF_ENHMETAFILE:
				strcpy(name, "CF_ENHMETAFILE");
				break;

			case CF_GDIOBJFIRST:
				strcpy(name, "CF_GDIOBJFIRST");
				break;

			case CF_GDIOBJLAST:
				strcpy(name, "CF_GDIOBJLAST");
				break;

			case CF_HDROP:
				strcpy(name, "CF_HDROP");
				break;

			case CF_LOCALE:
				strcpy(name, "CF_LOCALE");
				break;

			case CF_METAFILEPICT:
				strcpy(name, "CF_METAFILEPICT");
				break;

			case CF_OEMTEXT:
				strcpy(name, "CF_OEMTEXT");
				break;

			case CF_OWNERDISPLAY:
				strcpy(name, "CF_OWNERDISPLAY");
				break;

			case CF_PALETTE:
				strcpy(name, "CF_PALETTE");
				break;

			case CF_PENDATA:
				strcpy(name, "CF_PENDATA");
				break;

			case CF_PRIVATEFIRST:
				strcpy(name, "CF_PRIVATEFIRST");
				break;

			case CF_PRIVATELAST:
				strcpy(name, "CF_PRIVATELAST");
				break;

			case CF_RIFF:
				strcpy(name, "CF_RIFF");
				break;

			case CF_SYLK:
				strcpy(name, "CF_SYLK");
				break;

			case CF_TEXT:
				strcpy(name, "CF_TEXT");
				break;

			case CF_TIFF:
				strcpy(name, "CF_TIFF");
				break;

			case CF_UNICODETEXT:
				strcpy(name, "CF_UNICODETEXT");
				break;

			case CF_WAVE:
				strcpy(name, "CF_WAVE");
				break;
		}
	}

	return name;
}

static unsigned
_format_by_name (const char* name)
{
	unsigned format  = 0;
	char*    current = NULL;

	if (strcmp(name, "text/plain") == 0) {
		name = "CF_UNICODETEXT";
	}

	for (size_t i = 0; i <= CountClipboardFormats(); i++) {
		format  = EnumClipboardFormats(format);
		current = _format_to_name(format);

		if (strcmp(current, name) == 0) {
			NPN_MemFree(current);

			return format;
		}

		NPN_MemFree(current);
	}

	return 0;
}

static char*
_string (const NPVariant* value)
{
	NPString string = NPVARIANT_TO_STRING(*value);
	NPUTF8*  chars  = NPN_MemAlloc(string.UTF8Length + 1);

	memcpy(chars, string.UTF8Characters, string.UTF8Length);
	chars[string.UTF8Length] = 0;

	return chars;
}

static bool
Clipboard_Get (uint32_t argc, const NPVariant* argv, NPVariant* result)
{
	unsigned format = 0;

	OpenClipboard(0);

	if (argc == 0) {
		format = CF_UNICODETEXT;
	}
	else if (argc == 1) {
		if (!NPVARIANT_IS_STRING(argv[0])) {
			goto error;
		}

		char* name   = _string(&argv[0]);
		      format = _format_by_name(name);

		NPN_MemFree(name);

		if (format == 0) {
			goto error;
		}
	}
	else {
		goto error;
	}

	if (!IsClipboardFormatAvailable(format)) {
		goto end;
	}

	if (format == CF_TEXT || format == CF_UNICODETEXT) {
		wchar_t* data   = GetClipboardData(CF_UNICODETEXT);
		size_t   length = wcslen(data);
		char*    output = NPN_MemAlloc((length + 1) * sizeof(wchar_t));

		memset(output, 0, (length + 1) * sizeof(wchar_t));
		wcstombs(output, data, length);
		STRINGZ_TO_NPVARIANT(output, *result);
	}
	else {
		goto error;
	}

end:
	CloseClipboard();
	return true;

error:
	CloseClipboard();
	return false;
}

static bool
Clipboard_Set (uint32_t argc, const NPVariant* argv, NPVariant* result)
{
	unsigned format = 0;

	OpenClipboard(0);

	if (argc == 1) {
		format = CF_UNICODETEXT;
	}
	else if (argc == 2) {
		if (!NPVARIANT_IS_STRING(argv[1])) {
			goto error;
		}

		char* name   = _string(&argv[1]);
		      format = _format_by_name(name);

		NPN_MemFree(name);

		if (format == 0) {
			goto error;
		}
	}
	else {
		goto error;
	}

	if (format == CF_UNICODETEXT || format == CF_TEXT) {
		if (!NPVARIANT_IS_STRING(argv[0])) {
			goto error;
		}

		NPString string = NPVARIANT_TO_STRING(argv[0]);
		char*    chars  = _string(&argv[0]);
		HGLOBAL  global = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (string.UTF8Length + 1) * sizeof(wchar_t));
		wchar_t* result = GlobalLock(global);

		mbstowcs(result, chars, string.UTF8Length);
		NPN_MemFree(chars);
		GlobalUnlock(global);

		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, global);
	}
	else {
		goto error;
	}

end:
	CloseClipboard();
	return true;

error:
	CloseClipboard();
	return false;
}

static bool
Clipboard_Clear (uint32_t argc, const NPVariant* argv, NPVariant* result)
{
	OpenClipboard(0);
	EmptyClipboard();
	CloseClipboard();

	return true;
}

static bool
Clipboard_Available (uint32_t argc, const NPVariant* argv, NPVariant* result)
{
	unsigned format = 0;

	OpenClipboard(0);

	if (argc == 0) {
		format = CF_UNICODETEXT;
	}
	else if (argc == 1) {
		if (!NPVARIANT_IS_STRING(argv[0])) {
			goto error;
		}

		char* name   = _string(&argv[0]);
		      format = _format_by_name(name);

		NPN_MemFree(name);

		if (format == 0) {
			goto error;
		}
	}
	else {
		goto error;
	}

	BOOLEAN_TO_NPVARIANT(IsClipboardFormatAvailable(format), *result);

end:
	CloseClipboard();
	return true;

error:
	CloseClipboard();
	return false;
}

bool
Clipboard_HasMethod (NPObject* object, NPIdentifier name)
{
	(void) object;

	if (NPN_IdentifierIsString(name)) {
		NPUTF8* string = NPN_UTF8FromIdentifier(name);

		return strcmp(string, "get")       == 0 ||
		       strcmp(string, "set")       == 0 ||
		       strcmp(string, "clear")     == 0 ||
		       strcmp(string, "available") == 0;
	}

	return false;
}

bool
Clipboard_Invoke (NPObject* object, NPIdentifier name, const NPVariant* argv, uint32_t argc, NPVariant* result)
{
	(void) object;

	if (NPN_IdentifierIsString(name)) {
		NPUTF8* string = NPN_UTF8FromIdentifier(name);

		if (strcmp(string, "get") == 0) {
			return Clipboard_Get(argc, argv, result);
		}
		else if (strcmp(string, "set") == 0) {
			return Clipboard_Set(argc, argv, result);
		}
		else if (strcmp(string, "clear") == 0) {
			return Clipboard_Clear(argc, argv, result);
		}
		else if (strcmp(string, "available") == 0) {
			return Clipboard_Available(argc, argv, result);
		}
	}

	return false;
}

bool
Clipboard_HasProperty (NPObject* object, NPIdentifier name)
{
	(void) object;

	if (NPN_IdentifierIsString(name)) {
		return strcmp(NPN_UTF8FromIdentifier(name), "length") == 0;
	}

	uint32_t index = NPN_IntFromIdentifier(name);

	if (index >= CountClipboardFormats()) {
		return false;
	}

	return true;
}

bool
Clipboard_GetProperty (NPObject* object, NPIdentifier name, NPVariant* result)
{
	(void) object;

	if (NPN_IdentifierIsString(name)) {
		if (strcmp(NPN_UTF8FromIdentifier(name), "length") == 0) {
			INT32_TO_NPVARIANT(CountClipboardFormats(), *result);

			return true;
		}
	
		return false;
	}

	if (CountClipboardFormats() == 0) {
		return false;
	}

	uint32_t index = NPN_IntFromIdentifier(name);

	if (index >= CountClipboardFormats()) {
		return false;
	}

	OpenClipboard(0);

	unsigned int format = 0;

	for (size_t i = 0; i <= index; i++) {
		format = EnumClipboardFormats(format);
	}

	STRINGZ_TO_NPVARIANT(_format_to_name(format), *result);

	CloseClipboard();

	return true;
}

bool
Clipboard_Enumerate (NPObject* object, NPIdentifier** names, uint32_t* length)
{
	(void) object;

	*length = CountClipboardFormats() + 1;
	*names  = NPN_MemAlloc(sizeof(NPIdentifier) * *length);

	(*names)[0] = NPN_GetStringIdentifier("length");

	for (size_t i = 0; i < *length; i++) {
		(*names)[i + 1] = NPN_GetIntIdentifier(i);
	}

	return true;
}
