#include "main.h"
#include "functions.h"
#include "irc.h"

/* Remove carriage return from string */
const char *remove_creturn(char *str) {
	str = strtok(str, "\r");
	return str;
}

/* str_replace function - from stakoverflow */
char *str_replace(char *orig, char *rep, char *with) {
    char *result, *buf, *ins;
    int replen, withlen, frontlen, count;

	// check arguments
    if (!orig) {
        return NULL;
    }
    
    if (!rep) {
        rep = "";
    }
    replen = strlen(rep);
    
    if (!with) {
        with = "";
    }
    withlen = strlen(with);

    ins = orig;
    for (count = 0; buf = strstr(ins, rep); ++count) {
        ins = buf + replen;
    }

    buf = result = malloc(strlen(orig) + (withlen - replen) * count + 1);

    if (!result) {
        return NULL;
	}
	
    while (count--) {
        ins = strstr(orig, rep);
        frontlen = ins - orig;
        buf = strncpy(buf, orig, frontlen) + frontlen;
        buf = strcpy(buf, with) + withlen;
        orig += frontlen + replen; // move to next "end of rep"
    }
    
    strcpy(buf, orig);
    return result;
}

char *bot_nick_exists(char *str, char *nick) {
	char *searchstr = strstr(str, nick);
	if(searchstr) {
		return "0";
	}
	return "1";
}

int rand_int() {
	int r = rand() % 50;
	return r+1;
}

int join_channels(int fd, char *filefd) {
	FILE *filename = fopen(filefd, "r");
	
	char *search = " ";
	
	if(filename != NULL) {
		/* buffer for each topic line */
		char line[2048];
		char channel[256];
		char sendbuf[2305];
		
		/* read the file */
		while(fgets(line, sizeof(line), filename) != NULL) {
			
			if(strncmp("\n", line, 1)==0) {
				continue;
			}
			
			char *linetok;
			linetok = strtok(line, search);
			
			/* join the channels!*/
			sprintf(sendbuf, "JOIN %s\r\n", linetok);
			irc_send(fd, sendbuf);
			sleep(1);
		}
		
	}
	else
	{
		return 1;
	}
	
	return 0;
}

int set_channel_owner(char *owner, const char *channel, char *filefd) {
	FILE *file;
	
	char line[1024];
	if((file = fopen(filefd, "r"))) {
		while(fgets(line, sizeof(line), file)!=NULL) {
			char *dotok = strtok(line, " ");
			if(strncmp(channel, dotok, strlen(channel))==0) {
				fclose(file);
				return 1;
			}
		}
		fclose(file);
	}
	
	if((file = fopen(filefd, "a"))) {
		fprintf(file, "%s %s\n", channel, owner);
		fclose(file);
		return 0;
	}
	
	return 1;
}

int check_user_passwd(char *nick, const char *pass, char *filefd) {
	FILE *file;
	
	char line[1024];
	if((file = fopen(filefd, "r"))) {
		while(fgets(line, sizeof(line), file)!=NULL) {
			/* check if user exists */
			char *dotok = strtok(line, " ");
			if(strncmp(nick, dotok, strlen(nick))==0) {
				dotok = strtok(NULL, " ");
				if(strncmp(pass, dotok, strlen(pass))==0) {
					/* password correct */
					fclose(file);
					return 0;
				}
				else
				{
					/* password incorrect */
					fclose(file);
					return 1;
				}
			}
		}
		fclose(file);
	}
	
	/* user does not exist */
	return 2;
}

int check_channel_owner(char *nick, const char *channel, char *filefd) {
	FILE *file;
	
	char line[1024];
	if((file = fopen(filefd, "r"))) {
		while(fgets(line, sizeof(line), file)!=NULL) {
			/* check if user exists */
			char *dotok = strtok(line, " ");
			if(strncmp(channel, dotok, strlen(channel))==0) {
				dotok = strtok(NULL, " ");
				if(strncmp(nick, dotok, strlen(nick))==0) {
					/* owns channel */
					fclose(file);
					return 0;
				}
				else
				{
					/* doesn't */
					fclose(file);
					return 1;
				}
			}
		}
		fclose(file);
	}
	
	/* channel not registered */
	return 2;
}

// Not using this anymore
/*
int set_topic(char *channel, char *topic, char *filefd) {
	FILE *file;
	
	char line[1024];
	if((file = fopen(filefd, "r"))) {
		while(fgets(line, sizeof(line), file)!=NULL) {
			char *dotok = strtok(line, " ");
			if(strncmp(channel, dotok, strlen(channel))==0) {
				fclose(file);
				return 1;
			}
		}
		fclose(file);
	}
	
	if((file = fopen(filefd, "a"))) {
		fprintf(file, "%s¤%s\n", channel, topic);
		fclose(file);
		return 0;
	}
	
	return 1;
}
*/

int register_nick(char *nick, const char *pass, char *filefd) {
	FILE *file;
	
	if((file = fopen(filefd, "a"))) {
		fprintf(file, "%s %s\n", nick, pass);
		fclose(file);
		return 0;
	}
	
	return 1;
}
