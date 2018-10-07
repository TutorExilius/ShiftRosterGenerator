// App Informations here

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define APP_NAME "Shift Roster Generator"
#define AUTHOR "Tutor Exilius"
#define AUTHOR_EMAIL "tutorexilius@gmail.com"
#define COPYRIGHT "Copyright 2018"

#define STRING_VERSION "" STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#define FILE_VERSION VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH
#define STRING_COPYRIGHT AUTHOR " (" AUTHOR_EMAIL ") - " COPYRIGHT
