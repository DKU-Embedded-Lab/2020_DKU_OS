#
#	DKU Operating System Lab
#	    Lab2 (BST Synchronization)
#	    Student id : 
#	    Student name : 
#
#   Makfeile :
#       - Makefile for lab2 compilation.
#

CC = gcc
INC = -I${CURDIR}/include/
CFLAGS = -g $(INC)
LDFLAGS = -lpthread

OBJS_LAB2 = lab2_bst.o lab2_bst_test.o include/lab2_timeval.o
OBJS_BONUS = lab2_bonus.o lab2_bonus_test.o
OBJS_EXAMPLE = lab2_example.o 

SRCS = $(OBJS_LAB2:.o=.c) $(OBJS_BONUS:.o=.c) $(OBJS_EXAMPLE:.o=.c)

TARGET_LAB2 = lab2_bst
TARGET_BONUS = lab2_bonus
TARGET_EXAMPLE = lab2_example

.SUFFIXES : .c .o

.c.o:
	@echo "Compilingi lab2 BST synchronization $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET_LAB2) : $(OBJS_LAB2)
	$(CC) -o $(TARGET_LAB2) $(OBJS_LAB2) $(LDFLAGS)

$(TARGET_BONUS) : $(OBJS_BONUS)
	$(CC) -o $(TARGET_BONUS) $(OBJS_BONUS) $(LDFLAGS)

$(TARGET_EXAMPLE) : $(OBJS_EXAMPLE)
	$(CC) -o $(TARGET_EXAMPLE) $(OBJS_EXAMPLE) $(LDFLAGS)

all : $(TARGET_LAB2) $(TARGET_BONUS) $(TARGET_EXAMPLE)

dep : 
	gccmaedep $(INC) $(SRCS)

clean :
	@echo "Cleaning lab2 BST synchronization $< ..."
	rm -rf $(OBJS_LAB2) $(TARGET_LAB2) $(OBJS_BONUS) $(TARGET_BONUS) $(OBJS_EXAMPLE) $(TARGET_EXAMPLE)

new :
	$(MAKE) clean
	$(MAKE)
