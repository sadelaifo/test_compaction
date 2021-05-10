#! /bin/bash
my_db=mydb
echo "************************************"
echo testing fillrand
rm -rf mydb/
./main -b=fillrand $my_db | grep fraction

echo "************************************"
echo testing fillseq
rm -rf mydb/
./main -b=fillseq  $my_db | grep fraction

echo "************************************"
echo testing readseq
./main -b=readseq $my_db | grep fraction

echo "************************************"
echo testing readrand
./main -b=readrand $my_db | grep fraction

echo "************************************"
echo testing update
./main -b=update $my_db | grep fraction

echo ALL tests done
