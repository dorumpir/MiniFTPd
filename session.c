#include "session.h"
#include "privparent.h"
#include "ftpproto.h"


void begin_session(session_t* sess)
{
	// change proc's priority ~ gid / uid (IN ORDER!!)
	struct passwd* pw = getpwnam("nobody");
	if (pw == NULL )
		return;
	
	if (setegid(pw->pw_gid) < 0)
		ERR_EXIT("setgid");
	if (seteuid(pw->pw_uid) < 0)
		ERR_EXIT("setuid");

	// make a socket pair for communication
	int sockfds[2];
	if (socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds) < 0)
		ERR_EXIT("socketpair");

	pid_t pid;
	pid = fork();
	if (pid < 0)
		ERR_EXIT("fork");

	if (pid == 0)
	{
		// ftp server proc
		close(sockfds[0]);
		sess->child_fd = sockfds[1];
		handle_child(sess);
	}
	else
	{
		// nobody proc
		close(sockfds[1]);
		sess->parent_fd = sockfds[0];
		handle_parent(sess);
	}
}