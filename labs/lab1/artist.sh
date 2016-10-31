#!/bin/bash

echo "The process ID is: "$$""

#check if enough variables
if [ "$#" -lt 2 ]
then
	echo "Too few variables, exiting.."
	exit 1
fi

#check if the first argument is a string or length greater than 0
if [ -f "$1" ] || [ -z "$1" ]
then
	echo "Not a valid name, exiting..."
	exit 1
fi

#set up a variable to hold the artist name and then shift
artist=$1
shift

#loop through the files, checking the Artist tag
for i in $*
do	
	if [ ! -f "$i" ]
	then
		echo ""$i" is not a valid file"
	else

		artist_field=$(exif -m --tag='Artist' "$i" 2> /dev/null | grep "$artist" 2> /dev/null)
		if [ "$artist_field" != "" ]
		then
			echo "$i"
		fi
	fi
done
