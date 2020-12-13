#pragma once
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
using DataType = long long;
const DataType NullValue = -1;

class ConQueue {
   public:
    virtual auto get() -> DataType = 0;
    virtual auto put(DataType element) -> int = 0;
    virtual auto print() -> void = 0;
};

class ListQueue : public ConQueue {
   public:
    struct Node {
        DataType value;
        Node* next;
        Node(DataType _value) : value(_value), next(nullptr) {}
    };
    ListQueue() {
        head = tail = new Node(NullValue);
    }
    auto empty() -> bool {
        return head == tail;
    }

   protected:
    Node* head;
    Node* tail;
};

class AtomicListQueue : public ConQueue {
   public:
    struct Node {
        DataType value;
        std::atomic<Node*> next;
        Node(DataType _value) : value(_value), next(nullptr) {}
    };
    AtomicListQueue() {
        head = tail = new Node(NullValue);
    }
    auto empty() -> bool {
        return head == tail;
    }

   protected:
    std::atomic<Node*> head;
    std::atomic<Node*> tail;
};

class ArrayQueue : public ConQueue {
   public:
    ArrayQueue(size_t _length) {
        length = _length;
        buffer = new DataType[length];
        head = tail = 0;
    }
    auto empty() -> bool {
        return head == tail;
    }

    auto full() -> bool {
        return head == next(tail);
    }

   protected:
    inline auto next(long long pos) -> long long {
        return (pos + 1) % length;
    }
    DataType* buffer;
    size_t head, tail;
    size_t length;
};

class MutexNonBlockListQueue : public ListQueue {
   public:
    auto get() -> DataType override {
        std::unique_lock<std::mutex> l(readLock);
        if (empty()) {
            return NullValue;
        } else {
            Node* t = head;
            head = head->next;
            DataType v = head->value;
            l.unlock();
            delete t;
            return v;
        }
    }
    auto put(DataType element) -> int override {
        Node* t = new Node(element);
        std::unique_lock<std::mutex> l(writeLock);
        tail->next = t;
        tail = t;
        return 0;
    }

    auto print() -> void override {
        std::cout << "Non blocking - mutex - list based queue" << std::endl;
    }

   private:
    std::mutex readLock;
    std::mutex writeLock;
};

class MutexBlockListQueue : public ListQueue {
   public:
    auto get() -> DataType override {
        std::unique_lock<std::mutex> l(lock);
        isEmpty.wait(l, [&]() -> bool { return !empty(); });
        Node* t = head;
        head = head->next;
        DataType v = head->value;
        l.unlock();
        delete t;
        return v;
    }
    auto put(DataType element) -> int override {
        Node* t = new Node(element);
        std::unique_lock<std::mutex> l(lock);
        tail->next = t;
        tail = t;
        l.unlock();
        isEmpty.notify_one();
        return 0;
    }

    auto print() -> void override {
        std::cout << "Blocking - mutex - list based queue" << std::endl;
    }

   private:
    std::mutex lock;
    std::condition_variable isEmpty;
};

class CasNonBlockListQueue : public AtomicListQueue {
   public:
    auto get() -> DataType override {
        Node* p;
        do {
            p = head.load();
            if (p->next == nullptr) {
                return NullValue;
            }
        } while (!std::atomic_compare_exchange_weak(&head, &p, p->next));
        DataType v = p->next.load()->value;
        // delete p;
        // Memory leak here, hard to find a proper time to delete p, using sharedptr
        // may be a good choice
        return v;
    }
    auto put(DataType element) -> int override {
        Node* t = new Node(element);
        Node* p;
        Node* null = nullptr;
        do {
            p = tail.load();
            null = nullptr;
        } while (!std::atomic_compare_exchange_weak(&p->next, &null, t));

        tail.store(t);
        return 0;
    }

    auto print() -> void override {
        std::cout << "Non blocking - CAS - list based queue" << std::endl;
    }
};

class MutexBlockArrayQueue : public ArrayQueue {
   public:
    MutexBlockArrayQueue(size_t length) : ArrayQueue(length) {}
    auto get() -> DataType override {
        std::unique_lock<std::mutex> l(lock);
        isEmpty.wait(l, [&]() -> bool { return !empty(); });
        DataType v = buffer[head];
        head = next(head);
        l.unlock();
        isFull.notify_one();
        return v;
    }
    auto put(DataType element) -> int override {
        std::unique_lock<std::mutex> l(lock);
        isFull.wait(l, [&]() -> bool { return !full(); });
        buffer[tail] = element;
        tail = next(tail);
        l.unlock();
        isEmpty.notify_one();
        return 0;
    }

    auto print() -> void override {
        std::cout << "Blocking - mutex - array based queue" << std::endl;
    }

   private:
    std::mutex lock;
    std::condition_variable isEmpty;
    std::condition_variable isFull;
};
