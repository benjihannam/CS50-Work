#!/bin/bash

#check to see that there are the correct number of params
if [ $# -lt 3 ]
then
	echo "There are not enough arguments, exiting..."
	exit 1
fi

#store the params in variables
NEW_NAME=$1
NEW_PHOTO=$2
NEW_TEMP=$3

#check to see if the newDirectoryName is already existing and create the directory if it isn't
if ! mkdir "$NEW_NAME"
then
	echo "The directory name already exists, exiting..."
	exit 1
fi

#Check to see if PhotoDirectoryName or TemplateDirectoryName are not directories
if [ ! -d "$2" ]
then
	echo "$2 is not a directory, exiting..."
	exit 1
fi

if [ ! -d "$3" ]
then
	echo "$3 is not a directory, exiting..."
	exit 1
fi

#Check if PhotoDirectoryName has .jpg files, header.txt and title.txt
if [ ! -e $2/*.jpg ] || [ ! -e $2/header.txt ] || [ ! -e $2/title.txt ]
then
	echo "$2 does not have either .jpg files, a header.txt file or a title.txt file."
	exit 1
fi

#Check if templateDirectoryName is missing any of the templates
if [ ! -e "$3/empty.template" ] || [ ! -e "$3/header.template" ] || [ ! -e "$3/image.template" ] || [ ! -e "$3/index.template" ] || [ ! -e "$3/navigation.bottom.template" ] \
 || [ ! -e "$3/navigation.item.template" ] || [ ! -e "$3/navigation.top.template" ]
then
	echo "$3 is missing one of the templates."
	exit 1
fi

#Create the images and thumbnails subdirectories
mkdir "$NEW_NAME/images"
mkdir "$NEW_NAME/thumbs"

#copy the photos into the subdirectories
for j in $NEW_PHOTO/*.jpg
do
	scp "$j" $NEW_NAME/images
	base=$(basename $j)
	convert "$j" -thumbnail 100x100 "$NEW_NAME/thumbs/$base"
done

#Begin making the navigation.html file by writing in the top part
cat "$3/navigation.top.template" > "$NEW_NAME/navigation.html"

#Replace the words in the template files

	#replace all @__@ in header.template and generate header.html
	title=$(cat $2/title.txt | tr '\n' ' ')
	header=$(cat "$2/header.txt" | tr '\n' ' ')

	sed -e "s+@TITLE@+$title+g" -e "s+@HEADCAPTION@+$header+g" "$3/header.template" > $1/header.html 

	#replace all @__@ in index.template and generate index.html
	sed -e "s+@TITLE@+${title}+g" "$3/index.template" > $1/index.html
	
	#generate empty.html
	cat $3/empty.template > $1/empty.html
	
#Make the image.html files
for i in $2/*.jpg
do
	base_file=$(basename $i) 
	
	#Set up the varibales for the @__@ replacers
	if ! caption=$(exif -m --tag='ImageDescription' $i 2> /dev/null)
	then
		caption=""
	fi
	
	if ! date=$(exif -m --tag='DateTimeOriginal' $i 2> /dev/null)
	then
		date="Unknown"
	fi
	
	if ! credit=$(exif -m --tag='Artist' $i 2> /dev/null)
	then
		credit="Unkown"
	fi

        if copyright=$(exif -m --tag='Copyright' $i 2> /dev/null)
        then
                copyright="Unkown"
        fi

	image_page=$(echo $base_file | sed -e "s+.jpg+.html+g" )
	image_file=images/$base_file
	thumb=thumbs/$base_file
	
	#Now do the replacing
	sed -e "s+@CAPTION@+$caption+g" -e "s+@DATE@+$date+g" -e "s+@CREDIT@+$credit+g" -e "s+@COPYRIGHT@+$copyright+g" \
	-e "s+@IMAGEFILE@+$image_file+g" "$3/image.template" > $1/$image_page

	#append the navigation.item.template
	sed -e "s+@CAPTION@+$caption+g" -e "s+@IMAGEPAGE@+$image_page+g" -e "s+@THUMB@+$thumb+g" "$3/navigation.item.template" >> $1/navigation.html

done

#append the navigation.bottom.template
cat "$3/navigation.bottom.template" >> $1/navigation.html
