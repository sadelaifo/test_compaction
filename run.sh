#! /bin/bash
echo "************************************"
echo testing fillseq
rm -rf mydb/
./main -b=fillseq | grep fraction

echo "************************************"
echo testing fillrand
rm -rf mydb/
./main -b=fillseq | grep fraction

echo "************************************"
echo testing readseq
./main -b=readseq | grep fraction

echo "************************************"
echo testing readrand
./main -b=readrand | grep fraction

echo "************************************"
echo testing update
./main -b=update | grep fraction

echo ALL tests done
