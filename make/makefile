####################################################################################
#
#    File name : makefile
#
#    Description : This file contains automated linux command
#
####################################################################################

CC = gcc
BIN = ../bin/
OBJ = ../obj/
SRC = ../src/
CPATH = -I ../include/
CFLAGS = -c -g -Wall
CLOG =  -DDEBUG -DLOG
GCOV = -ftest-coverage -fprofile-arcs
COV = ../coverage/
final : server client

############################# CLIENT ###############################################

client : $(OBJ)client_functions.o $(OBJ)logger.o $(OBJ)client_main.o
	$(CC) $(OBJ)client_functions.o $(OBJ)logger.o $(OBJ)client_main.o -o client -lpthread
	mv client $(BIN)

$(OBJ)client_main.o : $(SRC)client_main.c
	$(CC) $(CFLAGS) $(SRC)client_main.c $(CPATH) $(CLOG)
	mv client_main.o $(OBJ)

$(OBJ)client_functions.o : $(SRC)client_functions.c
	$(CC) $(CFLAGS) $(SRC)client_functions.c $(CPATH) $(CLOG)
	mv client_functions.o $(OBJ)

############################ COMMON FUNCTIONS #####################################

$(OBJ)logger.o : $(SRC)logger.c
	$(CC) -c -g $(CLOG) $(SRC)logger.c $(CPATH)
	mv logger.o $(OBJ)


############################## SERVER ################################################

server : $(OBJ)server_main.o $(OBJ)server_functions.o $(OBJ)logger.o
	$(CC) $(OBJ)server_main.o $(OBJ)server_functions.o $(OBJ)logger.o -o server -lpthread
	mv server $(BIN)

$(OBJ)server_main.o : $(SRC)server_main.c
	$(CC) $(CFLAGS) $(SRC)server_main.c $(CPATH) $(CLOG)
	mv server_main.o $(OBJ)

$(OBJ)server_functions.o : $(SRC)server_functions.c
	$(CC) $(CFLAGS) $(SRC)server_functions.c $(CPATH) $(CLOG)
	mv server_functions.o $(OBJ)


##################################### CODE COVERAGE #####################################
gcov :
	$(CC) $(GCOV) $(CFLAGS) $(SRC)server_main.c $(SRC)server_functions.c $(SRC)logger.c $(CPATH)
	$(CC) -fprofile-arcs -o $(BIN)server *.o -lpthread
	rm *.o
	cp $(BIN)server $(COV)Se/
	$(CC) $(GCOV) $(CFLAGS) $(SRC)client_main.c $(SRC)client_functions.c $(SRC)logger.c $(CPATH)
	$(CC) -fprofile-arcs -o $(BIN)client *.o -lpthread
	rm *.o
	cp $(BIN)client $(COV)C1/

gcov_exe :
	gcov server_main client_main server_functions client_functions >> $(COV)coverage_report
	mv *.gcda $(COV)GCOV_FILES/
	mv *.gcno $(COV)GCOV_FILES/
	mv *gcov* $(COV)GCOV_FILES/
	mv coverage_report $(COV)

######################################## CLEAN #########################################
clean :
	rm $(BIN)client
	rm $(BIN)server
	rm $(COV)Se/server
	rm $(COV)C1/client
	rm $(OBJ)*.o



val_client:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ../bin/client 10.203.161.16


val_server:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ../bin/server


