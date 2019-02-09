--- com32/libutil/md5.c.orig	2019-02-09 22:08:17 UTC
+++ com32/libutil/md5.c
@@ -27,7 +27,7 @@
  */
 
 #include <string.h>
-#include <endian.h>
+#include <machine/endian.h>
 #include <md5.h>
 
 static void MD5Transform(uint32_t[4], const unsigned char[64]);
