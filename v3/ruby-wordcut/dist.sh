#!/bin/sh
tmp=ruby-wordcut-`cat VERSION`
rm -rf $tmp
mkdir $tmp
cp * -R $tmp
tar cvvzf $tmp.tar.gz $tmp
