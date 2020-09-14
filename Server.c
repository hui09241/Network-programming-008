#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */

void main()
{
	SOCKET	serv_sd, cli_sd[10],temp_sd;        /* socket 描述子 */
  	int   	cli_len, n1,n2,n,i,j;
  	char  	str[MAXLINE];

  	struct 	sockaddr_in   	serv, cli;
  	WSADATA wsadata;

    WSAStartup(0x101, &wsadata); //呼叫 WSAStartup() 註冊 WinSock DLL 的使用

  	serv_sd=socket(AF_INET, SOCK_STREAM, 0);// 開啟 TCP socket

   	//指定 socket 的 IP 位址和 port number
   	serv.sin_family      = AF_INET;
   	serv.sin_addr.s_addr = 0;
   	serv.sin_port        = htons(5678);	// 指定 IPPORT_ECHO 為 echo port

    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));

   	listen(serv_sd, 5) ; //呼叫 listen() 使 socket 進入「監聽」狀態

   	cli_len = sizeof(cli);

	int num=1;
	printf("server: waiting for client\n");

   	cli_sd[0]=accept(serv_sd, (LPSOCKADDR) &cli, &cli_len);
   	u_long iMode=1;
		ioctlsocket(cli_sd[0],FIONBIO,&iMode);
		ioctlsocket(serv_sd,FIONBIO,&iMode);
	if (iMode ==0)
		printf("This is blocking mode.\n");
	else
       	printf("This is non-blocking mode.\n");

	printf("client connected!!!\n");

   	while (1)
	{
		//printf("check\n");
		if((temp_sd=accept(serv_sd, (LPSOCKADDR) &cli, &cli_len))!=-1 )
		{
			printf("new client connected(%d)!!!\n",num+1);
			cli_sd[num]=temp_sd;
			ioctlsocket(cli_sd[num],FIONBIO,&iMode);
			if (iMode ==0)
				printf("This is blocking mode.\n");
			else
       			printf("This is non-blocking mode.\n");
			num++;
		}

		for(i=0;i<num;i++)
		{
			n=0;
			n=recv(cli_sd[i], str, MAXLINE, 0);

			if (n > 0 )// 顯示從 client 傳來的字串
	        {
	        	printf("Recv form %d: %s\n",i+1,str);
	        	send(cli_sd[i], str, strlen(str)+1, 0);
			}   	// 顯示從 client 傳來的字串

			int nError=WSAGetLastError();
			if(nError!=WSAEWOULDBLOCK && nError!=0)
			{
				printf("client %d Disconnected! error code:%d\n",i+1,nError);
			//	printf("client %d Disconnected!",i+1);
				closesocket(cli_sd[i]);


					for(j=i;j<num-1;j++)
					{
					cli_sd[j]=cli_sd[j+1];
					}
					//closesocket(cli_sd[num-1]);
					num--;


			}

		}
    }

	//結束 WinSock DLL 的使用
   	closesocket(serv_sd);
   	WSACleanup();
}
