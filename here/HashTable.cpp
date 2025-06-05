//TableHash теперь наследник TableBase (+ ArrayTable сделать .h и убрать в нем main(), если сливать в один проект)
#include <iostream>
#include <vector>
#include <functional>

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
class HashTable : public TableBase<tkey, tval> {
private:
    struct HashEntry {
        tkey key;
        tval value;
        bool occupied;
        bool deleted;
    };

    size_t capacity;
    vector<HashEntry> table;
    hash<tkey> hasher;
    function<size_t(tkey, size_t, size_t)> rehasher;

    size_t default_rehash(tkey key, size_t i, size_t cap) {
        return (hasher(key) + i * (1 + hasher(key) % (cap - 1))) % cap;
    }

    size_t FindIndex(const tkey& key) const {
        size_t ind = hasher(key) % capacity;
        size_t initial_ind = ind;
        size_t probes = 0;

        while (probes <= capacity) {
            if (table[ind].occupied && table[ind].key == key) {
                return ind;
            }

            if (!table[ind].occupied && !table[ind].deleted) {
                return (size_t)-1;
            }

            ind = rehasher(key, ++probes, capacity);
            if (ind == initial_ind && probes > 1) {
                return (size_t)-1;
            }
        }
        return (size_t)-1;
    }

public:
    HashTable(size_t initialCapacity)
        : TableBase<tkey, tval>(0), capacity(initialCapacity),
          table(initialCapacity),
          rehasher([this](tkey key, size_t i, size_t cap) {
              return this->default_rehash(key, i, cap);
          }) {
        for (auto& entry : table) {
            entry.occupied = false;
            entry.deleted = false;
        }
    }

    void Insert(const tkey& key, const tval& value) override {
        if (this->nRec >= static_cast<int>(capacity)) {
            cerr << "Хэш-таблица заполнена" << endl;
            return;
        }

        size_t i = 0;
        size_t first_deleted = static_cast<size_t>(-1);

        while (i < capacity) {
            size_t index = rehasher(key, i, capacity);

            if (table[index].occupied && table[index].key == key) {
                cerr << "Дублирование ключа" << endl;
                return;
            }

            if (table[index].deleted && first_deleted == static_cast<size_t>(-1)) {
                first_deleted = index;
            }

            if (!table[index].occupied) {
                size_t target_index = (first_deleted != static_cast<size_t>(-1)) ? first_deleted : index;
                table[target_index].key = key;
                table[target_index].value = value;
                table[target_index].occupied = true;
                table[target_index].deleted = false;
                this->nRec++;
                return;
            }

            ++i;
        }

        cerr << "Не удалось вставить элемент" << endl;
    }

    tval* Search(const tkey& key) override {
        size_t index = FindIndex(key);
        if (index != (size_t)-1) {
            return &table[index].value;
        } else {
            return nullptr;
        }
    }

    void Delete_node(const tkey& key) override {
        size_t index = FindIndex(key);
        if (index != (size_t)-1) {
            table[index].occupied = false;
            table[index].deleted = true;
            this->nRec--;
        }
    }

    void PrintTable() const {
        cout << "Хэш-таблица:" << endl;
        for (size_t i = 0; i < capacity; ++i) {cout << "[" << i << "]: ";
            if (table[i].occupied) {
                cout << "Key = " << table[i].key << ", Value = " << table[i].value
                     << ", Occupied = " << table[i].occupied
                     << ", Deleted = " << table[i].deleted << endl;
            } else {
                cout << "Empty" << endl;
            }
        }
        cout << "Размер: " << this->nRec << "/" << capacity << endl;
    }
};
int main() {
    TableBase<int, string>* t = new HashTable<int, string>(10);
    t->Insert(1, "hello");
    return 0;
}
