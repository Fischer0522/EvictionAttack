#! /bin/bash

echo "restore run dir and metadata file"
rm -rf ./occlum_instance/run
mv ./occlum_instance/run_back ./occlum_instance/run
mv ./occlum_instance/metadata ./occlum_instance/run/mount/__ROOT/metadata
echo "finish restoring run dir and metadata file"

cd occlum_instance && occlum run /bin/busybox cat /redis.conf

occlum run /bin/redis-server /redis.conf