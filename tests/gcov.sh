#! /bin/bash
for d in $(find $1 -type d)
do
    cd $d
    ls | grep ".*.gcno" | xargs gcov
    cd $1
done

lcov -c -d . -o lcov.info
genhtml lcov.info
open index.html
