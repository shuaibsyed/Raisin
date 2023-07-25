/* Raisin by Shuaib Syed */

/* I pledge on my honor that I have not given or received any
unauthorized assitance on this assignment. */

/* This program simulates a fictional simple processor called MAD Raisin.
This CPU has a very limited set of instructions, is able to to access a
small amount of memory adn is designed to for embedded systems. */

#include "raisin.h"
#include <stdio.h>

typedef unsigned int Opcode;

/* Function protoypes */
void print_opcode(Opcode opcode);
void print_register(unsigned int bits);
unsigned int read_bits(unsigned int byte, int msb, int lsb);
int op_uses_register(unsigned int opcode, int register);
void print_mem(Opcode opcode, unsigned int bits);

/* This function reads the bits of binary number that is being passed into
 * the function. It does this by shifting the bits using bitwise operators.
 */
unsigned int read_bits(unsigned int byte, int right, int left) {
    unsigned int result;
    result = (byte >> left) & ~(~0 << (right - left + 1));
    return result;
}

/* This function prints out the opcode that is being passed into it. */
void print_opcode(Opcode opcode) {
    switch (opcode) {
    case HALT:
        printf("halt");
        break;
    case SYSCALL:
        printf("syscall");
        break;
    case ADD:
        printf("add");
        break;
    case SUB:
        printf("sub");
        break;
    case MUL:
        printf("mul");
        break;
    case QUOT:
        printf("quot");
        break;
    case MOD:
        printf("mod");
        break;
    case AND:
        printf("and");
        break;
    case OR:
        printf("or");
        break;
    case NEG:
        printf("neg");
        break;
    case NOT:
        printf("not");
        break;
    case EQL:
        printf("eql");
        break;
    case NEQ:
        printf("neq");
        break;
    case LT:
        printf("lt");
        break;
    case LE:
        printf("le");
        break;
    case GT:
        printf("gt");
        break;
    case GE:
        printf("ge");
        break;
    case MOVE:
        printf("move");
        break;
    case LW:
        printf("lw");
        break;
    case SW:
        printf("sw");
        break;
    case LI:
        printf("li");
        break;
    case BRANCH:
        printf("branch");
        break;
    default:
        break;
    }
}

/* This function prints out the register number that is being used by the
 * CPU for a given instruction. */
void print_register(unsigned int bits) {
    switch (bits) {
    case R0:
        printf("R0");
        break;
    case R1:
        printf("R1");
        break;
    case R2:
        printf("R2");
        break;
    case R3:
        printf("R3");
        break;
    case R4:
        printf("R4");
        break;
    case R5:
        printf("R5");
        break;
    case R6:
        printf("R6");
        break;
    case R7:
        printf("R7");
        break;
    case R8:
        printf("R8");
        break;
    case R9:
        printf("R9");
        break;
    case R10:
        printf("R10");
        break;
    case R11:
        printf("R11");
        break;
    case R12:
        printf("R12");
        break;
    case R13:
        printf("R13");
        break;
    case R14:
        printf("R14");
        break;
    case R15:
        printf("R15");
        break;
    case R16:
        printf("R16");
        break;
    case R17:
        printf("R17");
        break;
    default:
        break;
    }
}

/* This function prints out the memory address if the instruction needs the
 * memory to be used. */
void print_mem(Opcode opcode, unsigned int bits) {
    switch (opcode) {
    case LW:
    case SW:
    case BRANCH:
        printf("%04d", bits);
        break;
    case LI:
        printf("%d", bits);
        break;
    default:
        break;
    }
}

/* This function returns a 0 if the opcode is one that doesn't use a
 * register and a 1 if the opcode does indeed use a register */
int op_uses_register(unsigned int opcode, int register_index) {
    if (opcode == HALT || opcode == SYSCALL || opcode == BRANCH) {
        return 0;
    }
    if (register_index == 1) {
        return 1;
    }
    if (register_index == 2 && opcode != NEG && opcode != NOT &&
        opcode != LW && opcode != SW && opcode != LI && opcode != BRANCH) {
        return 1;
    }
    return 0;
}

/* This function takes an instruction that is passed in and decpihers it
 * into an opcode which then will be printed out along with what registers
 * and/or memory address it uses. It returns a 1 if the opcode is valid and
 * prints out the coressponding instrcution. It returns a 0 and prints
 * nothing if the instruction is invalid. */
unsigned short print_instruction(unsigned int instruction) {
    Opcode op = read_bits(instruction, 32, 27);
    unsigned int register1 = read_bits(instruction, 26, 22);
    unsigned int register2 = read_bits(instruction, 21, 17);
    unsigned int addr_or_imm = read_bits(instruction, 16, 0);

    /* Checking if opcode is valid */
    if (op > 21 || op < 0) {
        return 0;
    }

    print_opcode(op);
    if (op == HALT || op == SYSCALL) {
        printf("\n");
        return 1;
    }

    /* Checks if the opcode uses register 1 and if the register number is
     * valid. */
    if (op_uses_register(op, 1) == 1 &&
        (register1 < 0 || register1 > 17)) {
        return 0;
    }

    /* Prints out the register number */
    if (op_uses_register(op, 1)) {
        printf(" ");
        print_register(register1);
    }

    /* Checks if the opcode uses register 2 and if the register number is
     * valid. */
    if (op_uses_register(op, 2) == 1 &&
        (register2 < 0 || register2 > 17)) {
        return 0;
    }

    /* Prints out the register number */
    if (op_uses_register(op, 2)) {
        printf(" ");
        print_register(register2);
    }

    /* Checks if the opcode is one that uses memory and prints it out if it
     * does. */
    if (op == LW || op == SW || op == LI || op == BRANCH) {
        printf(" ");
        print_mem(op, addr_or_imm);
    }

    printf("\n");
    return 1;
}

/* This function simulates the operating system loading a machine language
 * program into the CPU's memory so that it can be excuted. It copies the
 * contents of an array into the memory beginning at a starting address
 * (start_addr). This data that is being copied in contains words which the
 * number of is given by pgm_size. These words contain instrcutions but not
 * all of them are instructions so data_segment_size indicates how many of
 * the words are actual data rather than instrutctions. This returns 1 if
 * the parameters are valid and loads the program into memory. It returns 0
 * and doesn't do anything if the parameters are invalid. */
int load_program(unsigned int memory[], const unsigned int program[],
                 unsigned int start_addr, unsigned short pgm_size,
                 unsigned short data_segment_size) {
    /* Array index counters */
    int mem_index = 0;
    int program_index = 0;

    /* Checks each of the parameters to see if they are valid. */
    if (start_addr > NUM_WORDS) {
        return 0;
    } else if (start_addr % 4 != 0) {
        return 0;
    } else if (pgm_size > 65536) {
        return 0;
    } else if (start_addr + pgm_size > NUM_WORDS) {
        return 0;
    } else if (pgm_size - data_segment_size == 0) {
        return 0;
    }

    /* Copies data from the 2 different arrays. */
    for (mem_index = start_addr / 4; mem_index < start_addr / 4 + pgm_size;
         mem_index++, program_index++) {
        memory[mem_index] = program[program_index];
    }
    return 1;
}

/* This function is a disassembler that converts machine language into
 * assembly language. It takes a program that has been stored somewhere in
 * memory and prints out the output of the program using print_instruction
 * from above. The starting address of the program is given by start_addr
 * and the size of the program and how much of it is data rather than
 * instructions is given by pgm_size and data_segement_size respectively.
 * This will return 1 if all the parameters are valid and print out. It
 * will return 0 and do nothing if any one of the parameters are invalid.
 */
unsigned short disassemble(const unsigned int memory[],
                           unsigned int start_addr, unsigned int pgm_size,
                           unsigned int data_segment_size) {

    int mem_index = 0; /* Array index for memory*/
    int counter = 0; /* Counter for the number of instructions printed. */
    /* Number of instrcutions that need to be printed out. */
    int num_instr = pgm_size - data_segment_size;

    /* Checks if the parameters are valid. */
    if (pgm_size > 65536) {
        return 0;
    } else if (start_addr + pgm_size > NUM_WORDS) {
        return 0;
    } else if (pgm_size - data_segment_size == 0) {
        return 0;
    }

    /* Prints out the instructions until the counter runs out. After that
     * it prints out the data. */
    for (mem_index = start_addr / 4;
         mem_index < (start_addr / 4) + pgm_size; mem_index++, counter++) {
        if (counter < num_instr) {
            if (print_instruction(memory[mem_index]) == 0) {
                return 0;
            }
        } else {
            printf("%08x\n", memory[mem_index]);
        }
    }
    return 1;
}