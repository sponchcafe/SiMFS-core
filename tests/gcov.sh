#! /bin/bash
for d in $(find $1 -type d)
do
    cd $d
    ls | grep ".*.gcno" | xargs gcov
    cd $1
done
