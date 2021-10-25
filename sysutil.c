#include "sysutil.h"

/**
 * start a tcp server
 * @param  host IP / hostname
 * @param  port server port number
 * @return      if success, return a listening socket 
 */
int tcp_server(const char* host, unsigned short port)
{
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		ERR_EXIT("tcp_server");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	// get host
	if (host != NULL)
	{
		// IP -> network binary order form, stored in $2
		// if failed, return 0
		if (inet_aton(host, &servaddr.sin_addr) == 0)
		{
			struct hostent* hp;
			// IP/hostname -> hostname(stored as hostent type)
			// return pointer to the hostent
			hp = gethostbyname(host);
			if (hp == NULL)
				ERR_EXIT("gethostbyname");

			// servaddr.sin_addr = *(struct sockaddr_in*)hp->h_addr;
			memcpy(&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
		}
	}
	else
		// kernel choose an IP for you if set INADDR_ANY, which==(0.0.0.0) 
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// get port
	servaddr.sin_port = htons(port);

	// set socket to reusable
	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, \
		           (const char*)(&on), sizeof(on)) < 0)
		ERR_EXIT("setsockopt");

	// binding
	if (bind(listenfd, \
		     (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");

	// listen
	// SOMAXCONN is a hard coded value before linux 2.4, value is 128 
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	return listenfd;
}


int accept_timeout(int fd, \
	               struct sockaddr_in* addr, \
	               unsigned int wait_seconds) 
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;

		FD_ZERO(&accept_fdset);
		FD_SET(fd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		
		if (ret == -1)
			return -1;
		else if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
	}

	if (addr != NULL)
		ret = accept(fd, (struct sockaddr*)addr, &addrlen);
	else
		ret = accept(fd, NULL, NULL);

	return ret;
}


ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char *bufp = (char*)buf;

    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        else if (nread == 0)
            return count - nleft;

        bufp += nread;
        nleft -= nread;
    }

    return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwritten;
    char *bufp = (char*)buf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        else if (nwritten == 0)
            continue;

        bufp += nwritten;
        nleft -= nwritten;
    }

    return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while (1)
    {
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if (ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;
    char *bufp = buf;
    int nleft = maxline;
    while (1)
    {
        ret = recv_peek(sockfd, bufp, nleft);
        if (ret < 0)
            return ret;
        else if (ret == 0)
            return ret;

        nread = ret;
        int i;
        for (i=0; i<nread; i++)
        {
            if (bufp[i] == '\n')
            {
                ret = readn(sockfd, bufp, i+1);
                if (ret != i+1)
                    exit(EXIT_FAILURE);

                return ret;
            }
        }

        if (nread > nleft)
            exit(EXIT_FAILURE);

        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if (ret != nread)
            exit(EXIT_FAILURE);

        bufp += nread;
    }

    return -1;
}
