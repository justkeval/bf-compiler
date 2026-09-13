#include "lib/da_append.h"
#include "ir.h"

void optimize_ir(Commands *cmds, Commands *opt) {
    if (cmds->len == 0)
        return;
    da_append(opt, cmds->items[0]);
    for (int i = 1; i < cmds->len; i++) {
        Command last = opt->items[--opt->len];
        Command cur = cmds->items[i];
        if ((last.kind == cur.kind) &&
            (last.kind == CMDK_INC || last.kind == CMDK_DEC ||
             last.kind == CMDK_LEFT || last.kind == CMDK_RIGHT)) {
            Command cmd = {last.kind, last.value + cur.value};
            da_append(opt, cmd);
        } else if ((last.kind == CMDK_INC && cur.kind == CMDK_DEC) ||
                   (last.kind == CMDK_DEC && cur.kind == CMDK_INC) ||
                   (last.kind == CMDK_LEFT && cur.kind == CMDK_RIGHT) ||
                   (last.kind == CMDK_RIGHT && cur.kind == CMDK_LEFT)) {
            bool isMove = last.kind == CMDK_LEFT || last.kind == CMDK_RIGHT;
            CommandKind posKind = isMove ? CMDK_RIGHT : CMDK_INC;
            CommandKind negKind = isMove ? CMDK_LEFT : CMDK_DEC;
            int posVal = last.kind == posKind ? last.value : cur.value;
            int negVal = last.kind == negKind ? last.value : cur.value;
            int diff = posVal - negVal;
            if (diff > 0) {
                Command cmd = {posKind, diff};
                da_append(opt, cmd);
            } else if (diff < 0) {
                Command cmd = {negKind, -diff};
                da_append(opt, cmd);
            }
        } else if (last.kind == CMDK_LOOPSTART && cur.kind == CMDK_LOOPEND) {
            // skip and do nothing
        } else {
            da_append(opt, last);
            da_append(opt, cur);
        }
    }
}
