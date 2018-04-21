SRCDIR := "./src"
OBJDIR := "./obj"

SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJ_FILES := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LD_FLAGS := -lSDL2 -lSDL2_mixer -lm
CC_FLAGS := -Wall

game: $(OBJ_FILES)
	 gcc $(CC_FLAGS) -o $@ $^ $(LD_FLAGS)
