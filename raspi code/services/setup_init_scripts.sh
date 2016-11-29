#!/bin/bash

cp jrti-* /etc/init.d
chmod 755 /etc/init.d/jrti-*
chown root:root /etc/init.d/jrti-*

for f in jrti-*
do
	echo "copying $f"
	update-rc.d "$f" defaults
	service "$f" start
done
