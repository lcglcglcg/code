#/bin/bash

export MODULE;
MODULE+=" --add-module=src/lg_core"
#MODULE+=" --add-module=src/lg_json"
#MODULE+=" --add-module=src/lg_redis"
#MODULE+=" --add-module=src/lg_mysql"
#MODULE+=" --add-module=src/lg_ocilib"
#MODULE+=" --add-module=src/lg_mongodb"
#MODULE+=" --add-module=src/lg_var_code"

MODULE+=" --add-module=src/demo_module"
#MODULE+=" --add-module=src/upload_module"
#MODULE+=" --add-module=src/cms_module"
#MODULE+=" --add-module=src/lost_spider_module"
#MODULE+=" --add-module=src/manager_account_module"

sh configure --with-debug $MODULE && make

