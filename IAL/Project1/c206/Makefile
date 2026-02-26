PROJECT=c206
PROG=$(PROJECT)-test
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -lm -fcommon
SHELL=bash

# Podmienečne zistíme, či existujú advanced testy
ADV_SRC := $(wildcard $(PROJECT)-test-advanced.c)
ifneq ($(ADV_SRC),)
  HAVE_ADVANCED := 1
else
  HAVE_ADVANCED := 0
endif

# Zoznam binárok a referenčných výstupov podľa toho, či máme advanced
TESTS=$(PROJECT)-test
TEST_OUTPUTS=$(PROJECT)-test.out
ifeq ($(HAVE_ADVANCED),1)
  TESTS += $(PROJECT)-test-advanced
  TEST_OUTPUTS += $(PROJECT)-test-advanced.out
endif

.PHONY: all run advanced tests clean purge reference zadani zadani-en reseni

all: $(PROG)

run: $(PROG)
	@./$(PROG) > current-test.out
	@echo -e "\nTest output differences:"
	@diff -u --report-identical-files --text $(PROG).out current-test.out
	@rm -f current-test.out

tests: $(TESTS)

$(PROJECT)-test: $(PROJECT).c $(PROJECT)-test.c $(PROJECT)-test-utils.c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c $(PROJECT)-test.c $(PROJECT)-test-utils.c

$(PROJECT)-test.out: $(PROJECT)-test
	@if [ -f $(PROJECT)-test.out ]; then echo "The reference output already exists!"; exit 1; fi
	./$(PROJECT)-test > $(PROJECT)-test.out

# -------- Advanced len ak existuje zdroj --------
ifeq ($(HAVE_ADVANCED),1)

advanced: $(PROJECT)-test-advanced
	@./$(PROJECT)-test-advanced > current-advanced-test.out
	@echo -e "\nAdvanced test output differences:"
	@diff -u --report-identical-files --text $(PROJECT)-test-advanced.out current-advanced-test.out
	@rm -f current-advanced-test.out

$(PROJECT)-test-advanced: $(PROJECT).c $(PROJECT)-test-advanced.c $(PROJECT)-test-utils.c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c $(PROJECT)-test-advanced.c $(PROJECT)-test-utils.c

$(PROJECT)-test-advanced.out: $(PROJECT)-test-advanced
	@if [ -f $(PROJECT)-test-advanced.out ]; then echo "The reference output already exists!"; exit 1; fi
	./$(PROJECT)-test-advanced > $(PROJECT)-test-advanced.out

else

advanced:
	@echo "Advanced tests are not available (missing $(PROJECT)-test-advanced.c)."

endif
# -----------------------------------------------

reference: $(TEST_OUTPUTS)

zadani: $(PROJECT)-test.out
	@mkdir $@ 2>/dev/null || echo "Directory exists, continuing"
	@unifdef -DSTUDENT_BUILD=1 -o $@/$(PROJECT).c $(PROJECT).c || printf ""
	@cp $(PROJECT).h $@/
	@cp $(PROG).c $@/
	@cp $(PROJECT)-test-utils.* $@/
	@cp $(PROJECT)-test.out $@/
	@cp Makefile $@/
	@tar -czvf $(PROJECT)-`date +%Y-%m-%d-%H-%M`.tar.gz $@/*

zadani-en: $(PROJECT)-test.out
	@mkdir $@ 2>/dev/null || echo "Directory exists, continuing"
	@unifdef -DSTUDENT_BUILD=1 -o $@/$(PROJECT).c $(PROJECT).en.c || printf ""
	@cp $(PROJECT).en.h $@/$(PROJECT).h
	@cp $(PROG).c $@/
	@cp $(PROJECT)-test-utils.* $@/
	@cp $(PROJECT)-test.out $@/
	@cp Makefile $@/
	@tar -czvf $(PROJECT)-`date +%Y-%m-%d-%H-%M`.tar.gz $@/*

reseni:
	@mkdir $@

clean:
	@rm -f *.o $(PROG) $(TESTS)
	@rm -rf zadani reseni

purge: clean
	@rm -Rf $(PROJECT)-output
	@rm -f *.out *.txt *.html
