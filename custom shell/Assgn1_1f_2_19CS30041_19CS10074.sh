echo "Enter filename :"
read filename
echo "Enter Column no. :"
read n
awk '{print $'$n'}' $filename|tr A-Z' ' a-z'\n'|sort|uniq -c| awk '{print $2" "$1}' |sort -r -k 2 > 1e_output_"$n"_column.freq
