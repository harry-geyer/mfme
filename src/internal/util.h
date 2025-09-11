#pragma once


#define STR_EXPAND(tok)     #tok
#define STR(tok)            STR_EXPAND(tok)

#define PRINTF_FMT_CHECK(_fmt_arg, _el_arg)  __attribute__ ((format (printf, _fmt_arg, _el_arg)))

