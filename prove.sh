#!/bin/bash

if [ $# -eq 0 ]; then
    echo "usage: $0 <run number> or $0 <run number> <subrun number>"
    exit 1
fi

if [ $# -eq 1 ]; then
echo "Proving everything..."
grsiproof --max-workers 10 $GRSISYS/myAnalysis/CaenTestSelector.C analysis$1_???.root
mv CaenTest$1_000.root CaenTest$1.root
grsisort -l CaenTest$1.root
fi

if [ $# -eq 2 ]; then
echo "Proving run $1 subrun $2..."
grsiproof --max-workers 10 $GRSISYS/myAnalysis/CaenTestSelector.C analysis$1_$2.root
grsisort -l CaenTest$1_$2.root
fi

