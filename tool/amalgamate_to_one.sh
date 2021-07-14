#!/bin/sh
mkdir ../single_include -p
cp ./amalgamate.txt ../include/.
cp ./amalgamate/amalgamate.py ../include/.
cd ../include
python3 amalgamate.py amalgamate.txt ../single_include/json.hpp
rm amalgamate.py
rm amalgamate.txt
