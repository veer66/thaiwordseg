#!/bin/sh
for x in *.java ;do
    cat $x | sed 's/package $1/package $2/g' > $x.tmp && mv $x.tmp $x
done
