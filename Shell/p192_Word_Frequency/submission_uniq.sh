# Read from the file words.txt and output the word frequency list to stdout.
cat words.txt | awk 'BEGIN{FS=" "}{for(i=1;i<=NF;i++){print $i}}' | sort | uniq -c | awk 'BEGIN{FS=" "}{print $2,$1}' | sort -k2 -rn
