#!/bin/bash
[[ ! -d files_mod ]]&& mkdir files_mod

cp -r "data1d/temp/"* files_mod
for file in files_mod/*;do
tmpfile=$(mktemp)
 awk '{print NR  "," $s}'  $file > ${tmpfile}
 sed -e 's/\s\+/,/g' ${tmpfile} > $file
 rm -f ${tmpfile}
 done

