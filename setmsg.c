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
//x
void finish_with_error(MYSQL *con){
  	mysql_close(con);
}

void resetcam(int id){
MYSQL *con = mysql_init(NULL); 
	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}
char query[50]; 
snprintf(query,sizeof(query), "%s%d%s", "UPDATE cam SET cam = '0' WHERE id='",id,"'");
mysql_query(con, query); 
mysql_close(con); 
}

void initswitch(){
MYSQL *con = mysql_init(NULL); 
	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}

	if (mysql_query(con, "DROP TABLE IF EXISTS switch")) {
		finish_with_error(con);
	}
  
	if (mysql_query(con, "CREATE TABLE switch(id INT, switch TINYINT(1))")) {      
		finish_with_error(con);
	}

printf("end of initswitch\n"); 
}


void initcam(){
MYSQL *con = mysql_init(NULL); 
	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}

	if (mysql_query(con, "DROP TABLE IF EXISTS cam")) {
		finish_with_error(con);
	}
  
	if (mysql_query(con, "CREATE TABLE cam(id INT, cam TINYINT(1))")) {      
		finish_with_error(con);
	}

printf("end of initcam\n"); 
}
void setswitch(MYSQL *con, int id){
char query[32];
snprintf(query, sizeof(query), "%s%d%s", "INSERT INTO switch VALUES(",id,",0)");
	mysql_query(con, query); 
printf("end of setswitch\n"); 
}

void closedb(MYSQL *con){
	mysql_close(con); 

}

void setsensor(MYSQL *con, int id){
char query[33];
snprintf(query, sizeof(query), "%s%d%s", "INSERT INTO sensors VALUES(",id,",0)");
	mysql_query(con, query); 
printf("end of setsensor\n"); 
}

void setcam(MYSQL *con, int id){
char query[33];
snprintf(query, sizeof(query), "%s%d%s", "INSERT INTO cam VALUES(",id,",0)");
	mysql_query(con, query); 
printf("end of setsensor\n"); 
}

void initsensors(MYSQL *con){

	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}

	if (mysql_query(con, "DROP TABLE IF EXISTS sensors")) {
		finish_with_error(con);
	}
  
	if (mysql_query(con, "CREATE TABLE sensors(id INT, state TINYINT(1))")) {      
		finish_with_error(con);
	}
printf("end of initsensors"); 
}

void updatesensor(int id, int state){

MYSQL *con = mysql_init(NULL);
	
	if(mysql_real_connect(con, "localhost", "root", "salloum1", "testdb", 0, NULL, 0) == NULL){
		finish_with_error(con);
	}

char query[42];
snprintf(query, sizeof(query), "%s%d%s%d%s", "UPDATE sensors SET state='",state,"' WHERE id='",id,"'"); 
printf("%s\n", &query); 
mysql_query(con, query); 
mysql_close(con); 
}
