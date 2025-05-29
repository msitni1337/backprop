CXX             =   c++

INCLUDES		= 	includes

CXXFLAGS        =   -Wall -Wextra -Werror -MMD -MP -I$(INCLUDES) -g3 -fsanitize=address

#Binary files:
BIN_DIR			=	__bin__
LIB_NAME        =   ${BIN_DIR}/libbackprop.a
NAME        	=   $(LIB_NAME)

#Sources files:
SRC_DIR			=	src
LIB_SRC_DIR		=	${SRC_DIR}/lib
LIB_SRC			= 	$(shell find $(LIB_SRC_DIR) -type f -name '*.cpp')

#Build files:
BUILD_DIR   	=   __build__
LIB_BUILD_DIR   =   ${BUILD_DIR}/lib

#Build files:
LIB_OBJ         =  	$(LIB_SRC:$(LIB_SRC_DIR)/%.cpp=$(LIB_BUILD_DIR)/%.o)
OBJ				= 	${LIB_OBJ}
DEP				=	$(OBJ:.o=.d)

#Tools:
RM              =   rm -rf
AR              =   ar rcs

all: $(NAME)

#library build:
$(LIB_BUILD_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_NAME): $(LIB_OBJ)
	@mkdir -p $(dir $@)
	${AR} ${LIB_NAME} $(LIB_OBJ)

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(BIN_DIR)

re: fclean all

-include : $(DEP)
.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
