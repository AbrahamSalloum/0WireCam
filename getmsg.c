#include <stdio.h>
#include  <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <my_global.h> // THIS NEEDS TO GO FIRST
#include <mysql.h>
#include <inttypes.h>
#include <time.h>
		struct msgsock{
			int msgsock;
			char ip[100];
			int port;
			char mess[2];
			int id;
			char setswitch;  
			char setcam; 
			int msgsock1;
			int port1; 
		};  
//


int sndpic(struct msgsock *message); 
void getcam(struct msgsock *message); 
void getswitch(struct msgsock *message); 
void sndmsg(struct msgsock *message);
void initswitch(); 
void updatesensor(int id, int state); 
void setsensor(MYSQL *con, int n); 
void dbsensors(); 
void initsensors(MYSQL *con); 
int closedb(MYSQL *con); 
void setswitch(MYSQL *con, int id); 
int initdb(MYSQL *con);
void finish_with_error(MYSQL *con);
int getmsg(struct msgsock *message); 
void *runcheck(void *messagei); 
//



struct sockaddr_in server, server1;
struct msgsock message[3]; 




int main(){

//struct msgsock message[3]; 

pthread_t nthread[3];
int rthread[3];


	message[0].id = 0; 
	message[0].port=8181;
	message[0].port1=9090; 
	strcpy(message[0].ip, "10.1.1.111");
	message[0].setswitch =0; 
	MYSQL *con = mysql_init(NULL);
	initsensors(con); 
	initswitch();  
	initcam(); 
int n = 0;
//for(n=0; n <1; n++){
	setsensor(con, n);  
	setswitch(con, n);
	setcam(con, n);  
	closedb(con); 	
printf("done\n"); 


message[n].msgsock = socket(AF_INET, SOCK_STREAM,0); 
	printf("socket value is %d\n", message[n].msgsock); 
	if(!(message[n].msgsock)){
	printf("Could not create socket\n");
	}


 message[n].msgsock1 = socket(AF_INET, SOCK_STREAM,0); 
	printf("socket value is %d\n", message[n].msgsock1); 
	if(!(message[n].msgsock1)){
	printf("Could not create socket1\n");
	}



		server.sin_addr.s_addr = inet_addr(message[n].ip); 
		server.sin_family = AF_INET; 
		server.sin_port = htons(message[n].port);

		int x = connect(message[n].msgsock,(struct sockaddr *)&server,sizeof(server)); 
		printf("Connect returns: %d\n", x); 
	if(x < 0){
		printf("Could Not Bind Socket\n"); 
	}

		server1.sin_addr.s_addr = inet_addr(message[n].ip); 
		server1.sin_family = AF_INET; 
		server1.sin_port = htons(message[n].port1);
		int x1 = connect(message[n].msgsock1,(struct sockaddr *)&server1,sizeof(server1)); 
		printf("Connect1 returns: %d\n", x1); 
	if(x1 < 0){
		printf("Could Not Bind Socket\n"); 
	}
	printf("connected and bound\n");
	
	printf("return thread %d\n", rthread[n]); 
		printf("number thread %d\n", nthread[n]); 
		rthread[n] = pthread_create(&(nthread[n]),NULL,runcheck,(void*)&message[n]); 


//	}
while(1) {} 
}

void *runcheck(void *messagei){
struct msgsock *message	= (struct msgsock*)(messagei); 

//memset(&message->msgsock, '\0', sizeof(message->msgsock));
	while(1){
	printf("runcheck loop\n"); 
	getmsg(message);
	printf("getmsgD\n");	
	getswitch(message);
	printf("getswitchD\n");
	sndmsg(message); 
	printf("sndmsgD\n");
	getcam(message); 
	printf("getcamD\n"); 
	sndpic(message);
	printf("sndpicD\n"); 
	}	
	printf("Error Reading sensor QUIT\n"); 		
}

int getmsg(struct msgsock *message){
char msgc = (char)message->setswitch;
send(message->msgsock, &msgc, sizeof(char),0); 
int msg = recv(message->msgsock, &message->mess, 2,MSG_WAITALL);
printf("%s\n", message->mess);
	if(!(strcmp(message->mess, "hi"))){
		updatesensor(message->id, 1);
		return 1;  
	}
return 0; 
}

int sndpic(struct msgsock *message){
if(strncmp(&message->setcam,"2",1)){
printf("skipped");
return 0; 
}

char len[6]; 
memset(&len[0], '\0', sizeof(len));
		if((send(message->msgsock, "2", sizeof(char),0)) < 0){
		printf("sndpic Error Sending\n"); 
		}
	printf("send1\n");

		if((send(message->msgsock1, "Q", sizeof(char),0)) < 0){
		printf("sndpic Error Sending\n"); 
		}
	int msg0 = recv(message->msgsock1, &len, 5, MSG_WAITALL);// MSG_WAITALL);
		printf("length %s\n", len); 
	printf("trying img\n"); 
char image[atoi(len)]; 
memset(&image[0], '\0', sizeof(image));
printf(" lenght in INT %d\n", atoi(len)); 	
int msg = recv(message->msgsock1, &image, sizeof(image), MSG_WAITALL);//MSG_WAITALL);
			printf("recv return img\n ", msg);
sleep(5);  
	printf("send2\n"); 
	char filename[150];
	memset(&filename[0], '\0', sizeof(filename));
	snprintf(filename, sizeof(filename),"%s%d%s","/home/abraham/public_html/0/img/image",(int)time(NULL),".jpg");  
	FILE *fp;
	printf("%s\n", filename); 
	fp=fopen(filename, "wb");
	printf("fopen");	
	fwrite(image, 1, sizeof(image), fp); 
	fclose(fp); 	
	printf("done img write");
resetcam(message->id); 
return 0; 
}


void sndmsg(struct msgsock *message){
char swtch = (char)message->setswitch; 

printf("sndmsg %c\n", swtch);   
		if((send(message->msgsock,&swtch, sizeof(char),0)) < 0){
		printf("sndmsg Error Sending\n"); 
	}
}


void getswitch(struct msgsock *message){
	MYSQL *con = mysql_init(NULL); 
	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}

	char query[37];
	snprintf(query, sizeof(query), "%s%d", "SELECT switch FROM switch WHERE id=", message->id); 
	mysql_query(con, query); 
	MYSQL_RES *result = mysql_store_result(con);
	MYSQL_ROW row = mysql_fetch_row(result);
	message->setswitch = (char)row[0][0];  
	mysql_close(con); 

}

void getcam(struct msgsock *message){
	MYSQL *con = mysql_init(NULL); 
	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}

	char query[37];
	snprintf(query, sizeof(query), "%s%d", "SELECT cam FROM cam WHERE id=", message->id); 
	mysql_query(con, query); 
	MYSQL_RES *result = mysql_store_result(con);
	MYSQL_ROW row = mysql_fetch_row(result);
	message->setcam = (char)row[0][0];  
	printf("getcam %s\n", &message->setcam); 
	mysql_close(con); 
}
