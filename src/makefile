
####################################################################
#                                                                  #
#                                                                  #
#                        Makefile for tlg2u                        #
#                                                                  #
#                                                                  #
#     This makefile can build executables for Linux or Windows     #
#              either for 32 or 64 bit architectures               #
#    in a system where the appropriate compilers are installed.    #
#                                                                  #
#                The native compiler is 64 bit GCC.                #
#        The 32 bit GCC and Mingw32-gcc (32 and 64 bit) are        #
#            cross compilers installed under crossdev.             #
#                                                                  #
#                   Set the variables by editing                   #
#              ARCH=32 or 64 and OS=windows or linux               #
#              or set the same from the command line:              #
#                 e.g.   make ARCH=32 OS=windows                   #
#                                                                  #
#                                                                  #
#                                                                  #
#                                                                  #
####################################################################




ARCH = 64
OS = linux

ifeq ($(OS),windows) 
ifeq ($(ARCH),64)	
	CC = /usr/bin/x86_64-w64-mingw32-gcc
else
	CC = /usr/bin/i686-pc-mingw32-gcc
endif
  EXEC = ../tlg2u.exe
else 
ifeq ($(ARCH),64)	
	CC = gcc
else
  CC=/usr/bin/i686-pc-linux-gnu-gcc	
endif  
	EXEC = ../tlg2u
endif

#---------- Flags Selection -----------#

# Select one option

# Optimized for Intel Core Duo 
CFLAGS = -Wall -march=core2 -Os -pipe -s

# Flags for debugging 
# CFLAGS = -Wall -g

# Flags for generic executable
# CFLAGS = -Wall 
 
#-------- Compile executable ----------#

COMPILE = $(CC) -c $(CFLAGS) -o $@ -c $<


#   Source file list
SRCS = beta_code.c convert.c find_block.c accents.c\
			 get_num.c escape_codes.c id_code.c\
			 output_utf.c printing.c which_sigma.c tlg.c

# Object files to be built
OBJS = $(SRCS:%.c=%.o)	

all: $(EXEC) docs
	-$(RM) $(OBJS)  
	$(info ==================================================)
	$(info =         Compiled for $(ARCH) bit  $(OS)         )
	$(info ==================================================)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $^

%.o: %.c 
	$(COMPILE)

#-------- Build manpage in various formats ---------#

# Need groff for this

DOC_DIR = ../doc
MANPAGE = $(DOC_DIR)/tlg2u.1
GROFF = groff -t -e -mandoc -T

docs: $(MANPAGE)
	export MANPAGER="less -R"
	$(GROFF)ascii $< | col -bx | sed 's/[[:digit:]]\+m//g' > $(DOC_DIR)/tlg2u.txt
	$(GROFF)ps    $<   > $(DOC_DIR)/tlg2u.ps
	$(GROFF)html  $<   > $(DOC_DIR)/tlg2u.html
	$(GROFF)pdf   $<   > $(DOC_DIR)/tlg2u.pdf 

clean:
	-$(RM) $(EXEC) $(OBJS)  
	-$(RM) $(DOC_DIR)/tlg2u.{html,pdf,ps}

ctags:  
	ctags ./*.[c,h]
