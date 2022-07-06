#!/bin/bash
echo "Enter filename:"
read filename
for i in $(seq 1 150)
do
 echo \"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\",\"$(shuf -i 0-1000000 -n1)\" >> $filename
done
echo "Enter column no.:"
read col
n=$col
echo '1,"2,3,4,5,6,7,8,9",10' | awk -F "\"*,\"*" '{print $'$col'}' $filename > check.txt
sed 's/"//' check.txt > check_.txt
echo "Enter expression:"
read ex
cat check_.txt | grep "$ex" > check.txt
if [ -s check.txt ]
then
        echo "YES"
else
        echo "NO"
fi
rm check_.txt
