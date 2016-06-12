//
//  main.c
//  demo
//
//  Created by 李春光 on 16/3/7.
//  Copyright © 2016年 李春光. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <mysql.h>

char *host = "188.188.3.151";
char *name = "root";
char *pass = "lcglcg";
char *base = "demo";

int demo1()
{

    MYSQL *p = mysql_init(NULL);
    if (!p) {

	printf("err:: init\n");
	return -1;
    }

    if (!mysql_real_connect(p, host, name, pass, base, 0, 0, 0)) {

	printf("err:: conn\n");
	return -1;
    }

    mysql_query(p, "insert into demo values ('中国', 'zh')");
    mysql_close(p);
    return 0;
}

int demo2()
{

    MYSQL *p = mysql_init(NULL);
    if (!p) {

	printf("err:: init\n");
	return -1;
    }

    if (!mysql_real_connect(p, host, name, pass, base, 0, 0, 0)) {

	printf("err:: conn\n");
	return -1;
    }

    char sql_str[] = "select * from demo";
    mysql_real_query(p, sql_str, strlen(sql_str));
    MYSQL_RES *res = mysql_store_result(p);

    MYSQL_ROW row = NULL;
    while((row = mysql_fetch_row(res))) {

	fprintf(stdout, "[%s][%s]\n", row[0], row[1]);
    }

    mysql_free_result(res);
    mysql_close(p);
    return 0;
}

int main(int argc, const char * argv[])
{

    // demo1();
    demo2();
    return 0;
}

