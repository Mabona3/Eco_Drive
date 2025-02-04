ARD = arduino-cli
BUILD = build
FLAGS = --verbose
MAIN = main
BOARD = esp32:esp32:esp32
CONFIG = sketch.yaml

.PHONY: monitor compile upload clean

compile: $(MAIN)
	mkdir -p $(BUILD)
	$(ARD) compile $(MAIN) --output-dir $(BUILD) $(FLAGS) --config-file $(CONFIG)

upload: $(BUILD)
	make -C main upload

monitor:
	make -C main monitor

clean:
	rm -rf $(BUILD)

