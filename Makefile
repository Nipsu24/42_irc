# ******************************************************************************************************************************************** #
#                                                                                                                                              #
#         ::::::::::: :::::::::   ::::::::                 :::                                                                                 #
#            :+:     :+:    :+: :+:    :+:               :+:                                                                                   #
#           +:+     +:+    +:+ +:+                     +:+                                                                                     #
#          +#+     +#++:++#:  +#+                    +#+                                                                                       #
#         +#+     +#+    +#+ +#+                   +#+                                                                                         #
#        #+#     #+#    #+# #+#    #+#           #+#                                                                                           #
#   ########### ###    ###  ########           ###                                                                                             #
#                                                          :::       ::: :::::::::: :::::::::   ::::::::  :::::::::: :::::::::  :::     :::    #
#                                                         :+:       :+: :+:        :+:    :+: :+:    :+: :+:        :+:    :+: :+:     :+:     #
#                                                        +:+       +:+ +:+        +:+    +:+ +:+        +:+        +:+    +:+ +:+     +:+      #
#                                                       +#+  +:+  +#+ +#++:++#   +#++:++#+  +#++:++#++ +#++:++#   +#++:++#:  +#+     +:+       #
#   By: Timo Saari<tsaari@student.hive.fi>,            +#+ +#+#+ +#+ +#+        +#+    +#+        +#+ +#+        +#+    +#+  +#+   +#+         #
#       Matti Rinkinen<mrinkine@student.hive.fi>,      #+#+# #+#+#  #+#        #+#    #+# #+#    #+# #+#        #+#    #+#   #+#+#+#           #
#       Marius Meier<mmeier@student.hive.fi>           ###   ###   ########## #########   ########  ########## ###    ###     ###              #
#                                                                                                                                              #
# ******************************************************************************************************************************************** #

NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++11

SRC_DIR = ./src
OBJ_DIR = obj

FILES = main.cpp Server.cpp Client.cpp runServer.cpp MessageHandler.cpp Channel.cpp

OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(FILES:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ_FILES)
	@echo "\033[32m ircserv has been built successfully!\033[0m"

fsanitize:
	$(CC) -o $(NAME) $(FILES) -g -fsanitize=address -static-libsan

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
