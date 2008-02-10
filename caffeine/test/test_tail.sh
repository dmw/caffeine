#!/bin/sh

echo "Removing test_tail.txt..."
rm -vf test_tail.txt

echo "Creating a new file (test_tail.txt)..."
touch test_tail.txt

echo "Sending caf_tail to background"
./caf_tail test_tail.txt &

echo "Sending text to the file..."

caf_os=`uname -s`

case $caf_os in
        *BSD*)
                echo "BSD system"
                for x in `jot 100 1`;
                do
                        echo "text line $x" >> "test_tail.txt"
                        sleep 1
                done
                ;;
        *Linux*)
                echo "Linux System"
                for x in `seq 1 1 100`;
                        do
                        echo "text line $x" >> "test_tail.txt"
                        sleep 1
                done
                ;;
esac

