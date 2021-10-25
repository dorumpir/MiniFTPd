#include "ftpproto.h"
#include "sysutil.h"
#include "str.h"

void handle_child(session_t* sess)
{
	writen(sess->ctrl_fd, "220 (miniftpd 0.1)\n", strlen("220 (miniftpd 0.1)\n"));
	while (1)
	{
		memset(sess->cmdline, 0, sizeof(sess->cmdline));
		memset(sess->cmd, 0, sizeof(sess->cmd));
		memset(sess->arg, 0, sizeof(sess->arg));
		readline(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE);

		// parsing FTP cmd and arg
		
	}
}