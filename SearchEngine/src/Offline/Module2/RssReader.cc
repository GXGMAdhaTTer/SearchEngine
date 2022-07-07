#include "../../../Include/RssReader.h"
#include <sstream>
using std::istringstream;

string LineProcess(const string& readline) //去除空行函数 已弃用
{
	istringstream iss(readline);
    string res = "";
    string lines;
    while(getline(iss, lines)) {
        if (0 == lines.length()) continue;
        res += "\n" + lines;
    }
	return res;
}

/*解析xml中的内容并正则匹配后放入vector中*/
void RssReader::parseRssf(const string& filename) {
    XMLDocument doc;

    if (XML_SUCCESS != doc.LoadFile(filename.c_str())) {
        cout << "load xml file failed" << endl;
        return;
    }

    XMLElement* root = doc.RootElement();
    XMLElement* ptrNode = root->FirstChildElement()->FirstChildElement();

    while (nullptr != ptrNode) {
        if (!strcmp("item", ptrNode->Value())) {  //如果是item
            RssItem tempRssItem;

            for (const XMLNode* tempNode = ptrNode->FirstChild();
                 nullptr != tempNode && nullptr == tempNode->FirstChildElement();
                 tempNode = tempNode->NextSibling()) {
                if (!strcmp("title", tempNode->Value())) {
                    tempRssItem.title = dissolve(tempNode->ToElement()->GetText());
                } else if (!strcmp("link", tempNode->Value())) {
                    tempRssItem.link = dissolve(tempNode->ToElement()->GetText());
                } else if (!strcmp("description", tempNode->Value())) {
                    tempRssItem.description = dissolve(tempNode->ToElement()->GetText());
                } else if (!strcmp("content:encoded", tempNode->Value()) || !strcmp("content", tempNode->Value())) {
                    tempRssItem.content = dissolve(tempNode->ToElement()->GetText());
                    // tempRssItem.content = LineProcess(dissolve(tempNode->ToElement()->GetText()));
                }
            }

            _rss.push_back(tempRssItem);
        }

        ptrNode = ptrNode->NextSiblingElement();
    }
}

string RssReader::dissolve(const string& text) {
    string res = text;
    {
        regex pattern("<(.[^>]*)>");
        res = regex_replace(res, pattern, "");
    }
    {
        regex pattern("&nbsp;");
        res = regex_replace(res, pattern, "");
    }
    {
        regex pattern("showPlayer\\(\\{.*?\\}\\);");
        res = regex_replace(res, pattern, "");
    }
    { //删除空行
        regex pattern("^[\\s]*\n");
        // regex pattern("^\s*(?=\r?$)\n");
        res = regex_replace(res, pattern, "");
    }
    { //删除大段空格
        regex pattern("                                    ");
        res = regex_replace(res, pattern, " ");
    }
    return res;
}
