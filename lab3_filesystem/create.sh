#!/bin/bash

echo create files ...

gcc -o apd append.c 

for i in {0..9..1}; do
	mkdir mnt/$i
	for j in {0..99..1}; do
		echo "$i/$j-1" > mnt/$i/$j
	done
done

for i in {0..9..1}; do
	for j in {0..99..1}; do
		./apd mnt/$i/$j 2 "$i/$j-2"
	done

	for j in {0..99..1}; do
		./apd mnt/$i/$j 3 "$i/$j-3"
	done
done

echo done
