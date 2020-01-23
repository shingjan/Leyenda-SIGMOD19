#!/bin/bash
if [ -z "$1" ] || [ -z "$2" ]
then
    echo "Usage: test.sh [sort|radix] [1k|10m|100m|1g|10g|40g|100g]"
    exit 1
fi
echo "Running task: $1 $2"
TASK=$1
SIZE=$2
IN="input_$SIZE"
OUT="output_${TASK}_${SIZE}"
echo "Using input file $IN and output file $OUT"

echo "Removing old image"
docker images prune -f
echo "Building new image"
docker build . -t contest
echo "Running container"

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
echo "Work directory: $DIR"

docker run -it -m 30g -v /home/andrew/output:/output-disk -v /media/andrew/2nd_disk/jvm-overflow/input:/input-disk contest /bin/bash -c "/src/tester.sh $TASK $SIZE"

echo "Result in $DIR/output/$OUT"
