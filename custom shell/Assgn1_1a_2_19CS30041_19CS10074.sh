n=$1
i=2
while [ $n -ne 1 ]
do
 while [ $(expr $n / $i \* $i) == $n ] 
 do
 n=`expr $n / $i`
 echo -ne "$i "
 done
 i=$((i+1))
done
