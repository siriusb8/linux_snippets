

Usually it's a mistake to test if the user is root. POSIX does not even require a root user, but leaves it to the implementation to determine how permissions work. Code such as:

if (i_am_root) do_privileged_op(); else print_error();

will really annoy users with advanced privilege models where root is not necessary to perform the necessary privileged operations. I remember back in the early days of cd burning on Linux, I had to hack all over the cdrecord source to remove all the useless checks to see if it was running as root, when it worked just fine with permission to read /dev/sga.

Instead, you should always attempt the privileged operation you need to perform, and check for EPERM or similar if it fails to notify the user that they have insufficient privileges (and perhaps should retry running as root).

The one case where it's useful to check for root is checking if your program was invoked "suid-root". A reasonable test would be:

uid_t uid=getuid(), euid=geteuid();
if (uid<0 || uid!=euid) {
    /* We might have elevated privileges beyond that of the user who invoked
     * the program, due to suid bit. Be very careful about trusting any data! */
} else {
    /* Anything goes. */
}

Note that I allowed for the possibility (far-fetched, but best to be paranoid) that either of the calls to get uid/euid could fail, and that in the failure case we should assume we're suid and a malicious user has somehow caused the syscalls to fail in an attempt to hide that we're suid.

