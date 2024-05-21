#include "sylar/application.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    // 设置时区（TZ 环境变量）为使用系统的本地时间文件（/etc/localtime）。
    // 这样设置后，程序在运行时会根据本地时间文件来确定系统的时区设置。
    setenv("TZ", ":/etc/localtime", 1);
    // 根据环境变量 TZ 的设置更新当前进程的时区信息
    tzset();
    srand(time(0));
    sylar::Application app;
    if(app.init(argc, argv)) {
        return app.run();
    }
    return 0;
}
