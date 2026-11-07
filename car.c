#include <stdio.h>

#define MAX_CMD 5          /* 最大指令次数 */

/* ---------- 1. 结构体定义 ---------- */
typedef struct {
    int targetDistance;              /* 目标总距离，固定 100 */
    int currentDistance;             /* 当前已行驶总距离 */
    int commandCount;                /* 已发送指令次数 */
    int maxCommands;                 /* 允许的最大指令次数，5 */
    int history[MAX_CMD];            /* 每次选择的挡位记录 */
} CruiseSystem;

/* ---------- 2. 函数声明 ---------- */
void initSystem(CruiseSystem *sys);
void runCar(CruiseSystem *sys);
void showHistory(CruiseSystem sys);   /* 注意：题目要求按值传递 */

int main(void)
{
    CruiseSystem sys;

    initSystem(&sys);
    printf("=== 智能小车定点巡航系统 ===\n");
    printf("目标距离：%d 单位，最多可发送 %d 次指令\n", sys.targetDistance, sys.maxCommands);
    printf("挡位说明：1-安全(15)  2-普通(25)  3-狂暴(40-第几次指令)\n\n");

    runCar(&sys);

    showHistory(sys);           /* 无论成功与否都要调用 */
    return 0;
}

/* 初始化系统状态 */
void initSystem(CruiseSystem *sys)
{
    int i;
    sys->targetDistance  = 100;
    sys->currentDistance = 0;
    sys->commandCount    = 0;
    sys->maxCommands     = MAX_CMD;
    for (i = 0; i < MAX_CMD; i++)
        sys->history[i] = 0;
}

/* ---------- 3. 核心运行逻辑 ---------- */
void runCar(CruiseSystem *sys)
{
    int gear, move;

    while (sys->commandCount < sys->maxCommands) {

        /* 特殊提示：第 4 次指令选择前 */
        if (sys->commandCount + 1 == 4)
            printf("电机过热预警：连续高负荷运行，注意挡位选择！\n");

        printf("请输入第 %d 次指令的挡位(1/2/3)：", sys->commandCount + 1);

        if (scanf("%d", &gear) != 1) {          /* 输入非数字，清空缓冲区重来 */
            while (getchar() != '\n') ;
            printf("输入无效，请输入 1、2 或 3。\n");
            continue;
        }
        if (gear < 1 || gear > 3) {             /* 非法挡位不计入指令次数 */
            printf("输入无效，请输入 1、2 或 3。\n");
            continue;
        }

        sys->commandCount++;                        /* 这是第几次指令 */
        sys->history[sys->commandCount - 1] = gear;

        switch (gear) {
            case 1:  move = 15; break;              /* 安全模式 */
            case 2:  move = 25; break;              /* 普通模式 */
            default: move = 40 - sys->commandCount; /* 狂暴模式：40 - 第几次指令 */
                     break;
        }

        sys->currentDistance += move;
        printf(">> 第 %d 次指令：挡位 %d，前进 %d 单位，累计 %d/%d\n",
               sys->commandCount, gear, move, sys->currentDistance, sys->targetDistance);

        /* 特殊环境提示：执行完第 2 次指令后进度落后 */
        if (sys->commandCount == 2 && sys->currentDistance < 40)
            printf("警告：进度落后，建议提升挡位！\n");

        /* 成功：5 次内（含第 5 次）达到 100，立刻结束 */
        if (sys->currentDistance >= sys->targetDistance) {
            printf("巡航成功！顺利抵达终点，共耗费指令%d次。\n", sys->commandCount);
            return;
        }
        printf("\n");
    }

    /* 指令用尽仍未到达 */
    printf("巡航失败！电量耗尽，距离终点还差%d个单位。\n",
           sys->targetDistance - sys->currentDistance);
}

/* ---------- 4. 显示历史挡位记录 ---------- */
void showHistory(CruiseSystem sys)
{
    int i;
    printf("小车历史挡位记录为：");
    for (i = 0; i < sys.commandCount; i++)
        printf("%d ", sys.history[i]);
    printf("\n");
}
