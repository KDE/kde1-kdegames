/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define as __inline if that's what the C compiler calls it.  */
/* #undef inline */

/* Define if you need to in order for stat and other things to work.  */
/* #undef _POSIX_SOURCE */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define if the C++ compiler supports BOOL */
#define HAVE_BOOL 1

#define VERSION "0.1"

#define PACKAGE "kdegames"

/* defines if having libgif (always 1) */
#define HAVE_LIBGIF 1

/* defines if having libjpeg (always 1) */
#define HAVE_LIBJPEG 1

/* defines which to take for ksize_t */
#define ksize_t int

/* define if you have setenv */
#define HAVE_FUNC_SETENV 1

/* Define to 1 if NLS is requested.  */
#define ENABLE_NLS 1

/* Define if you have the getdtablesize function.  */
#define HAVE_GETDTABLESIZE 1

/* Define if you have the powl function.  */
#define HAVE_POWL 1

/* Define if you have the socket function.  */
#define HAVE_SOCKET 1

/* Define if you have the sqrtl function.  */
#define HAVE_SQRTL 1

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the vsnprintf function.  */
#define HAVE_VSNPRINTF 1

/* Define if you have the <crypt.h> header file.  */
/* #undef HAVE_CRYPT_H */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <krb5/krb5.h> header file.  */
/* #undef HAVE_KRB5_KRB5_H */

/* Define if you have the <lastlog.h> header file.  */
#define HAVE_LASTLOG_H 1

/* Define if you have the <malloc.h> header file.  */
#define HAVE_MALLOC_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <rpc/key_prot.h> header file.  */
#define HAVE_RPC_KEY_PROT_H 1

/* Define if you have the <rpc/rpc.h> header file.  */
#define HAVE_RPC_RPC_H 1

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/select.h> header file.  */
/* #undef HAVE_SYS_SELECT_H */

/* Define if you have the <sys/sockio.h> header file.  */
/* #undef HAVE_SYS_SOCKIO_H */

/* Define if you have the <sys/stropts.h> header file.  */
/* #undef HAVE_SYS_STROPTS_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <termio.h> header file.  */
#define HAVE_TERMIO_H 1

/* Define if you have the <termios.h> header file.  */
#define HAVE_TERMIOS_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

#ifndef HAVE_BOOL
#define HAVE_BOOL
typedef int bool;
#ifdef __cplusplus
const bool false = 0;
const bool true = 1;
#else
#define false (bool)0;
#define true (bool)1;
#endif
#endif
