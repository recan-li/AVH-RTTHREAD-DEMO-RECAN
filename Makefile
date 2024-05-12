
AVH_DEMO_PATH       := .
USER_CONFIG_PATH    := config
OUT_PATH            := out
AVH_SIMLIMIT_TIME   := 800
SHELL 				:= /bin/bash
V  					?= 0

all: source clean build run

help:
	@echo "make help 	-> Show this help msg."
	@echo "make source 	-> Install env parameters."
	@echo "make build 	-> Build thie demo."
	@echo "make clean 	-> Clean object files."
	@echo "make run 	-> Run this demo."
	@echo "make debug 	-> Build & run this demo."
	@echo "make all 	-> Source & clean & build & run all together."
	@echo "make pip 	-> Install python tools by pip."

debug: build run

source:
	@echo "Copy and source .bashrc ..."
	@cp -rf .bashrc ~/.bashrc
	@source ~/.bashrc
	@echo "Copy CMSIS-Build-Utils.cmake ..."
	@sudo cp -rf cmake/CMSIS-Build-Utils.cmake /opt/ctools/etc/CMSIS-Build-Utils.cmake
	@echo "All parameters have been installed."

build:
	@echo "Building ..."
	@test -e $(OUT_PATH) || mkdir -p $(OUT_PATH)
	cbuild --packs $(AVH_DEMO_PATH)/avh-cm85.cprj
	@cp -rf Objects/image.axf $(OUT_PATH)/image.axf

run:
	@echo "Running ..."
	/opt/VHT/bin/FVP_Corstone_SSE-310 --stat --simlimit $(AVH_SIMLIMIT_TIME) -f $(AVH_DEMO_PATH)/vht_config.txt $(OUT_PATH)/image.axf

clean:
	@echo "Clean ..."
	rm -rf Objects
	rm -rf $(OUT_PATH)

pip:
	pip install -r $(MQTT_DEMO_PATH)/requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

.PHONY: all source clean build run help
