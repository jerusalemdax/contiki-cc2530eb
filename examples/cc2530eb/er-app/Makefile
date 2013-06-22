all: er-app
# Use this target explicitly if requried: er-plugtest-server

CONTIKI=../../..

# variable for root Makefile.include
HAVE_BANKING=1
WITH_UIP6=1
# for some platforms
UIP_CONF_IPV6=1
UIP_CONF_RPL=1

# variable for this Makefile
# configure CoAP implementation (3|7) (er-coap-07 also supports CoAP draft 08)
WITH_COAP=7

# REST framework, requires WITH_COAP
CFLAGS += -DWITH_COAP=7
CFLAGS += -DREST=coap_rest_implementation
CFLAGS += -DUIP_CONF_TCP=0
APPS += er-coap-07
APPS += erbium

# optional rules to get assembly
#CUSTOM_RULE_C_TO_OBJECTDIR_O = 1
#CUSTOM_RULE_S_TO_OBJECTDIR_O = 1

include $(CONTIKI)/Makefile.include
