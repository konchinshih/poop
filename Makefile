.PHONY: all deps docker clean

all:
	make -C src -j 16
	cp src/ospf .

deps:
	sudo apt install -y docker docker.io docker-buildx docker-compose

docker:
	sudo docker buildx build . --tag ospf:latest
	sudo docker-compose run ospf
	sudo chown -R $(shell id -u):$(shell id -u) . 

clean:
	make -C src clean
	- rm ospf
