CC=gcc
CFLAGS=-Wall #-ansi
LDFLAGS=-L $(LIB) -lcommit
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

ifeq ($(DEBUG),1)
CFLAGS += -g
endif

all: directories TESTVERSION TEST TESTCOMMIT TESTCOMMITEXO4

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}
${LIB}:
	mkdir -p ${LIB}

# =====
# OBJET
# =====
${OBJ}/version.o: ${SRC}/version.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/commit.o: ${SRC}/commit.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/commitEXO4.o: ${SRC}/commitEXO4.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/testVersion.o: ${SRC}/testVersion.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/testCommit.o: ${SRC}/testCommit.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/testCommitEXO4.o: ${SRC}/testCommitEXO4.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/test.o: ${SRC}/test.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libcommit.a : ${OBJ}/version.o ${OBJ}/commit.o ${OBJ}/commitEXO4.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
TESTVERSION: $(LIB)/libcommit.a ${OBJ}/testVersion.o 
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

TESTCOMMIT: $(LIB)/libcommit.a ${OBJ}/testCommit.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

TESTCOMMITEXO4: $(LIB)/libcommit.a ${OBJ}/testCommitEXO4.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

TEST: ${OBJ}/test.o ${OBJ}/version.o
	${CC} -o ${BIN}/$@ $^

.PHONY: all proper clean cleanall

proper:
	rm -f ${INC}/*~ ${SRC}/*~ *~

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -rf ${OBJ} ${BIN} ${LIB}
