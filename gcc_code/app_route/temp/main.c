
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include <ctype.h>
#include <time.h>

#include "match.h"

#define SOCKET int
#define TRUE 1
#define FALSE 0

SOCKET *seFds = 0;
/* In network order, for network purposes */
struct in_addr *seLocalAddrs = 0;
unsigned short *seLocalPorts = 0;
/* In ASCII and local byte order, for logging purposes */
char **seFromHosts;
int *seFromPorts;
char **seToHosts;
int *seToPorts;

/* Offsets into list of allow and deny rules. Any rules
   prior to globalAllowRules and globalDenyRules are global rules. */

int *seAllowRules = 0;
int *seAllowRulesTotal = 0;
int globalAllowRules = 0;
int *seDenyRules = 0;
int *seDenyRulesTotal = 0;
int globalDenyRules = 0;

SOCKET *reFds = 0;
SOCKET *loFds = 0;
unsigned char *reAddresses = 0;
int *coInputRPos = 0;
int *coInputWPos = 0;
int *coOutputRPos = 0;
int *coOutputWPos = 0;
int *coClosed = 0;
int *coClosing = 0;
int *reClosed = 0;
int *loClosed = 0;
int *coBytesInput = 0;
int *coBytesOutput = 0;
int *coLog = 0;
int *coSe = 0;
char **coInput = 0;
char **coOutput = 0;
char **allowRules = 0;
char **denyRules = 0;
int *denyRulesFor = 0;
int seTotal = 0;
int coTotal = 0;
int allowRulesTotal = 0;
int denyRulesTotal = 0;
int maxfd = 0;
char *logFileName = 0;
char *pidLogFileName = 0;
int logFormatCommon = 0;
FILE *logFile = 0;

/* If 'newsize' bytes can be allocated, *data is set to point
   to them, the previous data is copied, and 1 is returned. 
   If 'size' bytes cannot be allocated, *data is UNCHANGED,
   and 0 is returned. */	

#define SAFE_REALLOC(x, y, z) safeRealloc((void **) (x), (y), (z))

int safeRealloc(void **data, int oldsize, int newsize);

/* 
se: (se)rver sockets
re: (re)mote sockets
lo: (lo)cal sockets (being redirected to)
co: connections
*/

#define bufferSpace 1024

void readConfiguration();

/* Signal handlers */
void plumber(int s);
void hup(int s);
void term(int s);

void initArrays(void);

void selectLoop(void);

void my_log(int i, int coSe, int result);


int getAddress(char *host, struct in_addr *iaddr);

char *logMessages[] = {
    "done-local-closed",
    "done-remote-closed",
    "accept-failed -",
    0,
    "local-socket-failed -",
    0,
    "local-bind-failed -",
    0,
    "local-connect-failed -",
    0,
    "not-allowed",
    0,
    "denied",
    0
};

#define logDone 0
#define logAcceptFailed 2
#define logLocalSocketFailed 4
#define logLocalBindFailed 6
#define logLocalConnectFailed 8
#define logNotAllowed 10
#define logDenied 12

#define logLocalClosedFirst 0
#define logRemoteClosedFirst 1

/* Option parsing */

typedef struct _rinetd_options RinetdOptions;
struct _rinetd_options
{
    char *conf_file;
};

RinetdOptions options = {
    "rinetd.conf"
};

int main(int argc, char *argv[])
{

    signal(SIGPIPE, plumber);
    signal(SIGHUP, hup);
    signal(SIGTERM, term);

    initArrays();
    readConfiguration();
    selectLoop();
    return 0;
}

int getConfLine(FILE *in, char *line, int space, int *lnum);

int patternBad(char *pattern);

void readConfiguration(void)
{
    FILE *in;
    char line[16384];
    int lnum = 0;
    int i;
    int ai;
    int di;
    in = fopen(options.conf_file, "r");
    if (!in) {
	fprintf(stderr, "rinetd: can't open %s\n", options.conf_file);
	exit(1);
    }
    while (1) {
	char *t = 0;
	if (!getConfLine(in, line, sizeof(line), &lnum)) {
	    break;
	}
	t = strtok(line, " \t\r\n");
	if (!strcmp(t, "logfile")) { 	
	    continue;
	} else if (!strcmp(t, "pidlogfile")) { 	
	    continue;
	} else if (!strcmp(t, "logcommon")) {
	    continue;
	} else if (!strcmp(t, "allow")) {
	    allowRulesTotal++;
	} else if (!strcmp(t, "deny")) {		
	    denyRulesTotal++;
	} else {	
	    /* A regular forwarding rule */
	    seTotal++;	
	}
    }	
    fclose(in);


    fprintf(stdout, "===============>%d\n", seTotal);

    seFds = (SOCKET *) malloc(sizeof(int) * seTotal);	
    if (!seFds) {
	goto lowMemory;
    }
    seLocalAddrs = (struct in_addr *) malloc(sizeof(struct in_addr) *
	    seTotal);	
    if (!seLocalAddrs) {
	goto lowMemory;
    }
    seLocalPorts = (unsigned short *) 
	malloc(sizeof(unsigned short) * seTotal);	
    if (!seLocalPorts) {
	goto lowMemory;
    }
    seFromHosts = (char **)
	malloc(sizeof(char *) * seTotal);
    if (!seFromHosts) {
	goto lowMemory;
    }
    seFromPorts = (int *)
	malloc(sizeof(int) * seTotal);	
    if (!seFromPorts) {
	goto lowMemory;
    }
    seToHosts = (char **)
	malloc(sizeof(char *) * seTotal);
    if (!seToHosts) {
	goto lowMemory;
    }
    seToPorts = (int *)
	malloc(sizeof(int) * seTotal);	
    if (!seToPorts) {
	goto lowMemory;
    }
    allowRules = (char **)
	malloc(sizeof(char *) * allowRulesTotal);
    if (!allowRules) {
	goto lowMemory;
    }
    denyRules = (char **)
	malloc(sizeof(char *) * denyRulesTotal);
    if (!denyRules) {
	goto lowMemory;
    }
    seAllowRules = (int *)
	malloc(sizeof(int) * seTotal);
    if (!seAllowRules) {
	goto lowMemory;
    }
    seAllowRulesTotal = (int *)
	malloc(sizeof(int) * seTotal);
    if (!seAllowRulesTotal) {
	goto lowMemory;
    }
    seDenyRules = (int *)
	malloc(sizeof(int) * seTotal);
    if (!seDenyRules) {
	goto lowMemory;
    }
    seDenyRulesTotal = (int *)
	malloc(sizeof(int) * seTotal);
    if (!seDenyRulesTotal) {
	goto lowMemory;
    }
    /* 2. Make a second pass to configure them. */	
    i = 0;
    ai = 0;
    di = 0;
    lnum = 0;
    in = fopen(options.conf_file, "r");
    if (!in) {
	goto lowMemory;
    }
    if (seTotal > 0) {
	seAllowRulesTotal[i] = 0;
	seDenyRulesTotal[i] = 0;
    }
    while (1) {
	char *bindAddress;
	unsigned short bindPort;
	char *connectAddress;
	char *bindPortS;
	char *connectPortS;
	unsigned short connectPort;
	struct in_addr iaddr;
	struct sockaddr_in saddr;
	struct servent *service;
	int j;
	if (!getConfLine(in, line, sizeof(line), &lnum)) {
	    break;
	}
	bindAddress = strtok(line, " \t\r\n");
	if (!bindAddress) {
	    fprintf(stderr, "rinetd: no bind address specified "
		    "on line %d.\n", lnum);	
	    continue;
	}	

	/* A regular forwarding rule. */
	bindPortS = strtok(0, " \t\r\n");
	if (!bindPortS) {
	    fprintf(stderr, "rinetd: no bind port "
		    "specified on line %d.\n", lnum);	
	    continue;
	}
	service = getservbyname(bindPortS, "tcp");	
	if (service) {
	    bindPort = ntohs(service->s_port);
	} else {
	    bindPort = atoi(bindPortS);
	}
	if ((bindPort == 0) || (bindPort >= 65536)) {
	    fprintf(stderr, "rinetd: bind port missing "
		    "or out of range on line %d.\n", lnum);
	    continue;
	}
	connectAddress = strtok(0, " \t\r\n");
	if (!connectAddress) {
	    fprintf(stderr, "rinetd: no connect address "
		    "specified on line %d.\n", lnum);	
	    continue;
	}	
	connectPortS = strtok(0, " \t\r\n");
	if (!connectPortS) {
	    fprintf(stderr, "rinetd: no connect port "
		    "specified on line %d.\n", lnum);	
	    continue;
	}
	service = getservbyname(connectPortS, "tcp");	
	if (service) {
	    connectPort = ntohs(service->s_port);
	} else {
	    connectPort = atoi(connectPortS);
	}
	if ((connectPort == 0) || (connectPort >= 65536)) {
	    fprintf(stderr, "rinetd: bind port missing "
		    "or out of range on line %d.\n", lnum);
	    continue;
	}
	/* Turn all of this stuff into reasonable addresses */
	if (!getAddress(bindAddress, &iaddr)) {
	    fprintf(stderr, "rinetd: host %s could not be "
		    "resolved on line %d.\n", 
		    bindAddress, lnum);
	    continue;
	}	
	/* Make a server socket */
	seFds[i] = socket(PF_INET, SOCK_STREAM, 0);
	if (seFds[i] == -1) {
	    fprintf(stderr, "rinetd: couldn't create "
		    "server socket!\n");
	    seFds[i] = -1;
	    continue;
	}
	if (seFds[i] > maxfd) {
	    maxfd = seFds[i];
	}
	saddr.sin_family = AF_INET;
	memcpy(&saddr.sin_addr, &iaddr, sizeof(iaddr));
	saddr.sin_port = htons(bindPort);
	j = 1;
	setsockopt(seFds[i], SOL_SOCKET, SO_REUSEADDR,
		(const char *) &j, sizeof(j));
	if (bind(seFds[i], (struct sockaddr *) 
		    &saddr, sizeof(saddr)) == -1) 
	{
	    /* Warn -- don't exit. */
	    fprintf(stderr, "rinetd: couldn't bind to "
		    "address %s port %d\n", 
		    bindAddress, bindPort);	
	    close(seFds[i]);
	    seFds[i] = -1;
	    continue;
	}
	if (listen(seFds[i], 5) == -1) {
	    /* Warn -- don't exit. */
	    fprintf(stderr, "rinetd: couldn't listen to "
		    "address %s port %d\n",
		    bindAddress, bindPort);	
	    close(seFds[i]);
	    seFds[i] = -1;
	    continue;
	}
	ioctl(seFds[i], FIONBIO, &j);
	if (!getAddress(connectAddress, &iaddr)) {
	    /* Warn -- don't exit. */
	    fprintf(stderr, "rinetd: host %s could not be "
		    "resolved on line %d.\n", 
		    bindAddress, lnum);
	    close(seFds[i]);
	    seFds[i] = -1;
	    continue;
	}	
	seLocalAddrs[i] = iaddr;
	seLocalPorts[i] = htons(connectPort);
	seFromHosts[i] = malloc(strlen(bindAddress) + 1);
	if (!seFromHosts[i]) {
	    goto lowMemory;
	}
	strcpy(seFromHosts[i], bindAddress);
	seFromPorts[i] = bindPort;
	seToHosts[i] = malloc(strlen(connectAddress) + 1);
	if (!seToHosts[i]) {
	    goto lowMemory;
	}
	strcpy(seToHosts[i], connectAddress);
	seToPorts[i] = connectPort;
	i++;
	if (i < seTotal) {
	    seAllowRulesTotal[i] = 0;
	    seDenyRulesTotal[i] = 0;
	}
    }
    /* Open the log file */
    if (logFile) {
	fclose(logFile);
	logFile = 0;
    }
    if (logFileName) {
	logFile = fopen(logFileName, "a");
	if (!logFile) {
	    fprintf(stderr, "rinetd: could not open %s to append.\n",
		    logFileName);
	}
    }

    return;
lowMemory:
    fprintf(stderr, "rinetd: not enough memory to start rinetd.\n");
    exit(1);
}

int getConfLine(FILE *in, char *line, int space, int *lnum)
{
    char *p;
    while (1) {
	if (!fgets(line, space, in)) {
	    return 0;
	}
	p = line;
	while (isspace(*p)) {
	    p++;
	}
	if (!(*p)) {
	    /* Blank lines are OK */
	    continue;
	}
	if (*p == '#') {
	    /* Comment lines are also OK */
	    continue;
	}
	(*lnum)++;
	return 1;
    }
}

void initArrays(void)
{
    int j;
    coTotal = 64;
    reFds = (SOCKET *) malloc(sizeof(int) * coTotal);
    loFds = (SOCKET *) malloc(sizeof(int) * coTotal);
    coInputRPos = (int *) malloc(sizeof(int) * coTotal);
    coInputWPos = (int *) malloc(sizeof(int) * coTotal);
    coOutputRPos = (int *) malloc(sizeof(int) * coTotal);
    coOutputWPos = (int *) malloc(sizeof(int) * coTotal);
    coClosed = (int *) malloc(sizeof(int) * coTotal);
    coClosing = (int *) malloc(sizeof(int) * coTotal);
    reClosed = (int *) malloc(sizeof(int) * coTotal);
    loClosed = (int *) malloc(sizeof(int) * coTotal);
    coInput = (char **) malloc(sizeof(char *) * coTotal);
    coOutput = (char **) malloc(sizeof(char *) * coTotal);
    coBytesInput = (int *) malloc(sizeof(int) * coTotal);
    coBytesOutput = (int *) malloc(sizeof(int) * coTotal);
    reAddresses = (unsigned char *) malloc(coTotal * 4);
    coLog = (int *) malloc(sizeof(int) * coTotal);
    coSe = (int *) malloc(sizeof(int) * coTotal);
    if ((!reFds) || (!loFds) || (!coInputRPos) || (!coInputWPos) ||
	    (!coOutputRPos) || (!coOutputWPos) || 
	    (!coClosed) || (!coClosing) ||
	    (!reClosed) || (!loClosed) ||
	    (!coInput) || (!coOutput) ||
	    (!coBytesInput) || (!coBytesOutput) || 
	    (!coLog) || (!coSe) || (!reAddresses)) 
    {
	fprintf(stderr, "rinetd: not enough memory to start rinetd.\n");
	exit(1);
    }	
    for (j = 0; (j < coTotal); j++) {
	coClosed[j] = 1;
	coInput[j] = (char *) malloc(sizeof(char) * bufferSpace);
	coOutput[j] = (char *) malloc(sizeof(char) * bufferSpace);
	if ((!coInput[j]) || (!coOutput[j])) {
	    fprintf(stderr, "rinetd: not enough memory to start "
		    "rinetd.\n");
	    exit(1);
	}
    }
}

void selectPass(void);

void selectLoop(void) {
    while (1) {
	selectPass();
    }
}

void handleRemoteWrite(int i);
void handleRemoteRead(int i);
void handleLocalWrite(int i);
void handleLocalRead(int i);
void handleCloseFromLocal(int i);
void handleCloseFromRemote(int i);
void handleAccept(int i);
void openLocalFd(int se, int i);
int getAddress(char *host, struct in_addr *iaddr);

void selectPass(void) {
    int i;
    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    /* Server sockets */
    for (i = 0; (i < seTotal); i++) {
	if (seFds[i] != -1) {
	    FD_SET(seFds[i], &readfds);
	}
    }
    /* Connection sockets */
    for (i = 0; (i < coTotal); i++) {
	if (coClosed[i]) {
	    continue;
	}
	if (coClosing[i]) {
	    if (!reClosed[i]) {
		FD_SET(reFds[i], &writefds);
	    }	
	    if (!loClosed[i]) {
		FD_SET(loFds[i], &writefds);
	    }	
	}
	/* Get more input if we have room for it */
	if ((!reClosed[i]) && (coInputRPos[i] < bufferSpace)) {
	    FD_SET(reFds[i], &readfds);
	}
	/* Send more output if we have any */	
	if ((!reClosed[i]) && (coOutputWPos[i] < coOutputRPos[i])) {
	    FD_SET(reFds[i], &writefds);
	}	
	/* Accept more output from the local 
	   server if there's room */
	if ((!loClosed[i]) && (coOutputRPos[i] < bufferSpace)) {
	    FD_SET(loFds[i], &readfds);
	}
	/* Send more input to the local server 
	   if we have any */
	if ((!loClosed[i]) && (coInputWPos[i] < coInputRPos[i])) {
	    FD_SET(loFds[i], &writefds);
	}	
    }
    select(maxfd + 1, &readfds, &writefds, 0, 0);
    for (i = 0; (i < seTotal); i++) {
	if (seFds[i] != -1) {
	    if (FD_ISSET(seFds[i], &readfds)) {
		handleAccept(i);
	    }
	}
    }
    for (i = 0; (i < coTotal); i++) {
	if (coClosed[i]) {
	    continue;
	}
	if (!reClosed[i]) {
	    if (FD_ISSET(reFds[i], &readfds)) {
		handleRemoteRead(i);
	    }
	}
	if (!reClosed[i]) {
	    if (FD_ISSET(reFds[i], &writefds)) {
		handleRemoteWrite(i);
	    }
	}
	if (!loClosed[i]) {
	    if (FD_ISSET(loFds[i], &readfds)) {
		handleLocalRead(i);
	    }
	}
	if (!loClosed[i]) {
	    if (FD_ISSET(loFds[i], &writefds)) {
		handleLocalWrite(i);
	    }
	}
	if (loClosed[i] && reClosed[i]) {
	    coClosed[i] = 1;
	}	
    }
}

void handleRemoteRead(int i)
{
    int got;
    if (bufferSpace == coInputRPos[i]) {
	return;
    }
    got = recv(reFds[i], coInput[i] + coInputRPos[i],
	    bufferSpace - coInputRPos[i], 0);
    
    fprintf(stdout, "[R][recv][%d][%d]\n", reFds[i], bufferSpace - coInputRPos[i]);
    //fprintf(stdout, "[R][recv]%s\n", coInput[i] + coInputRPos[i]);

    if (got == 0) {
	/* Prepare for closing */
	handleCloseFromRemote(i);
	return;
    }
    if (got < 0) {
	if ((errno) == EAGAIN) {
	    return;
	}
	if ((errno) == EINPROGRESS) {
	    return;
	}
	handleCloseFromRemote(i);
	return;
    }
    coBytesInput[i] += got;
    coInputRPos[i] += got;
}

void handleRemoteWrite(int i)
{
    int got;
    if (coClosing[i] && (coOutputWPos[i] == coOutputRPos[i])) {
	reClosed[i] = 1;
	coClosed[i] = 1;
	perror("rinetd: local closed and no more output");
	my_log(i, coSe[i], logDone | coLog[i]); 
	close(reFds[i]);
	return;
    }
    got = send(reFds[i], coOutput[i] + coOutputWPos[i],
	    coOutputRPos[i] - coOutputWPos[i], 0);
    
    fprintf(stdout, "[R][send][%d][%d]\n", reFds[i], coOutputRPos[i] - coOutputWPos[i]);
    //fprintf(stdout, "[R][send]%s\n", coOutput[i] + coOutputWPos[i]);
    if (got < 0) {
	if ((errno) == EAGAIN) {
	    return;
	}
	if ((errno) == EINPROGRESS) {
	    return;
	}
	handleCloseFromRemote(i);
	return;
    }
    coOutputWPos[i] += got;
    if (coOutputWPos[i] == coOutputRPos[i]) {
	coOutputWPos[i] = 0;
	coOutputRPos[i] = 0;
    }
    coBytesOutput[i] += got;
}

void handleLocalRead(int i)
{
    int got;
    if (bufferSpace == coOutputRPos[i]) {
	return;
    }
    got = recv(loFds[i], coOutput[i] + coOutputRPos[i], 
	    bufferSpace - coOutputRPos[i], 0);
    
    fprintf(stdout, "[L][recv][%d][%d]\n", loFds[i], bufferSpace - coOutputRPos[i]);
    //fprintf(stdout, "[L][recv]%s\n", coOutput[i] + coOutputRPos[i]);
    if (got == 0) {
	handleCloseFromLocal(i);
	return;
    }
    if (got < 0) {
	if ((errno) == EAGAIN) {
	    return;
	}
	if ((errno) == EINPROGRESS) {
	    return;
	}
	handleCloseFromLocal(i);
	return;
    }
    coOutputRPos[i] += got;
}

void handleLocalWrite(int i)
{
    int got;
    if (coClosing[i] && (coInputWPos[i] == coInputRPos[i])) {
	loClosed[i] = 1;
	coClosed[i] = 1;
	perror("remote closed and no more input");
	my_log(i, coSe[i], logDone | coLog[i]);
	close(loFds[i]);
	return;
    }
    got = send(loFds[i], coInput[i] + coInputWPos[i],
	    coInputRPos[i] - coInputWPos[i], 0);
   
    fprintf(stdout, "[L][send][%d][%d]\n", loFds[i], coInputRPos[i] - coInputWPos[i]);
    //fprintf(stdout, "[L][send]%s\n", coInput[i] + coInputWPos[i]);

    if (got < 0) {
	if ((errno) == EAGAIN) {
	    return;
	}
	if ((errno) == EINPROGRESS) {
	    return;
	}
	handleCloseFromLocal(i);
	return;
    }
    coInputWPos[i] += got;
    if (coInputWPos[i] == coInputRPos[i]) {
	coInputWPos[i] = 0;
	coInputRPos[i] = 0;
    }
}

void handleCloseFromLocal(int i)
{

    coClosing[i] = 1;
    perror("close from local");
    close(loFds[i]);
    loClosed[i] = 1;
    if (!reClosed[i]) {

	//int arg = 1024;
	//setsockopt(reFds[i], SOL_SOCKET, SO_SNDLOWAT, &arg, sizeof(arg));	
	coLog[i] = logLocalClosedFirst;
    } 
}

void handleCloseFromRemote(int i)
{
    coClosing[i] = 1;
    perror("close from remote");
    close(reFds[i]);
    reClosed[i] = 1;
    if (!loClosed[i]) {

	//int arg = 1024;
	//setsockopt(loFds[i], SOL_SOCKET, SO_SNDLOWAT, &arg, sizeof(arg));	
	loClosed[i] = 0;
	coLog[i] = logRemoteClosedFirst;
    }
}

void refuse(int index, int logCode);

void handleAccept(int i)
{
    struct sockaddr addr;
    struct sockaddr_in *sin;
    unsigned char address[4];
    char addressText[64];
    int j;
    socklen_t addrlen;
    int index = -1;
    int o;
    SOCKET nfd;
    addrlen = sizeof(addr);
    nfd = accept(seFds[i], &addr, &addrlen);
    if (nfd == -1) {
	my_log(-1, i, logAcceptFailed);
	return;
    }

    if (nfd > maxfd) {
	maxfd = nfd;
    }

    j = 1;
    ioctl(nfd, FIONBIO, &j);
    j = 0;

    setsockopt(nfd, SOL_SOCKET, SO_LINGER, &j, sizeof(j));

    for (j = 0; (j < coTotal); j++) {	
	if (coClosed[j]) {
	    index = j;
	    break;
	}
    }
    if (index == -1) {
	o = coTotal;
	coTotal *= 2;
	if (!SAFE_REALLOC(&reFds, sizeof(int) * o,
		    sizeof(SOCKET) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&loFds, sizeof(int) * o,
		    sizeof(SOCKET) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coInputRPos, 
		    sizeof(int) * o, sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coInputWPos, 
		    sizeof(int) * o, sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coOutputRPos, 
		    sizeof(int) * o, sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coOutputWPos, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coClosed, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coClosing, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&reClosed, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&loClosed, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coLog, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coSe, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coBytesInput, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&reAddresses, 4 * o, 
		    4 * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coBytesOutput, sizeof(int) * o, 
		    sizeof(int) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coInput, sizeof(char *) * o,
		    sizeof(char *) * coTotal)) 
	{
	    goto shortage;
	}
	if (!SAFE_REALLOC(&coOutput, sizeof(char *) * o,
		    sizeof(char *) * coTotal)) 
	{
	    goto shortage;
	}
	for (j = o; (j < coTotal); j++) {
	    coClosed[j] = 1;
	    coInput[j] = (char *) 
		malloc(sizeof(char) * bufferSpace);
	    if (!coInput[j]) {
		int k;
		for (k = o; (k < j); k++) {
		    free(coInput[k]);
		    free(coOutput[k]);
		}
		goto shortage;
	    }
	    coOutput[j] = (char *) 
		malloc(sizeof(char) * bufferSpace);
	    if (!coOutput[j]) {
		int k;
		free(coInput[j]);
		for (k = o; (k < j); k++) {
		    free(coInput[k]);
		    free(coOutput[k]);
		}
		goto shortage;
	    }
	}
	index = o;
    }
    coInputRPos[index] = 0;
    coInputWPos[index] = 0;
    coOutputRPos[index] = 0;
    coOutputWPos[index] = 0;
    coClosed[index] = 0;
    coClosing[index] = 0;
    reClosed[index] = 0;
    loClosed[index] = 0;
    reFds[index] = nfd;
    coBytesInput[index] = 0;
    coBytesOutput[index] = 0;
    coLog[index] = 0;
    coSe[index] = i;
    sin = (struct sockaddr_in *) &addr;
    memcpy(address, &(sin->sin_addr.s_addr), 4);
    memcpy(reAddresses + index * 4, address, 4);
    /* Now, do we want to accept this connection? 
       Format it for comparison to a pattern. */
    sprintf(addressText, "%d.%d.%d.%d",
	    address[0], address[1], address[2], address[3]);
    /* 1. Check global allow rules. If there are no
       global allow rules, it's presumed OK at
       this step. If there are any, and it doesn't
       match at least one, kick it out. */
    if (globalAllowRules) {
	int good = 0;
	for (j = 0; (j < globalAllowRules); j++) {
	    if (match(addressText, allowRules[j])) {
		good = 1;
		break;
	    }
	}
	if (!good) {
	    refuse(index, logNotAllowed);
	    return;
	}	
    }
    /* 2. Check global deny rules. If it matches
       any of the global deny rules, kick it out. */
    if (globalDenyRules) {			
	for (j = 0; (j < globalDenyRules); j++) {
	    if (match(addressText, denyRules[j])) {
		refuse(index, logDenied);
	    }
	}
    }
    /* 3. Check allow rules specific to this forwarding rule.
       If there are none, it's OK. If there are any,
       it must match at least one. */
    if (seAllowRulesTotal[i]) {
	int good = 0;
	for (j = 0; (j < seAllowRulesTotal[i]); j++) {
	    if (match(addressText, 
			allowRules[seAllowRules[i] + j])) {
		good = 1;
		break;
	    }
	}
	if (!good) {
	    refuse(index, logNotAllowed);
	    return;
	}	
    }
    /* 2. Check deny rules specific to this forwarding rule. If 
       it matches any of the deny rules, kick it out. */
    if (seDenyRulesTotal[i]) {			
	for (j = 0; (j < seDenyRulesTotal[i]); j++) {
	    if (match(addressText, 
			denyRules[seDenyRules[i] + j])) {
		refuse(index, logDenied);
	    }
	}
    }
    /* Now open a connection to the local server.
       This, too, is nonblocking. Why wait
       for anything when you don't have to? */
    openLocalFd(i, index);	
    return;
shortage:
    fprintf(stderr, "rinetd: not enough memory to "
	    "add slots. Currently %d slots.\n", o);
    /* Go back to the previous total number of slots */
    coTotal = o;	
}

void openLocalFd(int se, int i)
{
    int j;
    struct sockaddr_in saddr;
    loFds[i] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (loFds[i] == -1) {
	close(reFds[i]);
	reClosed[i] = 1;
	loClosed[i] = 1;
	coClosed[i] = 1;	
	my_log(i, coSe[i], logLocalSocketFailed);
	return;
    }
    if (loFds[i] > maxfd) {
	maxfd = loFds[i];
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = INADDR_ANY;
    saddr.sin_addr.s_addr = 0;
    if (bind(loFds[i], (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
	close(loFds[i]);
	close(reFds[i]);
	reClosed[i] = 1;
	loClosed[i] = 1;
	coClosed[i] = 1;	
	my_log(i, coSe[i], logLocalBindFailed);
	return;
    }
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr, &seLocalAddrs[se], sizeof(struct in_addr));
    saddr.sin_port = seLocalPorts[se];

    j = 1024;
    setsockopt(loFds[i], SOL_SOCKET, SO_SNDBUF, &j, sizeof(j));

    j = 1;
    ioctl(loFds[i], FIONBIO, &j);
    if (connect(loFds[i], (struct sockaddr *)&saddr, 
		sizeof(struct sockaddr_in)) == -1) 
    {
	if (((errno) != EINPROGRESS) &&
		((errno) != EAGAIN))
	{
	    perror("rinetd: connect");
	    close(loFds[i]);
	    close(reFds[i]);
	    reClosed[i] = 1;
	    loClosed[i] = 1;
	    coClosed[i] = 1;	
	    my_log(i, coSe[i], logLocalConnectFailed);
	    return;
	}
    }
}

int getAddress(char *host, struct in_addr *iaddr)
{
    char *p = host;
    int ishost = 0;
    while (*p) {
	if (!(isdigit(*p) || ((*p) == '.'))) {
	    ishost = 1;
	    break;
	}
	p++;
    }
    if (ishost) {
	struct hostent *h;
	h = gethostbyname(host);
	if (!h) {
	    return 0;
	}
	memcpy(
		(void *) &iaddr->s_addr,
		(void *) h->h_addr,
		4);
	return 1;
    } else {
	iaddr->s_addr = inet_addr(host);
	return 1;
    }
}

void plumber(int s)
{
    /* Just reinstall */
    signal(SIGPIPE, plumber);
}

void hup(int s)
{
    /* Learn the new rules */
    readConfiguration();
    /* And reinstall the signal handler */
    signal(SIGHUP, hup);
}

int safeRealloc(void **data, int oldsize, int newsize)
{
    void *newData = malloc(newsize + 1);
    if (!newData) {
	return 0;
    }
    if (newsize < oldsize) {
	memcpy(newData, *data, newsize);
    } else {	
	memcpy(newData, *data, oldsize);
    }
    *data = newData;
    return 1;
}

unsigned char nullAddress[4] = { 0, 0, 0, 0 };

struct tm *get_gmtoff(int *tz);

void my_log(int i, int coSe, int result)
{
    unsigned char *reAddress;
    int bytesOutput;
    int bytesInput;
    /* Bit of borrowing from Apache logging module here,
       thanks folks */
    int timz;
    struct tm *t;
    char tstr[1024];
    char sign;

    t = get_gmtoff(&timz);
    sign = (timz < 0 ? '-' : '+');
    if (timz < 0) {
	timz = -timz;
    }
    strftime(tstr, sizeof(tstr), "%d/%b/%Y:%H:%M:%S ", t);

    if (i != -1) {
	reAddress = reAddresses + i * 4;
	bytesOutput = coBytesOutput[i];
	bytesInput = coBytesInput[i];
    } else {
	reAddress = nullAddress;
	bytesOutput = 0;
	bytesInput = 0;
    }
    if (logFile) {
	if (logFormatCommon) {
	    /* Fake a common log format log file in a way that
	       most web analyzers can do something interesting with.
	       We lie and say the protocol is HTTP because we don't
	       want the web analyzer to reject the line. We also
	       lie and claim success (code 200) because we don't
	       want the web analyzer to ignore the line as an
	       error and not analyze the "URL." We put a result
	       message into our "URL" instead. The last field
	       is an extra, giving the number of input bytes,
	       after several placeholders meant to fill the 
	       positions frequently occupied by user agent, 
	       referrer, and server name information. */
	    fprintf(logFile, "%d.%d.%d.%d - - "
		    "[%s %c%.2d%.2d] "
		    "\"GET /rinetd-services/%s/%d/%s/%d/%s HTTP/1.0\" "
		    "200 %d - - - %d\n",
		    reAddress[0],
		    reAddress[1],
		    reAddress[2],
		    reAddress[3],
		    tstr,
		    sign,
		    timz / 60,
		    timz % 60,
		    seFromHosts[coSe], seFromPorts[coSe],
		    seToHosts[coSe], seToPorts[coSe],
		    logMessages[result],
		    bytesOutput,
		    bytesInput);
	} else {
	    /* Write an rinetd-specific log entry with a
	       less goofy format. */
	    fprintf(logFile, "%s\t%d.%d.%d.%d\t%s\t%d\t%s\t%d\t%d"
		    "\t%d\t%s\n",
		    tstr,
		    reAddress[0],
		    reAddress[1],
		    reAddress[2],
		    reAddress[3],
		    seFromHosts[coSe], seFromPorts[coSe],
		    seToHosts[coSe], seToPorts[coSe],
		    bytesInput,	
		    bytesOutput,	
		    logMessages[result]);
	}
    }
}

struct tm *get_gmtoff(int *tz) {
    time_t tt = time(NULL);
    struct tm gmt;
    struct tm *t;
    int days, hours, minutes;

    /* Assume we are never more than 24 hours away. */
    gmt = *gmtime(&tt); /* remember gmtime/localtime return ptr to static */
    t = localtime(&tt); /* buffer... so be careful */
    days = t->tm_yday - gmt.tm_yday;
    hours = ((days < -1 ? 24 : 1 < days ? -24 : days * 24)
	    + t->tm_hour - gmt.tm_hour);
    minutes = hours * 60 + t->tm_min - gmt.tm_min;
    *tz = minutes;
    return t;
}

int patternBad(char *pattern)
{
    char *p = pattern;
    while (*p) {
	if (isdigit(*p) || ((*p) == '?') || ((*p) == '*') ||
		((*p) == '.')) 
	{
	    p++;
	}
	return 0;
    }
    return 1;
}

void refuse(int index, int logCode)
{
    close(reFds[index]);
    reClosed[index] = 1;
    loClosed[index] = 1;
    coClosed[index] = 1;	
    my_log(index, coSe[index], logCode);
}

void term(int s)
{
    /* Obey the request, but first flush the log */
    if (logFile) {
	fclose(logFile);
    }
    exit(0);
}

