#include <stdio.h>
#include <hiredis/hiredis.h>
#include <hiredis/cluster.h>

int main() {
    // 创建集群上下文
    redisClusterContext *cc = redisClusterContextInit();
    if (cc == NULL) {
        printf("Failed to initialize Redis cluster context\n");
        return 1;
    }

    // 添加 Redis 集群节点
    redisClusterSetOptionAddNodes(cc, "127.0.0.1:7000");

    // 连接 Redis 集群
    if (redisClusterConnect2(cc) != REDIS_OK) {
        printf("Failed to connect to Redis cluster\n");
        redisClusterFree(cc);
        return 1;
    }

    // 执行 Redis 命令
    redisReply *reply = redisClusterCommand(cc, "GET mykey");
    if (reply == NULL) {
        printf("Error executing Redis command\n");
        redisClusterFree(cc);
        return 1;
    }

    // 处理命令结果
    printf("GET mykey: %s\n", reply->str);

    // 释放资源
    freeReplyObject(reply);
    redisClusterFree(cc);
    return 0;
}
