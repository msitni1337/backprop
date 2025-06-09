CXX             =   c++

INCLUDES		= 	includes/lib

CXXFLAGS        =   -Wall -Wextra -Werror -MMD -MP -I$(INCLUDES) -g3 -fsanitize=address

#Binary files:
BIN_DIR			=	__bin__
LIB_NAME        =   ${BIN_DIR}/libbackprop.a
POC_NAME        =   ${BIN_DIR}/poc_model
NAME        	=   $(LIB_NAME) $(POC_NAME)

#Sources files:
SRC_DIR			=	src
LIB_SRC_DIR		=	${SRC_DIR}/lib
LIB_SRC			= 	$(shell find $(LIB_SRC_DIR) -type f -name '*.cpp')
LIB_TPP			= 	$(shell find $(INCLUDES) -type f -name '*.tpp')
POC_SRC_DIR		=	${SRC_DIR}/models/POC
POC_SRC			= 	$(shell find $(POC_SRC_DIR) -type f -name '*.cpp')

#Build files:
BUILD_DIR   	=   __build__
LIB_BUILD_DIR   =   ${BUILD_DIR}/lib
POC_BUILD_DIR   =   ${BUILD_DIR}/models/POC

#Build files:
LIB_OBJ         =  	$(LIB_SRC:$(LIB_SRC_DIR)/%.cpp=$(LIB_BUILD_DIR)/%.o)
POC_OBJ         =  	$(POC_SRC:$(POC_SRC_DIR)/%.cpp=$(POC_BUILD_DIR)/%.o)
OBJ				= 	${LIB_OBJ} ${LIB_OBJ}
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

#POC build:
$(POC_BUILD_DIR)/%.o: $(POC_SRC_DIR)/%.cpp ${LIB_TPP}
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(POC_NAME): $(LIB_NAME) $(POC_OBJ) ${LIB_TPP}
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIB_NAME) $(POC_OBJ) -o $(POC_NAME)

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(BIN_DIR)

re: fclean all

-include : $(DEP)
.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
