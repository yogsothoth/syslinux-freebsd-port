--- com32/libutil/sha512crypt.c.orig	2019-02-09 22:15:04 UTC
+++ com32/libutil/sha512crypt.c
@@ -1,8 +1,8 @@
 /* SHA512-based Unix crypt implementation.
    Released into the Public Domain by Ulrich Drepper <drepper@redhat.com>.  */
 
-#include <alloca.h>
-#include <endian.h>
+#include <stdlib.h>
+#include <machine/endian.h>
 #include <errno.h>
 #include <limits.h>
 #include <stdbool.h>
