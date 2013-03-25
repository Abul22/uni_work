#!/bin/bash
for ((i=0;i<10;i++))
do
java -Djavax.net.ssl.trustStore=keystore Client&
done
