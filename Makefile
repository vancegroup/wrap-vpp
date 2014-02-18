#!/usr/bin/make -f

# List of versions to wrap
versions = 3.10 3.20 3.60 3.70 3.80

# Function to compute input header from a version
define inputheader
  $(firstword $(wildcard $(1)/?irtuoseAPI.h))
endef

# Function to compute an output header from a version
define outputheader
  $(1)/vpp.h
endef

# Dependencies shared by all versions
commondeps = vpp-boilerplate.h \
             wrap_virtuoseapi.py \
             $(lastword $(MAKEFILE_LIST))

# If we're building any other target than "clean", clean the boilerplate first.
ifneq ($(.DEFAULT_GOAL),clean)
  $(info Detected a build request - cleaning the boilerplate)
  $(info $(shell ./clean_boilerplate))
  $(info  )
endif

# Default target builds everybody.
all: $(foreach ver,$(versions),$(call outputheader,$(ver)))

# Clean target deletes the generated files.
clean:
	rm -f $(foreach ver,$(versions),$(call outputheader,$(ver)))
	-rm -f *.pyc
	-rm -f lextab.py

# Generate a build rule for each version. Pattern rules don't work here
# for some reason, sadly.

define define_build_rule
$(1)/vpp.h: $(call inputheader,$(1)) $(commondeps)
	./wrap_virtuoseapi.py $(call inputheader,$(1)) $(call outputheader,$(1))
endef

$(foreach ver,$(versions),$(eval $(call define_build_rule,$(ver))))

.PHONY = all clean

