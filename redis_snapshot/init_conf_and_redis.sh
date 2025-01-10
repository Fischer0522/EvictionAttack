#!/bin/bash

/bin/gen_file 

# Script completion message
echo "gen_file has finished executing,start a redis server"

/bin/redis-server /redis.conf