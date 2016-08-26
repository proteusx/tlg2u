#--------------------------------------------
# Makefile for tlg2u
#--------------------------------------------

                                                   
 #-----------------------------------------------------#
 #                                                     #
 #     If cross compiling for windows under linux      #
 #       or compiling in an actual Micro$oft OS        #
 #     you need Mingw32-gcc and set the variable:      #
 #                    OS = windows                     #
 #    If compiling for 32 bit you also need to set:    #
 #          CC = /usr/bin/i686-pc-mingw32-gcc          #
 #                                                     #
 #-----------------------------------------------------#


OS = linux

ifeq ($(OS),windows) 
	CC = /usr/bin/x86_64-w64-mingw32-gcc
  EXEC = ../tlg2u.exe
else 
	CC = gcc
	EXEC = ../tlg2u
endif

#-----------------------------------#
#                                   #
#    Micro$oft does not have rm     #
#    So change the line below to:   #
#             RM = del              #
#                                   #
#-----------------------------------#
RM = rm -f


#---------- Flags Selection -----------#

# Select one option

# Optimized for Intel Core Duo 
CFLAGS = -Wall -march=core2 -Os -pipe -s

# Flags for debugging 
# CFLAGS = -Wall -g

# Flags for generic executable
# CFLAGS = -Wall 
 
#--------------------------------------#

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
	$(info =              Compiled for $(OS)                 )
	$(info ==================================================)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $^

%.o: %.c 
	$(COMPILE)

# Build manpage in various formats

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
