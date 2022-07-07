## 可执行程序
EXE_OFFLINE_MODULE1:=bin/offline_module1
EXE_OFFLINE_MODULE2:=bin/offline_module2
EXE_ONLINE_SERVER:=bin/online_server
EXE_ONLINE_CLIENT:=bin/online_client
EXES:= $(EXE_OFFLINE_MODULE1) $(EXE_OFFLINE_MODULE2) $(EXE_ONLINE_SERVER) $(EXE_ONLINE_CLIENT)

## 源文件
SRC_OFFLINE_MODULE1:= $(wildcard src/Offline/Module1/*.cc)
SRC_OFFLINE_MODULE2:= $(wildcard src/Offline/Module2/*.cc)
SRC_ONLINE_SERVER:= $(wildcard src/Online/Server/*.cc)
SRC_ONLINE_CLIENT:= $(wildcard src/Online/Client/*.cc)
SRC:= $(SRC_OFFLINE_MODULE1) $(SRC_OFFLINE_MODULE2) $(SRC_ONLINE_SERVER) $(SRC_ONLINE_CLIENT)

## OBJ文件
OBJ_OFFLINE_MODULE1:= $(patsubst src/Offline/Module1/%.cc,obj/Offline/Module1/%.o,$(SRC_OFFLINE_MODULE1))
OBJ_OFFLINE_MODULE2:= $(patsubst src/Offline/Module2/%.cc,obj/Offline/Module2/%.o,$(SRC_OFFLINE_MODULE2))
OBJ_ONLINE_SERVER:= $(patsubst src/Online/Server/%.cc,obj/Online/Server/%.o,$(SRC_ONLINE_SERVER))
OBJ_ONLINE_CLIENT:= $(patsubst src/Online/Client/%.cc,obj/Online/Client/%.o,$(SRC_ONLINE_CLIENT))
OBJS:= $(OBJ_OFFLINE_MODULE1) $(OBJ_OFFLINE_MODULE2) $(OBJ_ONLINE_SERVER) $(OBJ_ONLINE_CLIENT)

## 编译OfflineModule1
OfflineModule1:$(EXE_OFFLINE_MODULE1)
OfflineModule2:$(EXE_OFFLINE_MODULE2)
OnlineServer:$(EXE_ONLINE_SERVER)
OnlineClient:$(EXE_ONLINE_CLIENT)

## 生成可执行文件
$(EXE_OFFLINE_MODULE1):$(OBJ_OFFLINE_MODULE1)
	g++ $^ -o $@ -Wall -g -std=c++11

$(EXE_OFFLINE_MODULE2):$(OBJ_OFFLINE_MODULE2)
	g++ $^ -o $@ -Wall -g -std=c++11

$(EXE_ONLINE_SERVER):$(OBJ_ONLINE_SERVER)
	g++ $^ -o $@ -Wall -g -lpthread -lredis++ -lhiredis -std=c++17

$(EXE_ONLINE_CLIENT):$(OBJ_ONLINE_CLIENT)
	g++ $^ -o $@ -Wall -g -std=c++11

## 生成.o文件
obj/Offline/Module1/%.o : src/Offline/Module1/%.cc
	g++ -c $^ -o $@ -Wall -g -std=c++11

obj/Offline/Module2/%.o : src/Offline/Module2/%.cc
	g++ -c $^ -o $@ -Wall -g -std=c++11

obj/Online/Server/%.o : src/Online/Server/%.cc
	g++ -c $^ -o $@ -Wall -g -std=c++17

obj/Online/Client/%.o : src/Online/Client/%.cc
	g++ -c $^ -o $@ -Wall -g -std=c++11


.PHONY:clean rebuild
clean:
	$(RM) $(EXES) $(OBJS)
rebuild:clean $(EXES)
