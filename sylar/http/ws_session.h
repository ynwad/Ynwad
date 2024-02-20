#ifndef __SYLAR_HTTP_WS_SESSION_H__
#define __SYLAR_HTTP_WS_SESSION_H__

#include "sylar/config.h"
#include <stdint.h>
#include "sylar/http/http_session.h"

namespace sylar{
namespace http{

#pragma pack(1)
struct WSFrameHead{
    enum OPCODE{
        /// 数据分片帧
        CONTINUE = 0,
        /// 文本帧
        TEXT_FRAME = 1,
        /// 二进制帧
        BIN_FRAME = 2,
        /// WebSocket控制帧有3种：Close(关闭帧)、Ping以及Pong
        /// 断开连接
        CLOSE = 8,
        /// PING
        PING = 0x9,
        /// PONG
        PONG = 0xA,
    };
    uint32_t opcode: 4;
    bool rsv3: 1;
    bool rsv2: 1;
    bool rsv1: 1;
    bool fin: 1;
    
    // 以字节为单位的“有效负载数据”长度，如果值为0-125，那么就表示负载数据的长度。
    // 如果是126，那么接下来的2个bytes解释为16bit的无符号整形作为负载数据的长度。
    // 如果是127，那么接下来的8个bytes解释为一个64bit的无符号整形（最高位的bit必须为0）作为负载数据的长度。
    uint32_t payload: 7;
    // mask标志位，定义“有效负载数据”是否添加掩码。如果设置为1，那么掩码的键值存在于Masking-Key中，
    // 根据5.3节描述，这个一般用于解码“有效负载数据”。所有的从客户端发送到服务端的帧都需要设置这个bit位为1
    bool mask: 1;

    std::string toString() const;
};
#pragma pack()

class WSFrameMessage{
public:
    typedef std::shared_ptr<WSFrameMessage> ptr;
    WSFrameMessage(int opcode = 0, const std::string& data = "");

    int getOpcode() const { return m_opcode;}
    void setOpcode(int v) { m_opcode = v;}

    const std::string& getData() const { return m_data;}
    std::string& getData() { return m_data;}
    void setData(const std::string& v) { m_data = v;}
private:
    int m_opcode;
    std::string m_data;
};

class WSSession : public HttpSession{
public:
    typedef std::shared_ptr<WSSession> ptr;
    WSSession(Socket::ptr sock, bool owner = true);

    /// server client
    HttpRequest::ptr handleShake();

    WSFrameMessage::ptr recvMessage();
    int32_t sendMessage(WSFrameMessage::ptr msg, bool fin = true);
    int32_t sendMessage(const std::string& msg, int32_t opcode = WSFrameHead::TEXT_FRAME, bool fin = true);
    int32_t ping();
    int32_t pong();

private:
    bool handleServerShake();
    bool handleClientShake();
};

// websocket_message消息最大长度
extern sylar::ConfigVar<uint32_t>::ptr g_websocket_message_max_size;

WSFrameMessage::ptr WSRecvMessage(Stream* stream, bool client);
int32_t WSSendMessage(Stream* stream, WSFrameMessage::ptr msg, bool client, bool fin);
int32_t WSPing(Stream* stream);
int32_t WSPong(Stream* stream);

}
}

#endif