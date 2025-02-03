ARD = arduino-cli
FLAGS = --verbose
BUILD = ../build
MAIN = main
BOARD = esp32:esp32:esp32
CONFIG = sketch.yaml

.PHONY: monitor compile upload clean

compile: $(MAIN)
	mkdir -p $(BUILD)
	$(ARD) compile $(MAIN) --output-dir $(BUILD) $(FLAGS) --config-file $(CONFIG)

upload:
	$(ARD) upload --config-file $(CONFIG) --build-path $(BUILD) $(FLAGS)
	$(ARD) monitor -c 115200 $(FLAGS)

monitor:
	$(ARD) monitor -c 115200 $(FLAGS)
	
