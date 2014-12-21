#include "main.h"
#include "irc.h"
#include "functions.h"
#include "dictionary.h"
#define BUF 2048
#define LINEBUF 512

char *owner, *nick;

char *process_string(char *in, int n) {
	int ii = -1, o, i, e;
	char *nothing = "0";
	char *buf = malloc(513);
	
	for(i = 0; i < n; i++) {

		ii++;
		buf[ii] = in[i];

		if((i > 0 && in[i] == '\n' && in[i-1] == '\r') || ii == 512) {

			buf[ii + 1] = '\0';
			o = ii;
			ii = -1;

			printf("<< %s", buf);

			in[n] = 0;
			if (!strncmp(buf, "PING", 4)) {
				buf[1] = 'O';
				sleep(2);
				return buf;
			}

			if(buf[0] == ':') {
				char *name;
				char *cmd;
				char *chan;
				char *e;
				char *pass;
				char *topic;
				char *topicchan;
				char *msg;
				char *b = malloc(1024);

				name = buf+1;
				e = strchr(name,'!');
				if (!e)
					return nothing;
				*e = 0;

				cmd = strchr(e+1,' ');
				if (!cmd)
					return nothing;
				cmd++;
				e = strchr(cmd,' ');
				if (!e)
					return nothing;
				*e = 0;

				chan = e+1;
				e = strchr(chan,' ');
				if (!e)
					e = strchr(chan,'\r');
				if (!e)
					e = strchr(chan,'\n');
				if (e)
					*e = 0;

				if (!strcmp(cmd, "JOIN")) {
					if(strncmp(name, nick, strlen(nick))==0) {
						return nothing;
					} else {
						sprintf(b,"PRIVMSG %s :Welcome %s\r\n",chan,name);
						return b;
					}
				} else if(!strcmp(cmd, "PRIVMSG")) {
							
					msg = strchr(e+1, ':');
					if(!msg)
						return nothing;
					if(msg)
						msg++;
						
					int k;	
					
					char *usestr = NULL;
					char *searchstr;
					for(k=0;dictionary[k].quote != 0;k++) {
						if(strncmp(bot_nick_exists(dictionary[k].quote, nick), "0", 1)) {
							usestr = str_replace(dictionary[k].quote,
											"*bot*", nick);
						}
						
						if(usestr==NULL) {
							searchstr = dictionary[k].quote;
						} else {
							searchstr = usestr;
						}
						
						if(strncmp(msg, searchstr, strlen(searchstr))==0) {
							sprintf(b,"PRIVMSG %s :%s, %s\r\n",chan,
								name,dictionary[k].reply);
							return b;	
						}
					}
					
					if(strncmp(msg, "@die", 4)==0) {
						if(strncmp(name, owner, strlen(owner))==0) {
							sprintf(b,"QUIT :Died\r\n");
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :You are not my owner, you cannot kill me.\r\n", name);
							return b;
						}
					}
					
					/*if(strncmp(msg, "@topic", 4)==0) {
						if(strncmp(name, owner, strlen(owner))==0) {
							(void)set_topic(e, topic, "./channels.log");
							sprintf(b,"PRIVMSG %s :Topic set.\r\n", name);
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :You are not the owner, you cannot set topics.\r\n", name);
							return b;
						}
					}*/
					
					if(strncmp(msg, "@help", 5)==0) {
						sprintf(b,"PRIVMSG %s :---HELP---\r\nPRIVMSG %s :@register <password> - Register your username\r\nPRIVMSG %s :@claim <channel> <user password> - Register IRC channel to your nickname\r\nPRIVMSG %s :@release <nickname> <user password> - Release your nickname if someone else logs on with it.\r\nPRIVMSG %s :@topic <channel> \"<channel topic>\" <user password> - Set your channel topic if you own the channel.\r\n", name, name, name, name, name);
						return b;
					}
					
					if(strncmp(msg, "@topic", 6)==0) {
						topicchan = strchr(msg, ' ');
						if(!topicchan) {
							sprintf(b,"PRIVMSG %s :You must provide a channel!.\r\n", name);
							return b;
						}
						if(topicchan)
							topicchan++;

						topic = strchr(topicchan, ' ');
						
						if(!topic) {
							sprintf(b,"PRIVMSG %s :Provide a topic\r\n", name);
							return b;
						}
						if(topic)
							*topic = 0;
						
						topic++;
						
						topic = strchr(topic, '"');
						
						if(!topic) {
							sprintf(b,"PRIVMSG %s :Provide your topic in quotes! (Ex: \"this topic is what it is\")\r\n", name);
							return b;
						}
						if(topic) {
							*topic = 0;
							topic++;
						}
						
						e = strchr(topic, '"');
						
						if(!e) {
							sprintf(b,"PRIVMSG %s :Close your topic quotes!\r\n", name);
							return b;
						}
						
						/*
						if(e) {
							*e = 0;
						}
						*/
						
						char *pass = strtok(e, " ");
						
						pass = strtok(NULL, " ");
						
						if(!pass) {
							sprintf(b,"PRIVMSG %s :Please provide your password!\r\n", name);
							return b;
						}
						
						if(check_user_passwd(name, remove_creturn(pass), "./users.log")==2) {
							sprintf(b,"PRIVMSG %s :This user is not registered.\r\n", name);
							return b;
						} else if(check_user_passwd(name, remove_creturn(pass), "./users.log")==1) {
							sprintf(b,"PRIVMSG %s :Wrong password. (%s)\r\n", name, remove_creturn(pass));
							return b;
						}
						
						if(check_channel_owner(name, topicchan, "./owners.log")==1) {
							sprintf(b,"PRIVMSG %s :You're not the channel owner!\r\n", name);
							return b;
						} else if(check_channel_owner(name, topicchan, "./owners.log")==2) {
							sprintf(b,"PRIVMSG %s :Channel not registered!\r\n", name);
							return b;
						} else {
							topic[strlen(topic)-1] = 0; /* strip last quote from string */
							sprintf(b,"JOIN %s\r\nTOPIC %s :%s\r\nPRIVMSG %s :Topic for %s set to \"%s\".\r\n", topicchan, topicchan, topic, name, topicchan, topic);
							return b;
						}
						
					}
					
					if(strncmp(msg, "@release", 8)==0) {
						e = strchr(msg, ' ');
						if(!e) {
							sprintf(b,"PRIVMSG %s :You must provide a username.\r\n", name);
							return b;
						}
						if(e)
							e++;
						
						pass = strtok(e, " ");
						pass = strtok(NULL, " ");
						if(!pass) {
							sprintf(b,"PRIVMSG %s :You must provide your password to claim your nickname!\r\n", name);
							return b;
						}
						
						if(strncmp(e, nick, strlen(nick))==0) {
							sprintf(b,"PRIVMSG %s :Cannot claim %s\r\n", name, nick);
							return b;
						}
						
						if(check_user_passwd(e, remove_creturn(pass), "./users.log")==2) {
							sprintf(b,"PRIVMSG %s :This user is not registered.\r\n", name);
							return b;
						}
						if(check_user_passwd(e, remove_creturn(pass), "./users.log")==1) {
							sprintf(b,"PRIVMSG %s :Wrong password. (%s)\r\n", name, pass);
							return b;
						}
						
						sprintf(b,"SETNICK %s\r\n", remove_creturn(e));
						return b;
						
						
					}
					
					if(strncmp(msg, "@claim", 6)==0) {
						
						e = strchr(msg, ' ');
						if(!e)
							return nothing;
						if(e)
							e++;
						
						pass = strtok(e, " ");
						pass = strtok(NULL, " ");
						if(!pass) {
							sprintf(b,"PRIVMSG %s :You must provide a password to grab a channel!\r\n", name);
							return b;
						}
							
						if(check_user_passwd(name, remove_creturn(pass), "./users.log")==2) {
							sprintf(b,"PRIVMSG %s :You are not registered, see @help\r\n", name);
							return b;
						}
						if(check_user_passwd(name, remove_creturn(pass), "./users.log")==1) {
							sprintf(b,"PRIVMSG %s :Wrong password. (%s)\r\n", name, pass);
							return b;
						}
						
						if(set_channel_owner(name, e, "./owners.log")==0) {
							sprintf(b,"JOIN %s\r\nPRIVMSG %s :You now own %s\r\n", remove_creturn(e), name, e);
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :Failed to set you as owner (%s)\r\n", name, remove_creturn(e));
							return b;
						}
					}
					
					if(strncmp(msg, "@login", 6)==0) {
						
						e = strchr(msg, ' ');
						if(!e)
							return nothing;
						if(e)
							e++;
						
						if(check_user_passwd(name, remove_creturn(e), "./users.log")==2) {
							sprintf(b,"PRIVMSG %s :You are not registered, see @help\r\n", name);
							return b;
						}
						if(check_user_passwd(name, remove_creturn(e), "./users.log")==1) {
							sprintf(b,"PRIVMSG %s :Wrong password. (%s)\r\n", name, remove_creturn(e));
							return b;
						}
						
						sprintf(b,"HOST %s\r\nPRIVMSG %s :You're now known as %s (hidden/%s)\r\n", name, name, name, name);
						return b;
					}
					
					if(strncmp(msg, "@register", 9)==0) {
						
						e = strchr(msg, ' ');
						if(!e) {
							sprintf(b,"PRIVMSG %s :You must provide a password.\r\n", name);
							return b;
						}
						if(e)
							e++;
						
						if(check_user_passwd(name, remove_creturn(e), "./users.log")!=2) {
							sprintf(b,"PRIVMSG %s :This user already exists.\r\n", name);
							return b;
						}
						
						if(register_nick(name, remove_creturn(e), "./users.log")==0) {
							sprintf(b,"PRIVMSG %s :You now own %s (password: %s)\r\n", name, name, remove_creturn(e));
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :Failed to register %s\r\n", name, name);
							return b;
						}
					}
					
					return nothing;
				} else {
					return nothing;
				}
			}
		}
	} // for loop
}

int main(int argc, char **argv) {
	int socketfd, n;
	char in[BUF+1], out[BUF+1], c[512];
	char *pos, *action;

	if(argc != 5) {
		printf("Usage: %s <address> <port> <nick> <owner>\n", argv[0]);
		exit(1);
	} else if(atoi(argv[2]) < 1 || atoi(argv[2]) > 50000) {
		printf("Invalid port specified.\n");
		exit(1);
	} else if(irc_connect(argv[1], atoi(argv[2]), &socketfd) == 0) {
		printf("Failed to connect to %s.\n", argv[1]);
		exit(1);
	}

	nick = argv[3];
	owner = argv[4];

	if(strlen(nick)>48) {
		printf("Error: irc bot nickname too long\n");
		exit(1);
	}
	
	if(strlen(owner)>48) {
		printf("Error: bot owner nickname too long\n");
		exit(1);
	}

	/* write to buffer */
	sprintf(c, "NICK %s\r\n", nick);
	irc_send(socketfd, c);
	sprintf(c, "USER %s %s %s :%s\r\n", nick, nick, nick, nick);
	irc_send(socketfd, c);

	/* do the service jobs */
	if(join_channels(socketfd, "./owners.log") == 2) {
		return 1;
	}

	while(1) {
		in[0] = 0;
		n = irc_read(socketfd, in);

		if (n > 0) {
			char *str = process_string(in, n);
			if(strncmp(str, "0", 1)!=0) {
				irc_send(socketfd, str);
			}
			if(strncmp(str, "QUIT", 4)==0) {
				break;
			}
		} // if(n > 0)
	} // while(1)
	return 0;
}
