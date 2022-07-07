#include "../../../Include/SearchEngineClient.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#define KEYRECOMMAND 0
#define WEBPAGESEARCH 1
#define BUFFERMAX 65536
#define QUIT 9

using namespace nlohmann;
using std::cin;
using std::cout;
using std::endl;

void SearchEngineClient::connectionInit() {
    const string ip = GetPath("IP");
    const unsigned short port = static_cast<unsigned short>(stoi(GetPath("PORT")));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    int ret = connect(_sock.fd(), (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == ret) {
        perror("Connection failed!");
        exit(EXIT_FAILURE);
    } else if (0 == ret) {
        cout << "连接成功！" << endl;
    }
}

void SearchEngineClient::start() {
    char buf[BUFFERMAX] = {0};
    while (1) {
        int type;
        cin >> type;
        if (KEYRECOMMAND == type) {
            string word;
            cin >> word;
            json myJ;
            myJ["msgID"] = KEYRECOMMAND;
            myJ["msg"] = word;
            string response = myJ.dump(4);
            _sockIO.writen(response.c_str(), response.size()+1);
        } else if (WEBPAGESEARCH == type) {
            string msg;
            getline(cin, msg);
            json myJ;
            myJ["msgID"] = WEBPAGESEARCH;
            myJ["msg"] = msg;
            string response = myJ.dump(4);
            _sockIO.writen(response.c_str(), response.size()+1);
        } else if (QUIT == type) {
            cout << "再见！" << endl;
            return;
        } else {
            cout << "输入错误！" << endl;
            continue;
        }

        bzero(buf, BUFFERMAX);
        // _sockIO.readLine(buf, BUFFERMAX);
        _sockIO.readJson(buf, BUFFERMAX);
        json result = json::parse(string(buf));
        if (404 == result["msgID"]) { //查询失败
            cout << result["msg"] << endl;
        } else if (100 == result["msgID"]) { //是关键词推荐结果
            for (auto& word : result["msg"]) {
                cout << word << " ";
            }
            cout << endl << endl;
        } else if (200 == result["msgID"]) { //是网页查询结果
            for (auto& page : result["msg"]) {
                cout << "title: " << page["title"] << endl;
                cout << "url: " << page["url"] << endl;
                cout << "summary:" << page["summary"] << endl << endl;
            }
        } else if (666 == result["msgID"]) {
            cout << result["msg"] << endl;
            return;
        }
        else {
            cout << "返回错误" << endl;
        }
    }
}
