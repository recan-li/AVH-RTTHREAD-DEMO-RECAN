
AVH_DEMO_PATH       := .
USER_CONFIG_PATH    := config
OUT_PATH            := out
AVH_SIMLIMIT_TIME   := 80000000
SHELL 				:= /bin/bash
V  					?= 0
TARGET_FILE_SUFFIX 	:= axf

define KILL_EXE
	exe=$(1);\
	pid=`ps -ef | grep -v grep | grep $$exe | awk '{print $$2}'`;\
	if [ "$$pid" != "" ]; then\
		kill -9 $$pid;\
	fi
endef

all:  clean build run

help:
	@echo "make help 	-> Show this help msg."
	@echo "make build 	-> Build thie demo."
	@echo "make clean 	-> Clean object files."
	@echo "make run     -> Run this demo."
	@echo "make debug 	-> Build & run this demo."
	@echo "make all 	-> Source & clean & build & run all together."
	@echo "make stop    -> Stop to run this demo."

debug: build run stop

build:
	@echo "Building ..."
	@test -e $(OUT_PATH) || mkdir -p $(OUT_PATH)
	cbuild --packs $(AVH_DEMO_PATH)/AVH-FVP_MPS2_Cortex-M85.cprj
	@cp -rf $(AVH_DEMO_PATH)/Objects/image.$(TARGET_FILE_SUFFIX) $(OUT_PATH)

run:
	@echo "Running ..."
	@make stop
	python rpc_fs/rpc_fs_server.py &
	python ai/ai_bridge_server.py &
#	@sleep 1
	/opt/VHT/bin/FVP_MPS2_Cortex-M85 --stat --simlimit $(AVH_SIMLIMIT_TIME) -f $(AVH_DEMO_PATH)/vht_config.txt $(OUT_PATH)/image.$(TARGET_FILE_SUFFIX) 
#	@sleep 3
#	@python avh_rtt_debug_client.py debug
#	@make stop

stop:
	@$(call KILL_EXE,/opt/VHT/bin/models/FVP_MPS2_Cortex-M85)
	@$(call KILL_EXE,rpc_fs_server.py)
	@$(call KILL_EXE,ai_bridge_server.py)
	@$(call KILL_EXE,avh_rtt_debug_client.py)
#	ps -ef | grep -v grep | grep /opt/VHT/bin/models/FVP_MPS2_Cortex-M85 | awk '{print $$2}' | xargs kill -9
#	ps -ef | grep -v grep | grep zbar_app/zbar_auth_server.py | awk '{print $$2}' | xargs kill -9

objdump:
	arm-none-eabi-objdump -l -x -d  $(OUT_PATH)/image.axf > $(OUT_PATH)/image.axf.asm

clean:
	@echo "Clean ..."
	rm -rf $(AVH_DEMO_PATH)/Objects/
	rm -rf $(OUT_PATH)

.PHONY: all source clean build run help