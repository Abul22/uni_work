#!/bin/sh

# Shell script to find and compile the agents in the current directory
#
# Stefan Bird <stbird@seatiger.org>, 2004

# Make doesn't want to pass us these, so just pass them as arguments
COMPILE_COMMAND=$1



# Make sure there are actually some agents in the directory!!
ls agent_*.c &> /dev/null
if [ $? -ne 0 ]
then   
   echo "ERROR: No agents found."
   echo "Make sure your agent is in a file named 'agent_`whoami`.c'"
   exit 1
fi

# Get a list of the names and files, and a count of how many agents there are

AGENT_NAMES=`grep '^double *agent_' agent_*.c | awk '{ print $2 }' | sed -e 's/agent_\([-_a-zA-Z0-9]*\)(.*/\1/' | sort -u`
AGENT_FILES=`grep '^double *agent_' *.c | awk '{ print $1 }' | sed -e 's/:.*$//' | sort -u`
NUM_AGENTS=`echo $AGENT_NAMES | wc -w`

# If any precompiled sample agents were included, add them to the list

if [ -f agent_*.o.sample ]
then
   for sampleAgent in `ls agent_*.o.sample`
   do
      cp -f $sampleAgent `echo $sampleAgent | sed -e 's/\.sample$//'`
      AGENT_NAMES="$AGENT_NAMES `cat $sampleAgent.list`"
      count=`cat $sampleAgent.list | wc -w`
      NUM_AGENTS=`expr $NUM_AGENTS + $count`
   done   
fi

# The name of the C source file we will write
OUTPUT_FILE=get_agents.c

# Remove it if it already exists
rm -f $OUTPUT_FILE

# Output a header to try to stop people from meddling
echo '/* === DO NOT EDIT THIS FILE ===' >> $OUTPUT_FILE
echo ' *' >> $OUTPUT_FILE
echo ' * This file has been automatically generated. ' >> $OUTPUT_FILE
echo ' * To update the list of agents rerun make.' >> $OUTPUT_FILE
echo ' *' >> $OUTPUT_FILE
echo ' * Stefan Bird <stbird@seatiger.org>, 2004' >> $OUTPUT_FILE
echo ' */' >> $OUTPUT_FILE
echo >> $OUTPUT_FILE
echo '#include <stdlib.h>' >> $OUTPUT_FILE
echo '#include "c4.h"' >> $OUTPUT_FILE
echo >> $OUTPUT_FILE

# Write the function signatures
for agent in $AGENT_NAMES;
do
   echo "extern double agent_$agent (Game_state *current_state, int player, int x, int y);" >> $OUTPUT_FILE
done

# This method simply returns the number of agents known about
echo >> $OUTPUT_FILE
echo "int count_agents()" >> $OUTPUT_FILE
echo "{" >> $OUTPUT_FILE
echo "   return `expr $NUM_AGENTS`;" >> $OUTPUT_FILE
echo "}" >> $OUTPUT_FILE

# This method returns the actual agents, along with their names
echo >> $OUTPUT_FILE
echo 'Agent **get_agents()' >> $OUTPUT_FILE
echo '{' >> $OUTPUT_FILE
echo "   Agent **funcs = emalloc(sizeof(Agent *) * count_agents());" >> $OUTPUT_FILE
count=0

for agent in $AGENT_NAMES;
do
   echo >> $OUTPUT_FILE
   echo "   funcs[$count] = emalloc(sizeof(Agent));" >> $OUTPUT_FILE
   echo "   funcs[$count]->agentFunction = agent_$agent;" >> $OUTPUT_FILE
   echo "   funcs[$count]->name = \"$agent\";" >> $OUTPUT_FILE
   
   count=`expr $count + 1`

   echo " * Found agent '$agent'"
done
   
echo >> $OUTPUT_FILE
echo "   return funcs;" >> $OUTPUT_FILE
echo "}" >> $OUTPUT_FILE

# Compile all of the agent source files
for agentSourceFile in $AGENT_FILES;
do
   agentObjectFile=`echo $agentSourceFile | sed -e 's/\.c$/.o/'`
   
   command="$COMPILE_COMMAND -c $agentSourceFile -o $agentObjectFile"
   echo $command
   $command
done
