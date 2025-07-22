.SILENT:

NAME		=	ft_ls

SRCDIR		=	src
SRCS		=	$(SRCDIR)/main.c	$(SRCDIR)/parser.c	$(SRCDIR)/utils.c	$(SRCDIR)/env.c	$(SRCDIR)/error.c \
				$(SRCDIR)/list.c	$(SRCDIR)/memory.c	$(SRCDIR)/sort.c	$(SRCDIR)/display.c

OBJDIR		=	.obj
OBJS		=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

DEPDIR		=	.dep
DEPS		=	$(patsubst$(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRCS))

CC			=	gcc
CC_FLAGS	=	-Wall -Wextra -Werror -g

LIBFTDIR	= ft_libft
LIBFT		= $(LIBFTDIR)/libft.a

PRINTFDIR	= ft_printf
PRINTF		= $(PRINTFDIR)/ft_printf.a

$(OBJDIR)/%.o :	$(SRCDIR)/%.c
	echo -n "Compiling " $< " ... "
	$(CC) $(CC_FLAGS) -c $< -o $@
	$(CC) $(CC_FLAGS) -MM $< | sed 's,\($*\)\.o[ :]*,$(DEPDIR)/\1.o $@ : ,g' > $(DEPDIR)/$*.d
	echo -e '\x1b[32m>>> OK <<<\x1b[37m'

all:	$(LIBFT) $(PRINTF) $(OBJDIR) $(DEPDIR) $(OBJS) $(NAME)

$(NAME): $(OBJDIR) $(DEPDIR) $(OBJS)
	echo -e '\033[0;34mObjects compilation: \x1b[32m>>> OK <<<\x1b[37m'
	echo ""
	echo -n "Compiling " $(NAME) " ... "
	$(CC) $(CC_FLAGS) $(OBJS) $(PRINTF) $(LIBFT) -lm -o $(NAME)
	echo -e '\x1b[32m>>> OK <<<\x1b[37m'
	echo -en '\033[0;34mExecutable '
	echo -n  $(NAME)
	echo -e  ' compiled: \x1b[32m>>> OK <<<\x1b[37m'
	echo ""

$(LIBFT):
	echo -e '\033[0;34mLibft compilation ... \x1b[37m'
	make -C $(LIBFTDIR)
	echo -e  'Libft compiled: \x1b[32m>>> OK <<<\x1b[37m'
	echo ""
	
$(PRINTF):
	echo -e '\033[0;34mPrintf compilation ... \x1b[37m'
	make -C $(PRINTFDIR)
	echo -e  'Printf compiled: \x1b[32m>>> OK <<<\x1b[37m'
	echo ""

$(OBJDIR):
	mkdir -v $(OBJDIR)

$(DEPDIR):
	mkdir -v $(DEPDIR)

clean:
	rm -rfv $(OBJDIR) $(DEPDIR)
	make -C $(LIBFTDIR) clean
	make -C $(PRINTFDIR) clean
	echo -e '\033[0;34mObject and Dependencies files removed: \x1b[32m>>> OK <<<\x1b[37m'

fclean: clean
	rm -fv $(NAME) $(NAME).d
	make -C $(LIBFTDIR) fclean
	make -C $(PRINTFDIR) fclean
	echo -e '\033[0;34mExecutable file removed: \x1b[32m>>> OK <<<\x1b[37m'

-include $(DEPS)

re: fclean all

.PHONY: all clean fclean