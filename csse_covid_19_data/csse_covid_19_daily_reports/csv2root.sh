#!/bin/bash

files=""
for file in *csv; do
   if [ -z "$files" ]; then
      files=$file
   else
      files="$files $file"
   fi
done
# echo $files
command="root -l -b -q csv2root.C'(\""$files"\")'"
echo $command
# $command
