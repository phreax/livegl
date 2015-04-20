MAIN=livegl
CP=cp -f

all: $(MAIN)

$(MAIN):
	make -C src/
	$(CP) src/$(MAIN) .

clean:
	rm $(MAIN)
