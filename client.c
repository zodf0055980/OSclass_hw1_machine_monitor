#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char **argv)
{
	while(1) {
		int sockfd;
		sockfd=socket(AF_INET,SOCK_STREAM,0);

		if(sockfd==-1) {
			printf("create error");
			exit(1);
		}

		struct sockaddr_in info;
		bzero(&info,sizeof(info));
		info.sin_family=AF_INET;
		info.sin_addr.s_addr=inet_addr("127.0.0.1");
		info.sin_port=htons(59487);

		int err;
		err=connect(sockfd,(struct sockaddr *)&info,sizeof(info));
		printf("%d",err);
		if(err==-1) {
			printf("connect error");
			exit(1);
		}
		printf("=====================================================\n");
		printf("(a)list all process ids\n");
		printf("(b)thread's IDs\n");
		printf("(c)child's PIDs\n");
		printf("(d)process name\n");
		printf("(e)state of process(D,R,S,T,t,W,X,Z)\n");
		printf("(f)command line of excuting process(cmdline)\n");
		printf("(g)parent's PID\n");
		printf("(h)all ancients of PIDs\n");
		printf("(i)virtual memory size(VmSize)\n");
		printf("(j)physical memory size(VmRss)\n");
		printf("(k)exit\n");
		printf("witch?");
		char number;
		scanf(" %c",&number);
		char pid[5]= {};
		printf("%c\n",number);
		if((number!='a')&&(number!='k')) {
			printf("pid?");
			scanf(" %s",&pid);
		} else if(number=='k') {
			exit(1);
		}

		char work[6];
		work[0]=number;
		strcat(work,pid);
		char receive[1000]= {};
		send(sockfd,work,sizeof(work),0);
		recv(sockfd,receive,sizeof(receive),0);
		printf(" %s",receive);
		return 0;
	}
	return 0;
}
