#!/bin/sh


#You can use this file to check that your submission for assignment2
#will be correctly processed by our submission script

if [ $# -ne 1 ]
then
	echo "Usage: $0 agent_name"
	exit 1;
fi

ROUNDS=50;
OPPONENTS="random simple medium"

make clean
make

echo " " 
echo "***************** Game results *****************" 

for OPPONENT in $OPPONENTS  
do
rm -f gameresults.txt
ROUND=0

while [ $ROUND -lt `expr $ROUNDS / 2` ] 
do
 ./c4 $1 $OPPONENT --fast | tail -1 >> gameresults.txt
 ./c4 $OPPONENT $1 --fast | tail -1 >> gameresults.txt

 ROUND=`expr $ROUND + 1`
done

PLAYER_WINS=`grep $1 gameresults.txt | wc -l `
PLAYER_WINS=`expr $PLAYER_WINS`
OPPONENT_WINS=`grep $OPPONENT gameresults.txt | wc -l `
OPPONENT_WINS=`expr $OPPONENT_WINS`

DRAWS=`expr $ROUNDS - $PLAYER_WINS - $OPPONENT_WINS`

echo " " 
echo "Results of $1 vs. $OPPONENT ($ROUNDS rounds):" 
echo " " 

[ $DRAWS -eq 0 ] && echo "$1: $PLAYER_WINS   $OPPONENT: $OPPONENT_WINS"   
[ $DRAWS -eq 1 ] && echo "$1: $PLAYER_WINS   $OPPONENT: $OPPONENT_WINS   (1 tie)"   
[ $DRAWS -gt 1 ] && echo "$1: $PLAYER_WINS   $OPPONENT: $OPPONENT_WINS   ($DRAWS ties)"   

[ $PLAYER_WINS -gt $OPPONENT_WINS ] && echo "   *** $1 beat $OPPONENT ***" 
done
