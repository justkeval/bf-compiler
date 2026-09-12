#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "strbuf.h"
#include "file_ops.h"

#include "errors.h"
#include "parser.c"
#include "optimizer.c"
#include "compiler.c"

bool compile_source_to_c(StrBuf* source, StrBuf* c_code, Errors* errors);

int main(int argc, char** argv) {
    char* program = argv[0];

    if (argc < 2) {
        fprintf(stderr, "Error: No source file provided.\n");
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <program.b>\n", program);
        return 1;
    }

    char* source_filepath = argv[1];
    
    StrBuf source = {0};
    read_file(source_filepath, &source);

    StrBuf c_code = {0};
    Errors errors = {0};    
    if (!compile_source_to_c(&source, &c_code, &errors)) {
        for (int i = 0; i < errors.len; i++) {
            Error err = errors.items[i];
            fprintf(stderr, "Error at position %d: %s\n", err.pos, err.message);
        }
        return 1;
    }

    const char* output_filepath = "out.c";
    write_file(output_filepath, &c_code);

    free(source.items);
    free(c_code.items);
    free(errors.items);
    return 0;
}

bool compile_source_to_c(StrBuf* source, StrBuf* c_code, Errors* errors) {
    
    Commands cmds = {0};
    if (!source_to_ir(source, &cmds, errors)) {
        return false;
    }

    Commands optimized = {0};
    optimize_ir(&cmds, &optimized);

    ir_to_c(&optimized, c_code);

    free(cmds.items);
    free(optimized.items);
    return true;
}