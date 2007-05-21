#!/bin/sh

rm test_tail.txt && touch test_tail.txt && echo 'hola' >> test_tail.txt && ./caf_tail test_tail.txt 
