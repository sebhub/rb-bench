APP_PIECES = init
APP_PIECES += rbtree-ec
APP_PIECES += rbtree-jffs2
APP_PIECES += rbtree-linux
APP_PIECES += rbtree-rtems-compact-extract
APP_PIECES += rbtree-rtems-compact-insert
APP_PIECES += rbtree-rtems-compact-next
APP_PIECES += rbtree-rtems-extract
APP_PIECES += rbtree-rtems-insert
APP_PIECES += rbtree-rtems-next
APP_PIECES += test-bheap
APP_PIECES += test-rbtree-boost
APP_PIECES += test-rbtree-bsd
APP_PIECES += test-rbtree-bsd-for-rtems
APP_PIECES += test-rbtree-chain
APP_PIECES += test-rbtree-ec
APP_PIECES += test-rbtree-jffs2
APP_PIECES += test-rbtree-linux
APP_PIECES += test-rbtree-llrb
APP_PIECES += test-rbtree-rb
APP_PIECES += test-rbtree-rb-new
APP_PIECES += test-rbtree-rb-old
APP_PIECES += test-rbtree-rtems
APP_PIECES += test-rbtree-rtems-compact
APP_PIECES += test-rbtree-tailq

APP_O_FILES = $(APP_PIECES:%=%.o)
APP_DEP_FILES = $(APP_PIECES:%=%.d)

APP = app.exe

DEPFLAGS = -MT $@ -MD -MP -MF $*.d

CFLAGS += $(DEPFLAGS) -O2 -g -Wall -Wextra

CXXFLAGS += $(CFLAGS)

all: $(APP)

$(APP): $(APP_O_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $(APP)

clean:
	rm -f $(APP) $(APP_O_FILES) $(APP_DEP_FILES)

REPORTS = $(wildcard reports/*.xml)

images: $(REPORTS:%.xml=%.png)

%.png: %.xml
	python plot.py $< $@

%.d:
	touch $@

-include $(APP_DEP_FILES)
