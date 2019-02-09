--- libinstaller/bin2c.pl.orig	2019-02-09 22:18:21 UTC
+++ libinstaller/bin2c.pl
@@ -27,7 +27,7 @@ if ( !defined($table_name) ) {
 
 $pad = 1 if ($pad < 1);
 
-printf "unsigned char %s[] = {\n", $table_name;
+printf "static unsigned char _%s[] = {\n", $table_name;
 
 $pos = 0;
 $linelen = 8;
@@ -76,6 +76,7 @@ if (defined $ENV{'SOURCE_DATE_EPOCH'}) {
 } else {
     @st = stat STDIN;
     printf "\nconst int %s_mtime = %d;\n", $table_name, $st[9];
+    printf "\nunsigned char *%s = _%s;\n", $table_name, $table_name;
 }
 
 exit 0;
