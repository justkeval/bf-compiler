#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "strbuf.h"
#include "file_ops.h"

#include "errors.h"
#include "parser.c"
#include "optimizer.c"
#include "compiler.c"
#include "compiler_config.h"
#include "program_options.c"

bool compile_source_to_c(StrBuf* source, StrBuf* c_code, CompilerConfig config, Errors* errors);

int main(int argc, char** argv) {
    ProgramOptions defaults = {
        .output_filepath = "out.c",
        .memory_size = 30000,
        .cell_size = 32
    };
    ProgramOptions opt = parse_options(argc, argv, defaults);

    StrBuf source = {0};
    char* err;
    if (!read_file(opt.source_filepath, &source, &err)) {
        fprintf(stderr, "Error opening file %s: %s", opt.source_filepath, err);
        exit(1);
    }

    CompilerConfig config = {
        .memory_size = opt.memory_size,
        .cell_size = opt.cell_size,
    };
    StrBuf c_code = {0};
    Errors errors = {0};
    if (!compile_source_to_c(&source, &c_code, config, &errors)) {
        for (int i = 0; i < errors.len; i++) {
            Error err = errors.items[i];
            fprintf(stderr, "Error at position %d: %s\n", err.pos, err.message);
        }
        return 1;
    }

    write_file(opt.output_filepath, &c_code);

    free(source.items);
    free(c_code.items);
    free(errors.items);
    return 0;
}

bool compile_source_to_c(StrBuf* source, StrBuf* c_code, CompilerConfig config, Errors* errors) {
    
    Commands cmds = {0};
    if (!source_to_ir(source, &cmds, errors)) {
        return false;
    }

    Commands optimized = {0};
    optimize_ir(&cmds, &optimized);

    ir_to_c(&optimized, c_code, config);

    free(cmds.items);
    free(optimized.items);
    return true;
}