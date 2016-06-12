
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#include "SSDB_client.h"

int main(int argc, char **argv)
{

    // connect to server
    ssdb::Client *client = ssdb::Client::connect("/tmp/content.sock", 0);
    if(client == NULL){
	printf("fail to connect to server!\n");
	return 0;
    }

    ssdb::Status s;
    std::string val;
    std::vector<std::string> ret;

#if 0
    for (int i = 0; i < 1000; i++) {

	char buf[128];
	sprintf(buf, "%04d-lcg", i);
	client->set(buf, "1234");
    }
#endif

#if 1
    client->keys("01", "z", 10, &ret);
    for(int i = 0; i < (int)ret.size(); i++)
	printf("%s\n", ret[i].c_str());
#endif

#if 0
    std::string a = "";
    std::string b = "";
    s = client->get_kv_range(&a, &b);
    printf("%s::%s\n", a.c_str(), b.c_str());
    if(s.ok())
	fprintf(stdout, "ok\n");
    else
	fprintf(stdout, "err\n");

    client->rscan("", "", 10, &ret);
    for(int i = 0; i < (int)ret.size(); i++)
	printf("%d::%s\n", i, ret[i].c_str());


    int64_t size;
    client->dbsize(&size);
    fprintf(stdout, "SIZE=%zd\n", size);

    s = client->get("lcglcg", &val);
    printf("lcglcg::%s\n", val.c_str());
#endif

    return 0;
}


