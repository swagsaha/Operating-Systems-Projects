cd data1c

pwd=$PWD
ext=$(find . -type f | perl -ne 'print $1 if m/\.([^.\/]+)$/' | sort -u)
for f in $ext;
do
mkdir -p $f;
done;
mkdir -p Nil



cd data

dirlist=$(find $1 -mindepth 1 -maxdepth 5 -type d)
for dir in $dirlist;  
do pushd $dir; 
filelist=$(find . -maxdepth 1 -type f); 
for file in $filelist;
do filename=$(basename -- "$file"); 
extension="${filename##*.}"; 
fullpath="$pwd/$extension";  
 mv $file  /$fullpath;
done; 
popd;
done

filelist=$(find . -maxdepth 1 -type f); 
for file in $filelist;
do filename=$(basename -- "$file"); 
extension="${filename##*.}"; 
fullpath="$pwd/$extension";  
 mv $file  /$fullpath
  done


cd $pwd

rm -r data
filelist=$(find . -maxdepth 1 -type f); 
for file in $filelist;
do NIL="Nil"
 fullpath="$pwd/$NIL";
 echo $fullpath 
 mv $file /$fullpath;
 done


