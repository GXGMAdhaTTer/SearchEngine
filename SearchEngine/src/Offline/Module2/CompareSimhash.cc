#include "../../../Include/CompareSimhash.h"

bool CompareSimhash::cut(const WebPage& page) {
    uint64_t u64 = 0;
    // _simhasher.make(page.getDoc(), topN, u64);
    /* _simhasher.make(page.getDocTitle() + page.getDocLink() + page.getDocDescription() + page.getDocContent(), topN, u64); */

    if (page.getDocContent() == "") {
        _simhasher.make(page.getDocTitle(), topN, u64);
    } else {
        _simhasher.make(page.getDocContent(), topN, u64);
    }

    for (auto& Node : _hashList) {
        if (Node.isExist(u64)) {
            return true;
        }
    }

    HashNode node(u64);
    _hashList.push_back(node);
    return false;
}
