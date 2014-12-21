/* functions */
const char *remove_creturn(char *str);
char *str_replace(char *orig, char *rep, char *with);
char *bot_nick_exists(char *str, char *nick);
int rand_int();
int join_channels(int fd, char *filefd);
int set_channel_owner(char *owner, const char *channel, char *filefd);
int check_user_passwd(char *nick, const char *pass, char *filefd);
//int set_topic(char *channel, char *topic, char *filefd);
int register_nick(char *nick, const char *pass, char *filefd);
