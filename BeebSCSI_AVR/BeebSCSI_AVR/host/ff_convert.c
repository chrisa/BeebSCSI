#include "beebscsi.h"
#include "fatfs/ff.h"

WCHAR ff_convert (	/* Converted character, Returns zero on error */
	WCHAR	chr,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
)
{
        return chr;
}

WCHAR ff_wtoupper (	/* Returns upper converted character */
	WCHAR chr		/* Unicode character to be upper converted (BMP only) */
)
{
        return chr;
}
