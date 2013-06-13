all: broadcast-example unicast-sender unicast-receiver
APPS=servreg-hack
CONTIKI=../../..

HAVE_BANKING=1
WITH_UIP6=1
UIP_CONF_IPV6=1
UIP_CONF_RPL=1
CFLAGS+= -DUIP_CONF_IPV6_RPL

include $(CONTIKI)/Makefile.include
