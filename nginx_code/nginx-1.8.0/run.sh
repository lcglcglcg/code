#/bin/bash

export MODULE;
MODULE+=" --add-module=src/lg_core"
MODULE+=" --add-module=src/lg_leveldb"
# MODULE+=" --add-module=src/demo_module"
MODULE+=" --add-module=src/site_builder_module"
# MODULE+=" --add-module=src/manager_site_builder_module"
sh configure $MODULE && make

