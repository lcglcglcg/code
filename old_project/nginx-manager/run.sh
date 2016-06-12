#/bin/bash
make clean
./configure --add-module=src/ad_system_manager_module --with-debug --error-log-path=/var/log/nginx-manager/wangmeng.log
make
