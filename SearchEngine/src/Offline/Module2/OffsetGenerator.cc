#include "../../../Include/OffsetGenerator.h"
#include <utility>

OffsetGenerator::OffsetGenerator(const vector<WebPage>& pages,
                                 vector<pair<size_t, size_t>>& offsetTable)
    : _pages(pages), _offsetTable(offsetTable) {}

void OffsetGenerator::process() {
    size_t curOffset = 0;
    for (auto& page : _pages) {
        size_t pageLength = page.getDoc().size();
        _offsetTable.push_back(make_pair(curOffset, pageLength));
        curOffset += pageLength;
    }
    cout << "OffsetTable generating completed!" << endl;
}
