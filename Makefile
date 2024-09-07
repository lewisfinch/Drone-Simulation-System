all: routing transit transit_service graph_viewer

routing: build
	cd libs/routing; make

transit: build routing
	cd libs/transit; make

transit_service: build routing transit
	cd apps/transit_service; make

graph_viewer: build routing
	cd apps/graph_viewer; make

build:
	mkdir -p build

clean:
	cd apps/transit_service; make clean
	rm -rf build
