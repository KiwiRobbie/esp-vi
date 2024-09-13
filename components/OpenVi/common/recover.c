/*      $OpenBSD: recover.c,v 1.32 2022/02/20 19:45:51 tb Exp $    */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022-2024 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include "../include/compat.h"

#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/time.h>
// #include <sys/wait.h>

/*
 * We include <sys/file.h>, because the open #defines were found there
 * on historical systems.  We also include <bsd_fcntl.h> because the open(2)
 * #defines are found there on newer systems.
 */

#include <sys/file.h>

#include <stddef.h>
#include <stdint.h>
#include <bitstring.h>
#ifdef __solaris__
#define _XPG7
#endif /* ifdef __solaris__ */
#include <dirent.h>
#include <errno.h>
#include <bsd_fcntl.h>
#include <limits.h>
#include <paths.h>
// #include <pwd.h>
#include <stdio.h>
#include <bsd_err.h>
#include <bsd_stdlib.h>
#include <bsd_string.h>
#include <time.h>
#include <bsd_unistd.h>

#include "errc.h"
#include "common.h"
#include "pathnames.h"

#undef open

/*
 * Recovery code.
 *
 * The basic scheme is as follows:  In the EXF structure, we maintain full
 * paths of a b+tree file and a mail recovery file.  The former is the file
 * used as backing store by the DB package.  The latter is the file that
 * contains an email message to be sent to the user if we crash.  The two
 * simple states of recovery are:
 *
 *      + first starting the edit session:
 *              the b+tree file exists and is mode 700, the mail recovery
 *              file doesn't exist.
 *      + after the file has been modified:
 *              the b+tree file exists and is mode 600, the mail recovery
 *              file exists, and is exclusively locked.
 *
 * In the EXF structure we maintain a file descriptor that is the locked
 * file descriptor for the mail recovery file.  NOTE: we sometimes have to
 * do locking with fcntl(2).  This is a problem because if you close(2) any
 * file descriptor associated with the file, ALL of the locks go away.  Be
 * sure to remember that if you have to modify the recovery code.  (It has
 * been rhetorically asked of what the designers could have been thinking
 * when they did that interface.  The answer is simple: they weren't.)
 *
 * To find out if a recovery file/backing file pair are in use, try to get
 * a lock on the recovery file.
 *
 * To find out if a backing file can be deleted at boot time, check for an
 * owner execute bit.  (Yes, I know it's ugly, but it's either that or put
 * special stuff into the backing file itself, or correlate the files at
 * boot time, neither of which looks like fun.)  Note also that there's a
 * window between when the file is created and the X bit is set.  It's small,
 * but it's there.  To fix the window, check for 0 length files as well.
 *
 * To find out if a file can be recovered, check the F_RCV_ON bit.  Note,
 * this DOES NOT mean that any initialization has been done, only that we
 * haven't yet failed at setting up or doing recovery.
 *
 * To preserve a recovery file/backing file pair, set the F_RCV_NORM bit.
 * If that bit is not set when ending a file session:
 *      If the EXF structure paths (rcv_path and rcv_mpath) are not NULL,
 *      they are unlink(2)'d, and free(3)'d.
 *      If the EXF file descriptor (rcv_fd) is not -1, it is closed.
 *
 * The backing b+tree file is set up when a file is first edited, so that
 * the DB package can use it for on-disk caching and/or to snapshot the
 * file.  When the file is first modified, the mail recovery file is created,
 * the backing file permissions are updated, the file is sync(2)'d to disk,
 * and the timer is started.  Then, at RCV_PERIOD second intervals, the
 * b+tree file is synced to disk.  RCV_PERIOD is measured using SIGALRM, which
 * means that the data structures (SCR, EXF, the underlying tree structures)
 * must be consistent when the signal arrives.
 *
 * The recovery mail file contains normal mail headers, with two additions,
 * which occur in THIS order, as the FIRST TWO headers:
 *
 *      X-vi-recover-file: file_name
 *      X-vi-recover-path: recover_path
 *
 * Since newlines delimit the headers, this means that file names cannot have
 * newlines in them, but that's probably okay.  As these files aren't intended
 * to be long-lived, changing their format won't be too painful.
 *
 * Btree files are named "vi.XXXX" and recovery files are named "recover.XXXX".
 */

#define VI_FHEADER "X-vi-recover-file: "
#define VI_PHEADER "X-vi-recover-path: "

int rcv_copy(SCR *, int, char *);
void rcv_email(SCR *, int);
int rcv_mailfile(SCR *, int, char *);
char *rcv_gets(char *, size_t, int);

int rcv_mktemp(SCR *, char *, char *, int);
int rcv_openat(SCR *, int, const char *, int *);

/*
 * rcv_tmp --
 *      Build a file name that will be used as the recovery file.
 *
 * PUBLIC: int rcv_tmp(SCR *, EXF *, char *);
 */

int rcv_tmp(SCR *sp, EXF *ep, char *name)
{
        //     msgq(sp, M_ERR,
        //             "Modifications not recoverable if the session fails");
        return (1);
}

/*
 * rcv_init --
 *      Force the file to be snapshotted for recovery.
 *
 * PUBLIC: int rcv_init(SCR *);
 */

int rcv_init(SCR *sp)
{
        // msgq(sp, M_ERR,
        //      "Modifications not recoverable if the session fails");
        return (1);
}

/*
 * rcv_sync --
 *      Sync the file, optionally:
 *              flagging the backup file to be preserved
 *              snapshotting the backup file and send email to the user
 *              sending email to the user if the file was modified
 *              ending the file session
 *
 * PUBLIC: int rcv_sync(SCR *, unsigned int);
 */

int rcv_sync(SCR *sp, unsigned int flags)
{
        return (1);
}

/*
 * rcv_mailfile --
 *      Build the file to mail to the user.
 */

int rcv_mailfile(SCR *sp, int issync, char *cp_path)
{
        return (1);
}

/*
 * rcv_openat --
 *      Open a recovery file in the specified dir and lock it.
 *
 * PUBLIC: int rcv_openat(SCR *, int, const char *, int *)
 */

int rcv_openat(SCR *sp, int dfd, const char *name, int *locked)
{
        return -1;
}

/*
 *      people making love
 *      never exactly the same
 *      just like a snowflake
 *
 * rcv_list --
 *      List the files that can be recovered by this user.
 *
 * PUBLIC: int rcv_list(SCR *);
 */

int rcv_list(SCR *sp)
{
        return (1);
}

/*
 * rcv_read --
 *      Start a recovered file as the file to edit.
 *
 * PUBLIC: int rcv_read(SCR *, FREF *);
 */

int rcv_read(SCR *sp, FREF *frp)
{

        return (1);
}

/*
 * rcv_copy --
 *      Copy a recovery file.
 */

int rcv_copy(SCR *sp, int wfd, char *fname)
{
        int nr, nw, off, rfd;
        char buf[8 * 1024];

        if ((rfd = open(fname, O_RDONLY)) == -1)
                goto err;
        while ((nr = read(rfd, buf, sizeof(buf))) > 0)
                for (off = 0; nr; nr -= nw, off += nw)
                        if ((nw = write(wfd, buf + off, nr)) < 0)
                                goto err;
        if (nr == 0)
                return (0);

err:
        msgq_str(sp, M_SYSERR, fname, "%s");
        return (1);
}

/*
 * rcv_gets --
 *      Fgets(3) for a file descriptor.
 */

char *
rcv_gets(char *buf, size_t len, int fd)
{
        int nr;
        char *p;

        if ((nr = read(fd, buf, len - 1)) == -1)
                return (NULL);
        buf[nr] = '\0';
        if ((p = strchr(buf, '\n')) == NULL)
                return (NULL);
        (void)lseek(fd, (off_t)((p - buf) + 1), SEEK_SET);
        return (buf);
}

/*
 * rcv_mktemp --
 *      Paranoid make temporary file routine.
 */

int rcv_mktemp(SCR *sp, char *path, char *dname, int perms)
{
        int fd;

        /*
         * !!!
         * We expect mkstemp(3) to set the permissions correctly.  On
         * historic System V systems, mkstemp didn't.  Do it here, on
         * GP's.  This also protects us from users with stupid umasks.
         *
         * XXX
         * The variable perms should really be a mode_t.
         */

        if ((fd = mkstemp(path)) == -1 || fchmod(fd, perms) == -1)
        {
                msgq_str(sp, M_SYSERR, dname, "%s");
                if (fd != -1)
                {
                        close(fd);
                        unlink(path);
                        fd = -1;
                }
        }
        return (fd);
}

/*
 * rcv_email --
 *      Send email.
 */

void rcv_email(SCR *sp, int fd)
{
        // struct stat sb;
        // pid_t pid;

        // /*
        //  * In secure mode, our pledge(2) includes neither "proc"
        //  * nor "exec".  So simply skip sending the mail.
        //  * Later vi -r still works because rcv_mailfile()
        //  * already did all the necessary setup.
        //  */

        // if (O_ISSET(sp, O_SECURE))
        //         return;

        // if (_PATH_SENDMAIL[0] != '/' || stat(_PATH_SENDMAIL, &sb) == -1)
        //         msgq_str(sp, M_SYSERR,
        //             _PATH_SENDMAIL, "not sending email: %s");
        // else {

        //         /*
        //          * !!!
        //          * If you need to port this to a system that doesn't have
        //          * sendmail, the -t flag causes sendmail to read the message
        //          * for the recipients instead of specifying them some other
        //          * way.
        //          */

        //         switch (pid = fork()) {
        //         case -1:                /* Error. */
        //                 msgq(sp, M_SYSERR, "fork");
        //                 break;
        //         case 0:                 /* Sendmail. */
        //                 if (lseek(fd, 0, SEEK_SET) == -1) {
        //                         msgq(sp, M_SYSERR, "lseek");
        //                         _exit(127);
        //                 }
        //                 if (fd != STDIN_FILENO) {
        //                         if (dup2(fd, STDIN_FILENO) == -1) {
        //                                 msgq(sp, M_SYSERR, "dup2");
        //                                 _exit(127);
        //                         }
        //                         close(fd);
        //                 }
        //                 execl(_PATH_SENDMAIL, "sendmail", "-t", (char *)NULL);
        //                 msgq(sp, M_SYSERR, _PATH_SENDMAIL);
        //                 _exit(127);
        //         default:                /* Parent. */
        //                 while (waitpid(pid, NULL, 0) == -1 && errno == EINTR)
        //                         continue;
        //                 break;
        //         }

        // }
}
