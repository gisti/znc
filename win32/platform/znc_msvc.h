/*
 * Copyright (C) 2004-2013 ZNC-MSVC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ZNC_MSVC_H
#define _ZNC_MSVC_H

// this is something like a config.h for MSVC
// it's force-included using /FI in upstream ZNC code

#if defined(_WIN32) && defined(_MSC_VER)
#define WIN_MSVC
#endif

// some basic Windows headers:
#ifndef _TARGET_WINVER_H
#error target_winver.h has not been included before znc_msvc.h!
#endif

#include <Windows.h>
#include <Winsock2.h> // do not use Winsock v1.
#include <Shlwapi.h>
#undef StrCmp // a method in CString is called "StrCmp". Avoid renaming it.
#include <Shlobj.h>
#include <Wincrypt.h>

// C(++) headers:
#include <stdint.h>
#include <direct.h>
#include <time.h>
#include <process.h>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

// ICU headers:
#define U_USING_ICU_NAMESPACE 0
#include <unicode/timezone.h>
#include <unicode/calendar.h>
#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>

// linker magic:
#ifdef HAVE_LIBSSL
#pragma comment(lib, "ssleay32")
#pragma comment(lib, "libeay32")
#endif /* HAVE_LIBSSL */

// map some general types:
#define ssize_t SSIZE_T
typedef long suseconds_t;

// map some POSIX function calls to ISO underscore or Win32 equivalents:
#define getcwd _getcwd
#define snprintf _snprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtoll _strtoi64
#define strtoull _strtoui64
#define getpid _getpid
#define isatty _isatty
#define ftruncate _chsize
#define mkdir(a, b) mkdir(a)
#define fsync(a) _commit(a)

__inline void usleep(int useconds) { ::Sleep(useconds / 1000); }
__inline void sleep(int seconds) { ::Sleep(seconds * 1000); }

// enable built-in ZNC charset conversion:
#define HAVE_ICU

// getopt and getopt_long are provided via getopt.c, a Win32 getopt implementation
#define HAVE_GETOPT_LONG

// there's no point in disabling IPv6:
#define HAVE_IPV6

// on Windows, load stuff from exe directory by default:
#define RUN_FROM_SOURCE

// we use .dll for modules:
#define ZNC_MODULE_FILE_EXT ".dll"

// set up some basics that are not provided in the MinGW headers that we ship:
typedef int _mode_t;
typedef _mode_t mode_t;
typedef short uid_t;
typedef short gid_t;

// define this as empty:
#define __MINGW_NOTHROW

// fake some includes that "just appear" in other headers on POSIX:
#include <sys/stat.h>

// some other things that are not provided by MinGW headers:
#define S_ISLNK(x) (0 == 1)
#define S_ISSOCK(x) (0 == 1)
#define O_NOCTTY 0 // unsupported
__inline int fchmod(int, mode_t) { return 0; } // always pretend it worked
__inline int chmod(const char*, mode_t) { return 0; } // always pretend it worked

#include "znc/exports.h"

// gettimeofday from dh_time.c:
ZNC_API int gettimeofday(struct timeval*, struct timezone*);

// from znc_msvc_platform.cpp:
ZNC_API struct tm* localtime_r(const time_t*, struct tm*);
ZNC_API struct tm* gmtime_r(const time_t*, struct tm*);
ZNC_API char* ctime_r(const time_t*, char*);
ZNC_API int setenv(const char*, const char*, int overwrite);
ZNC_API int unsetenv(const char *);
ZNC_API std::string getpass(const char* prompt);
ZNC_API size_t strftime_validating(char* strDest, size_t maxsize, const char* format, const struct tm* timeptr);
ZNC_API int rand_r(unsigned int *seedp);
ZNC_API std::string strftime_format_to_icu(const std::string& a_format);

#include "znc_msvc_shared.h"

// suppress some warnings from ZNC code:
#pragma warning(disable:4996) // disable "The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name"

// suppress some warnings related to DLL exporting classes:
#pragma warning(disable:4251) // disable "... needs to have dll-interface to be used by clients of class ..."
#pragma warning(disable:4275) // disable "non dll-interface class ... used as base for dll-interface class ..."

#endif // _ZNC_MSVC_H
