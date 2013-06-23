all: rest-server-example coap-client-example

TARGET=cc2530eb

CONTIKI=../../..

HAVE_BANKING=1
WITH_UIP6=1
UIP_CONF_IPV6=1
UIP_CONF_RPL=1

WITH_COAP = 1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CFLAGS += -DWITH_COAP
APPS += rest-coap

include $(CONTIKI)/Makefile.include
