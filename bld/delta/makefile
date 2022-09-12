# Quick-n-dirty makefile to be used in the oracle for creduce

# With these parameters, matrix_test usually hangs within 75 iterations on an 8 core box
# Note: these values must match in check.sh, oracle.sh, and makefile
READERS=30
DEPOSITORS=0
CONNECTIONS=$(shell echo $$(( $(READERS) + $(DEPOSITORS) )))

CC     := gcc
IFLAGS := -DHAVE_OPENSSL_ECC -DHAVE_OPENSSL -D_GNU_SOURCE -DDROP_SUPPORT_FOR_SLAW_V1 -pthread -DQUOTED_PREFIX=\"/usr\"
CFLAGS := -g -O2 -fsanitize=address -pthread -lssl -lcrypto -lm -lyaml
DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

all: unitize matrix_test

# Preprocess it
# Takes advantage of the concatenated 'unity' source files for loam and plasma
# Note that, when you run creduce, it will overwrite this matrix_test.i with a shorter one
# (Not run automatically to keep make from freaking out during creduce)
unitize:
	echo "Assumes you've done 'mesongen.sh c --unity on'"
	(cd ../../obj-*; \
	cat ../libLoam/c/ob-hash.c ../libLoam/c/ob-hash-city.c libLoam/c/*@@loam_c@sha/loam_c-unity.c libPlasma/c/*@@plasma_c@sha/plasma_c-unity.c libPlasma/c/*@@matrix_test@exe/matrix_test-unity.c > matrix_test.c; \
	cc -IlibLoam/c -IlibPlasma/c -I../libPlasma/c -IlibPlasma/c/../.. -I../libPlasma/c/../.. $(IFLAGS) -E matrix_test.c ) > matrix_test.i

# Compile it
# To manually protect a few functions, move them to main.c
matrix_test: matrix_test.i
	cp matrix_test.i main.i
	if test -f $(DIR)/main.c; then cat $(DIR)/main.c >> main.i; fi
	$(CC) main.i $(CFLAGS) -o matrix_test

# Try to hang.
check: matrix_test
	N=1; \
	while test $$N -lt 120 ; \
	do \
		if ! sh -x $(DIR)/check.sh > check.log.$$N 2>&1; then echo "TEST FAILED, STATUS"; exit 1; fi; \
		if egrep -i 'abort|ERROR: AddressSanitizer' check.log.$$N; then echo "TEST FAILED, ASAN"; exit 1; fi; \
		if test $$(grep '^reader' < check.log.$$N | wc -l) -lt $(READERS); then echo "TEST FAILED, READERS"; exit 1; fi; \
		if test $$(grep '^depositor' < check.log.$$N | wc -l) -lt $(DEPOSITORS); then echo "TEST FAILED, DEPOSITORS"; exit 1; fi; \
		if test $$(grep 'Connected securely' < check.log.$$N | wc -l) -lt $(CONNECTIONS); then echo "TEST FAILED, CONNECTIONS"; exit 1; fi; \
		echo $$N; \
		N=$$(expr $$N + 1); \
	done

clean:
	rm -rf matrix_test matrix_test.c
