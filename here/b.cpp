#include <iostream>
#include <vector>
#include <functional>

using namespace std;

template <typename tkey, typename tval>
class HashTable {
private:
    struct HashEntry {
        tkey key;
        tval value;
        bool occupied;
        bool deleted;
    };

    size_t capacity;
    size_t size;
    vector<HashEntry> table;
    hash<tkey> hasher;
    function<size_t(tkey, size_t, size_t)> rehasher;

    size_t default_rehash(tkey key, size_t i, size_t cap) {
        return (hasher(key) + i * (1 + hasher(key) % (cap - 1))) % cap;
    }

public:
    HashTable(size_t initialCapacity)
        : capacity(initialCapacity), size(0), table(initialCapacity),
          rehasher([this](tkey key, size_t i, size_t cap) {
              return this->default_rehash(key, i, cap);
          }) {
        for (size_t i = 0; i < capacity; ++i) {
            table[i].occupied = false;
            table[i].deleted = false;
        }
    }

    bool Insert(const tkey& key, const tval& value) {
        if (size >= capacity) {
            cout << "Хэш-таблица заполнена" << endl;
            return false;
        }

        size_t num = -1;
        size_t ind = hasher(key) % capacity;
        size_t i = 0;

        while (true) {
            if (table[ind].occupied && table[ind].key == key) {
                cout << "Дублирование ключа" << endl;
                return false;
            }

            if (table[ind].deleted && num == -1) {
                num = ind;
            }

            if (!table[ind].occupied && num == -1) {
                table[ind].key = key;
                table[ind].value = value;
                table[ind].occupied = true;
                table[ind].deleted = false;
                size++;
                return true;
            }

            if (!table[ind].occupied && num != -1) {
                table[num].key = key;
                table[num].value = value;
                table[num].occupied = true;
                table[num].deleted = false;
                size++;
                return true;
            }

            ind = rehasher(key, ++i, capacity) % capacity;
        }
    }

    size_t FindIndex(const tkey& key) {
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

            ind = rehasher(key, ++probes, capacity) % capacity;
            if (ind == initial_ind && probes > 1) {
                return (size_t)-1;
            }
        }
        return (size_t)-1;
    }

    tval* Search(const tkey& key) {
        size_t index = FindIndex(key);
        if (index != (size_t)-1) {
            return &table[index].value;
        } else {
            return nullptr;
        }
    }

    bool Delete_node(const tkey& key) {
        size_t index = FindIndex(key);
        if (index != (size_t)-1) {
            table[index].deleted = true;
            table[index].occupied = false;
            size--;
            return true;
        } else {
            return false;
        }
    }

    void PrintTable() const {
        cout << "Хэш-таблица:" << endl;
        for (size_t i = 0; i < capacity; ++i) {
            cout << "[" << i << "]: ";
            if (table[i].occupied) {
                cout << "Key = " << table[i].key << ", Value = " << table[i].value
                     << ", Occupied = " << table[i].occupied
                     << ", Deleted = " << table[i].deleted << endl;
            } else {
                cout << "Empty" << endl;
            }
        }
        cout << "Размер: " << size << "/" << capacity << endl;
    }

    size_t get_size() const {
        return size;
    }
};

int main() {
    return 0;
}
