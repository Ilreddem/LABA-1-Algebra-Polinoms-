#include <iostream>
#include <vector>
using namespace std;

template <typename tkey, typename tval>
class TableBase {
protected:
    int nRec;
public:
    TableBase(int k) : nRec(0) {}
    virtual void Insert(const tkey& k, const tval& val) = 0;
    virtual void Delete_node(const tkey& k) = 0;
    virtual tval* Search(const tkey& k) = 0;
    virtual ~TableBase() = default;
};

template <typename tkey, typename tval>
class ArrayTable : public TableBase<tkey, tval> {
    vector<pair<tkey, tval>> array;
    int nReserved;
public:
    ArrayTable(int k) : TableBase<tkey, tval>(k), nReserved(k) {
        array.reserve(k);
    }

    virtual ~ArrayTable() = default;

    void Insert(const tkey& k, const tval& val) override {
        if (this->nRec < nReserved) {
            array.emplace_back(k, val);
            this->nRec++;
        } else {
            cerr << "Table full" << endl;
        }
    }

    tval* Search(const tkey& key) override {
        for (auto& pair : array)
            if (pair.first == key)
                return &pair.second;
        return nullptr;
    }

    void Delete_node(const tkey& key) override {
        for (size_t i = 0; i < array.size(); ++i) {
            if (array[i].first == key) {
                swap(array[i], array.back());
                array.pop_back();
                this->nRec--;
                return;
            }
        }
    }
};

int main() {
    TableBase<int, string>* t = new ArrayTable<int, string>(10);
    t->Insert(0, "ac");
    t->Insert(2, "b");

    string* x = t->Search(0);
    if (x) cout << *x << endl;
    else cout << "Not found" << endl;

    string* y = t->Search(2);
    if (y) cout << *y << endl;
    else cout << "Not found" << endl;

    t->Delete_node(0);  // Используем новое имя
    string* z = t->Search(0);
    if (z) cout << *z << endl;
    else cout << "Not found" << endl;

    delete t;
    return 0;
}
