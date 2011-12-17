for f in $(find . -type f) ; do
	mv $f.$$.tmp $f
done
