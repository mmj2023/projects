if command -v rg &>/dev/null; then
 echo "Cat Bat Fat" | rg "." # looks for a match for any character in the whole string can used with the combination of any of the other regex operators when needing to match any chracter.
 echo "Cat Bat Fat" | rg "C" # looks for a match for C in the whole string
 echo "Cat Bat Fat" | rg "^." # looks for a match for any character in the beginning of the string
 echo "Cat Bat Fat" | rg ".$" # looks for a match for any character in the end of the string
 echo "Cat Bat Fat" | rg "t$" # looks for a match for t in the end of the string
 echo "Cat Bat Fat" | rg "^C" # looks for a match for C in the beginning of the string
 echo "Cat Bat Fat" | rg "C|B|F" # looks for a match for C, B, or F in the whole string due to C|B|F
 echo "Cat Bat Fat" | rg "(C|B|F)" # looks for a match for C, B, or F due to (C|B|F) () enables grouping to be used in the regex later as \1, \2, etc.
 echo "Cat Bat Fat" | rg "^(C|B|F)" # Matches the first character of each line with ^ and looks for a match for C, B, or F due to (C|B|F)
 else
  echo "rg not found"
fi
