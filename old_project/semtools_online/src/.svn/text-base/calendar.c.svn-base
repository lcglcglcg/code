
#include "calendar.h"

time_t calendar_string_to_time(char *date_str)
{

    struct tm date_tm;
    memset(&date_tm, 0, sizeof(struct tm));
    if (!strptime(date_str, "%Y-%m-%d", &date_tm))
	return 0;

    return (mktime(&date_tm) + 86400);
}

void calendar_time_to_string(time_t date_time, char *date_str)
{

    struct tm date_tm;
    gmtime_r(&date_time, &date_tm);
    sprintf(date_str, "%04d-%02d-%02d",
	    date_tm.tm_year + 1900,
	    date_tm.tm_mon + 1,
	    date_tm.tm_mday);
}

calendar_node_t *calendar_node_find(calendar_user_t *user, time_t date)
{

    calendar_node_t *node = NULL;
    if (!user->head)return NULL;

    pthread_mutex_lock(&user->lock);
    for (node = user->head; node; node = node->next) {

	if (node->date == date) {

	    pthread_mutex_unlock(&user->lock);
	    return node;
	}
    }

    pthread_mutex_unlock(&user->lock);
    return NULL;
}

calendar_node_t *calendar_node_find_str(calendar_user_t *user, char *date_str)
{

    time_t date = 0;
    if (!(date = calendar_string_to_time(date_str)))
	return NULL;

    return calendar_node_find(user, date);
}

void calendar_node_add(calendar_user_t *user, time_t date)
{
    if (!date)return;
    if (calendar_node_find(user, date))
	return;

    calendar_node_t *node = NULL;
    if (!(node = malloc(sizeof(calendar_node_t))))
	return;

    node->date = date;
    pthread_mutex_lock(&user->lock);
    node->next = user->head;
    user->head = node;
    pthread_mutex_unlock(&user->lock);
}

void calendar_node_add_str(calendar_user_t *user, char *date_str)
{

    time_t date = 0;
    if (!(date = calendar_string_to_time(date_str)))
	return;

    calendar_node_add(user, date);
}

calendar_user_t *calendar_user_find(calendar_t *calendar, int user_id)
{

    calendar_user_t *user = NULL;
    if (!calendar->head)return NULL;

    pthread_mutex_lock(&calendar->lock);
    for (user = calendar->head; user; user = user->next) {

	if (user->user_id == user_id) {

	    pthread_mutex_unlock(&calendar->lock);
	    return user;
	}
    }

    pthread_mutex_unlock(&calendar->lock);
    return NULL;
}

calendar_user_t *calendar_user_add(calendar_t *calendar, int user_id)
{

    calendar_user_t *user = NULL;
    if ((user = calendar_user_find(calendar, user_id)))
	return user;

    if (!(user = malloc(sizeof(calendar_user_t))))
	return NULL;

    if (pthread_mutex_init(&user->lock, NULL) != 0) {

	free(user);
	return NULL;
    }

    user->head = NULL;
    user->user_id = user_id;

    pthread_mutex_lock(&calendar->lock);
    user->next = calendar->head;
    calendar->head = user;
    pthread_mutex_unlock(&calendar->lock);

    return user;
}

void calendar_add(calendar_t *calendar, int user_id, time_t date)
{

    calendar_user_t *user = NULL;
    if ((user = calendar_user_find(calendar, user_id)))
	return calendar_node_add(user, date);

    if (!(user = malloc(sizeof(calendar_user_t))))
	return;

    if (pthread_mutex_init(&user->lock, NULL) != 0)
	return free(user);

    user->head = NULL;
    user->user_id = user_id;

    pthread_mutex_lock(&calendar->lock);
    user->next = calendar->head;
    calendar->head = user;
    pthread_mutex_unlock(&calendar->lock);

    calendar_node_add(user, date);
}

void _calendar_load(calendar_t *calendar, char *buffer)
{

    if (!buffer)return;

    char *str = NULL;
    for (str = buffer; *str; str++) {

	if (*str == ':'){

	    *str = 0;
	    str++;
	    break;
	}
    }

    if (!*str)return;

    int user_id = atoll(buffer);
    time_t time_date = atoll(str);
    if (!user_id || !time_date)return;

    calendar_user_t *user = calendar_user_add(calendar, user_id);
    calendar_node_add(user, time_date);
}

void calendar_load(calendar_t *calendar, char *path)
{

    FILE *fp = NULL;
    if (!(fp = fopen(path, "r")))
	return;

    char buffer[1024];
    while(fgets(buffer, 1024, fp))
	_calendar_load(calendar, buffer);

    fclose(fp);
}

void calendar_save(calendar_t *calendar, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0755)) == -1)
	return;

    size_t size;
    char buffer[256];
    calendar_user_t *user = NULL;
    calendar_node_t *node = NULL;

    for (user = calendar->head; user; user = user->next) {

	for (node = user->head; node; node = node->next) {

	    size = sprintf(buffer, "%u:%lu\n", user->user_id, node->date);
	    if (write(fd, buffer, size) != size) {

		close(fd);
		return;
	    }
	}
    }

    close(fd);
}

calendar_t *calendar_init(void)
{

    calendar_t *calendar = NULL;
    if (!(calendar = malloc(sizeof(calendar_t))))
	return NULL;

    if (pthread_mutex_init(&calendar->lock, NULL) != 0) {

	free(calendar);
	return NULL;
    }

    calendar->head = NULL;
    return calendar;
}

void calendar_node_free(calendar_user_t *user)
{

    calendar_node_t *node = user->head;
    calendar_node_t *node_free = NULL;

    while(node){

	node_free = node;
	node = node->next;
	free(node_free);
    }
}

void calendar_free(calendar_t *calendar)
{

    calendar_user_t *user = calendar->head;
    calendar_user_t *user_free = NULL;

    while(user){

	user_free = user;
	user = user->next;
	calendar_node_free(user_free);
	pthread_mutex_destroy(&user_free->lock);
	free(user_free);
    }
}

void calendar_print(calendar_t *calendar)
{

    calendar_user_t *user = NULL;
    calendar_node_t *node = NULL;

    for (user = calendar->head; user; user = user->next) {

	for (node = user->head; node; node = node->next) {

	    char date_str[20] = {0};
	    calendar_time_to_string(node->date, date_str);
	    fprintf(stdout, "[%d][%s]\n", user->user_id, date_str);
	}
    }
}







