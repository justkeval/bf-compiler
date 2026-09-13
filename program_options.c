#include <stdlib.h>
#include <stdio.h>

char* shift_args(int *argc, char ***argv) {
    if (*argc <= 0) return NULL;
    char *result = (**argv);
    (*argv)++;
    (*argc)--;
    return result;
}

typedef struct {
    char* program;
    char* source_filepath;
    char* output_filepath;
    int memory_size;
    int cell_size;
} ProgramOptions;

void print_usage(ProgramOptions defaults) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s [options...] <program.b>\n", defaults.program);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -h, --help            Print help\n");
    fprintf(stderr, "  -o, --output <out.c>  Output C file path\n");
    fprintf(stderr, "  -m, --memory <30000>  Memory Size\n");
    fprintf(stderr, "  -c, --cell <8|16|32>  Memory Cell Size\n");
}

ProgramOptions parse_options(int argc, char** argv, ProgramOptions defaults) {
    ProgramOptions res = {0};
    res.program = shift_args(&argc, &argv);

    res.source_filepath = NULL;
    res.output_filepath = defaults.output_filepath;
    res.memory_size = defaults.memory_size;

    while (argc > 0) {
        char* arg = shift_args(&argc, &argv);
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_usage(res);
            exit(0);
        } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--output") == 0) {
            if (res.output_filepath != NULL) {
                fprintf(stderr, "Error: Option %s provided multiple times", arg);
                print_usage(res);
                exit(1);
            }
            char* filepath = shift_args(&argc, &argv);
            if (filepath == NULL) {
                fprintf(stderr, "Error: No output filepath provided after flag %s", arg);
                print_usage(res);
                exit(1);
            }
        } else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--memory") == 0) {
            if (res.memory_size != 0) {
                fprintf(stderr, "Error: Option %s provided multiple times", arg);
                print_usage(res);
                exit(1);
            }
            char* memory = shift_args(&argc, &argv);
            if (memory == NULL) {
                fprintf(stderr, "Error: No memory size provided after flag %s", arg);
                print_usage(res);
                exit(1);
            }
            int mem_size = atoi(memory);
            if (mem_size <= 0) {
                fprintf(stderr, "Error: Invalid memory size provided.");
                print_usage(res);
                exit(1);
            }
        } else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--cell") == 0) {
            if (res.cell_size != 0) {
                fprintf(stderr, "Error: Option %s provided multiple times", arg);
                print_usage(res);
                exit(1);
            }
            char* cell = shift_args(&argc, &argv);
            if (cell == NULL) {
                fprintf(stderr, "Error: No cell size provided after flag %s", arg);
                print_usage(res);
                exit(1);
            }
            int cell_size = atoi(cell);
            if (cell_size != 8 && cell_size != 16 && cell_size != 32) {
                fprintf(stderr, "Error: Invalid cell size provided.");
                print_usage(res);
                exit(1);
            }
        } else if (arg[0] == '-') {
            fprintf(stderr, "Error: Unknown option: %s", arg);
            print_usage(res);
            exit(1);
        } else {
            if (res.source_filepath != NULL) {
                fprintf(stderr, "Error: Too many arguments");
                print_usage(res);
                exit(1);
            }
            res.source_filepath = arg;
        }
    }
    return res;
}