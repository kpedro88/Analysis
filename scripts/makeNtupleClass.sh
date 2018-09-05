#!/bin/bash

source sourceNtupleClass.sh

INITIALIZED=""

for ((i=0; i < ${#SAMPLES[@]}; i++)); do
	SAMPLE=${SAMPLES[$i]}
	DIR=${DIRS[$i]}
	TREE=${TREES[$i]}

	root -b -l -q 'makeTempClass.C+("'$DIR/$SAMPLE'","'$TREE'","TempClass")'

	if [[ -z "$INITIALIZED" ]]; then
		mv TempClass.h NtupleClass.h
		INITIALIZED=yes
		continue
	fi

	# find max length of lines in files, pad it a little
	len1=$(wc -L < NtupleClass.h)
	len2=$(wc -L < TempClass.h)
	len=$(( $len1 > $len2 ? $len1 : $len2 ))
	len=$(( $len*2 ))

	# diff side-by-side format:
	# xxxx * yyyy
	# * can be ), <, |, >
	# split into two sides, strip trailing whitespace
	# keep any new lines from TempClass
	diff -y -W $len --left-column NtupleClass.h TempClass.h | expand | \
	awk -v sample=$SAMPLE ' 
		BEGIN { len = 0; }
			  { if ( len == 0 ) len = length($0)-1;
				leftside = substr($0,1,len);
				sub(/[ \t]+$/,"",leftside);
				diffchar = substr($0,len+1,1);
				rightside = substr($0,len+3,length($0));
				sub(/[ \t]+$/,"",rightside);
				if  ( length(leftside)>0 || length(rightside)==0 )
				{
					print leftside;
				}
				if  ( (diffchar == "|" || diffchar == ">") && 
						index(rightside,sample)==0 && index(rightside,"ROOT version")==0 && 
						index(rightside,"fChain(0)")==0 && index(rightside,".root")==0 &&
						length(rightside)>0 )
				{
					 print rightside;
				}
			  }
		' > NtupleClass_.h

	mv NtupleClass_.h NtupleClass.h
done

sed -i 's/TempClass/NtupleClass/g' NtupleClass.h
rm TempClass.h TempClass.C > /dev/null 2>&1

# fix header (optionally)
if [[ -z "$HEADER" ]]; then
	exit 0
fi
awk -v header="$HEADER" '
BEGIN { started = 0; }
	  { if ( index($0,"class NtupleClass {") != 0 ) started = 0;
		if ( !started ) print $0;
		if ( index($0,"#define NtupleClass_h") != 0 ) {
			started = 1;
			print header;
		}
	  }
' < NtupleClass.h > NtupleClass_.h
mv NtupleClass_.h NtupleClass.h
