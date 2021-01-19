CC = gcc
H = processFile.h types.h 
C = main.c processFile.c 
EXEC = a6

build: ${C} ${H}
	${CC} ${C} -pthread -o ${EXEC}

test:
	@echo "Running a6 on assignment_6_input_file.txt"
	./${EXEC} assignment_6_input_file.txt

clean:
	@echo "Cleaning up..."
	rm -f ${EXEC} assignment_6_output_file.txt

