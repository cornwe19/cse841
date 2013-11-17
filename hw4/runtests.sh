#!/bin/bash

for i in 1 2 3
do
   set -x
   ./dntext -d db.dat -f "DN$i.txt" -o "synapse$i" -Y 8 -l
   ./dntext -d db.dat -f "DN$i.txt" -o "results$i.txt"
   ./dntext -d db.dat -f "DN$i.think.txt" -o "results$i.think.txt"
   set +x
done
