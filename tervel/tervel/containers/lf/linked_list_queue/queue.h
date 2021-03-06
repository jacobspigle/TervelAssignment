#ifndef __TERVEL_CONTAINERS_LF_LINKED_LIST_QUEUE_QUEUE_H_
#define __TERVEL_CONTAINERS_LF_LINKED_LIST_QUEUE_QUEUE_H_

#ifndef NO_TERVEL
  #include <tervel/util/memory/hp/hp_element.h>
  #include <tervel/util/memory/hp/hazard_pointer.h>
#endif
#include <assert.h>
namespace tervel {
namespace containers {
namespace lf {

template<typename T>
class Queue {
 public:
  #ifndef NO_TERVEL
  class Node : public tervel::util::memory::hp::Element {
  #else
  class Node {
  #endif
   public:
    Node() {};
    Node(T &v) : v_(v) {};

    T& value() { return v_; }
    Node * next() { return next_.load(); }

    /**
     * @brief returns whether or not the next_ member variable was changed
     * @details using a compare_and_exchange operation on the next_ member,
     * try to replace nullptr with 'other'. Return whether or not the operation
     * was successful
     *
     * @param other the value to have next_ set to
     * @return whether or not it was success
     */
    bool set_next(Node * &expVal, Node *newVal) {
      return next_.compare_exchange_strong(expVal, newVal);
    }

    // using tervel::util::memory::hp::Element;
    bool on_watch(std::atomic<void *> *address, void *expected);
    // using tervel::util::memory::hp::Element;
    bool on_is_watched();

#ifdef NO_TERVEL
    void safe_delete() {};
#endif
    void access() { access_.fetch_add(1); };
    void unaccess() { access_.fetch_add(-1); };
    bool is_accessed() { return access_.load() != 0; };

    T v_;
    std::atomic<Node *> next_ {nullptr};
    std::atomic<int64_t> access_ {0};
  };


  class Accessor {
   public:
#ifndef NO_TERVEL
    static const tervel::util::memory::hp::HazardPointer::SlotID kSlot =
      tervel::util::memory::hp::HazardPointer::SlotID::SHORTUSE;

    static const tervel::util::memory::hp::HazardPointer::SlotID kSlot2 =
      tervel::util::memory::hp::HazardPointer::SlotID::SHORTUSE;
#endif
    Accessor() {};

    /**
     * @brief Accessor destructor function
     * @details Internally it handles the unwatching of a Node object it watched
     */
    ~Accessor() { uninit(); };

    /**
     * @brief Used to attempt to initialize the Accessor object
     * @details This function loads a value from the address and then attempts
     * to apply to memory protection to it.
     * Please see other implementation examples on how to apply memory protection.
     * It returns whether or not it is successful.
     * If nullptr is loaded then it returns true without calling memory
     * protection functions.
     * If success is returned, then node_ should be set equal to the loaded value
     *
     * @param address the address the reference to node was read from
     * @return whether or not memory protection was successfully applied
     */
    bool init(Node *node,  std::atomic<Node *> * const address);
    /**
     * @brief handles the unwatching of a Node object it watched
     * @details handles the unwatching of a Node object it watched
     */
    void uninit();
    void unaccess_node_only();

    bool isValid() { return next_ != nullptr; };
    T& value() { return next_->value(); };

    Node * ptr() { return node_; };
    Node * ptr_next() { return next_; };

    Node * node_ {nullptr};
    Node * next_ {nullptr};
  };

  Queue();
  ~Queue();


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
/// Capacity Functions
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  /**
   * @brief returns whether or not the queue's container is empty
   * @details returns whether or not the queue's container is empty
   * @return  returns whether or not the queue's container is empty
   */
  bool empty();


  /**
   * @brief returns the number of elements in the queue's container.
   * @details returns the number of elements in the queue's container.
   * @return returns the number of elements in the queue's container.
   */
  int64_t size() { return size_.load(); };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
/// Modifiers
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  /**
   * @brief inserts element at the end
   * @details inserts element at the end
   *
   * @param value the element to insert element at the end
   * @return whether or not it was successful
   * False is returned if the container is full
   */
  bool enqueue(T &value);

  /**
   * @brief removes the first element
   * @details removes the first element
   *
   * @param access used to retrieve the removed value.
   * @return whether or not an element was removed
   * False is returned if the container is empty
   *
   * Implementation Note/Hint:
   * After removing a value from the container, initialize the accessor object,
   * then call the appropriate safe delete function.
   * The initialization will prevent the object from being freed until the next
   * check of the unsafe list, which will free it if all Accessor objects
   * referencing it have had their destructor's called.
   */
  bool dequeue(Accessor &access);



 private:
  std::atomic<Node *> head_;
  Node * head() { return head_.load(); };
  bool set_head(Node * &expVal, Node *newVal) { return head_.compare_exchange_strong(expVal, newVal); };

  std::atomic<Node *> tail_;
  Node * tail() { return tail_.load(); };
  bool set_tail(Node * &expVal, Node *newVal) { return tail_.compare_exchange_strong(expVal, newVal); };
  std::atomic<int64_t> size_{0};
  void size(int i) { size_.fetch_add(i); };
};

}  // namespace lf
}  // namespace containers
}  // namespace tervel
#endif  // __TERVEL_CONTAINERS_LF_LINKED_LIST_QUEUE_QUEUE_H_

#include <tervel/containers/lf/linked_list_queue/queue_imp.h>
