PROJECT:=mfme

PROJECT_DIR?=.
BUILD_DIR?=$(PROJECT_DIR)/build
SRC_DIR:=$(PROJECT_DIR)/src
INCLUDE_DIR:=$(SRC_DIR)/include
LIB_DIR:=$(PROJECT_DIR)/libs
RCS_DIR:=$(PROJECT_DIR)/resources

MONGOOSE_DIR:=$(LIB_DIR)/mongoose

CFLAGS:=-Wall -Werror -pedantic
CFLAGS+=-g -Og
CFLAGS+=-I$(INCLUDE_DIR) -I$(MONGOOSE_DIR) -I$(SRC_DIR)/internal
LDFLAGS:=-L$(BUILD_DIR)/libs -lmongoose
LDFLAGS+=-lsqlite3

BIN_DIR:=$(BUILD_DIR)/bin
OBJ_DIR:=$(BUILD_DIR)/obj
WRT_DIR:=$(BUILD_DIR)/webroot

SRCS:=$(shell find $(SRC_DIR) -name "*.c")
OBJS:=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
EXE:=$(BIN_DIR)/$(PROJECT)
RCS:=$(shell find $(RCS_DIR) -type f)
ASSETS:=$(patsubst $(RCS_DIR)/%,$(WRT_DIR)/%,$(RCS))

default: $(EXE)

clean:
	rm -rf $(BUILD_DIR)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS) $(BUILD_DIR)/libs/libmongoose.a
	mkdir -p $(@D)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/libs/mongoose.o: $(MONGOOSE_DIR)/mongoose.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/libs/libmongoose.a: $(BUILD_DIR)/libs/mongoose.o
	mkdir -p $(@D)
	$(AR) rcs $@ $<

$(ASSETS): $(WRT_DIR)/%: $(RCS_DIR)/%
	mkdir -p $(@D)
	cp $< $@

serve: $(EXE) $(ASSETS)
	pushd $(BUILD_DIR) && $(shell realpath --relative-to=$(BUILD_DIR) $(EXE))

db_clean:
	rm -rf $(BUILD_DIR)/files.db $(BUILD_DIR)/files/

debug:
	pushd $(BUILD_DIR) && gdb $(shell realpath --relative-to=$(BUILD_DIR) $(EXE))

.PHONY: clean serve

