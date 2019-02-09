--- com32/libutil/sha256crypt.c.orig	2019-02-09 22:10:40 UTC
+++ com32/libutil/sha256crypt.c
@@ -1,8 +1,8 @@
 /* SHA256-based Unix crypt implementation.
    Released into the Public Domain by Ulrich Drepper <drepper@redhat.com>.  */
 
-#include <alloca.h>
-#include <endian.h>
+#include <stdlib.h>
+#include <machine/endian.h>
 #include <errno.h>
 #include <limits.h>
 #include <stdint.h>
