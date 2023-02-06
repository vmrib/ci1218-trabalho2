CC 			:= gcc
CFLAGS 		:= -Wall -g
LDFLAGS		:=
RM			:= rm -rf

BINARY 		:= escalona

SOURCEDIR	:= source
BUILDDIR 	:= build
INCLUDEDIR 	:= include

SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.c')
OBJECTS		:= $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
BUILDSDIRS	:= $(sort $(patsubst $(SOURCEDIR)/%, $(BUILDDIR)/%, $(dir $(SOURCES))))

.PHONY : all clean purge

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c | $(BUILDSDIRS)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(BUILDSDIRS):
	mkdir -p $@

clean:
	$(RM) $(OBJECTS)

purge:
	$(RM) $(OBJECTS) $(BINARY) $(BUILDDIR)

run: $(BINARY)
	./$(BINARY) < teste1.in