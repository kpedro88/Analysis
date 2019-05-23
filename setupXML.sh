#!/bin/bash

VERSION=v1.9

cd KMVA
for FILE in pugixml.hpp pugixml.cpp pugiconfig.hpp; do
	wget -O ${FILE} https://github.com/zeux/pugixml/raw/${VERSION}/src/${FILE}
done
