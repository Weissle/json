#!/bin/sh
cp ./amalgamate.txt ../include/.
cp ./amalgamate/amalgamate.py ../include/.
cd ../include
python3 amalgamate.py amalgamate.txt ../json.hpp
rm amalgamate.py
rm amalgamate.txt
