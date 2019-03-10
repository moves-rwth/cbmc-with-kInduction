#!/bin/bash

# $1: File to slice.
# $2: Output file, relative to the location of the original file. Warning: Can overwrite files if they have the same name!

if [ -z $2 ]; then
	out="sliced"
else
	out="$2"
fi

tmp=$(mktemp)
frama-c -slice-calls "__VERIFIER_error" $1 -then-on 'Slicing export' -print -ocode "$tmp"
vars=$(awk '/__VERIFIER_nondet/ {print $1}' "$tmp" | tail -n +2)
while read -r var
do
	assumes=$(cat $1 | grep -A 2 "$var = __VERIFIER_nondet" | tail -n +2)
	cmd="awk -i inplace '/$var = __VERIFIER_nondet/ { print; print \"$assumes\"; next }1' $tmp"
	eval $cmd
done <<< $vars
sed -i '1s/^/extern void __VERIFIER_assume(int);\n/' "$tmp"
mv "$tmp" $(dirname $(readlink -f $1))/"$out"
