#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    int fd, new_fd;
    struct addrinfo hints;
    struct addrinfo *res;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPV4或IPV6都要回傳  
    hints.ai_socktype = SOCK_STREAM; //使用TCP
    hints.ai_flags = AI_PASSIVE; // 自己填入IP

    getaddrinfo(NULL, "80", &hints, &res); //取得主機名稱或服務的資訊，port參數設為80，並將結果載入res

    //chdir("/temp");
    //fork();
    //signal(SIGCLD, SIG_IGN);
    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);  //開啟socket 使用TCP/IP協定 傳輸使用TCP
    bind(fd, res->ai_addr, res->ai_addrlen);  //將socket綁到80port
    listen(fd,10); //開始聆聽 佇列設為10

    addr_size = sizeof their_addr;
    while(1){
    new_fd = accept(fd, (struct sockaddr *)&their_addr, &addr_size);  //接受客戶端連線


    char buffer[8192];
    int byte_count,i,file_fd,ret;

    byte_count = recv(new_fd, buffer, 8192,0);  //讀取需求 緩存至buffer 大小8192byte  byte_count為接收到的大小

    if (strncmp(buffer,"GET ",4)==0 || strncmp(buffer,"get ",4)==0 ){  //只接收GET請求封包
        for (i=0;i<byte_count;i++) {
            if (buffer[i]=='\r'||buffer[i]=='\n'){
                buffer[i] = 0;
            }
        }
        if (strncmp(buffer,"GET / ",6)==0 || strncmp(buffer,"get / ",6)==0 ) {  //請求根目錄時回應index.html
            strcpy(buffer,"GET /index.html "); //多一個空白是為了讓底下能偵測到空白
        }
        for(i=4;i<byte_count;i++) {  //將GET XXXX.html後面的字元去掉
            if(buffer[i] == ' ') {
                buffer[i] = 0;
                byte_count=i;  //儲存去掉字元後的大小
            break;
            }
        }
            file_fd = open(&buffer[5],O_RDONLY); //以唯獨打開GET /XXX.html 中第5個字後的XXX.html
            //檢查請求的副檔名，並給予相對應的內容類型(Content-Type) 當都不符合時，設狀態碼為400
            if (strncmp(&buffer[byte_count-4],"html",4)==0) {
                sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"); 
            }
            else if (strncmp(&buffer[byte_count-3],"jpg",3)==0){
                sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: image/jpeg\r\n\r\n"); 
            }
            else if (strncmp(&buffer[byte_count-3],"png",3)==0){
                sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: image/png\r\n\r\n"); 
            }
            else {
                sprintf(buffer,"HTTP/1.0 400 Bad Request\r\n\r\n"); 
            }        
            if (file_fd==-1){  //當讀取檔案失敗時，設狀態碼為400
                sprintf(buffer,"HTTP/1.0 400 Bad Request\r\n\r\n"); 
            }   
            send(new_fd,&buffer,strlen(buffer),0);   //傳送狀態碼和格式

            while ((ret=read(file_fd, buffer, 8192))>0) {  //讀取檔案大小值到讀取完畢，並將檔案丟到buffer
                send(new_fd,&buffer,ret,0);   //傳送檔案
            }
            close(new_fd);  //關閉客戶端socket

        }



        
    }
}
    