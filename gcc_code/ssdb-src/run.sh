#/bin/sh

rm -rf /tmp/*

./ssdb-server -d conf/content.conf
./ssdb-server -d conf/group.conf
./ssdb-server -d conf/link_external.conf
./ssdb-server -d conf/link_internal.conf
./ssdb-server -d conf/meta.conf
./ssdb-server -d conf/time_str.conf
./ssdb-server -d conf/title.conf

sleep 1
chmod 777 /tmp/*

