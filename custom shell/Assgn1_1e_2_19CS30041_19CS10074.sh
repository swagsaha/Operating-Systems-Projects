_Ver=0
while getopts "v" OPTION
do
  case $OPTION in
    v) _Ver=1
       ;;
  esac
done
function log () {
    if [[ $_Ver -eq 1 ]]; then
        echo "$@"
    fi
}
curl -o example.html http://www.example.com/
log "IP address is being printed."
curl http://ip.jsontest.com/
curl http://headers.jsontest.com/>header.json
IFS=','
paths=($REQ_HEADERS)
for (( i=0; i<${#paths[@]}; i++ ))
do
   echo -ne "${paths[$i]} :"
   jq '.'${paths[$i]}'' header.json
done
rm header.json
for file in JSONData/*.json
do
  curl --data-urlencode "json=$(cat $file)" http://validate.jsontest.com/ | jq -r '.validate' >> ch.txt
done
f="ch.txt"
i=0
while read line; do   
  if [ $line = "true" ]
   then
	log ""$i".json is valid."
        echo ""$i".json" >> valid.txt
   else
	log ""$i".json is invalid."
        echo ""$i".json" >> invalid.txt
  fi
i=$((i+1))  
done < $f  
rm ch.txt
