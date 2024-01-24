#ifndef __SYLAR_ADDRESS_H__
#define __SYLAR_ADDRESS_H__

#include <memory>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <map>

namespace sylar{

class IPAddress;

class Address{
public:
    typedef std::shared_ptr<Address> ptr;
    /**
     * @brief 通过sockaddr指针创建Address
     * @param[in] addr sockaddr指针
     * @param[in] addrlen sockaddr的长度
     * @return 返回和sockaddr相匹配的Address,失败返回nullptr
     */
    static Address::ptr Create(const sockaddr* addr, socklen_t addrlen);

    /**
     * @brief 通过host地址返回对应条件的所有Address
     * @param[out] result 保存满足条件的Address
     * @param[in] host 域名,服务器名等.举例: www.sylar.top[:80] (方括号为可选内容)
     * @param[in] family 协议族(AF_INT, AF_INT6, AF_UNIX)
     * @param[in] type socketl类型SOCK_STREAM、SOCK_DGRAM 等
     * @param[in] protocol 协议,IPPROTO_TCP、IPPROTO_UDP 等
     * @return 返回是否转换成功
     */
    static bool Lookup(std::vector<Address::ptr>& result, const std::string& host,
            int family = AF_INET, int type = 0, int protocol = 0);
    /**
     * @brief 通过host地址返回对应条件的任意Address
     * @param[in] host 域名,服务器名等.举例: www.sylar.top[:80] (方括号为可选内容)
     * @param[in] family 协议族(AF_INT, AF_INT6, AF_UNIX)
     * @param[in] type socketl类型SOCK_STREAM、SOCK_DGRAM 等
     * @param[in] protocol 协议,IPPROTO_TCP、IPPROTO_UDP 等
     * @return 返回满足条件的任意Address,失败返回nullptr
     */
    static Address::ptr LookupAny(const std::string& host,
            int family = AF_INET, int type = 0, int protocol = 0);
    /**
     * @brief 通过host地址返回对应条件的任意IPAddress
     * @param[in] host 域名,服务器名等.举例: www.sylar.top[:80] (方括号为可选内容)
     * @param[in] family 协议族(AF_INT, AF_INT6, AF_UNIX)
     * @param[in] type socketl类型SOCK_STREAM、SOCK_DGRAM 等
     * @param[in] protocol 协议,IPPROTO_TCP、IPPROTO_UDP 等
     * @return 返回满足条件的任意IPAddress,失败返回nullptr
     */
    static Address::ptr LookupAnyIPAddress(const std::string& host,
            int family = AF_INET, int type = 0, int protocol = 0);

    /**
     * @brief 返回本机所有网卡的<网卡名, 地址, 子网掩码位数>
     * @param[out] result 保存本机所有地址
     * @param[in] family 协议族(AF_INT, AF_INT6, AF_UNIX)
     * @return 是否获取成功
     */
    static bool GetInterfaceAddresses(std::multimap<std::string
                    ,std::pair<Address::ptr, uint32_t> >& result,
                    int family = AF_INET);
    /**
     * @brief 获取指定网卡的地址和子网掩码位数
     * @param[out] result 保存指定网卡所有地址
     * @param[in] iface 网卡名称
     * @param[in] family 协议族(AF_INT, AF_INT6, AF_UNIX)
     * @return 是否获取成功
     */
    static bool GetInterfaceAddresses(std::vector<std::pair<Address::ptr, uint32_t> >&result
                    ,const std::string& iface, int family = AF_INET);

    /**
     * @brief 虚析构函数
     */
    virtual ~Address() {}

    /**
     * @brief 返回协议簇
     */
    int getFamily() const;

    /**
     * @brief 返回sockaddr指针,只读
     */
    virtual const sockaddr* getAddr() const = 0;

    /**
     * @brief 返回sockaddr指针,读写
     */
    virtual sockaddr* getAddr() = 0;

    /**
     * @brief 返回sockaddr的长度
     */
    virtual socklen_t getAddrLen() const = 0;

    /**
     * @brief 可读性输出地址
     */
    virtual std::ostream& insert(std::ostream& os) const = 0;

    /**
     * @brief 返回可读性字符串
     */
    std::string toString() const;

    /**
     * @brief 小于号比较函数
     */
    bool operator<(const Address& rhs) const;

    /**
     * @brief 等于函数
     */
    bool operator==(const Address& rhs) const;

    /**
     * @brief 不等于函数
     */
    bool operator!=(const Address& rhs) const;
}


}

#endif