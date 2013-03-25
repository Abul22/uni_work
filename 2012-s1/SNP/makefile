GS = -g
JC = javac
.SUFFIXES: .java .class
	.java.class:
	        $(JC) $(JFLAGS) $*.java

CLASSES = \
	  IterativeServer.java \
	  Client.java \
	  Server.java \
	  ConcurrentServer.java \
	  MulticastClient.java \
	  MulticastServer.java \
	  alm/server/ALMServer.java \
	  alm/client/ALMClient.java \
	  alm/util/ClientDeail.java \
	  alm/util/MassageInfo.java \

default: classes

classes: $(CLASSES:.java=.class)

clean:
	        $(RM) *.class
