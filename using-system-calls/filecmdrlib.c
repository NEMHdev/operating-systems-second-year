/*
 * Replace the following string of 0s with your student number
 * 150108595
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
  
// you will probably need other system library header files included here

#include "filecmdrlib.h"

#define MODE_S_LEN  11
#define TIME_S_LEN  17

/* 
 * is_user_exec helper function to test user execute permission for given 
 * file mode and owner uid and gid.
 * Uses getuid() to get the uid of the calling process and getgid() to get the 
 * gid of the calling process.
 * This function is not part of the filecmdrlib interface.
 */ 
bool is_user_exec(mode_t mode, uid_t ouid, gid_t ogid) {
    if (ouid == getuid())
        return mode & S_IXUSR;
    
    if (ogid == getgid())
        return mode & S_IXGRP;
        
    return mode & S_IXOTH;
}

/* 
 * The following functions are defined in filecmdrlib.h and need to be
 * modified to comply with their definitions and the coursework specification.
 */
 
/////////////////////////////////////////////////////////////////////////execfile/////////////////
int execfile(char *path) {
	/* int execl(path, const char *arg, ...);
	if (exefile(path) == -1) {
               int errsv = errno;
               printf("execfile() failed\n");
               if (errsv == ...) { ... }
           } */
	execl(path, NULL);	   
    return 0;
}
/////////////////////////////////////////////////////////////////////////listdir/////////////////
int listdir(char *path){
 
 DIR           *d;
  struct dirent *dir;
  d = opendir(path);	// open the directory
  if (d)
  {
    while ((dir = readdir(d)) != NULL)	// check that there are objects in the directory
    {
		chdir(path);				// allow the method to use the path
		printfinf(dir->d_name);		// use printfinf format to display contents
    }

    closedir(d);	// close the directory when finished
  }
return 0;
}
    
/* }
} */
////////////////////////////////////////////////////////////////////////listfile////////////////////////
int listfile(char *path) {
	
	
    FILE *f;
	
    char c;
    f=fopen(path,"rt");	// open file

    while((c=fgetc(f))!=EOF){	// get contents of the file
        printf("%c",c);			// display the contents of the file
    }

    fclose(f);					// close the file when finished

    return 0;
}
////////////////////////////////////////////////////////////////////////mode2str/////////////////////////
char *mode2str(mode_t mode, uid_t ouid, gid_t ogid) { 
    char *mode_s = (char *) calloc(MODE_S_LEN, sizeof(char));
	
	switch (mode & S_IFMT) {
    
    case S_IFDIR:   strcat(mode_s, "d");	//Directory
	break;
	case S_IFREG:	is_user_exec(mode, ouid, ogid) ? strcat(mode_s, "e") :strcat(mode_s, "f");	//Regular File
	break;
	case S_IFLNK:   strcat(mode_s, "l");   	//Symbolic link
	break;
    default:        strcat(mode_s, "o");	//Other File            
	break;
	}

    ((mode & 000400) ? strcat(mode_s, "r") : strcat(mode_s, "-"));	//bit patterns 
    ((mode & 000200) ? strcat(mode_s, "w") : strcat(mode_s, "-"));
    ((mode & 000100) ? strcat(mode_s, "x") : strcat(mode_s, "-"));
	
    ((mode & 000040) ? strcat(mode_s, "r") : strcat(mode_s, "-"));
    ((mode & 000020) ? strcat(mode_s, "w") : strcat(mode_s, "-"));
    ((mode & 000010) ? strcat(mode_s, "x") : strcat(mode_s, "-"));
    
	((mode & 000004) ? strcat(mode_s, "r") : strcat(mode_s, "-"));
    ((mode & 000002) ? strcat(mode_s, "w") : strcat(mode_s, "-"));
    ((mode & 000001) ? strcat(mode_s, "x") : strcat(mode_s, "-"));

	return mode_s; // return mode in final format
}
//////////////////////////////////////////////////////////////////////printfinf/////////////
int printfinf(char *path) {
    //printf("Print information about %s here\n", path);
    struct stat sb;
	
	if (lstat(path, &sb) == -1) {	
        
		return FTYPE_ERR;			// error check to see if the file is a valid type
        exit(EXIT_FAILURE);
		
    }
	off_t fsize;					// finding the file size
	fsize = sb.st_size;
	
	struct passwd *pwd;				// get the username from uid
	pwd = getpwuid(sb.st_uid);
	if (pwd == NULL) {				// error check
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
	char *name;
	name = pwd->pw_name;
	char *lmt = time2str(sb.st_mtime);		// call the time2str method with relevant parameters
	
	char *md = mode2str(sb.st_mode, sb.st_uid, sb.st_gid);	//call the mode2str method with relevant parameters
	
	int space = 8;
	printf("%s %*d %s %s %s\n",md, space, fsize, name, lmt, path);
	free(md);		// free the memory location for mode2str
	free(lmt);		// free the memory location for time2str
	
	switch (sb.st_mode & S_IFMT) {
		case S_IFDIR:   return FTYPE_DIR;	//Directory
		case S_IFREG:	return is_user_exec(sb.st_mode, sb.st_uid, sb.st_gid) ? FTYPE_EXE : FTYPE_REG;	//Regular File
		case S_IFLNK:   return FTYPE_LNK;   		//Symbolic link
		default:     	return FTYPE_OTH;			//Other File            
	}
}
///////////////////////////////////////////////////////////////////////////time2str////////////////
char *time2str(time_t time) {
    static char *str_fmt = "%d/%m/%y %H:%M";		// format string for date and time
    char *time_s = calloc(TIME_S_LEN, sizeof(char));
	struct tm *timeS;
	timeS = localtime(&time);
	strftime(time_s, TIME_S_LEN, str_fmt, timeS); //if(strftime() == 0) manpage
	return time_s;
}
////////////////////////////////////////////////////////////////////////////useraction/////////////
int useraction(int ftype, char *path) {
    static const char *action_prompt[] = {
        "Do you want to list the directory %s (y/n): ",
        "Do you want to execute %s (y/n): ",
        "Do you want to list the file %s (y/n): " 
    };
	
	printf(action_prompt[ftype], path);
	
	char input[150];
	
	if (ftype == 0){					//directory
		scanf("%s",input);
		if ((strcmp(input, "y")== 0)  || (strcmp(input,"Y")== 0)){
			listdir(path);
	}
	}
	
	if (ftype == 1){					// user executable regular file
		scanf("%s",input);
		if ((strcmp(input, "y")== 0)  || (strcmp(input,"Y")== 0)){
			execfile(path);
	}
	}
	
	if (ftype == 2){					// regular file that is not user executable
		scanf("%s",input);
		if ((strcmp(input, "y")== 0)  || (strcmp(input,"Y")== 0)){
			listfile(path);
	}
	}
		
    return 0;
}
