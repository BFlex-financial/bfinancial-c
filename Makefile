# Makefile for BFinancial - C library - bflib
# Written by Renato Fermi <repiazza@gmail.com> in August 2024

CC    = gcc

SRC_PATH = src
INCLUDE_PATH = include
MINGW_INCLUDE_PATH = D:\msys64\mingw64\include
MINGW_LIB_PATH = D:\msys64\mingw64\lib
MOCKS_PATH = tests
CCOPT = -Wall -Wextra

ifdef _WIN32
    CCOPT += -mwindows -D_WIN32
    LIBS = -L$(MINGW_LIB_PATH) -lmingw32 -lmsvcrt -lcurl -lcjson
else
    CCOPT += -DLINUX
    LIBS = -lcurl -lcjson
endif

INCDIR = -I.
INCDIR += -I$(INCLUDE_PATH)/
INCDIR += -I$(MINGW_INCLUDE_PATH)/

OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

DEBUG_ADD_FLAGS = -O2
ifdef DEBUG
    DEBUG_ADD_FLAGS = -g -ggdb
endif

BFLIB_NAME = bflib
BFLIB_SHARED = $(LIB_DIR)/$(BFLIB_NAME).so
BFLIB_DLL = $(LIB_DIR)/$(BFLIB_NAME).dll
BFLIB_OBJS = $(OBJ_DIR)/bflib.o

SPAWN_MOCK_EXEC = spawn_mock
MEDIA_MOCK_EXEC = media_mock

SPAWN_MOCK_OBJS = $(OBJ_DIR)/spawn_mock.o
MEDIA_MOCK_OBJS = $(OBJ_DIR)/media_mock.o

# Targets
all: clean directories $(BFLIB_NAME) mocks

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

$(BFLIB_NAME): $(BFLIB_OBJS)
ifdef _WIN32
	$(CC) -shared -o $(BFLIB_DLL) $(BFLIB_OBJS) $(LIBS)
else
	$(CC) -shared -fPIC -o $(BFLIB_SHARED) $(BFLIB_OBJS) $(LIBS)
endif

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

# Mocks compilation
mocks: $(BIN_DIR)/$(SPAWN_MOCK_EXEC) $(BIN_DIR)/$(MEDIA_MOCK_EXEC)

$(BIN_DIR)/$(SPAWN_MOCK_EXEC): $(SPAWN_MOCK_OBJS) $(BFLIB_NAME)
ifdef _WIN32
	$(CC) -o $@ $(SPAWN_MOCK_OBJS) -L$(LIB_DIR) -lbflib $(LIBS) $(INCDIR)
else
	$(CC) -o $@ $(SPAWN_MOCK_OBJS) -L$(LIB_DIR) -lbflib $(LIBS) $(INCDIR) -Wl,-rpath,$(LIB_DIR)
endif

$(BIN_DIR)/$(MEDIA_MOCK_EXEC): $(MEDIA_MOCK_OBJS) $(BFLIB_NAME)
ifdef _WIN32
	$(CC) -o $@ $(MEDIA_MOCK_OBJS) -L$(LIB_DIR) -lbflib $(LIBS) $(INCDIR)
else
	$(CC) -o $@ $(MEDIA_MOCK_OBJS) -L$(LIB_DIR) -lbflib $(LIBS) $(INCDIR) -Wl,-rpath,$(LIB_DIR)
endif

$(OBJ_DIR)/spawn_mock.o: $(MOCKS_PATH)/spawn_mock.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

$(OBJ_DIR)/media_mock.o: $(MOCKS_PATH)/media_mock.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

.PHONY: all clean directories $(BFLIB_NAME) mocks
