#!/bin/bash

# HOWTO:
# cd to the root of the project, with an empty directory called "src" within it, and a .git repo initialized
# Pass the path(s) of the .img files in as arguments. Bash expansions with wildcards can be used (eg /blah/*.img).

LOOP_MOUNT="./src"
IMAGES="$@"

git status

if [ $? -ne 0 ]
then
	echo "This must be run in the root of the git project"
	exit 1
fi

for THIS_IMAGE in ${IMAGES}
do
	IMAGE_FILENAME=$(basename "${THIS_IMAGE}") 
	echo "Mounting ${IMAGE_FILENAME} ..."
	
	sudo umount "${LOOP_MOUNT}"
	sudo mount -v -o loop -t hfs "${THIS_IMAGE}" "${LOOP_MOUNT}"

	if [ $? -ne 0 ]
	then
		echo "Loop mount failed, stopping."
		exit 2
	fi

	echo "Committing ..."

	git add -A
	git status

	git commit -v -m "${IMAGE_FILENAME}"

	echo
	echo "Continue"
	echo
done

echo "Done."
