#!/bin/sh
VOLUME_SIZE=128
IMAGE_SIZE=1024
NNODES=4

mpirun --oversubscribe -n $NNODES ./ImageComposition $VOLUME_SIZE $IMAGE_SIZE
