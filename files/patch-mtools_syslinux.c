--- mtools/syslinux.c.orig	2019-02-09 23:16:17 UTC
+++ mtools/syslinux.c
@@ -20,12 +20,12 @@
  */
 
 #define _GNU_SOURCE
-#include <alloca.h>
+//#include <alloca.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <getopt.h>
 #include <inttypes.h>
-#include <mntent.h>
+//#include <mntent.h>
 #include <paths.h>
 #include <stdio.h>
 #include <string.h>
@@ -44,6 +44,8 @@
 #include "syslxfs.h"
 #include "syslxrw.h"
 
+int verbose=0;
+
 const char *program;
 pid_t mypid;
 
@@ -127,6 +129,53 @@ static int move_file(char *filename)
     return status;
 }
 
+/*
+ * load a file to be used as boot image
+ */
+static int load_boot_image(const char *name)
+{
+    int l, fd;
+    struct stat sb;
+    unsigned char *buf;
+
+    memset(&sb, 0, sizeof(sb));
+    fd = open(name, O_RDONLY);
+    if (fd < 0) {
+	fprintf(stderr, "cannot open boot image %s\n", name);
+	return 1;
+    }
+    if (fstat(fd, &sb)) {
+	fprintf(stderr, "cannot stat boot image %s\n", name);
+	return 1;
+    }
+    if ( (sb.st_mode & S_IFREG) == 0) {
+	fprintf(stderr, "boot image %s is not a file 0x%x\n", name, sb.st_mode);
+	return 1;
+    }
+    if (sb.st_size < 1024 || sb.st_size > 0x8000) {
+	fprintf(stderr, "boot image %s bad size %lld\n", name, (long long int)sb.st_size);
+	return 1;
+    }
+    buf = calloc(1, sb.st_size);
+    if (buf == NULL) {
+	fprintf(stderr, "malloc failed for boot image %s\n", name);
+	return 1;
+    }
+    l = read(fd, buf, sb.st_size);
+    if (l != sb.st_size) {
+	fprintf(stderr, "read failed for boot image %s got %d\n", name, l);
+	return 1;
+    }
+    if (buf[0] != 0xeb || buf[0x200] != 0x0d) {
+	fprintf(stderr, "bad content for boot image %s\n", name);
+	return 1;
+    }
+    syslinux_bootsect = buf;
+    syslinux_ldlinux = buf + 512;
+    syslinux_ldlinux_len = sb.st_size - 512;
+    return 0;
+}
+
 int main(int argc, char *argv[])
 {
     static unsigned char sectbuf[SECTOR_SIZE];
@@ -153,9 +202,15 @@ int main(int argc, char *argv[])
 
     parse_options(argc, argv, MODE_SYSLINUX);
 
+    if (opt.verbose)
+	verbose = 1;
+
     if (!opt.device)
 	usage(EX_USAGE, MODE_SYSLINUX);
 
+    if (opt.bimage != NULL)
+	load_boot_image(opt.bimage);
+
     if (opt.sectors || opt.heads || opt.reset_adv || opt.set_once
 	|| (opt.update_only > 0) || opt.menu_save) {
 	fprintf(stderr,
@@ -220,10 +275,9 @@ int main(int argc, char *argv[])
 	    "MTOOLS_SKIP_CHECK=1\n"
 	    "MTOOLS_FAT_COMPATIBILITY=1\n"
 	    "drive s:\n"
-	    "  file=\"/proc/%lu/fd/%d\"\n"
+	    "drive s: file=\"%s\"\n"
 	    "  offset=%llu\n",
-	    (unsigned long)mypid,
-	    dev_fd, (unsigned long long)opt.offset);
+    	    opt.device, (unsigned long long)opt.offset);
 
     if (ferror(mtc) || fclose(mtc))
 	die_err(mtools_conf);
@@ -242,9 +296,11 @@ int main(int argc, char *argv[])
     syslinux_reset_adv(syslinux_adv);
 
     /* This command may fail legitimately */
+    if (verbose) fprintf(stderr, "doing mattrib\n");
     status = system("mattrib -h -r -s s:/ldlinux.sys 2>/dev/null");
     (void)status;		/* Keep _FORTIFY_SOURCE happy */
 
+    if (verbose) fprintf(stderr, "doing mcopy\n");
     mtp = popen("mcopy -D o -D O -o - s:/ldlinux.sys", "w");
     if (!mtp ||
 	fwrite((const void _force *)syslinux_ldlinux,
@@ -264,7 +320,9 @@ int main(int argc, char *argv[])
 		       + SECTOR_SIZE - 1) >> SECTOR_SHIFT;
     sectors = calloc(ldlinux_sectors, sizeof *sectors);
     fs = libfat_open(libfat_xpread, dev_fd);
+    if (verbose) fprintf(stderr, "libfat_open returns %p\n", fs);
     ldlinux_cluster = libfat_searchdir(fs, 0, "LDLINUX SYS", NULL);
+    if (verbose) fprintf(stderr, "libfat_searchdir returns %d\n", ldlinux_cluster);
     secp = sectors;
     nsectors = 0;
     s = libfat_clustertosector(fs, ldlinux_cluster);
@@ -272,6 +330,7 @@ int main(int argc, char *argv[])
 	*secp++ = s;
 	nsectors++;
 	s = libfat_nextsector(fs, s);
+	if (verbose) fprintf(stderr, "libfat_nextsector returns %d\n", (int)s);
     }
     libfat_close(fs);
 
