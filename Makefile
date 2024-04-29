.PHONY: all clean

all:
	make -C src all -j 16
	cp src/ospf .

clean:
	make -C src clean
	rm ospf
