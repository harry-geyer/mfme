PROJECT:=mfme

PROJECT_DIR?=.
BUILD_DIR?=$(PROJECT_DIR)/build
SRC_DIR:=$(PROJECT_DIR)/src
INCLUDE_DIR:=$(SRC_DIR)/include
LIB_DIR:=$(PROJECT_DIR)/libs

MONGOOSE_DIR:=$(LIB_DIR)/mongoose

CFLAGS:=-Wall -Werror -pedantic
CFLAGS+=-I$(INCLUDE_DIR) -I$(MONGOOSE_DIR)
LDFLAGS:=-L$(BUILD_DIR)/libs -lmongoose

BIN_DIR:=$(BUILD_DIR)/bin
OBJ_DIR:=$(BUILD_DIR)/obj

SRCS:=$(shell find $(SRC_DIR) -name "*.c")
OBJS:=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
EXE:=$(BIN_DIR)/$(PROJECT)

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

