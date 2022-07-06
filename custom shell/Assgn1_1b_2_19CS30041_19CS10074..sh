mkdir 1.b.files.out
for file in 1.b.files/*
do
	cat $file | sort -n -o "1.b.files.out/$(basename $file)"
done
sort -n 1.b.files.out/*.txt > res.txt
tr ' ' '\n' < res.txt | sort -n | uniq -c | awk '{print $2" "$1}' > 1.b.out.txt
rm res.txt
