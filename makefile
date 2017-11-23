OBJS = $(patsubst %.c,%.o,$(shell find -name "*.c"))
LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_primitives -lzip
OUT = ./gcview
CFLAGS = -O3 -Wall -fgnu89-inline -std=c99 
PREFIX = /usr/bin

# link
$(OUT): $(OBJS)
	@echo "[INFO] linking"
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(OUT)

install: $(OUT)
	install $(OUT) $(PREFIX)

debug: CFLAGS += -ggdb
debug: $(OUT)

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
%.o: %.c
	@echo "[INFO] generating deps for $*.o"
	gcc -MM $(CFLAGS) $*.c > $*.d
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	@echo

# remove compilation products
clean:
	$(RM) $(OBJS) $%.d

