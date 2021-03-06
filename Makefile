netv2_base:
	rm -rf build
	./netv2_base.py

netv2_pcie:
	rm -rf build
	./netv2_pcie.py

netv2_video:
	rm -rf build
	./netv2_video.py

load:
	./load.py

firmware:
	cd firmware && make clean all

load-firmware:
	litex_term --kernel firmware/firmware.bin COM8

.PHONY: load firmware load-firmware
