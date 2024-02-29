#include "sylar/email/email.h"
#include "sylar/email/smtp.h"

void test() {
    // ZEMGQSEHWUAGPEDR
    // REORZUWTFABISGRM
    sylar::EMail::ptr email = sylar::EMail::Create(
            "maskyy2024@163.com", "ZEMGQSEHWUAGPEDR",
            "hello world", "<B>hi xxx</B>hell world", {"974033266@qq.com"});
    sylar::EMailEntity::ptr entity = sylar::EMailEntity::CreateAttach("sylar/sylar.h");
    if(entity) {
        // email->addEntity(entity);
    }

    entity = sylar::EMailEntity::CreateAttach("sylar/address.cc");
    if(entity) {
        // email->addEntity(entity);
    }

    auto client = sylar::SmtpClient::Create("smtp.163.com", 465, true);
    if(!client) {
        std::cout << "connect smtp.163.com:25 fail" << std::endl;
        return;
    }
    else{
        std::cout << "connect smtp.163.com:25 成功" << std::endl;
    }

    auto result = client->send(email, 1000, true);
    std::cout << "result=" << result->result << " msg=" << result->msg << std::endl;
    std::cout << client->getDebugInfo() << std::endl;
}

int main(int argc, char** argv) {
    sylar::IOManager iom(1);
    iom.schedule(test);
    iom.stop();
    return 0;
}
