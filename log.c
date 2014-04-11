#include "log.h"

#ifdef ANDROID
//TODO: add some function if in Android

#else
#include <syslog.h>
#include <unistd.h>

//TODO: 添加log日志输出位置自定义的函数

void log_bug(const char* format, ...)
{
	va_list ap, aq;

	va_start(ap, format);
	va_copy(aq, ap);

	fflush(stdout);
	fputs("BUG: ", stderr);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputs(".\n", stderr);

	if (!isatty(STDERR_FILENO))
		vsyslog(LOG_CRIT, format, aq);
	va_end(aq);

	abort();
}

void log_error(const char* format, ...)
{
	va_list ap, aq;

	va_start(ap, format);
	va_copy(aq, ap);

	fflush(stdout);
	fputs("ERROR: ", stderr);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputs(".\n", stderr);

	if (!isatty(STDERR_FILENO))
		vsyslog(LOG_ERR, format, aq);
	va_end(aq);
}

void log_warn(const char* format, ...)
{
	va_list ap, aq;

	va_start(ap, format);
	va_copy(aq, ap);

	fflush(stdout);
	fputs("WARN: ", stderr);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputs(".\n", stderr);

	if (!isatty(STDERR_FILENO))
		vsyslog(LOG_WARNING, format, aq);
	va_end(aq);
}

void log_info(const char* format, ...)
{
	va_list ap, aq;

	va_start(ap, format);
	va_copy(aq, ap);

	fflush(stdout);
	fputs("INFO: ", stderr);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputs(".\n", stderr);

	if (!isatty(STDERR_FILENO))
		vsyslog(LOG_INFO, format, aq);
	va_end(aq);
}

#ifdef DEBUG
void log_debug(const char* format, ...)
{
	va_list ap;

	fflush(stdout);
	fputs("DEBUG: ", stderr);
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputs(".\n", stderr);
}
#endif

#endif
