#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
bomfile=${SCRIPT_DIR}/snapshot.yaml

occlum-g++ gen_file.cc -o gen_file
# 1. Init Occlum Workspace
rm -rf occlum_instance
occlum new occlum_instance
cd occlum_instance

TCS_NUM=$(($(nproc) * 2))
new_json="$(jq --argjson THREAD_NUM ${TCS_NUM} '.resource_limits.user_space_size="500MB" |
    .resource_limits.user_space_max_size = "500MB" |
    .resource_limits.kernel_space_heap_size = "500MB" |
    .resource_limits.kernel_space_heap_max_size="500MB" |
    .resource_limits.max_num_of_threads = $THREAD_NUM ' Occlum.json)" && \
echo "${new_json}" > Occlum.json

#
# 2. Copy files into Occlum Workspace and Build
rm -rf image
copy_bom -f $bomfile --root image --include-dir /opt/occlum/etc/template

SGX_MODE=HW occlum build
# 3. Run redis server
occlum run /bin/gen_file