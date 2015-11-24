SUBDIRECTORIES:= exo-2 exo-3 exo-4 exo-5 exo-6
.PHONY: all $(SUBDIRECTORIES) proper clean check

all: $(SUBDIRECTORIES)

$(SUBDIRECTORIES):
	@echo ""
	@echo "---------------------------------------------------------------------------"
	@echo "## $@ ##"
	@echo ""
	@cd $@ && $(MAKE)

proper:
	rm -f *~

clean :
	-for d in $(SUBDIRECTORIES); do (cd $$d; $(MAKE) clean ); done

check:
	@./check.sh
