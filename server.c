#include "server.h"
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void *function();
char buffer[6];
int sockfd,clientfd,err;

int main(int argc, char **argv)
{
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1) {
		printf("socket error");
		exit(1);
	}
	struct sockaddr_in serverinfo;
	struct sockaddr_in clientinfo;
	bzero (&serverinfo,sizeof(serverinfo));
	serverinfo.sin_family=AF_INET;
	serverinfo.sin_addr.s_addr=INADDR_ANY;
	serverinfo.sin_port=htons(59487);
	err=bind(sockfd,(struct sockaddr*)&serverinfo,sizeof(serverinfo));
	if(err<0)
		printf("ERROR bind");
	listen(sockfd,7);
	socklen_t clen;
	clientfd=accept(sockfd,(struct sockaddr*)&clientinfo,&clen);

	while(1) {
		recv(clientfd,buffer,sizeof(buffer),0);
		pthread_t thread;
		pthread_create(&thread,NULL,&function,NULL);
		pthread_join(thread,NULL);
	}
	return 0;
}
void *function()
{
	char pid[5];
	int i;
	for(i=0; i<5; i++) {
		pid[i]=buffer[i+1];
	}
	int pidn;
	pidn=atoi(pid);
	if(buffer[0]=='a') {
		DIR *dir;
		struct dirent *ptr;
		dir=opendir("/proc");
		char *tname=malloc(sizeof(char)*100);
		int numberpid[1000];
		while((ptr=readdir(dir))!=NULL) {
			tname=ptr->d_name;
			if(tname[0]=='1'||tname[0]=='2'||tname[0]=='3'||tname[0]=='4'||tname[0]=='5'
			   ||tname[0]=='6'||tname[0]=='7'||tname[0]=='8'||tname[0]=='9') {
				strcat(numberpid,tname);
				strcat(numberpid," ");

			}
		}
		send(clientfd,numberpid,sizeof(numberpid),0);
//		free(tname);
	} else if(buffer[0]=='b') {
		DIR *dir;
		struct dirent *ptr;
		char str[80];
		sprintf(str,"/proc/%d/task",pidn);
		dir=opendir(str);
		char *tname=malloc(sizeof(char)*100);
		int numbertid[500];
		while((ptr=readdir(dir))!=NULL) {
			tname=ptr->d_name;
			if(tname[0]=='1'||tname[0]=='2'||tname[0]=='3'||tname[0]=='4'||tname[0]=='5'
			   ||tname[0]=='6'||tname[0]=='7'||tname[0]=='8'||tname[0]=='9') {
				strcat(numbertid,tname);
				strcat(numbertid," ");
			}
		}
		send(clientfd,numbertid,sizeof(numbertid),0);
		//      free(tname);
	} else if(buffer[0]=='c') {
		char str[80];
		sprintf(str,"/proc/%d/task/%d/children",pidn,pidn);
		FILE *file;
		file=fopen(str,"r");
		char out[100];
		fgets(out,100,file);
		fclose(file);
		send(clientfd,out,sizeof(out),0);
	} else if(buffer[0]=='d') {
		char str[80];
		sprintf(str,"/proc/%d/status",pidn);
		FILE *file=fopen(str,"r");
		char out[100];
		fgets(out,100,file);
		fclose(file);
		send(clientfd,out,sizeof(out),0);
	} else if(buffer[0]=='e'||buffer[0]=='g'||buffer[0]=='i'||buffer[0]=='j') {
		char str[80];
		sprintf(str,"/proc/%d/status",pidn);
		FILE *file=fopen(str,"r");
		char trash[1000];
		int choose;
		if(buffer[0]=='e') {
			choose=1;
		} else if(buffer[0]=='g') {
			choose=5;
		} else if(buffer[0]=='i') {
			choose=16;
		} else {
			choose=20;
		}
		int i;
		for(i=0; i<choose; i++) {
			fgets(trash,1000,file);
		}
		char out[100];
		fgets(out,100,file);
		fclose(file);
		send(clientfd,out,sizeof(out),0);
	} else if(buffer[0]=='f') {
		char str[80];
		sprintf(str,"/proc/%d/cmdline",pidn);
		FILE *file=fopen(str,"r");
		char out[100];
		fgets(out,100,file);
		fclose(file);
		send(clientfd,out,sizeof(out),0);

	} else if(buffer[0]=='h') {
		char str[80];
		char out[100];
		char m[100]= {};
		int pidnow=pidn;
		while(pidnow>1) {
			sprintf(str,"/proc/%d/status",pidnow);
			FILE *file=fopen(str,"r");
			char trash[1000];
			int choose;
			int i;
			for(i=0; i<5; i++) {
				fgets(trash,1000,file);
			}
			char get[45];
			fgets(get,45,file);
			fclose(file);
			int k;
			char a[10];
			for(k=0; k<9; k++)
				a[k]=get[6+k];
			pidnow=atoi(a);
			strcat(m,a);
			strcat(m," ");
		}
		strcat(m,"end");
		send(clientfd,m,sizeof(m),0);
	} else {
		return 0;
	}

	return 0;
}
