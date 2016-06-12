#/bin/bash

export MODULE;
MODULE+=" --add-module=src/lg_core"
MODULE+=" --add-module=src/lg_json"
# MODULE+=" --add-module=src/lg_redis"
# MODULE+=" --add-module=src/lg_gdlib"
# MODULE+=" --add-module=src/lg_ocilib"
MODULE+=" --add-module=src/ad_system_display_module"
sh configure $MODULE && make

