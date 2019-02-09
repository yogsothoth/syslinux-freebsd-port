--- libinstaller/syslxopt.h.orig	2019-02-09 22:37:03 UTC
+++ libinstaller/syslxopt.h
@@ -18,6 +18,8 @@ struct sys_options {
     int install_mbr;
     int activate_partition;
     const char *bootsecfile;
+    int verbose;
+    const char *bimage;
 };
 
 enum long_only_opt {
