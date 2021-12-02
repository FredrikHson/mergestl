#!/bin/bash

mergestring=()

for i in $@; do
    IFS=$'\n'
    if [[ "$i" == file://* ]]; then
        filename=$(python -c "import sys, urllib.parse as ul; print(ul.unquote_plus(\"$i\"))"| perl -pe 's#file://##')
    else
        filename=$1
    fi
    mergestring+=" -i '$filename'"
done
if [[ "$1" == file://* ]]; then
    infilename=$(python -c "import sys, urllib.parse as ul; print(ul.unquote_plus(\"$i\"))"| perl -pe 's#file://##')
else
    infilename=$1
fi
filename="'$(dirname $infilename)/$(echo "$@" | sha1sum | awk '{ print $1 }').stl'"
eval mergestl "${mergestring[@]}" -o "$filename"
