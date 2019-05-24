OUT_PUT := cpe_54_08

OBJ_DIR := $(shell pwd)

SRC_BUSINESS := $(OBJ_DIR)/Business
SRC_MXDATABASE := $(OBJ_DIR)/MXDatabase
SRC_SOCKET := $(OBJ_DIR)/Socket
SRC_THREAD := $(OBJ_DIR)/Thread
SRC_TOOL := $(OBJ_DIR)/Tool
SRC_WORK := $(OBJ_DIR)/Work

HEAD_INCLUDE := ../include

LIB_PATH := ../lib
LINK_LIB := -lpthread -L$(LIB_PATH) -lbsapi -L$(LIB_PATH) -lMxHandlePool -L$(LIB_PATH) -lServerData -L$(LIB_PATH) -lmxcomlib -L$(LIB_PATH) -lmxdbvar -L$(LIB_PATH) -lchen -luuid

OBJS := $(patsubst $(SRC_BUSINESS)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_BUSINESS)/*cpp)) \
	$(patsubst $(SRC_MXDATABASE)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_MXDATABASE)/*cpp)) \
	$(patsubst $(SRC_SOCKET)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_SOCKET)/*cpp)) \
	$(patsubst $(SRC_THREAD)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_THREAD)/*cpp)) \
	$(patsubst $(SRC_TOOL)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_TOOL)/*cpp)) \
	$(patsubst $(SRC_WORK)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_WORK)/*cpp)) \
	$(OBJ_DIR)/CPE_54_08.o

INCLUDE_DIR = -I$(SRC_BUSINESS) -I$(SRC_MXDATABASE) -I$(SRC_SOCKET) -I$(SRC_THREAD) -I$(SRC_TOOL) -I$(SRC_WORK) -I$(HEAD_INCLUDE)

All:$(OUT_PUT)

$(OUT_PUT):$(OBJS)
	g++ -o $(OUT_PUT) $(OBJS) $(LINK_LIB)

$(patsubst $(SRC_BUSINESS)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_BUSINESS)/*.cpp)):$(OBJ_DIR)/%.o:$(SRC_BUSINESS)/%.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@ -std=c++0x

$(patsubst $(SRC_MXDATABASE)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_MXDATABASE)/*.cpp)):$(OBJ_DIR)/%.o:$(SRC_MXDATABASE)/%.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@ -std=c++0x
	
$(patsubst $(SRC_SOCKET)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_SOCKET)/*.cpp)):$(OBJ_DIR)/%.o:$(SRC_SOCKET)/%.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@
	
$(patsubst $(SRC_THREAD)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_THREAD)/*.cpp)):$(OBJ_DIR)/%.o:$(SRC_THREAD)/%.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@
	
$(patsubst $(SRC_TOOL)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_TOOL)/*.cpp)):$(OBJ_DIR)/%.o:$(SRC_TOOL)/%.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@
	
$(patsubst $(SRC_WORK)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_WORK)/*.cpp)):$(OBJ_DIR)/%.o:$(SRC_WORK)/%.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@ -std=c++0x

$(OBJ_DIR)/CPE_54_08.o:$(OBJ_DIR)/CPE_54_08.cpp
	g++ -c $< $(INCLUDE_DIR) -o $@ -std=c++0x

clean:
	rm -rf $(OUT_PUT) $(OBJ_DIR)/*.o
