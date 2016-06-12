
#include "lg_leveldb.h"
#include "leveldb/db.h"

void *lg_leveldb_open(char *path)
{

    leveldb::DB *db = NULL;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    if (!status.ok()) return NULL;

    return db;
}

int lg_leveldb_set(void *p_db, char *key, char *value)
{

    leveldb::DB *db = (leveldb::DB *)p_db;
    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);
    if (!status.ok()) return -1;

    return 0;
}

int lg_leveldb_get(void *p_db, char *key, char **value, ngx_pool_t *pool)
{

    std::string str;
    leveldb::DB *db = (leveldb::DB *)p_db;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &str);
    if (!status.ok()) return -1;
    if (str.size() <= 0) return -1;

    char *buffer = (char *)ngx_pcalloc(pool, str.size()  + 1);
    if (!buffer) return -1;

    memcpy(buffer, str.c_str(), str.size());
    buffer[str.size()] = 0;
    
    *value = buffer;
    return str.size();
}


