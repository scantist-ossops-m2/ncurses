/****************************************************************************
 * Copyright (c) 2013 Free Software Foundation, Inc.                        *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/
/*
 * $Id: test_vidputs.c,v 1.3 2013/01/13 00:58:54 tom Exp $
 *
 * Demonstrate the vidputs and vidattr functions.
 * Thomas Dickey - 2013/01/12
 */

#define USE_TINFO
#include <test.priv.h>

#if HAVE_SETUPTERM

#define valid(s) ((s != 0) && s != (char *)-1)

static FILE *my_fp;
static bool p_opt = FALSE;

static
TPUTS_PROTO(outc, c)
{
    int rc = c;

    rc = putc(c, my_fp);
    TPUTS_RETURN(rc);
}

static bool
outs(char *s)
{
    if (valid(s)) {
	tputs(s, 1, outc);
	return TRUE;
    }
    return FALSE;
}

static void
cleanup(void)
{
    outs(exit_attribute_mode);
    if (!outs(orig_colors))
	outs(orig_pair);
    outs(cursor_normal);
}

static void
change_attr(chtype attr)
{
    if (p_opt) {
	vidputs(attr, outc);
    } else {
	vidattr(attr);
    }
}

static void
test_vidputs(void)
{
    fprintf(my_fp, "Name: ");
    change_attr(A_BOLD);
    fputs("Bold", my_fp);
    change_attr(A_REVERSE);
    fputs(" Reverse", my_fp);
    change_attr(A_NORMAL);
    fputs("\n", my_fp);
}

static void
usage(void)
{
    static const char *tbl[] =
    {
	"Usage: test_vidputs [options]"
	,""
	,"Options:"
	,"  -e      use stderr (default stdout)"
	,"  -p      use vidputs (default vidattr)"
    };
    unsigned n;
    for (n = 0; n < SIZEOF(tbl); ++n)
	fprintf(stderr, "%s\n", tbl[n]);
    ExitProgram(EXIT_FAILURE);
}

int
main(int argc GCC_UNUSED, char *argv[]GCC_UNUSED)
{
    int ch;

    my_fp = stdout;

    while ((ch = getopt(argc, argv, "ep")) != -1) {
	switch (ch) {
	case 'e':
	    my_fp = stderr;
	    break;
	case 'p':
	    p_opt = TRUE;
	    break;
	default:
	    usage();
	    break;
	}
    }
    if (optind < argc)
	usage();

    setupterm((char *) 0, 1, (int *) 0);
    test_vidputs();
    cleanup();
    ExitProgram(EXIT_SUCCESS);
}
#else
int
main(int argc GCC_UNUSED,
     char *argv[]GCC_UNUSED)
{
    fprintf(stderr, "This program requires terminfo\n");
    exit(EXIT_FAILURE);
}
#endif