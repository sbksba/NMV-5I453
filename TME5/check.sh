#!/bin/bash
#check.sh

echo "###########"
echo "#  EXO 2  #"
echo "###########"
echo ""
/tmp/linux-4.2.3/scripts/checkpatch.pl -f exo-2/taskmonitor.c
echo "###########"
echo "#  EXO 3  #"
echo "###########"
echo ""
/tmp/linux-4.2.3/scripts/checkpatch.pl -f exo-3/hellosysfs.c
echo "###########"
echo "#  EXO 4  #"
echo "###########"
echo ""
/tmp/linux-4.2.3/scripts/checkpatch.pl -f exo-4/taskmonitor.c
echo "###########"
echo "#  EXO 5  #"
echo "###########"
echo ""
/tmp/linux-4.2.3/scripts/checkpatch.pl -f exo-5/helloioctl.c
echo "###########"
echo "#  EXO 6  #"
echo "###########"
echo ""
/tmp/linux-4.2.3/scripts/checkpatch.pl -f exo-6/taskmonitor.c
