#!/bin/sh 
#
# Autottex
#
# Author : Vee Satayamas <virasj@users.sourceforge.net>
#
# Usage: autottex.sh <main Thai Latex file (omit .tex.th)>
#
# Version : 0.2.1
#
# Copyright (C) 2003 Vee Satayamas
#
# Redistribution and use in source and binary forms, with 
# or without modification, are permitted provided that the 
# following conditions are met:
#
# 1. Redistributions of source code must retain the above 
#    copyright notice, this list of conditions and the 
#    following disclaimer.
# 2. Redistributions in binary form must reproduce the above 
#    copyright notice, this list of conditions and the following 
#    disclaimer in the documentation and/or other materials 
#    provided with the distribution.
# 3. The name of the author may not be used to endorse or 
#    promote products derived from this software without 
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND 
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
# AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, 
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
# OF SUCH DAMAGE.

texfiles=""
thaitexfiles=*.th.tex

for thaitexfile in $thaitexfiles ;do
    texfile=`echo $thaitexfile | sed 's/\.th\./\./'`
    texfiles="$texfile $texfiles"
done

echo -e "all : $1.dvi"
echo
echo -e "showpdf : $1.pdf"
echo -e "\tacroread $<"
echo
echo -e "$1.pdf : $1.dvi"
echo -e "\tdvipdf $<"
echo
echo -e "showdvi : $1.dvi"
echo -e "\txdvi $<"
echo 
echo -e "$1.dvi : $texfiles"
echo -e "\tlatex $1"
echo -e "\tlatex $1"
echo -e "\tbibtex $1"
echo -e "\tmakeindex -T $1"
echo -e "\tlatex $1"
echo -e "\tlatex $1"


for thaitexfile in $thaitexfiles ;do
    echo 
    texfile=`echo $thaitexfile | sed 's/\.th\./\./'`
    echo -e "$texfile : $thaitexfile"
    echo -e "\tswath -f latex < \$< > \$@"
done

echo "clean:"
echo -e "\trm -f $texfiles"
for texfile in $texfiles ;do
    filename=`echo $texfile | sed s/\.tex$//`
done
echo -e "\trm -f *.dvi *.pdf *.aux *.lof *.log *.idx *.ind *.ilg *.toc *.bbl"


echo "update:"
echo -e "\t./autottex.sh $1 > Makefile"

echo 
echo "distclean : clean"
echo -e "\trm -f *~ *.tar.gz"

echo
cdate=`date +%Y%m%d`
echo "dist : distclean"
echo -e "\trm -rf $1-$cdate"
echo -e "\tmkdir $1-$cdate"
echo -e "\tfor x in * ;do if [ -f \$\$x ];then cp \$\$x $1-$cdate ;fi ;done"
echo -e "\ttar cvvzf $1-$cdate.tar.gz $1-$cdate"
echo -e "\trm -rf $1-$cdate"

